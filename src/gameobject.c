#include "gameobject.h"

gameobjectdef_t gameobject_definitions[MAX_GAMEOBJECT_DEFINITIONS];

int gameobject_set_flags(gameobject_t *go, int flags)
{
    go->flags |= flags;
    return 1;
}

int gameobject_unset_flags(gameobject_t *go, int flags)
{
    go->flags &= ~flags;
    return 1;
}

int gameobject_isontile(gameobject_t *go, int x, int y)
{
    gameobjectdef_t *godef = &gameobject_definitions[go->object_id];
    int width;
    int height;

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
    gameobjectdef_t *godef = &gameobject_definitions[go_id];
    return godef->type;
}

int gameobject_get_dir(int go_id)
{
    gameobjectdef_t *godef = &gameobject_definitions[go_id];
    return godef->direction;
}