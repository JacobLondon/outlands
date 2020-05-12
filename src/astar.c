#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "util.h"
#include "globals.h"
#include "astar.h"

/**
 * TODO: A* is not very memory efficient. It should be given a list of coordinates,
 * and generate a node map from that. Right now it is a grid, taking up a LOT of
 * memory. Many of these extra nodes will never be used.
 */

#define NEIGHBORS_MAX 4

typedef struct node_tag {
	float global_goal;
	float local_goal;
	int x;
	int y;
	bool obstacle;
	bool visited;
	struct node_tag *neighbors[NEIGHBORS_MAX];
	struct node_tag *parent;
} node;

// non-resizeable, max size sets in stone
// does NOT own items in buf
typedef struct queue_tag {
	void **buf;
	void **front;
	size_t size;
	size_t cap;
	size_t item_size;
} queue;

queue *queue_new(size_t size, size_t count);
void queue_del(queue *self);
void queue_clear(queue *self);
bool queue_empty(queue *self);
void queue_enq(queue *self, void *n);
void *queue_front(queue *self);
void queue_deq(queue *self);


static bool node_dist(const node *a, const node *b);
static void node_neighbor_add(node *self, node *neighbor);

static void astar_reset(void);
static void astar_solve(int si, int sj, int ei, int ej);


static size_t astar_width = 0;
static size_t astar_height = 0;
static node *astar_nodes = NULL;
static queue *astar_untested = NULL;
static bool (*astar_check_obstacle)(int i, int j) = NULL; /* return true if the coordinate is an obstacle */
static bool initialized = false;


queue *queue_new(size_t size, size_t count)
{
	queue *self = allocate(sizeof(queue));
	assert(self);
	self->buf = allocate(size * count * 2);
	assert(self->buf);
	memset(self->buf, 0, size * size * 2);
	self->front = self->buf;
	self->size = 0;
	self->item_size = size;
	self->cap = count * 2;
	return self;
}

void queue_del(queue *self)
{
	assert(self);
	dealloc(self->buf);
	dealloc(self);
	memset(self, 0, sizeof(queue));
}

void queue_clear(queue *self)
{
	size_t i;
	assert(self);
	for (i = 0; i < self->size; i++) {
		self->front[i] = NULL;
	}
	self->front = &self->buf[self->cap / 2];
	self->size = 0;
}

bool queue_empty(queue *self)
{
	assert(self);
	return self->size == 0;
}

void queue_enq(queue *self, void *n)
{
	assert(self);
	assert(n);
	assert(&self->front[self->size + 1] < &self->buf[self->cap]);
	self->front[self->size] = n;
	self->size++;
}

void *queue_front(queue *self)
{
	assert(self);
	return self->front[0];
}

void queue_deq(queue *self)
{
	assert(self);
	assert(&self->front[1] < &self->buf[self->cap]);
	self->front[0] = NULL;
	self->front++;
	self->size--;
}


static bool node_dist(const node *a, const node *b)
{
	return fast_sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

static void node_neighbor_add(node *self, node *neighbor)
{
	unsigned char i;
	assert(self);
	assert(neighbor);
	for (i = 0; i < NEIGHBORS_MAX; i++) {
		if (self->neighbors[i] == NULL) {
			self->neighbors[i] = neighbor;
			return;
		}
	}
	assert(i != NEIGHBORS_MAX);
}


void astar_init(size_t width, size_t height, bool (*check_obstacle_cb)(int i, int j))
{
	int i, j;
	assert(check_obstacle_cb);
	initialized = true;

	astar_width = width;
	astar_height = height;
	astar_check_obstacle = check_obstacle_cb;
	astar_nodes = allocate(width * height * sizeof(node));
	assert(astar_nodes);
	memset(astar_nodes, 0, width * height * sizeof(node));

	// needs to be more than longest possible path as the front moves
	astar_untested = queue_new(sizeof(node), 2 * width * height);

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			astar_nodes[i * width + j].x = j;
			astar_nodes[i * width + j].y = i;
			if (i > 0) {
				node_neighbor_add(&astar_nodes[i * width + j], &astar_nodes[(i - 1) * width + j]);
			}
			if (i < height - 1) {
				node_neighbor_add(&astar_nodes[i * width + j], &astar_nodes[(i + 1) * width + j]);
			}
			if (j > 0) {
				node_neighbor_add(&astar_nodes[i * width + j], &astar_nodes[i * width + (j - 1)]);
			}
			if (j < width - 1) {
				node_neighbor_add(&astar_nodes[i * width + j], &astar_nodes[i * width + (j + 1)]);
			}
		}
	}
}

