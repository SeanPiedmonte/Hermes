#include "Reader.h"

size_t read_to_memory_from_file(FILE *source, u8 *buffer, size_t size) {
    return fread(buffer, size, 1, source);
}
