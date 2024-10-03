
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "heap.h"

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity) {
    heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
    heap->capacity = capacity;
    heap->size = 0;
    heap->data = (heap_node_t *)malloc(capacity * sizeof(heap_node_t));
    return heap;
}

void heap_free(heap_t *heap) {
    free(heap->data);
    free(heap);
}

unsigned int heap_size(heap_t *heap) { return heap->size; }

unsigned int heap_parent(unsigned int index) { return ((index - 1) / 2 ); }

unsigned int heap_left_child(unsigned int index) { return ((2 * index) + 1 ); }

unsigned int heap_right_child(unsigned int index) { return ((2 * index) + 2 ); }

unsigned int heap_level(unsigned int index) { return (int)log2(index + 1); }

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_node_t temp = heap->data[index1];
    heap->data[index1] = heap->data[index2];
    heap->data[index2] = temp;
}  

void heap_bubble_up(heap_t *heap, int index) {
    if (index == 0) {
        return;
    }
    unsigned int parent = heap_parent(index);
    if ((heap->data[parent].value).as_int > (heap->data[index].value).as_int) {
        heap_swap(heap, parent, index);
        heap_bubble_up(heap, parent);
    }
}

void heap_bubble_down(heap_t *heap, int index) {
    int smallest = index;
    int left = heap_left_child(index);
    int right = heap_right_child(index);
    if (left < heap->size && (heap->data[left].value).as_int < (heap->data[index].value).as_int) {
        smallest = left;
    }

    if (right < heap->size && (heap->data[right].value).as_int < (heap->data[index].value).as_int) {
        smallest = right;
    }

    if (smallest != index) {
        heap_swap(heap, index, smallest);
        heap_bubble_down(heap, smallest);
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity) {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return (heap_value_t){.as_int = KEY_NOT_PRESENT};
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}

unsigned long long rand_between(unsigned long long min,
                                unsigned long long max) {
    unsigned long long range = max - min;
    return min + (rand() % range);
}

void test_heap(void) {
    heap_t *heap = heap_create(200);
    for (heap_key_t ix = 0; ix < 20; ix++) {
        heap_key_t key = rand_between(0, 1000);
        heap_insert(heap, key, (heap_value_t){.as_int = key});
        heap_print(heap);
    }
    for (int ix = 0; ix < 10; ix++) {
        heap_value_t key = heap_remove_min(heap);
        printf("Removed %llu\n", key.as_int);
        heap_print(heap);
    }
    exit(0);
}
int main(int argc, char *argv[]) {
    srand(time(NULL));

    test_heap();
}
