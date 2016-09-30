#include "unity.h"
#include "Sum.h"

void setUp(void){}
void tearDown(void){}

void test_simpleSum(void){
    TEST_ASSERT_EQUAL_INT(10, Sum(4,6));
}
