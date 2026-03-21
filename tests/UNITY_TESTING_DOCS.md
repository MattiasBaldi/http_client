# Unity Testing Framework

Unity is a lightweight unit testing framework for C. It provides macros to verify that your code behaves as expected.

## Basic Structure

```c
#include "unity/unity.h"

void test_something(void) {
    // Arrange
    int result = add(2, 3);

    // Act & Assert
    TEST_ASSERT_EQUAL_INT(5, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_something);
    return UNITY_END();
}
```

## Test Lifecycle

**setUp()** - Runs before each test:
```c
void setUp(void) {
    // Initialize test fixtures, allocate memory, etc.
}
```

**Test function** - Your actual test:
```c
void test_example(void) {
    // Verify behavior
}
```

**tearDown()** - Runs after each test:
```c
void tearDown(void) {
    // Clean up, free memory, etc.
}
```

## Common Assertions

### Integer Assertions
```c
TEST_ASSERT_EQUAL_INT(expected, actual);
TEST_ASSERT(condition);                    // True/false
```

### String Assertions
```c
TEST_ASSERT_EQUAL_STRING("hello", result);
TEST_ASSERT_NULL(pointer);
TEST_ASSERT_NOT_NULL(pointer);
```

### Range Assertions
```c
TEST_ASSERT_INT_WITHIN(delta, expected, actual);
```

### Floating Point
```c
TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual);
```

## Example: Testing parse_url()

```c
void test_parse_url_basic(void) {
    parsed_url result = parse_url("https://example.com/api");

    TEST_ASSERT_NOT_NULL(result.protocol);
    TEST_ASSERT_EQUAL_STRING("https", result.protocol);
    TEST_ASSERT_EQUAL_STRING("example.com", result.host);
    TEST_ASSERT_EQUAL_INT(443, result.port);
    TEST_ASSERT_EQUAL_STRING("/api", result.path);
}

void test_parse_url_with_port(void) {
    parsed_url result = parse_url("http://localhost:8080/");

    TEST_ASSERT_EQUAL_STRING("http", result.protocol);
    TEST_ASSERT_EQUAL_STRING("localhost", result.host);
    TEST_ASSERT_EQUAL_INT(8080, result.port);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parse_url_basic);
    RUN_TEST(test_parse_url_with_port);
    return UNITY_END();
}
```

## Running Tests

```bash
make              # Build and run all tests
make clean        # Remove build artifacts
make run-tests    # Run compiled tests without rebuilding
```

## Test Output

```
test_main.c:23:test_dummy_example:PASS
test_main.c:24:test_parse_url:PASS

-----------------------
2 Tests 0 Failures 0 Ignored
OK
```

## Key Rules

1. **Test function names** must start with `test_`
2. **One assertion per concept** - test one behavior per assertion
3. **Arrange-Act-Assert pattern** - set up, execute, verify
4. **Fail fast** - assert early, don't continue after failure
5. **No printf()** for assertions - use TEST_ASSERT_* macros

## When a Test Fails

```
test_main.c:18:test_parse_url:FAIL
Expected 443
Actual 80
```

The framework tells you:
- Which file and line failed
- What test failed
- What was expected vs actual
