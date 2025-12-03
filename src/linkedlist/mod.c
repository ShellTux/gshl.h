#include "linkedlist/mod.h"

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(linkedlist)
{
    typedef GSHL_LinkedListDecl(Grades, i32) Grades;
    Grades grades = {};

    GSHL_TEST_EQUAL(grades.head, NULL);
    GSHL_TEST_EQUAL(grades.tail, NULL);

    GSHL_LinkedList_push_back(&grades, 5);
    GSHL_LinkedList_push_back(&grades, 4);
    GSHL_LinkedList_push_back(&grades, 9);
    GSHL_LinkedList_push_back(&grades, 1);

    GSHL_TEST_EQUAL(grades.head->item, 5);
    GSHL_TEST_EQUAL(grades.tail->item, 1);

    GSHL_LinkedList_destroy(&grades);
}

#endif
