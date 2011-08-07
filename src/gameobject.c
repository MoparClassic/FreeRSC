#include "gameobject.h"
#include "common.h"

#include <assert.h>

gameobjectdef_t gameobject_definitions[MAX_GAMEOBJECT_DEFINITIONS];

int gameobject_isontile(gameobject_t *go, int x, int y)
{
    gameobjectdef_t *godef = &gameobject_definitions[go->object_id];
    int width;
    int height;

    assert(x >= 0 && x < MAX_MAP_WIDTH);
    assert(y >= 0 && y < MAX_MAP_HEIGHT);

    if (godef->type == 1) {
        width = 1;
        height = 1;
    }
    else if (godef->direction == 0 || godef->direction == 4) {
        width = godef->width;
        height = godef->height;
    }
    else {
        width = godef->height;
        height = godef->width;
    }

    if (godef->type == 0) {
        return x >= go->x && x <= (go->x + width) &&
               y >= go->y && y <= (go->y + height);
    }
    else {
        return x == go->x && y == go->y;
    }
}

int gameobject_get_type(int go_id)
{
    assert(go_id >= 0 && go_id < MAX_GAMEOBJECT_DEFINITIONS);
    return gameobject_definitions[go_id].type;
}

int gameobject_get_dir(int go_id)
{
    assert(go_id >= 0 && go_id < MAX_GAMEOBJECT_DEFINITIONS);
    return gameobject_definitions[go_id].direction;
}