#include <stdio.h>
#include <pthread.h> // pthread_create, pthread_join, pthread_exit
#include <stdlib.h>  // atoi()
#include <time.h>

#define ARRAY_SIZE 100000000

int thread_count = 0;
int array[ARRAY_SIZE];
int min_result[ARRAY_SIZE];
int max_result[ARRAY_SIZE];

void* find_min_max(void* thread_id) {
    int id = *(int*)thread_id;
    int start = id * (ARRAY_SIZE / thread_count);
    int end = (id + 1) * (ARRAY_SIZE / thread_count);
    if (id == thread_count - 1) {
        end = ARRAY_SIZE; // Обработка остатка
    }
    int min = array[start];
    int max = array[start];

    for (int i = start; i < end; i++) {
        if (array[i] < min) min = array[i];
        if (array[i] > max) max = array[i];
    }

    min_result[id] = min;
    max_result[id] = max;

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: At least one argument is required.\n");
        return 1;
    }

    thread_count = atoi(argv[1]);
    if (thread_count <= 0 || thread_count > ARRAY_SIZE) {
        printf("Error: Invalid number of threads.\n");
        return 1;
    }

    // Заполнение массива случайными значениями
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand();
    }

    pthread_t threads[thread_count];
    int thread_ids[thread_count];

    clock_t start = clock();

    for (int i = 0; i < thread_count; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, find_min_max, (void*)&thread_ids[i]) != 0) {
            printf("Error: Failed to create thread %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    int min = min_result[0];
    int max = max_result[0];

    for (int i = 1; i < thread_count; i++) {
        if (min_result[i] < min) min = min_result[i];
        if (max_result[i] > max) max = max_result[i];
    }

    clock_t end = clock();

    double time = (double)(end - start) / ((double) CLOCKS_PER_SEC);

    printf("Количество потоков: %d\n", thread_count);
    printf("Время выполнения: %.5f сек\n", time);
    printf("Минимальный элемент: %d\n", min);
    printf("Максимальный элемент: %d\n", max);

    return 0;
}
