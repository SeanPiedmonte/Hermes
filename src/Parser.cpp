#include "Parser.h"

i32 parse_header(u8 *buffer, u64 len, u64 offset, Message *msg) {
    i32 res = 0;
    if (len < 11) {
        return 1;
    }

    msg->Type = (MessageType)buffer[offset];
    msg->StockLocate = static_cast<u16>(buffer[offset+2] << 8 | buffer[offset+1]);
    msg->TrackingNumber = be16_conv(buffer[offset+3], buffer[offset+4]);
    msg->Timestamp = be48_conv(&buffer[offset+5]);
    return res;
}

i32 extract_stock(u8 *buffer, u64 len, u64 offset, u8 *stock) {
    u64 i = 0;
    i32 res = 0;
    if (len < 8) {
        return 1;
    }

    while (buffer[offset+i] != ' ') {
        stock[i] = buffer[offset+i];
        i++;
    }
    stock[i] = '\0';
    return res;
}

i32 parse_add_message(u8 *buffer, u64 len, u64 offset, Message *msg) {
    i32 res = 0;
    msg->AddOrder.OrderReferenceNumber = be64_conv(&buffer[offset]);
    msg->AddOrder.BuySellIndicator = buffer[offset+8];
    msg->AddOrder.Shares           = be32_conv(&buffer[offset+9]);
    res = extract_stock(buffer, offset+12, len, msg->Stock);
    msg->AddOrder.Price = be32_conv(&buffer[offset+21]);
    msg->AddOrder.Attribution[0] = buffer[offset+25];  
    msg->AddOrder.Attribution[1] = buffer[offset+26];  
    msg->AddOrder.Attribution[2] = buffer[offset+27];  
    msg->AddOrder.Attribution[3] = buffer[offset+28];  
    return res;
}

i32 process_message(u8 *buffer, u64 len, Message *msg) {
    u64 offset = 0; 
    i32 res = 0;
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
