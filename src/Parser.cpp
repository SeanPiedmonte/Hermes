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
   
    // having to take in all 8 bytes while I figure out why the ' ' portion is nonexistent
    while (/*buffer[offset+i] != ' ' &&*/ i < 8) {
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
        //res = parse_header(buffer, len, offset, msg);
        //offset+=11;
        switch (msg->Type) {
            case SYSTEM_EVENT_MESSAGE:
                break;
            case STOCK_DIRECTORY_MESSAGE:
                break;
            case STOCK_TRADING_ACTION_MESSAGE:
                break;
            case REG_SHO_SHORT_SALE_PRICE_TEST_RESTRICTED_INDICATOR:
                break;
            case MARKET_PARTICIPANT_POSITION_MESSAGE:
                break;
            case MARKET_WIDE_CIRCUIT_BREAKER_DECLINE_LEVEL_MESSAGE:
                break;
            case MARKET_WIDE_CIRCUIT_BREAKER_STATUS_MESSAGE:
                break;
            case QUOTING_PERIOD_UPDATE:
                break;
            case LULD_AUCTION_COLLAR:
                break;
            case OPERATIONAL_HALT:
                break;
            case ADD_ORDER_NO_MPID_ATTRIBUTION_MESSAGE:
                res = parse_header(buffer, len, offset, msg);
                offset+=11;
                msg->Type = ADD_ORDER_NO_MPID_ATTRIBUTION_MESSAGE;
                res = parse_add_message(buffer, len, offset+1, msg);
                offset += ADD_ORDER_NO_MPID_SIZE;
                break;
            case ADD_ORDER_MPID_ATTRIBUTION_MESSAGE:
                res = parse_header(buffer, len, offset, msg);
                offset+=11;
                msg->Type = ADD_ORDER_MPID_ATTRIBUTION_MESSAGE;
                res = parse_add_message(buffer, len, offset+1, msg);
                offset += ADD_ORDER_MPID_SIZE;
                break;
            case ADD_ORDER_ORDER_EXECUTED_MESSAGE:
                break;
            case ADD_ORDER_ORDER_EXECUTED_WITH_PRICE_MESSAGE:
                break;
            case ORDER_CANCEL_MESSAGE:
                break;
            case ORDER_DELETE_MESSAGE:
                break;
            case ORDER_REPLACE_MESSAGE:
                break;
            case TRADE_MESSAGE:
                break;
            case CROSS_TRADE_MESSAGE:
                break;
            case BROKEN_TRADE_MESSAGE:
                break;
            case NOII_MESSAGE:
                break;
            case RETAIL_INTEREST_MESSAGE:
                break;
            case DIRECT_LISTING_WITH_CAPITAL_RAISE_MESSAGE:
                break;
            default:
                //printf("Error Not a Correct Message\n");
        }
    }

    return res;
}
