#include <stdio.h>
#include "lib1.h"

int main() {
    int choice;
    while (1) {
        printf("Введите команду: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int A, B;
            scanf("%d %d", &A, &B);
            printf("Количество простых чисел: %d\n", PrimeCount(A, B));
        } else if (choice == 2) {
            float A, B;
            scanf("%f %f", &A, &B);
            printf("Площадь: %f\n", Square(A, B));
        } else {
            break;
        }
    }
    return 0;
}
