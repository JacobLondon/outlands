#ifndef OUTLANDS_ASTAR_H
#define OUTLANDS_ASTAR_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Give the width and height of the walkable area.
 * @param check_obstacle_cb returns true if the item at i and j is an obstacle, or NOT walkable
 */
void astar_init(size_t width, size_t height, bool (*check_obstacle_cb)(int i, int j));
void astar_cleanup(void);
/**
 * Give an array of x and y to copy the calculated path IN REVERSE ORDER
 * size is the max you want to copy. This is problematic because if you
 * don't make size large enough, you can't get the next available node.
 * This is for safety, and will assert if size isn't big enough.
 * 
 * It returns the number of steps total there were, regardless of size
 * si/sj = start i and start j
 * ei/ej = end i and end j
 */
size_t astar_path(int *xs, int *ys, size_t size, int si, int sj, int ei, int ej);

#endif // OUTLANDS_ASTAR_H
