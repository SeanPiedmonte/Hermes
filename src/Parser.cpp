#include "Parser.h"

i32 parse_header(u8 *buffer, i32 len, i32 offset, Message *msg) {
    u32 res = 0;
    msg->Type = (MessageType)buffer[offset];
    msg->StockLocate = buffer[offset+2] << 8 | buffer[offset+1];
    msg->TrackingNumber = buffer[offset+4] << 8 | buffer[offset+3];
    msg->Timestamp = static_cast<u64>(buffer[offset+10]) << 40 | 
                     static_cast<u64>(buffer[offset+9]) << 32 | 
                     static_cast<u64>(buffer[offset+8]) << 24 | 
                     static_cast<u64>(buffer[offset+7]) << 16 | 
                     static_cast<u64>(buffer[offset+6]) << 8 |
                     static_cast<u64>(buffer[offset+5]);
    return res;
}

i32 extract_stock(u8 *buffer, i32 len, i32 offset, u8 *stock) {
    i32 res = 0, i = 0;
    while (buffer[offset] != ' ') {
        stock[i] = buffer[offset+i];
        i++;
    }
    stock[i] = '\0';
    return res;
}

i32 parse_add_message(u8 *buffer, i32 len, i32 offset, Message *msg) {
    i32 res = 0;
    msg->AddOrder.OrderReferenceNumber = static_cast<u64>(buffer[offset+7]) << 56 |
                                         static_cast<u64>(buffer[offset+6]) << 48 |
                                         static_cast<u64>(buffer[offset+5]) << 40 |
                                         static_cast<u64>(buffer[offset+4]) << 32 |
                                         static_cast<u64>(buffer[offset+3]) << 24 |
                                         static_cast<u64>(buffer[offset+2]) << 16 |
                                         static_cast<u64>(buffer[offset+1]) << 8  |
                                         static_cast<u64>(buffer[offset]);
    msg->AddOrder.BuySellIndicator = buffer[offset+8];
    msg->AddOrder.Shares           = static_cast<u32>(buffer[offset+12]) << 24 |
                                     static_cast<u32>(buffer[offset+11]) << 16 |
                                     static_cast<u32>(buffer[offset+10]) << 8  |
                                     static_cast<u32>(buffer[offset+9]);
    res = extract_stock(buffer, offset+12, len, msg->Stock);
    msg->AddOrder.Price = static_cast<u32>(buffer[offset+24]) << 24 |
                          static_cast<u32>(buffer[offset+23]) << 16 |
                          static_cast<u32>(buffer[offset+22]) << 8  |
                          static_cast<u32>(buffer[offset+21]);
    msg->AddOrder.Attribution[0] = buffer[offset+25];  
    msg->AddOrder.Attribution[1] = buffer[offset+26];  
    msg->AddOrder.Attribution[2] = buffer[offset+27];  
    msg->AddOrder.Attribution[3] = buffer[offset+28];  
    return res;
}

i32 process_message(u8 *buffer, i32 len, Message *msg) {
    i32 offset = 0, res = 0;
    while (offset < len) {
        res = parse_header(buffer, len, offset, msg);
        offset+=11;
        switch (msg->Type) {
            case SYSTEM_EVENT_MESSAGE:
            case STOCK_DIRECTORY_MESSAGE:
            case STOCK_TRADING_ACTION_MESSAGE:
            case REG_SHO_SHORT_SALE_PRICE_TEST_RESTRICTED_INDICATOR:
            case MARKET_PARTICIPANT_POSITION_MESSAGE:
            case MARKET_WIDE_CIRCUIT_BREAKER_DECLINE_LEVEL_MESSAGE:
            case MARKET_WIDE_CIRCUIT_BREAKER_STATUS_MESSAGE:
            case QUOTING_PERIOD_UPDATE:
            case LULD_AUCTION_COLLAR:
            case OPERATIONAL_HALT:
            case ADD_ORDER_NO_MPID_ATTRIBUTION_MESSAGE:
                msg->Type = ADD_ORDER_NO_MPID_ATTRIBUTION_MESSAGE;
                res = parse_add_message(buffer, len, offset+1, msg);
                offset += ADD_ORDER_NO_MPID_SIZE;
                break;
            case ADD_ORDER_MPID_ATTRIBUTION_MESSAGE:
                msg->Type = ADD_ORDER_MPID_ATTRIBUTION_MESSAGE;
                res = parse_add_message(buffer, len, offset+1, msg);
                offset += ADD_ORDER_MPID_SIZE;
                break;
            case ADD_ORDER_ORDER_EXECUTED_MESSAGE:
            case ADD_ORDER_ORDER_EXECUTED_WITH_PRICE_MESSAGE:
            case ORDER_CANCEL_MESSAGE:
            case ORDER_DELETE_MESSAGE:
            case ORDER_REPLACE_MESSAGE:
            case TRADE_MESSAGE:
            case CROSS_TRADE_MESSAGE:
            case BROKEN_TRADE_MESSAGE:
            case NOII_MESSAGE:
            case RETAIL_INTEREST_MESSAGE:
            case DIRECT_LISTING_WITH_CAPITAL_RAISE_MESSAGE:
            default:
                printf("Error Not a Correct Message\n");
        }
    }

    return res;
}
