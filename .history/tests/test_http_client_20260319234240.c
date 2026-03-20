#include "unity/unity.h"
#include "../main.h"

void setUp(void) {
    // Setup code before each test
    // printf("setup\n");
    // fflush(stdout); 
}

void tearDown(void) {
    // Cleanup code after each test
    // printf("tear down\n");
    // fflush(stdout); 
}

// TODO: Add your tests here
void test_parse_url(void) {
    parsed_url parsed = parse_url("https://test.com"); 
    TEST_ASSERT(parsed.protocol == "http" || parsed.protocol == "https"); 
}
 
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parse_url);
    return UNITY_END();
}
