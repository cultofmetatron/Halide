#include "Halide.h"

// This test defines a reduction that writes to a large area, reads
// from an even larger area, and then just realizes it over a smaller
// area

using namespace Halide;

// Custom error handler. If we don't define this, it'll just print out
// an error message and quit
bool error_occurred = false;
void halide_error(char *msg) {
    printf("%s\n", msg);
    error_occurred = true;
}

extern "C" void set_error_handler(void (*)(char *));

int main(int argc, char **argv) {
    Var x;
    Func f;
    RDom r(0, 20);
    f(x) = x;
    f(r) = f(r-1) + f(r+1);

    f.setErrorHandler(&halide_error);
    Image<int> result = f.realize(10);

    if (!error_occurred) {
        printf("There should have been an out-of-bounds error\n");
        return 1;
    }

    printf("Success!\n");
    return 0;
}
