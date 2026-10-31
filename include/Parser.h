#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "StockType.h"

#define ITCH_HEADER_SIZE 11
#define ADD_ORDER_NO_MPID_SIZE 25 // This is minus the size of the HEADER
#define ADD_ORDER_MPID_SIZE 29 // This is minus the size of the HEADER

i32 process_message(u8 *buffer, i32 len, Message *msg);

i32 parse_add_message(u8 *buffer, i32 len, i32 offset, Message *msg);

#endif
