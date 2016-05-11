#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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
};

// Doesn't need the weird `typedef` format due to not having pointers to its own type
typedef struct LL {
    linkedListNode *start;
    linkedListNode *end;
} linkedList;

linkedListNode *getmem_(size_t bytes);

void *malloc_(size_t bytes);

void free_(void *ptr);

// Should be global so that the functions can access it without using arguments
// Just to match the type signatures of the original versions
linkedList list;

int main(void) {
    void *memoryBank = malloc(BYTES_IN_MEGABYTE);

    linkedListNode *first = (linkedListNode *) malloc(sizeof *first);

    first->prev = NULL;
    first->next = NULL;
    first->start = memoryBank;
    first->end = memoryBank + 16777216;
    first->used = false;

    list.start = first;
    list.end = first;

    // Test code
    void *temp = malloc_(6);
    free_(temp);

    free(first);
    free(memoryBank);
    return 0;
}

linkedListNode *getmem_(size_t bytes) {
    linkedListNode *check = list.start;

    // Loop through linked list from the start till it finds an open chunk large enough
    while (true) {
        if (check->end - check->start >= bytes) {
            return check;
        } else {
            if (check->next) {
                check = check->next;
            } else {
                return NULL;
            }
        }
    }
}

void *malloc_(size_t bytes) {
    linkedListNode *match = getmem_(bytes);
    if (match == NULL) {
        return NULL;
    }

    match->used = true;

    linkedListNode *next = (linkedListNode *) malloc(sizeof *next);
    next->prev = match;
    next->next = match->next;

    match->next = next;

    next->start = match->end;

    match->end = match->start + bytes;

    next->end = next->start + bytes;

    next->used = false;

    if(list.end == match) {
        list.end = next;
    }
    return (void *) match->start;
}

void free_(void *ptr) {
    linkedListNode *check = list.start;

    while (true) {
        if (check->start == ptr) {
            break;
        } else {
            if (check->next) {
                check = check->next;
            } else {
                check = NULL;
            }
        }
    }

    if (check == NULL) {
        return;
    }

    check->used = false;

    if (check->used == false && check->next != NULL && check->next->used == false) {
        check->end = check->next->end;
        if (list.end == check->next) {
            list.end = check;
        }
        linkedListNode *temp = check->next->next;
        free(check->next);
        check->next = temp;
    }
}
