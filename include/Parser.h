#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "StockType.h"
#include "BigEndianConv.h"

#define ITCH_HEADER_SIZE 11
#define ADD_ORDER_NO_MPID_SIZE 25 // This is minus the size of the HEADER
#define ADD_ORDER_MPID_SIZE 29 // This is minus the size of the HEADER

i32 process_message(u8 *buffer, u64 len, Message *msg);

i32 parse_add_message(u8 *buffer, i32 len, u64 offset, Message *msg);
i32 parse_header(u8 *buffer, u64 len, u64 offset, Message *msg);
i32 extract_stock(u8 *buffer, u64 len, u64 offset, u8 *stock);

#endif
