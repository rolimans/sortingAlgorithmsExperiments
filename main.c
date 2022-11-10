#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define CACHE_SIZE (3 * MB)
#define EXAGERATED_CACHE_SIZE (5 * CACHE_SIZE)

/*------------------------------------------------*/
/* Code to remove data from the processor caches. */
void cleanCache() {
    unsigned char *dummyBuffer = (unsigned char *)malloc(EXAGERATED_CACHE_SIZE);
    uint64_t i;
    for (i = 0; i < EXAGERATED_CACHE_SIZE; i++) {
        dummyBuffer[i] += 1;
    }
    free(dummyBuffer);
}

// Gen random number, accept negative, max inclusive
int64_t getRandomNumber(int64_t min, int64_t max) {
    int64_t range = max - min + 1;
    int64_t random = rand() % range;
    return random + min;
}

int64_t *getRandomArray(int32_t size, int64_t min, int64_t max) {
    int64_t *array = (int64_t *)malloc(size * sizeof(int64_t));
    int32_t i;
    for (i = 0; i < size; i++) {
        array[i] = getRandomNumber(min, max);
    }
    return array;
}

int32_t cmpForStdQSort(const void *a, const void *b) {
    int64_t x = *(int64_t *)a;
    int64_t y = *(int64_t *)b;
    if (x < y) {
        return -1;
    }
    if (x > y) {
        return 1;
    }
    return 0;
}

void stdQSort(int64_t *array, int32_t size) {
    qsort(array, size, sizeof(int64_t),
          (int32_t(*)(const void *, const void *)) & cmpForStdQSort);
}

void countingSort(int64_t *array, int32_t size, int32_t min, int32_t max) {
    int32_t *countingArray = (int32_t *)calloc(max - min + 1, sizeof(int32_t));
    int64_t i;
    for (i = 0; i < size; i++) {
        countingArray[array[i] - min] += 1;
    }
    int32_t j = 0;
    for (i = 0; i < max - min + 1; i++) {
        int32_t count = countingArray[i];
        while (count > 0) {
            array[j] = i + min;
            j++;
            count--;
        }
    }
    free(countingArray);
}

void insertionSort(int64_t *array, int32_t size) {
    int32_t i;
    for (i = 1; i < size; i++) {
        int32_t j = i;
        while (j > 0 && array[j - 1] > array[j]) {
            int64_t temp = array[j];
            array[j] = array[j - 1];
            array[j - 1] = temp;
            j--;
        }
    }
}

void mergeSort(int64_t *array, int32_t size) {
    if (size <= 1) {
        return;
    }
    int32_t middle = size / 2;
    mergeSort(array, middle);
    mergeSort(array + middle, size - middle);
    int64_t *tempArray = (int64_t *)malloc(size * sizeof(int64_t));
    int32_t i = 0;
    int32_t j = middle;
    int32_t k = 0;
    while (i < middle && j < size) {
        if (array[i] < array[j]) {
            tempArray[k] = array[i];
            i++;
        } else {
            tempArray[k] = array[j];
            j++;
        }
        k++;
    }
    while (i < middle) {
        tempArray[k] = array[i];
        i++;
        k++;
    }
    while (j < size) {
        tempArray[k] = array[j];
        j++;
        k++;
    }
    memcpy(array, tempArray, size * sizeof(int64_t));
    free(tempArray);
}

bool isSorted(int64_t *array, int32_t size) {
    int32_t i;
    for (i = 0; i < size - 1; i++) {
        if (array[i] > array[i + 1]) {
            return false;
        }
    }
    return true;
}

bool arraysHaveTheSameElements(int64_t *array1, int64_t *array2, int32_t size,
                               int64_t min, int64_t max) {
    int32_t *countingArray = (int32_t *)calloc(max - min + 1, sizeof(int32_t));
    int64_t i;
    for (i = 0; i < size; i++) {
        countingArray[array1[i] - min] += 1;
        countingArray[array2[i] - min] -= 1;
    }
    for (i = 0; i < max - min + 1; i++) {
        if (countingArray[i] != 0) {
            free(countingArray);
            return false;
        }
    }
    free(countingArray);
    return true;
}

