#include <stdio.h>
#include <dlfcn.h>

typedef int (*PrimeCountFunc)(int, int);
typedef float (*SquareFunc)(float, float);

int main() {
    void *lib_handle = dlopen("./lib1/lib1.so", RTLD_LAZY);
    if (!lib_handle) {
        fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
        return 1;
    }

    PrimeCountFunc PrimeCount = (PrimeCountFunc)dlsym(lib_handle, "PrimeCount");
    SquareFunc Square = (SquareFunc)dlsym(lib_handle, "Square");

    int choice;
    while (1) {
        printf("Введите команду: ");
        scanf("%d", &choice);
        if (choice == 0) {
            dlclose(lib_handle);
            lib_handle = dlopen("./lib2/lib2.so", RTLD_LAZY);
            if (!lib_handle) {
                fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
                return 1;
            }
            PrimeCount = (PrimeCountFunc)dlsym(lib_handle, "PrimeCount");
            Square = (SquareFunc)dlsym(lib_handle, "Square");
        } else if (choice == 1) {
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

    dlclose(lib_handle);
    return 0;
}
