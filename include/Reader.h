#ifndef READER_H
#define READER_H

#include "stdlib.h"
#include "stdio.h"

int read_to_memory_from_file(FILE *source, unsigned char *buffer, int pos, int size);

#endif
