#ifndef READER_H
#define READER_H

#include <stdlib.h>
#include <stdio.h>
#include "Protocol.h"

size_t read_to_memory_from_file(FILE *source, u8 *buffer, size_t size);

#endif
