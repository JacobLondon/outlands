#include <stdio.h>
#include <outlands.h>

char myarray[10][10] = {
	"          ",
	"######### ",
	"          ",
	" #########",
	"          ",
	"######### ",
	"          ",
	" #########",
	"          ",
	"######### ",
};

static bool my_obstacle_cb(int i, int j)
{
	return myarray[i][j] == '#';
}

int main(void)
{
	int xs[100] = { 0 };
	int ys[100] = { 0 };
	size_t size;
	int i, j;

	astar_init(10, 10, my_obstacle_cb);
	size = astar_path(xs, ys, 100, 0, 0, 9, 9);
	printf("Path is %zu units\n", size);

	for (i = 0; i < size; i++) {
		myarray[ys[i]][xs[i]] = '.';
	}

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("%c ", myarray[i][j]);
		}
		printf("\n");
	}

	astar_cleanup();

	return 0;
}
