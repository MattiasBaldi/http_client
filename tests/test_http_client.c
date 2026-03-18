#include "../src/unity/unity.h"

void setUp(void) {
    // Setup code before each test
}

void tearDown(void) {
    // Cleanup code after each test
}

void test_dummy_example(void) {
    TEST_ASSERT_EQUAL_INT(1, 1);
}

// TODO: Add your tests here
// void test_url_parsing_hostname(void) {
//     // char *hostname = parse_hostname("https://example.com/path");
//     // TEST_ASSERT_EQUAL_STRING("example.com", hostname);
// }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_dummy_example);
    // RUN_TEST(test_url_parsing_hostname);
    return UNITY_END();
}
