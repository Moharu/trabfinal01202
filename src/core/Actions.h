// Action types
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

// Action structure
typedef struct actions {
    int type;
    float params[20];
} Action;
