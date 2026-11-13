#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "StockType.h"
#include "BigEndianConv.h"
#include "Error.h"

#define ITCH_HEADER_SIZE 13
#define STOCK_DIRECTORY_MESSAGE_SIZE 28
#define STOCK_TRADING_ACTION_MESSAGE_SIZE 14
#define REG_SHO_SHORT_SALE_PRICE_TEST_RESTRICTED_INDICATOR_SIZE 9
#define MARKET_PARTICIPANT_POSITION_MESSAGE_SIZE 15
#define MARKET_WIDE_CIRCUIT_BREAKER_DECLINE_LEVEL_MESSAGE_SIZE 24
#define MARKET_WIDE_CIRCUIT_BREAKER_STATUS_MESSAGE_SIZE 1
#define QUOTING_PERIOD_UPDATE_SIZE 17
#define LULD_AUCTION_COLLAR_SIZE 24 
#define OPERATIONAL_HALT_SIZE 10 
#define ADD_ORDER_NO_MPID_SIZE 25 // This is minus the size of the HEADER
#define ADD_ORDER_MPID_SIZE 29 // This is minus the size of the HEADER
#define ADD_ORDER_ORDER_EXECUTED_MESSAGE_SIZE 20 
#define ADD_ORDER_ORDER_EXECUTED_WITH_MESSAGE_SIZE 25 
#define ORDER_CANCEL_MESSAGE_SIZE 12 
#define ORDER_DELETE_MESSAGE_SIZE 8 
#define ORDER_REPLACE_MESSAGE_SIZE 24 
#define TRADE_MESSAGE_SIZE 33
#define CROSS_TRADE_MESSAGE_SIZE 29
#define BROKEN_TRADE_MESSAGE_SIZE 8
#define NOII_MESSAGE_SIZE 39
#define RETAIL_INTEREST_MESSAGE_SIZE 9
#define DIRECT_LISTING_WITH_CAPITAL_RAISE_MESSAGE_SIZE 37

i32 process_message(u8 *buffer, u64 len, Message *msg, u8 *truncated_buffer);

i32 parse_add_message(u8 *buffer, i32 len, u64 offset, Message *msg);
i32 parse_header(u8 *buffer, u64 len, u64 offset, Message *msg);
i32 extract_stock(u8 *buffer, u64 len, u64 offset, u8 *stock);

#endif
