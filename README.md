# GSHL

GSHL is a single header library to easily port this library to any
project.

Just copy and paste a single file, in stb-style.

## Examples

### Printing

Print to the console or to a file like printf you can write custom
format specifiers

./examples/print.c:

``` c
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
```

### Hash Table

./examples/hash-table.c:

``` c
#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#undef GSHL_TESTS
#include "../gshl.h"

static usize hash_djb2_string(const union GSHL_HashTableKey key)
{
    return hash_djb2(key.string);
}

usize hash_isize(const GSHL_HashTableKey key)
{
    const isize value = key.isize;

    // Use bit manipulation and modulus to ensure it's within the table size
    return (usize)(value ^ (value >> (sizeof(isize) * 8 - 1)));
}

static isize keycmp_string(const HashTableKey key1, const HashTableKey key2)
{
    return strcmp(key1.string, key2.string);
}

int main(void)
{
    {
        HashTable ht = {};
        // NOTE: When key type is not a primitive type (ex: a pointer to a null
        // terminated string) we need to provide a function to do key
        // comparison. Otherwise a default comparison function will be used to
        // compare keys
        HashTable_init(&ht, char *, i32, hash_djb2_string,
                       .keycmp = keycmp_string);

        HashTable_insert(&ht, .string = "foo", .i32 = 35);
        HashTable_insert(&ht, .string = "bar", .i32 = 34);
        HashTable_insert(&ht, .string = "baz", .i32 = 69);

        // NOTE: 2nd and 4th arguments are printf-like format strings to print
        // key and value respectively. And the 3rd and 5th arguments are the
        // names available in the `union GSHL_HashTableValue`, feel free to
        // check all of it's possible values.
        // It's up to the user to ensure the correct names
        HashTable_print(ht, "\"%s\"", string, "%i", i32);

        const char keys[][10] = {"baz", "asdf", "foo"};
        ARRAY_FOREACH(keys, char *key)
        {
            // NOTE: Here the hash table is passed by pointer to avoid copying
            // it's entire struct, but expect HashTable_search to not modify
            // it's internal state.
            const HashTableValue *value = HashTable_search(&ht, .string = key);
            if (value) {
                printf("ht[\"%s\"] = Some(%i)\n", key, value->i32);
            }
            else {
                printf("ht[\"%s\"] = None\n", key);
            }
        }

        if (!HashTable_destroy(&ht)) {
            return 1;
        }
    }

    srand(0);

    {
        HashTable triple = {};
        HashTable_init(&triple, isize, isize, hash_isize);

        for (usize i = 0; i < 50; ++i) {
            const isize key = rand();
            HashTable_insert(&triple, .isize = key, .isize = key * 3);
        }
        HashTable_insert(&triple, .isize = 23, .isize = 23 * 3);

        HashTable_print(triple, "%li", isize, "%li", isize);

        const isize keys[] = {1059961393, 569, 23, 1914544919};
        ARRAY_FOREACH(keys, isize key)
        {
            const HashTableValue *value =
                HashTable_search(&triple, .isize = key);
            if (value) {
                printf("%li * 3 = %li\n", key, value->isize);
            }
            else {
                printf("%li * 3 = ?\n", key);
            }
        }

        HashTable_destroy(&triple);
    }

    return 0;
}
```

### Logging

./examples/log.c:

``` c
#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

#include <fcntl.h>

int main(void)
{
    // NOTE: GSHL_LOG environment variable can be used to provide the verbosity
    // level:
    // GSHL_LOG=debug,info ./program
    log_init(.fd = STDERR_FILENO, .mask = INFO | WARNING, .print.file = true,
             // O_WRONLY: Open for writing only
             // O_CREAT: Create the file if doesn't exist
             // O_TRUNC: Truncate the file (Overwrite existing content)
             // O_APPEND: Append to the file
             .file_fd =
                 open("example.log", O_WRONLY | O_CREAT | O_TRUNC, 0644));
    log_print_config();

    log(DEBUG, "debug message");
    log(INFO, "%i %i", 6, 7);
    log(WARNING, "%i + %i = %i", 34, 35, 34 + 35);
    log(ERROR, "%s", "error message");

    return 0;
}
```

### Tagged Union

If you want rust enums in c look at `./examples/tagged-union.c`:

``` c
#define GSHL_STRIP_PREFIX
#define GSHL_IMPLEMENTATION
#include "../gshl.h"

typedef struct Circle {
    u64 radius;
} Circle;

typedef struct Rectangle {
    u64 width, height;
} Rectangle;

typedef struct Triangle {
    u64 base, height;
} Triangle;

typedef TaggedUnionTypeDecl(Shape, Circle, Rectangle, Triangle) Shape;

static usize write_Shape(GSHL_FormatString *string, const FormatSpecifier *fs)
{
    const Shape *shape = fs->value.pointer;

    match(
        *shape, Circle,
        {
            return format_write(string, "Shape(Circle {{ .radius = %lu }})",
                                asCircle->radius);
        },
        Rectangle,
        {
            return format_write(
                string, "Shape(Rectangle {{ .width = %lu, .height = %lu }})",
                asRectangle->width, asRectangle->height);
        },
        Triangle,
        {
            return format_write(
                string, "Shape(Triangle {{ .base = %lu, .height = %lu }})",
                asTriangle->base, asTriangle->height);
        });
}

int main(void)
{
    if (!format_specifier_register((FormatSpecifier){
            .kind = GSHL_FORMAT_SPECIFIER_POINTER,
            .va_size = sizeof(Shape *),
            .write = write_Shape,
            .specifiers = {"Shape"},
        })) {
        dprintln(STDERR_FILENO, "Failed to register Shape");
    }

    static const Shape shapes[] = {
        TaggedUnionValue(Circle, .radius = 50),
        TaggedUnionValue(Rectangle, .width = 30, .height = 50),
        TaggedUnionValue(Triangle, .base = 10, .height = 34),
        // Or short hand
        TUValue(Circle, .radius = 50),
        TUValue(Rectangle, .width = 30, .height = 50),
        TUValue(Triangle, .base = 10, .height = 34),
    };

    ARRAY_FOREACH(shapes, Shape shape) { println("shape = {Shape}", &shape); }

    return 0;
}
```