int main(int32_t argc, char **argv) {

    int32_t SEED, SIZE;
    int64_t MIN_VALUE, MAX_VALUE;

    if (argc != 5) {
        printf("Usage: %s SEED SIZE MIN_VALUE MAX_VALUE\n", argv[0]);
        return -1;
    }

    SEED = atoi(argv[1]);
    SIZE = atoi(argv[2]);
    MIN_VALUE = atoi(argv[3]);
    MAX_VALUE = atoi(argv[4]);

    printf("SEED: %d SIZE: %d MIN_VALUE: %d MAX_VALUE: %d\n", SEED, SIZE,
           MIN_VALUE, MAX_VALUE);

    srand(SEED);

    int64_t *array = getRandomArray(SIZE, MIN_VALUE, MAX_VALUE);

    printf(isSorted(array, SIZE) ? "Initial array is sorted\n"
                                 : "Initial array is not sorted\n");

    // Allocate arrays for the different sorting algorithms.
    int64_t *arrayForStdQSort = (int64_t *)malloc(SIZE * sizeof(int64_t));
    int64_t *arrayForCountingSort = (int64_t *)malloc(SIZE * sizeof(int64_t));
    int64_t *arrayForInsertionSort = (int64_t *)malloc(SIZE * sizeof(int64_t));
    int64_t *arrayForMergeSort = (int64_t *)malloc(SIZE * sizeof(int64_t));

    // Copy the initial array to the arrays for the different sorting
    // algorithms.
    memcpy(arrayForStdQSort, array, SIZE * sizeof(int64_t));
    memcpy(arrayForCountingSort, array, SIZE * sizeof(int64_t));
    memcpy(arrayForInsertionSort, array, SIZE * sizeof(int64_t));
    memcpy(arrayForMergeSort, array, SIZE * sizeof(int64_t));

    // Standard Quick Sort
    cleanCache();

    stdQSort(arrayForStdQSort, SIZE);

    printf(isSorted(arrayForStdQSort, SIZE) &&
                   arraysHaveTheSameElements(arrayForStdQSort, array, SIZE,
                                             MIN_VALUE, MAX_VALUE)
               ? "stdQsort worked\n"
               : "stdQsort failed\n");

    // Counting Sort
    cleanCache();

    countingSort(arrayForCountingSort, SIZE, MIN_VALUE, MAX_VALUE);

    printf(isSorted(arrayForCountingSort, SIZE) &&
                   arraysHaveTheSameElements(arrayForCountingSort, array, SIZE,
                                             MIN_VALUE, MAX_VALUE)
               ? "countingSort worked\n"
               : "countingSort failed\n");

    // Insertion Sort
    cleanCache();

    if (SIZE <= 200000) {
        insertionSort(arrayForInsertionSort, SIZE);
    }

    printf(isSorted(arrayForInsertionSort, SIZE) &&
                   arraysHaveTheSameElements(arrayForInsertionSort, array, SIZE,
                                             MIN_VALUE, MAX_VALUE)
               ? "insertionSort worked\n"
               : "insertionSort failed\n");

    // Merge Sort
    cleanCache();

    mergeSort(arrayForMergeSort, SIZE);

    printf(isSorted(arrayForMergeSort, SIZE) &&
                   arraysHaveTheSameElements(arrayForMergeSort, array, SIZE,
                                             MIN_VALUE, MAX_VALUE)
               ? "mergeSort worked\n"
               : "mergeSort failed\n");

    // Free all the allocated memory.
    free(arrayForStdQSort);
    free(arrayForCountingSort);
    free(arrayForInsertionSort);
    free(arrayForMergeSort);
    free(array);

    return 0;
}