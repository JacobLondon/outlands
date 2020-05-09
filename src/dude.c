#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include "scene_object.h"
#include "texture_manager.h"
#include "globals.h"
#include "ship.h"
#include "util.h"
#include "dude.h"

/* Dudes are the guys you control.
 * Dudes move based on a job array.
 * Other files as needed can put jobs
 * targeted at specific dude id's, and
 * the dude(s) with the specific id's
 * will do the action. Each dude can only
 * have 1 action at a time, if another
 * action comes in, the current job is
 * overwritten with the new one. This
 * makes adding external interfaces to making
 * dudes do things simple, by only having
 * a way to get a dude's id, and to request
 * a job of them.
 * 
 * Dudes can only be requested to move
 * to certain squares, and cannot move
 * into a square already occupied by another
 * dude, regardless of their team id (good / evil)
 * 
 * Dudes can however pass thru their team-mates
 * to get to a square. The dude request struct
 * will be public, and the dude request function
 * (places callbacks in the job list) will also
 * be public.
 * 
 * Everything else will be done here internally.
 */

#define DUDES_MAX ((unsigned char)128) // 2^7

// TODO: Dude's, when selected, should be highlighted, gotta record whether selected or not
typedef struct dude_tag {
	// add name?
	int x;
	int y;
	int health;
	so *object;
} dude;

typedef struct dude_def_tag {
	char *name;
	char *png;
	int width;
	int height;
	int health;
} dude_def;

typedef struct job_tag {
	unsigned char id;
	bool done;
	int x; // target x position
	int y; // target y position
	int state;
} job;

static dude_def dude_definitions[] = {
	{ .name = "Humans", .png = "assets/human_m.png", .width = 1, .height = 1, .health = 100 },
	{ NULL, NULL, 0, 0 }
};


static unsigned char num_dudes = 0; // no. of dudes

// bit 0-6 is the unique dude index, each dude gets one
// bit 7 is the team id, 0 == team 0, 1 == team 1
// so dude 0-63 is team 0 while dude 64-127 is team 1
static job jobs[DUDES_MAX] = { 0 };
static dude dudes[DUDES_MAX] = { 0 };
static const job NO_JOB = { 0 };

static int sel_start_x = 0;
static int sel_start_y = 0;
static int sel_curr_x = 0;
static int sel_curr_y = 0;
static int sel_x = 0;
static int sel_y = 0;
static int sel_w = 0;
static int sel_h = 0;


// given x and y, return true if x and y is not taken by another dude
static bool is_available(int x, int y);
static void select_dudes(void);
static void work_on_job(int id);


void dude_load(size_t numberof, char *name, ship *other)
{
	int i, tmp, start;
	int *xs, *ys;
	size_t num_walkable;
	Texture2D *t;
	anim *a;
	dude_def *d = NULL;

	assert(name);
	assert(other);
	assert(num_dudes + numberof < DUDES_MAX);

	num_walkable = ship_find_walkable_batch(other, &xs, &ys);

	// find definition
	for (i = 0; dude_definitions[i].name != NULL; i++) {
		if (streq(name, dude_definitions[i].name)) {
			d = &dude_definitions[i];
			break;
		}
	}
	assert(("Dude definition not found", d != NULL));
	t = texman_load(d->png);

	// put all of the dudes in
	for (; num_dudes < numberof; num_dudes++) {
		// generate an spot to take
		tmp = rand_range(0, num_walkable);

		// do linear search for next available square
		if (!is_available(xs[tmp], ys[tmp])) {
			start = tmp;
			for (tmp = (tmp + 1) % num_walkable;
			     tmp != start;
				 tmp = (tmp + 1) % num_walkable)
			{
				if (is_available(xs[tmp], ys[tmp])) {
					break;
				}
			}
			assert(("No spot for dude found", tmp != start));
		}

		dudes[num_dudes].x = xs[tmp];
		dudes[num_dudes].y = ys[tmp];
		a = anim_new(t, d->width, d->height);
		dudes[num_dudes].object = so_new_own(a);
		dudes[num_dudes].health = d->health;
	}
}

void dude_unload(void)
{
	int i;
	for (i = 0; i < num_dudes; i++) {
		so_del(dudes[i].object);
	}
	memset(dudes, 0, sizeof(dudes));
	num_dudes = 0;
}

void dude_update(void)
{
	int i;
	for (i = 0; i < num_dudes; i++) {
		if (dudes[i].health <= 0) {
			continue;
		}
		so_update(dudes[i].object);
		if (memcmp(&jobs[i], &NO_JOB, sizeof(job)) != 0) {
			work_on_job(i);
		}
	}
}

