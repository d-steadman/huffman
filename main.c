#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

/* Exit the program verbosely */
#define die(fmt, ...) \
    do { \
        fprintf(stderr, "[die] %s:%d " fmt "\n", \
                __func__, __LINE__,##__VA_ARGS__); \
        exit(1); \
    } while (0)

#define die_errno(errnum, fmt, ...) \
    do { \
        fprintf(stderr, "[die] %s:%d " fmt ": %s\n", \
                __func__, __LINE__,##__VA_ARGS__, strerror(errnum)); \
        exit(1); \
    } while (0)

#define EXTENSION ".hzip"
#define READ_BUFFER_SIZE 4096

void byte_occurrences(FILE *file, unsigned int *count){

}

/* Functions to generate heap array */
void count_to_nodes(const int *acount, struct node *heap){
    for (unsigned char byte = 0; byte < 0xFF; byte++){
        if (acount[byte] == 0)
            continue;

        struct node *new_node = malloc(sizeof(struct node));

        if (new_node == NULL)
            die("Out of memory");

        new_node->byte = byte;
        new_node->count = acount[byte];
        heap = new_node;
        heap++;
    }
}


int main(int argc, char **argv) {
    /* Parse commandline args */

    char *input_filename = NULL;
    char *output_filename = NULL;
    bool operation_mode = true; // true for zipping, false for unzipping

    int opt, nargs;
    const char *short_opts = ":hvuo:";
    struct option long_opts[] = {
            {"help",    no_argument,       NULL, 'h'},
            {"verbose", no_argument,       NULL, 'v'},
            {"unzip",   no_argument,       NULL, 'u'},
            {"output",  required_argument, NULL, 'o'},
            {NULL, 0,                      NULL, 0}
    };

    while (1) {
        opt = getopt_long(argc, argv, short_opts, long_opts, NULL);
        if (opt == -1) {
            /* processed all command-line options */
            break;
        }

        switch (opt) {
            case 'h':   // Help
                break;
            case 'v':   // Verbose
                break;
            case 'u':   // Unzip
                operation_mode = false;
                break;
            case 'o':   // Output filename
                output_filename = optarg;
                break;
            case '?':
                die("unknown option '%c'", optopt, optopt);
                break;
            case ':':
                die("missing option argument for option %c", optopt);
                break;
            default:
                die("unexpected getopt_long return value: %c\n", (char) opt);
        }
    }

    // optind is the index of the first (and hopefully only) positional argument
    nargs = argc - optind;
    if (nargs != 1)
        die("expected one positional argument, but found %d", nargs);

    input_filename = argv[optind];

    // If no output name was specified, add ".hzip" to the end
    if (output_filename == NULL){
        output_filename = malloc(strlen(input_filename) + strlen(EXTENSION));
        strcat(output_filename, input_filename);
        strcat(output_filename, EXTENSION);
    }

    printf("Input: %s\n", input_filename);
    printf("Output: %s\n", output_filename);

    /* File opening */
    FILE *file = fopen(input_filename, "rb");
    if (file == NULL)
        die_errno(errno, "fopen");

    /* Count occurrences of each character */
    unsigned int ascii_count[0xFF] = { 0 };
    unsigned char buffer[READ_BUFFER_SIZE];
    size_t n_bytes = 0; // How many bytes did we actually read into buffer?
    unsigned char byte;  // For readability of indexing


    while ((n_bytes = fread(buffer, sizeof(char), READ_BUFFER_SIZE, file)) != 0){
        for (int i = 0; i < n_bytes; i++){
            byte = buffer[i];
            ascii_count[byte]++;
        }
    }

    /* Compile counts into a heap */

    // To establish array size, find how many bytes have a non-zero count
    size_t used_bytes = 0;
    for (byte = 0; byte < 0xFF; byte++)
        used_bytes += ascii_count[byte] > 0 ? 1 : 0;

    struct heap heap;
    struct node array[used_bytes];

    // Create nodes place them into the array
    size_t array_index = 0;
    for (byte = 0; byte < 0xFF; byte++){
        if (ascii_count[byte] > 0) {
            struct node new_node;
            new_node.byte = byte;
            new_node.count = ascii_count[byte];
            array[array_index] = new_node;

            array_index++;
        }
    }

    // Init and build heap
    init_heap(&heap, array, used_bytes);
    build_heap(&heap);
    print_heap(&heap);

    struct node min;
    while (heap.size){
        printf("(Heap size: %lu) ", heap.size);
        min = pop_min(&heap);
        printf("Byte: %c, Count: %lu\n", min.byte, min.count);
    }

    /* Build Huffman tree */


    /* Output into file */

    /* Clean up & deallocate */

    return 0;
}
