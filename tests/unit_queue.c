/* unit_queue.c: Test SMQ Concurrent Queue of Requests (Unit) */

#include "smq/thread.h"
#include "smq/queue.h"
#include "smq/utils.h"

#include <assert.h>

/* Constants */

Request REQUESTS[] = {
    { "m0", "u0", "b0" },
    { "m1", "u1", "b1" },
    { "m2", "u2", "b2" },
    { "m3", "u3", "b3" },
    { "m4", "u4", "b4" },
    { NULL, NULL },
};

/* Functions */

int test_00_queue_create() {
    Queue *q = queue_create();
    assert(q);
    assert(q->head == NULL);
    assert(q->tail == NULL);
    assert(q->size == 0);

    free(q);
    return EXIT_SUCCESS;
}

int test_01_queue_push() {
    Queue *q = queue_create();
    assert(q);

    for (size_t r = 0; REQUESTS[r].url; r++) {
    	queue_push(q, &REQUESTS[r]);
    	assert(q->head == &REQUESTS[0]);
    	assert(q->size == r + 1);
    	assert(q->tail == &REQUESTS[r]);
    }

    free(q);
    return EXIT_SUCCESS;
}

int test_02_queue_pop() {
    Queue *q = queue_create();
    assert(q);

    for (size_t r = 0; REQUESTS[r].url; r++) {
    	queue_push(q, &REQUESTS[r]);
    	assert(q->head == &REQUESTS[0]);
    	assert(q->size == r + 1);
    	assert(q->tail == &REQUESTS[r]);
    }

    for (size_t r = 0; REQUESTS[r].url; r++) {
    	assert(queue_pop(q, 1000) == &REQUESTS[r]);
    }

    free(q);
    return EXIT_SUCCESS;
}

int test_03_queue_delete() {
    Queue *q = queue_create();
    assert(q);

    for (size_t r = 0; REQUESTS[r].url; r++) {
    	Request *n = request_create(
    	    REQUESTS[r].method,
    	    REQUESTS[r].url,
    	    REQUESTS[r].body
    	);

    	queue_push(q, n);
    	assert(q->size == r + 1);
    	assert(q->tail == n);
    }

    queue_delete(q);
    return EXIT_SUCCESS;
}

int test_04_queue_shutdown() {
    Queue *q = queue_create();
    assert(q);

    assert(q->running);
    queue_shutdown(q);
    assert(!q->running);

    assert(q->size == 0);
    queue_push(q, &REQUESTS[0]);
    assert(q->size == 0);

    assert(queue_pop(q, 1000) == NULL);
    assert(q->size == 0);

    queue_delete(q);
    return EXIT_SUCCESS;
}

/* Main execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0. Test queue_create\n");
        fprintf(stderr, "    1. Test queue_push\n");
        fprintf(stderr, "    2. Test queue_pop\n");
        fprintf(stderr, "    3. Test queue_delete\n");
        fprintf(stderr, "    4. Test queue_shutdown\n");
        return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_queue_create(); break;
        case 1:  status = test_01_queue_push(); break;
        case 2:  status = test_02_queue_pop(); break;
        case 3:  status = test_03_queue_delete(); break;
        case 4:  status = test_04_queue_shutdown(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }

    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
