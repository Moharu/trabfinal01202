#include "unity.h"
#include "GameReducer.h"
#include <string.h>


void setUp(void){}
void tearDown(void){}

void test_simpleSum(void){
    char result[50] = "\0";
    gameReducer("a", "b", result);
    TEST_ASSERT_EQUAL_INT(0, strcmp("ab", result));
}
