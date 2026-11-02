#include "Reader.h"

int read_to_memory_from_file(FILE *source, unsigned char *buffer, int pos, int size) {
    return fread(buffer, size, 1, source);
}
