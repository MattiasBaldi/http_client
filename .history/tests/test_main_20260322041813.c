#include "unity/unity.h"
#include "../main.h"
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

// ---- ARGS PARSING TESTS ----
void test_parse_request_default_method(void) {
    char *argv[] = {"https://www.example.com/api"};
    int argc = 1;
    request http_request = {};
    int request_status = parse_request(argc, argv, &http_request);

    TEST_ASSERT_EQUAL_INT(0, request_status);
    TEST_ASSERT_EQUAL_STRING("GET", http_request.method);
    TEST_ASSERT_EQUAL_STRING("https://www.example.com/api", http_request.url);
}

void test_parse_request_method_post(void) {
    char *argv[] = {"-X", "POST", "https://www.example.com/api"};
    int argc = 3;
    request http_request = {};
    int request_status = parse_request(argc, argv, &http_request);

    TEST_ASSERT_EQUAL_INT(0, request_status);
    TEST_ASSERT_EQUAL_STRING("POST", http_request.method);
    TEST_ASSERT_EQUAL_STRING("https://www.example.com/api", http_request.url);
}

void test_parse_request_with_header(void) {
    char *argv[] = {"-H", "Content-Type: application/json", "https://www.example.com/api"};
    int argc = 3;
    request http_request = {};
    int request_status = parse_request(argc, argv, &http_request);

    TEST_ASSERT_EQUAL_INT(0, request_status);
    TEST_ASSERT_EQUAL_STRING("GET", http_request.method);
    TEST_ASSERT_EQUAL_INT(1, http_request.headers_count);
    TEST_ASSERT_EQUAL_STRING("Content-Type: application/json", http_request.headers[0]);
}

void test_parse_request_with_body(void) {
    char *argv[] = {"-X", "POST", "-d", "{\"key\":\"value\"}", "https://www.example.com/api"};
    int argc = 5;
    request http_request = {};
    int request_status = parse_request(argc, argv, &http_request);

    TEST_ASSERT_EQUAL_INT(0, request_status);
    TEST_ASSERT_EQUAL_STRING("POST", http_request.method);
    TEST_ASSERT_EQUAL_STRING("{\"key\":\"value\"}", http_request.body);
}

void test_parse_request_missing_url(void) {
    char *argv[] = {"-X", "POST"};
    int argc = 2;
    request http_request = {};
    int request_status = parse_request(argc, argv, &http_request);

    TEST_ASSERT_EQUAL_INT(1, request_status);
    TEST_ASSERT_EQUAL_STRING("URL required", http_request.err);
}

void test_parse_request_missing_flag_arg(void) {
    char *argv[] = {"-X"};
    int argc = 1;
    request http_request = {};
    int request_status = parse_request(argc, argv, &http_request);

    TEST_ASSERT_EQUAL_INT(1, request_status);
    TEST_ASSERT_EQUAL_STRING("-X requires an argument", http_request.err);
}


// ---- URL PARSING TESTS ----
void test_parse_url_http(void) {
    url parsed;
    int parse_status = parse_url("http://example.com/api", &parsed);
    TEST_ASSERT_EQUAL_INT(0, parse_status);
    TEST_ASSERT_EQUAL_STRING("http", parsed.protocol);
    TEST_ASSERT_EQUAL_STRING("example.com", parsed.host);
    TEST_ASSERT_EQUAL_STRING("80", parsed.port);
    TEST_ASSERT_EQUAL_STRING("/api", parsed.path);
}

void test_parse_url_https(void) {
    url parsed;
    int parse_status = parse_url("https://example.com/api", &parsed);
    TEST_ASSERT_EQUAL_INT(0, parse_status);
    TEST_ASSERT_EQUAL_STRING("https", parsed.protocol);
    TEST_ASSERT_EQUAL_STRING("example.com", parsed.host);
    TEST_ASSERT_EQUAL_STRING("443", parsed.port);
    TEST_ASSERT_EQUAL_STRING("/api", parsed.path);
}

void test_parse_url_invalid(void) {
    url parsed;
    int parse_status = parse_url("www.test.com", &parsed);
    TEST_ASSERT_EQUAL_INT(1, parse_status);
}

void test_parse_url_null(void) {
    url parsed;
    TEST_ASSERT_EQUAL_INT(1, parse_url(NULL, &parsed));
    TEST_ASSERT_EQUAL_INT(1, parse_url("http://example.com", NULL));
}

void test_parse_url_default_path(void) {
    url parsed;
    int parse_status = parse_url("http://example.com", &parsed);
    TEST_ASSERT_EQUAL_INT(0, parse_status);
    TEST_ASSERT_EQUAL_STRING("/", parsed.path);
}

// ---- TLS TESTS ----
void test_set_tls_invalid_fd(void) {
    // Passing an invalid socket fd should fail the TLS handshake and return NULL
    SSL *ssl = set_tls(-1);
    TEST_ASSERT_NULL(ssl);
}