void astar_cleanup(void)
{
	assert(astar_nodes);
	assert(astar_untested);
	initialized = false;
	memset(astar_nodes, 0, astar_width * astar_height * sizeof(node));
	dealloc(astar_nodes);
	astar_nodes = NULL;
	queue_del(astar_untested);
	astar_untested = NULL;
	astar_width = 0;
	astar_height = 0;
	astar_check_obstacle = NULL;
}

static void astar_reset(void)
{
	int i, j;
	for (i = 0; i < astar_height; i++) {
		for (j = 0; j < astar_width; j++) {
			if (astar_check_obstacle(i, j)) {
				astar_nodes[i * astar_width + j].obstacle = true;
			}
			else {
				astar_nodes[i * astar_width + j].obstacle = false;
			}
			astar_nodes[i * astar_width + j].parent = NULL;
			astar_nodes[i * astar_width + j].visited = false;
			astar_nodes[i * astar_width + j].global_goal = INFINITY;
			astar_nodes[i * astar_width + j].local_goal = INFINITY;
		}
	}
	queue_clear(astar_untested);
}

static void astar_solve(int si, int sj, int ei, int ej)
{
	node *start;
	node *end;
	node *current;
	unsigned char i;
	float possible_goal;

	astar_reset();

	start = &astar_nodes[si * astar_width + sj];
	end = &astar_nodes[ei * astar_width + ej];
	current = start;
	current->local_goal = 0.0f;
	current->global_goal = node_dist(start, end);
	queue_enq(astar_untested, start);

	while (!queue_empty(astar_untested)) {
		// ignore visited nodes
		while (!queue_empty(astar_untested) && ((node *)astar_untested->front[0])->visited) {
			queue_deq(astar_untested);
		}
		// popped last node
		if (queue_empty(astar_untested)) {
			break;
		}
		current = queue_front(astar_untested);
		current->visited = true;

		// check neighbors
		for (i = 0; i < NEIGHBORS_MAX && current->neighbors[i]; i++)
		{
			// record the neighbor if it wasn't visited yet
			if (!current->neighbors[i]->visited && !current->neighbors[i]->obstacle) {
				queue_enq(astar_untested, current->neighbors[i]);
			}
			// find local goals
			possible_goal = current->local_goal + node_dist(current, current->neighbors[i]);
			if (possible_goal < current->neighbors[i]->local_goal) {
				current->neighbors[i]->parent = current;
				current->neighbors[i]->local_goal = possible_goal;
				current->neighbors[i]->global_goal = current->neighbors[i]->local_goal + node_dist(current->neighbors[i], end);
			}
		}
	}
}

size_t astar_path(int *xs, int *ys, size_t size, int si, int sj, int ei, int ej)
{
	size_t count = 0;
	node *n;
	assert(initialized);
	assert(xs);
	assert(ys);
	assert(0 <= si && si < astar_height);
	assert(0 <= sj && sj < astar_width);
	assert(0 <= ei && ei < astar_height);
	assert(0 <= ej && ej < astar_width);
	astar_solve(si, sj, ei, ej);
	for (n = &astar_nodes[ei * astar_width + ej]; n->parent; n = n->parent) {
		if (count < size) {
			xs[count] = n->x;
			ys[count] = n->y;
		}
		count++;
	}
	assert(size >= count);
	return count;
}
