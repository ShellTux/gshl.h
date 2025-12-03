#define GSHL_STRIP_PREFIX
#define GSHL_IMPLEMENTATION
#include "../gshl.h"

typedef struct Grade {
    u32 grade;
} Grade;

// How to print person using GSHL_println
usize write_Grade(FormatString *string, const FormatSpecifier *fs)
{
    const Grade grade = *(Grade *)&fs->value.u32;

    return format_write(string, "Grade {{ %u }}", grade.grade);
}

FORMAT_SPECIFIER_REGISTER(Grade, .kind = GSHL_FORMAT_SPECIFIER_U32,
                          .va_size = sizeof(Grade), .write = write_Grade,
                          .specifiers = {"Grade"});

typedef LinkedListDecl(Grades, Grade) Grades;

int main(void)
{
    srand(0);

    {
        Grades grades = {};

        LinkedList_push_back(&grades, (Grade){1});
        LinkedList_push_back(&grades, (Grade){2});
        LinkedList_push_back(&grades, (Grade){3});
        LinkedList_push_back(&grades, (Grade){4});

        LinkedList_push_front(&grades, (Grade){5});

        LinkedList_print(grades, "{Grade}");

        LinkedList_destroy(&grades);
    }

    return 0;
}
