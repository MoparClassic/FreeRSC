#include "entityutil.h"
#include "common.h"
#include "linkedlist.h"
#include "tile.h"

#include <assert.h>

inline int
within_range(int x, int y, int targ_x, int targ_y)
{
    int xdif = x - targ_x;
    int ydif = y - targ_y;
    return xdif <= 16 && xdif >= -15 &&
           ydif <= 16 && ydif >= -15;
}

int
get_viewed_area(int mob_x, int mob_y, linkedlist_t *tile_list,
                int x1, int y1, int x2, int y2)
{
    int start_x, start_y, end_x, end_y;
    int x_width, y_width;
    int x, y;

    assert(tile_list); /* linkedlist_t must not be NULL */

    /* All other arguments must be positive integers */
    assert(mob_x >= 0);
    assert(mob_y >= 0);
    assert(x1 >= 0);
    assert(x2 >= 0);
    assert(y1 >= 0);
    assert(y2 >= 0);

    start_x = mob_x - x1;
    if (start_x < 0) {
        start_x = 0;
    }

    start_y = mob_y - y1;
    if (start_y < 0) {
        start_y = 0;
    }

    end_x = mob_x + x2;
    if (end_x >= MAX_MAP_WIDTH) {
        end_x = MAX_MAP_WIDTH - 1;
    }

    end_y = mob_y + y2;
    if (end_y >= MAX_MAP_HEIGHT) {
        end_y = MAX_MAP_HEIGHT - 1;
    }

    if (start_x > end_x) {
        x_width = start_x - end_x;
    }
    else {
        x_width = end_x - start_x;
    }

    if (start_y > end_y) {
        y_width = start_y - end_y;
    }
    else {
        y_width = end_y - start_y;
    }

    for (x = 0; (x + start_x) < end_x; ++x) {
        for (y = 0; (y + start_y) < end_y; ++y) {
            linkedlist_prepend(tile_list,
                               &tiles[x + start_x][y + start_y]);
        }
    }
}