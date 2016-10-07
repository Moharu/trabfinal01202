#include "unity.h"
#include "GameReducer.h"

void setUp(void){}
void tearDown(void){}

GameState sampleState(){
    GameState state;
    state.height = 5;
    state.velocity = 0;
    state.maxHeight = 10;
    state.gravity = -10;
    state.elapsedTime = 1.0/30;
    return state;
}

void test_simpleFrame(void){
    GameState initialState = sampleState();
    Action action;
    action.type = ACTION_NONE;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.height, 4.9944);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.velocity, -0.3333);
}

void test_flap(void){
    GameState initialState = sampleState();
    initialState.height = 4.9944;
    initialState.velocity = -0.3333;
    Action action;
    action.type = ACTION_FLAP;
    action.params[0] = 6;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.height, 5.1888);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.velocity, 5.6666);
}

void test_upperCollision(void){
    GameState initialState = sampleState();
    initialState.height = 9.9800;
    initialState.velocity = 0;
    Action action;
    action.type = ACTION_FLAP;
    action.params[0] = 6;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.height, 10);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.velocity, 0);
}

void test_lowerCollision(void){
    GameState initialState = sampleState();
    initialState.height = 0.1;
    initialState.velocity = -5;
    Action action;
    action.type = ACTION_NONE;
    GameState newState = gameReducer(initialState, action);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.height, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, newState.velocity, 0);
}