void test_set_tls_closed_socket(void) {
    // A valid-looking but closed fd should also fail
    int fds[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    close(fds[0]);
    close(fds[1]);
    SSL *ssl = set_tls(fds[0]);
    TEST_ASSERT_NULL(ssl);
}

// ---- REQUEST TESTS ----
void test_send_request_format(void) {
    // Use socketpair to capture what send_request writes
    int fds[2];
    TEST_ASSERT_EQUAL_INT(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

    request req = { .method = "GET", .url = "http://example.com/index.html" };
    url url = { .host = "example.com", .path = "/index.html" };
    int result = send_request(fds[0], NULL, &req, &url);
    TEST_ASSERT_EQUAL_INT(0, result);

    // Read what was sent from the other end
    char buf[2048] = {0};
    ssize_t n = recv(fds[1], buf, sizeof(buf) - 1, 0);
    TEST_ASSERT_GREATER_THAN(0, n);
    buf[n] = '\0';

    // Verify request line
    TEST_ASSERT_NOT_NULL(strstr(buf, "GET /index.html HTTP/1.1\r\n"));
    // Verify Host header
    TEST_ASSERT_NOT_NULL(strstr(buf, "Host: example.com\r\n"));
    // Verify Connection: close
    TEST_ASSERT_NOT_NULL(strstr(buf, "Connection: close\r\n"));
    // Verify ends with double CRLF
    TEST_ASSERT_NOT_NULL(strstr(buf, "\r\n\r\n"));

    close(fds[0]);
    close(fds[1]);
}

void test_send_request_post(void) {
    int fds[2];
    TEST_ASSERT_EQUAL_INT(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

    request req = { .method = "POST", .url = "http://api.example.com/submit" };
    url url = { .host = "api.example.com", .path = "/submit" };
    int result = send_request(fds[0], NULL, &req, &url);
    TEST_ASSERT_EQUAL_INT(0, result);

    char buf[2048] = {0};
    ssize_t n = recv(fds[1], buf, sizeof(buf) - 1, 0);
    TEST_ASSERT_GREATER_THAN(0, n);
    buf[n] = '\0';

    TEST_ASSERT_NOT_NULL(strstr(buf, "POST /submit HTTP/1.1\r\n"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Host: api.example.com\r\n"));

    close(fds[0]);
    close(fds[1]);
}

void test_send_request_closed_socket(void) {
    // Sending on a closed socket should fail
    int fds[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    close(fds[0]);
    close(fds[1]);

    request req = { .method = "GET", .url = "http://example.com/" };
    url url = { .host = "example.com", .path = "/" };
    int result = send_request(fds[0], NULL, &req, &url);
    TEST_ASSERT_EQUAL_INT(1, result);
}

// ---- RESPONSE TESTS ----
void test_read_response_success(void) {
    int fds[2];
    TEST_ASSERT_EQUAL_INT(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

    // Write a mock HTTP response to one end
    const char *mock_response = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello";
    send(fds[1], mock_response, strlen(mock_response), 0);
    close(fds[1]); // Close writer so read_response sees EOF

    // Redirect stdout to /dev/null so test output stays clean
    FILE *orig_stdout = stdout;
    stdout = fopen("/dev/null", "w");

    int result = read_response(fds[0], NULL);

    fclose(stdout);
    stdout = orig_stdout;

    TEST_ASSERT_EQUAL_INT(0, result);

    close(fds[0]);
}

void test_read_response_empty(void) {
    int fds[2];
    TEST_ASSERT_EQUAL_INT(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

    // Close writer immediately — simulates empty response
    close(fds[1]);

    FILE *orig_stdout = stdout;
    stdout = fopen("/dev/null", "w");

    int result = read_response(fds[0], NULL);

    fclose(stdout);
    stdout = orig_stdout;

    TEST_ASSERT_EQUAL_INT(0, result);

    close(fds[0]);
}

int main(void) {
    UNITY_BEGIN();

    // ARGS parsing
    RUN_TEST(test_parse_request_default_method);
    RUN_TEST(test_parse_request_method_post);
    RUN_TEST(test_parse_request_with_header);
    RUN_TEST(test_parse_request_with_body);
    RUN_TEST(test_parse_request_missing_url);
    RUN_TEST(test_parse_request_missing_flag_arg); 


    // URL parsing
    RUN_TEST(test_parse_url_http);
    RUN_TEST(test_parse_url_https);
    RUN_TEST(test_parse_url_invalid);
    RUN_TEST(test_parse_url_null);
    RUN_TEST(test_parse_url_default_path);

    // TLS
    RUN_TEST(test_set_tls_invalid_fd);
    RUN_TEST(test_set_tls_closed_socket);

    // Request
    RUN_TEST(test_send_request_format);
    RUN_TEST(test_send_request_post);
    RUN_TEST(test_send_request_closed_socket);

    // Response
    RUN_TEST(test_read_response_success);
    RUN_TEST(test_read_response_empty);

    return UNITY_END();
}
