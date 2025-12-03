#ifndef INCLUDE_LINKEDLIST_MOD_H_
#define INCLUDE_LINKEDLIST_MOD_H_

#include "stdlib/mod.h"
#include "types/mod.h"

typedef struct GSHL_LinkedListInfo {
    struct {
        usize size;
        usize offset;
    } node;
    struct {
        usize size;
        usize offset;
    } data;
} GSHL_LinkedListInfo;

#define GSHL_LinkedListDecl(NAME, TYPE)                                        \
    struct NAME {                                                              \
        struct NAME##_Node {                                                   \
            union {                                                            \
                TYPE data;                                                     \
                TYPE item;                                                     \
                TYPE value;                                                    \
            };                                                                 \
            struct NAME##_Node *next, *prev;                                   \
        } *head, *tail;                                                        \
    }

// Add an element to the end of the list
#define GSHL_LinkedList_push_back(LL, ITEM)                                    \
    do {                                                                       \
        void *_new_node = malloc(sizeof(*(LL)->head));                         \
        GSHL_ASSERT(_new_node != NULL);                                        \
                                                                               \
        if ((LL)->tail != NULL) {                                              \
            (LL)->tail->next = _new_node;                                      \
            (LL)->tail->next->data = ITEM;                                     \
            (LL)->tail->next->next = NULL;                                     \
            (LL)->tail->next->prev = (LL)->tail;                               \
        }                                                                      \
        else {                                                                 \
            (LL)->head = _new_node;                                            \
            GSHL_ASSERT((LL)->head != NULL);                                   \
            (LL)->head->data = ITEM;                                           \
            (LL)->head->next = NULL;                                           \
            (LL)->head->prev = (LL)->tail;                                     \
        }                                                                      \
        (LL)->tail = _new_node;                                                \
    } while (0)

// Add an element to the front of the list
#define GSHL_LinkedList_push_front(LL, ITEM)                                   \
    do {                                                                       \
        void *_new_node = malloc(sizeof(*(LL)->head));                         \
        GSHL_ASSERT(_new_node != NULL);                                        \
                                                                               \
        if ((LL)->head != NULL) {                                              \
            (LL)->head->prev = _new_node;                                      \
            (LL)->head->prev->data = ITEM;                                     \
            (LL)->head->prev->next = (LL)->head;                               \
            (LL)->head->prev->prev = NULL;                                     \
        }                                                                      \
        else {                                                                 \
            (LL)->tail = _new_node;                                            \
            (LL)->tail->data = ITEM;                                           \
            (LL)->tail->next = (LL)->head;                                     \
            (LL)->tail->prev = NULL;                                           \
        }                                                                      \
        (LL)->head = _new_node;                                                \
    } while (0)

#define GSHL_LinkedList_destroy(LL)                                            \
    do {                                                                       \
        for (__auto_type *current = (LL)->head; current != NULL;) {            \
            __auto_type *next_node = current->next;                            \
            free(current);                                                     \
            current = next_node;                                               \
        }                                                                      \
    } while (0)

#define GSHL_LinkedList_print(LL, ELEMENT_FORMAT)                              \
    do {                                                                       \
        GSHL_FormatString _string = {};                                        \
        for (__auto_type *current = (LL).head; current != NULL;                \
             current = current->next) {                                        \
            GSHL_format_write(&_string, ELEMENT_FORMAT, current->data);        \
            if (current->next != NULL) {                                       \
                GSHL_format_write(&_string, " -> ");                           \
            }                                                                  \
        }                                                                      \
        GSHL_format_write(&_string, "\n");                                     \
        assert(write(STDOUT_FILENO, _string.items, _string.count) > 0);        \
    } while (0)

#ifdef GSHL_STRIP_PREFIX
#    define LinkedListDecl GSHL_LinkedListDecl
#    define LinkedList_push_back GSHL_LinkedList_push_back
#    define LinkedList_push_front GSHL_LinkedList_push_front
#    define LinkedList_destroy GSHL_LinkedList_destroy
#    define LinkedList_print GSHL_LinkedList_print
#endif

#endif // INCLUDE_LINKEDLIST_MOD_H_
