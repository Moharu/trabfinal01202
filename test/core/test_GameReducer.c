#include "unity.h"
#include "GameReducer.h"

void setUp(void){}
void tearDown(void){}

GameState sampleState(){
    GameState state;
    Physics statePhysics;
    statePhysics.height = 5;
    statePhysics.velocity = 0;
    statePhysics.maxHeight = 10;
    statePhysics.gravity = -10;
    statePhysics.elapsedTime = 1.0/30;
    statePhysics.hVelocity = 10;
    statePhysics.hPosition = 0;
    state.physics = statePhysics;
    return state;
}

void test_simpleFrame(void){
    GameState initialState = sampleState();
    Action action;
    action.type = ACTION_NONE;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.height, 4.9944);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.velocity, -0.3333);
}

void test_flap(void){
    GameState initialState = sampleState();
    initialState.physics.height = 4.9944;
    initialState.physics.velocity = -0.3333;
    Action action;
    action.type = ACTION_FLAP;
    action.params[0] = 6;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.height, 5.1888);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.velocity, 5.6666);
}

void test_upperCollision(void){
    GameState initialState = sampleState();
    initialState.physics.height = 9.9800;
    initialState.physics.velocity = 0;
    Action action;
    action.type = ACTION_FLAP;
    action.params[0] = 6;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.height, 10);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.velocity, 0);
}

void test_lowerCollision(void){
    GameState initialState = sampleState();
    initialState.physics.height = 0.1;
    initialState.physics.velocity = -5;
    Action action;
    action.type = ACTION_NONE;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.height, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.velocity, 0);
}

void test_horizontalMovement(void){
    GameState initialState = sampleState();
    Action action;
    action.type = ACTION_NONE;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.physics.hPosition, 0.3333);
}
