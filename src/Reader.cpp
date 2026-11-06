#include "Reader.h"

size_t read_to_memory_from_file(FILE *source, u8 *buffer, size_t size) {
    size_t res = fread(buffer, 1, size, source);
    printf("SIZE: %lu\n", size);
    printf("RES: %lu\n", res);
    return res; 
}
