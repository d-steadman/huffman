#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
                die("unknown option '%c' (decimal: %d)", optopt, optopt);
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

    /* Count occurrences of each character */
    char ascii[0xFF];

    /* Compile counts into a list */

    /* Build Huffman tree */

    /* Output into file */

    /* Clean up & deallocate */

    return 0;
}
