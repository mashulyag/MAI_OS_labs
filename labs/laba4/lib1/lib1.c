#include "lib1.h"
#include <stdbool.h>
#include <stdlib.h>

int PrimeCount(int A, int B) {
    int prime_count = 0;

    if ((A > B) || (A < 0) || (B < 0)) {
        exit(EXIT_FAILURE);
    }

    bool flag = false;
    if (A == 1) {
        flag = true;
    }

    for (int i = A; i <= B; ++i) {
        int counter = 0;
        for (int j = 2; j < i; ++j) {
            if (i % j == 0) {
                counter++;
                break;
            }
        }

        if (counter == 0) {
            prime_count++;
        }
    }

    if (flag) {
        prime_count--;
    }

    return prime_count;
}

float Square(float A, float B) {
    return A * B;
}
