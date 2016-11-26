// This header defines the possible (inter)action types

/*
    ACTION_NONE: No user interaction
*/
#define ACTION_NONE 0

/*
    ACTION_FLAP: User requested to flap wings
        Params:
        [0]: Flap acceleration
*/
#define ACTION_FLAP 1

/*
    ACTION_SHOOT: User requested to shoot (expert mode)
        Params:
        [0]: Projectile speed
*/
#define ACTION_SHOOT 2

// Action structure
typedef struct actions {
    int type;
    float params[20];
} Action;
