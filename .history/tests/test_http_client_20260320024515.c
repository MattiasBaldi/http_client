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
void test_parse_url_http(void) {
    parsed_url parsed;
    int parse_status = parse_url("http://example.com/api", &parsed);
    TEST_ASSERT_EQUAL_INT(0, parse_status);
    
    printf("%s\n", parsed.host); 

    TEST_ASSERT_EQUAL_STRING("http", parsed.protocol); 
    TEST_ASSERT_EQUAL_STRING("example.com", parsed.host);
    TEST_ASSERT_EQUAL_STRING("80", parsed.port);
    TEST_ASSERT_EQUAL_STRING("/api", parsed.path);   
}
 
void test_parse_url_https(void) {
    parsed_url parsed;
    int parse_status = parse_url("https://example.com/api", &parsed);
    TEST_ASSERT_EQUAL_INT(0, parse_status);
    TEST_ASSERT_EQUAL_STRING("https", parsed.protocol); 
    TEST_ASSERT_EQUAL_STRING("example.com", parsed.host);
    TEST_ASSERT_EQUAL_STRING("443", parsed.port); 
    TEST_ASSERT_EQUAL_STRING("/api", parsed.path); 
}

void test_parse_url_invalid(void) {
    parsed_url parsed;
    int parse_status = parse_url("www.test.com", &parsed);
    printf("TEST"); 
    TEST_ASSERT_EQUAL_INT(1, parse_status);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parse_url_http);
    RUN_TEST(test_parse_url_https);
    RUN_TEST(test_parse_url_invalid);
    return UNITY_END();
}
