#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define BYTES_IN_MEGABYTE 1*1024*1024

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
    first->end = memoryBank + BYTES_IN_MEGABYTE;
    first->used = false;

    list.start = first;
    list.end = first;

    // Test code
    void *mem1 = malloc_(6);
    void *mem2 = malloc_(20);
    void *mem3 = malloc_(40);

    free_(mem2);
    free_(mem1);
    free_(mem3);

    // Cleanup code in case there aren't sufficient `free_` calls for the number of `malloc_` calls
    // It's like memory leak-ception

    // If it is done properly however, list.start should equal list.end
    // This means that if done properly, all that should be necessary is a call to free(list.start)
    linkedListNode *temp = list.start;
    while (true) {
        if (temp->next != NULL) {
            linkedListNode *temp2 = temp->next;
            free(temp);
            temp = temp2;
        } else {
            free(temp);
            break;
        }
    }
    free(first);
    free(memoryBank);
    return 0;
}

linkedListNode *getmem_(size_t bytes) {
    linkedListNode *check = list.start;

    // Loop through linked list from the start till it finds an open chunk large enough
    while (true) {
        if (!check->used && check->end - check->start >= (ptrdiff_t) bytes) {
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

    next->end = match->end;

    match->end = match->start + bytes;

    next->start = match->end;
    next->used = false;

    if(list.end == match) {
        list.end = next;
    }
    return (void *) match->start;
}

// Not sure if this is implemented correctly
// Just loops though all blocks till it finds one where the start equals `ptr`
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

    // At least if it can't be joined with another block of memory, it'll be marked as free
    check->used = false;

    // Check if the memory block after it is also not used
    // If it's free, join them
    if (check->next != NULL && check->next->used == false) {
        check->end = check->next->end;
        if (list.end == check->next) {
            list.end = check;
        }
        if (check->next->next != NULL) {
            linkedListNode *temp = check->next->next;
            free(check->next);
            check->next = temp;
            check->next->prev = check;
        }
    }

    // Check if the memory block before it is also not used
    // If it's free, join them
    if (check->prev != NULL && check->prev->used == false) {
        check->start = check->prev->start;
        if (list.start == check->prev) {
            list.start = check;
        }
        if (check->prev->prev != NULL) {
            linkedListNode *temp = check->prev->prev;
            free(check->prev);
            check->prev = temp;
            check->prev->next = check;
        }
    }
}
