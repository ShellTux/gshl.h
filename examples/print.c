#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

enum Day {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
};

struct Foo {
    i32 bar;
    char *baz;
    enum Day day;
};

usize write_struct_Foo(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    const struct Foo *foo = fs->value.pointer;

    return GSHL_format_write(string,
                             "struct Foo {{\n"
                             "  .bar = %i,\n"
                             "  .baz = \"%s\",\n"
                             "  .day = {enum Day}\n"
                             "}}",
                             foo->bar, foo->baz, foo->day);
}

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

FORMAT_SPECIFIER_REGISTER(Foo, .kind = GSHL_FORMAT_SPECIFIER_POINTER,
                          .write = write_struct_Foo,
                          .va_size = sizeof(struct Foo *),
                          .specifiers = {"struct Foo", "Foo"});

FORMAT_SPECIFIER_REGISTER(Day, .kind = GSHL_FORMAT_SPECIFIER_I32,
                          .va_size = sizeof(enum Day), .write = write_enum_Day,
                          .specifiers = {"enum Day", "Day"});

int main(void)
{
    // You can register formats mannually
    // GSHL_format_specifier_register_Foo();
    // GSHL_format_specifier_register_Day();

    format_specifiers_print();

    for (usize number = 0; number <= 10; number += 1) {
        println("{usize}! = %lu", number, factorial(number));
    }

    {
        const struct Foo foo = {
            .bar = 69,
            .baz = "Hello world!",
            .day = Friday,
        };

        println("{Foo}", &foo);
        println("foo = {struct Foo}", &foo);
    }

    println("NULL = {pointer}", NULL);

    int var;
    println("&var = %p %u", &var, -34);

    println("{string} %s", "Hello", "world!");

    return 0;
}