void dude_draw(void)
{
	int i;
	static char num[8];
	for (i = 0; i < num_dudes; i++) {
		if (dudes[i].health <= 0) {
			continue;
		}

		// draw dude
		so_set_pos(
			dudes[i].object,
			dudes[i].x * GRID_PIX_WIDTH,
			dudes[i].y * GRID_PIX_HEIGHT
		);
		so_draw(dudes[i].object);
		memset(num, 0, sizeof(num));
		sprintf(num, "%d", i);
		num[sizeof(num) - 1] = 0;
		DrawText(num,
			dudes[i].x * GRID_PIX_WIDTH,
			dudes[i].y * GRID_PIX_HEIGHT,
			16, BLACK);
		DrawText(num,
			dudes[i].x * GRID_PIX_WIDTH,
			dudes[i].y * GRID_PIX_HEIGHT,
			12, WHITE);
		
		// draw dude's job
		if (memcmp(&jobs[i], &NO_JOB, sizeof(job)) != 0) {
			DrawRectangle(
				jobs[i].x * GRID_PIX_WIDTH,
				jobs[i].y * GRID_PIX_HEIGHT,
				GRID_PIX_WIDTH,
				GRID_PIX_HEIGHT,
				(Color){0, 100, 100, 100}
			);
		}
	}
}

void dude_job_assign(unsigned char id, int x, int y)
{
	assert(0 <= id && id < DUDES_MAX);
	jobs[id].id = id;
	jobs[id].x = x;
	jobs[id].y = y;
	jobs[id].state = 0;
	jobs[id].done = false;
}

void dude_select_update(ship *other)
{
	static int state = 0;
	static unsigned char selected[DUDES_MAX] = { 0 };
	static unsigned char idx = 0;
	int i, tmp;

	assert(other);

	switch (state) {
		case 0: { // wait for a selection to start
			if (IsMouseButtonDown(1)) {
				sel_start_x = GetMouseX();
				sel_start_y = GetMouseY();
				sel_curr_x = sel_start_x;
				sel_curr_y = sel_start_y;
				state = 1;
			}
			else {
				sel_start_x = 0;
				sel_start_y = 0;
				sel_curr_x = 0;
				sel_curr_y = 0;
			}
			break;
		}
		case 1: { // selection just started
			if (IsMouseButtonDown(1)) {
				// still being pressed, update current
				sel_curr_x = GetMouseX();
				sel_curr_y = GetMouseY();
			}
			else if (IsMouseButtonUp(1)) {
				// selection complete.
				sel_curr_x = GetMouseX();
				sel_curr_y = GetMouseY();
				state = 2;
			}
			break;
		}
		case 2: { // save the id's of those in the selection
			for (i = 0; i < num_dudes; i++) {
				if (dudes[i].health <= 0) {
					continue;
				}

				// dude is to be selected!
				if (dudes[i].x >= sel_x / GRID_PIX_WIDTH && dudes[i].x <= (sel_x + sel_w) / GRID_PIX_WIDTH &&
				    dudes[i].y >= sel_y / GRID_PIX_HEIGHT && dudes[i].y <= (sel_y + sel_h) / GRID_PIX_HEIGHT)
				{
					selected[idx++] = i;
				}
			}
			sel_x = 0;
			sel_y = 0;
			sel_w = 0;
			sel_h = 0;
			sel_start_x = 0;
			sel_start_y = 0;
			sel_curr_x = 0;
			sel_curr_y = 0;
			state = 3;
			break;
		}
		case 3: { // dudes are selected, click on spot to move them there
			if (IsMouseButtonDown(1)) {
				sel_curr_x = GetMouseX();
				sel_curr_y = GetMouseY();
				if (ship_is_walkable(other, sel_curr_x / GRID_PIX_WIDTH, sel_curr_y / GRID_PIX_HEIGHT)) {
					for (i = 0; i < idx; i++) {
						dude_job_assign(selected[i], sel_curr_x / GRID_PIX_WIDTH, sel_curr_y / GRID_PIX_HEIGHT);
					}
				}
				sel_curr_x = 0;
				sel_curr_y = 0;
				state = 4;
			}
			break;
		}
		case 4: { // reset
			if (IsMouseButtonUp(1)) {
				memset(selected, 0, sizeof(selected));
				idx = 0;
				state = 0;
			}
			break;
		}
	}

	// ensure top left corner is adjusted based on
	// mouse quadrant from start as origin
	if (sel_start_x > sel_curr_x) {
		sel_x = sel_curr_x;
		sel_w = sel_start_x - sel_curr_x;
	}
	else {
		sel_x = sel_start_x;
		sel_w = sel_curr_x - sel_start_x;
	}
	if (sel_start_y > sel_curr_y) {
		sel_y = sel_curr_y;
		sel_h = sel_start_y - sel_curr_y;
	}
	else {
		sel_y = sel_start_y;
		sel_h = sel_curr_y - sel_start_y;
	}
}

void dude_select_draw(void)
{
	DrawRectangle(sel_x, sel_y, sel_w, sel_h, (Color){0, 100, 100, 100});
}

static bool is_available(int x, int y)
{
	int i;
	for (i = 0; i < num_dudes; i++) {
		if (x == dudes[i].x && y == dudes[i].y) {
			return false;
		}
	}
	return true;
}

static void work_on_job(int id)
{
	assert(0 <= id && id < DUDES_MAX);
	// TODO: A-Star to path find to location

	if (jobs[id].done) {
		jobs[id] = NO_JOB;
		return;
	}
	if (jobs[id].state == 0) {
		printf("%d working\n", id);
	}
	else if (jobs[id].state == 60) {
		jobs[id].done = true;
		return;
	}
	jobs[id].state++;
}