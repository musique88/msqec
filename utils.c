#include "utils.h"

uint num_places (uint n) {
    uint r = 1;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}