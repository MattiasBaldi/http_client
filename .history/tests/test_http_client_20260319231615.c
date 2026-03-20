#include "unity/unity.h"
#include "../main.h"

void setUp(void) {
    // Setup code before each test
    printf("test"); 
}

void tearDown(void) {
    // Cleanup code after each test
}

void test_dummy_example(void) {
    TEST_ASSERT_EQUAL_INT(1, 1);
}

// TODO: Add your tests here
void test_parse_url(void) {
    
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_dummy_example);
    // RUN_TEST(test_url_parsing_hostname);
    return UNITY_END();
}
