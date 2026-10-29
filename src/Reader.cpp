#include "Reader.h"

void ReadToMemoryFromFile(char *source, unsigned char *buffer, int pos, int size) {
    FILE *file = fopen(source, "r");
    fread(buffer, size, 1, file);
}
