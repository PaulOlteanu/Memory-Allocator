#include <stdlib.h>

#define BYTES_IN_MEGABYTE 16777216

// Nodes for the linked linked list
// The weird `typedef`ing is to be able to use pointers to itself
typedef struct LLN linkedListNode;
struct LLN {
    linkedListNode *prev;
    linkedListNode *next;

    bool used = false;
};

// Doesn't need the weird `typedef` format due to not having pointers to its own type
typedef struct LL {
    linkedListNode *start;
    linkedListNode *end;
} linkedList;

int main(int argc, char const *argv[]) {
    void *customBeginning = malloc(BYTES_IN_MEGABYTE);

    free(customBeginning);
    return 0;
}
