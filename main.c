#include <stdlib.h>
#include <stdbool.h>

#define BYTES_IN_MEGABYTE 16777216

// Nodes for the linked linked list
// The weird `typedef`ing is to be able to use pointers to itself
typedef struct LLN linkedListNode;
struct LLN {
    linkedListNode *prev;
    linkedListNode *next;

    void *start;
    void *end;
    bool used;

    bool errored;
};

// Doesn't need the weird `typedef` format due to not having pointers to its own type
typedef struct LL {
    linkedListNode *start;
    linkedListNode *end;
} linkedList;

void *malloc_(size_t bytes);

linkedListNode getmem_(size_t bytes);

// When implementing, remember to check for possible merges after freeing the memory
void free_(void *ptr);

// Should be global so that the functions can access it without using arguments
// Just to match the type signatures of the original versions
linkedList list;

int main(void) {
    void *memoryBank = malloc(BYTES_IN_MEGABYTE);

    linkedListNode first;

    first.start = memoryBank;
    first.end = memoryBank + 16777216;
    first.used = false;

    list.start = &first;

    free(memoryBank);
    return 0;
}

void *malloc_(size_t bytes) {
    linkedListNode match = getmem_(bytes);
    if (match.errored) {
        return NULL;
    }

    return NULL;
}

linkedListNode getmem_(size_t bytes) {
    linkedListNode check = *list.start;

    linkedListNode error;

    error.errored = true;

    while (true) {
        if (check.end - check.start >= bytes) {
            return check;
        } else {
            if (check.next) {
                check = *check.next;
            } else {
                return error;
            }
        }
    }
}
