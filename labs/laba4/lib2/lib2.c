#include "lib2.h"
#include <stdlib.h>
#include <math.h>

int PrimeCount(int A, int B) {
    if ((A > B) || (A < 0) || (B < 0)) {
        exit(EXIT_FAILURE);
    }

    if (B == 1) {
        return 0;
    }

    int *sieve = (int *)malloc((B + 1) * sizeof(int));
    for (int i = 0; i <= B; i++) {
        sieve[i] = 1;
    }
    sieve[0] = 0;
    sieve[1] = 0;

    for (int i = 2; i * i <= B; ++i) {
        if (sieve[i] == 1) {
            for (int j = i * i; j <= B; j += i) {
                sieve[j] = 0;
            }
        }
    }

    int counter = 0;
    for (int i = A; i <= B; ++i) {
        if (sieve[i] == 1) {
            counter++;
        }
    }

    free(sieve);
    return counter;
}

float Square(float A, float B) {
    return A * B / 2;
}
