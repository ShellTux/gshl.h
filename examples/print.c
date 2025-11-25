#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

struct Foo {
    i32 bar;
    char *baz;
};

usize write_struct_Foo(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    const struct Foo *foo = fs->value.pointer;

    return GSHL_format_write(string,
                             "struct Foo {{\n"
                             "  .bar = %i,\n"
                             "  .baz = \"%s\",\n"
                             "}}",
                             foo->bar, foo->baz);
}

enum Day {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
};

usize write_enum_Day(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    const enum Day day = fs->value.i32;

    switch (day) {
    case Monday:
        return GSHL_format_write(string, "Monday");
    case Tuesday:
        return GSHL_format_write(string, "Tuesday");
    case Wednesday:
        return GSHL_format_write(string, "Wednesday");
    case Thursday:
        return GSHL_format_write(string, "Thursday");
    case Friday:
        return GSHL_format_write(string, "Friday");
    case Saturday:
        return GSHL_format_write(string, "Saturday");
    case Sunday:
        return GSHL_format_write(string, "Sunday");
    }

    return GSHL_format_write(string, "");
}

int main(void)
{
    if (!GSHL_format_specifier_register((FormatSpecifier){
            .kind = GSHL_FORMAT_SPECIFIER_POINTER,
            .write = write_struct_Foo,
            .va_size = sizeof(struct Foo *),
            .opts = {},
            .specifiers = {"struct Foo", "Foo"},
        })) {
        dprint(STDERR_FILENO, "Failed to register %s", "struct Foo");
    }

    if (!GSHL_format_specifier_register((FormatSpecifier){
            .kind = GSHL_FORMAT_SPECIFIER_I32,
            .write = write_enum_Day,
            .va_size = sizeof(enum Day),
            .opts = {},
            .specifiers = {"enum Day", "Day"},
        })) {
        dprintln(STDERR_FILENO, "Failed to register %s", "enum Day");
    }

    print_registered_format_specifiers();

    for (usize number = 0; number <= 10; number += 1) {
        println("{usize}! = %lu", number, factorial(number));
    }

    {
        const struct Foo foo = {
            .bar = 69,
            .baz = "Hello world!",
        };

        println("{Foo}", &foo);
        println("foo = {struct Foo}", &foo);
    }

    {
        const enum Day wed = Wednesday;
        println("day: {enum Day}\n", wed);
    }

    println("NULL = {pointer}", NULL);

    int var;
    println("&var = %p", &var);

    println("{string} %s", "Hello", "world!");

    return 0;
}
