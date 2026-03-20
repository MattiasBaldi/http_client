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
    //parsed_url parsed = parse_url("https://test.com"); 
    //TEST_ASSERT(parsed.protocol == "http" || parsed.protocol == "https"); 

    // failed url 
    //parsed_url fail_parse = parse_url("www.test.com"); 
}
 
void test_parse_url_invalid(void) {
    parsed_url parsed;
    int err = parse_url("www.test.com", &parsed);
    TEST_ASSERT_EQUAL_INT(10, err);
}

int main(void) {
    UNITY_BEGIN();
    // RUN_TEST(test_parse_url);
    //RUN_TEST(test_parse_url_invalid);
    return UNITY_END();
}
