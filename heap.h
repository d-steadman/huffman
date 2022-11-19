/*
Created by Wil Steadman on 11/16/22.
A specialized minimum heap implementation for creating Huffman trees.
*/

#ifndef HUFFMAN_HEAP_H
#define HUFFMAN_HEAP_H

#include <stdbool.h>
#include <stdio.h>

struct heap {
    struct node *heap;  // Heap can be efficiently stored as an array
    size_t size;
};

struct node {
    unsigned char byte;
    size_t count;

    struct node *left, *right;  // Used later to create Huffman tree
};


void init_heap(struct heap *heap, struct node *array, size_t size){
    heap->heap = array;
    heap->size = size;
}


static size_t get_count(struct heap *heap, size_t i){
    return heap->heap[i].count;
}


static void swap(struct heap *heap, size_t i, size_t j){
    struct node temp = heap->heap[i];
    heap->heap[i] = heap->heap[j];
    heap->heap[j] = temp;
}


static void min_heapify(struct heap *heap, size_t i){
    size_t smallest = i;
    size_t left = i*2 + 1;
    size_t right = i*2 + 2;

    if (left < heap->size && get_count(heap, left) < get_count(heap, smallest))
        smallest = left;

    if (right < heap->size && get_count(heap, right) < get_count(heap, smallest))
        smallest = right;

    if (smallest != i){
        swap(heap, i, smallest);
        min_heapify(heap, smallest);
    }
}


void build_heap(struct heap *heap){
    // Move up the non-leaf layers of the tree
    // NOTE: i needs to be a signed int so that it can go below zero
    for (int i = (heap->size / 2) - 1; i >= 0; i--){
        min_heapify(heap, i);
    }
}


struct node pop_min(struct heap *heap){
    struct node minimum = heap->heap[0];

    /* Swap the root with the last element, then re-heapify */
    swap(heap, 0, heap->size-1);
    heap->size--;
    min_heapify(heap, 0);

    return minimum;
}


void print_heap(struct heap *heap){
    for (size_t i = 0; i < heap->size; i++){
        printf("(byte: %c, count: %lu) ", heap->heap[i].byte, heap->heap[i].count);
    }
    printf("\n");
}

#endif //HUFFMAN_HEAP_H
