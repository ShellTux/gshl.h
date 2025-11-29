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
