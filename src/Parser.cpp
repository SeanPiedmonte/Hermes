#include "Parser.h"
#include "StockType.h"

i32 parse_header(u8 *buffer, u64 len, u64 offset, Message *msg) {
    i32 res = 0;
    if (len - offset < 13 && len - offset != 0) {
        return ERROR_TRUNCATED_HEADER;
    } else if (len - offset == 0) {
        return ERROR_ZERO_HEADER_LEN;
    }
    
    msg->Length = static_cast<u16>((buffer[offset] << 8) | buffer[offset+1]);
    msg->Type = (MessageType)buffer[offset+2];
    msg->StockLocate = static_cast<u16>(buffer[offset+4] << 8 | buffer[offset+3]);
    msg->TrackingNumber = be16_conv(buffer[offset+5], buffer[offset+6]);
    msg->Timestamp = be48_conv(&buffer[offset+7]);
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
        res = parse_header(buffer, len, offset, msg);
        if (res) {
            return res;
        }

        offset+=ITCH_HEADER_SIZE;
        switch (msg->Type) {
            case SYSTEM_EVENT_MESSAGE:
                offset++;
                break;
            case STOCK_DIRECTORY_MESSAGE:
                offset += STOCK_DIRECTORY_MESSAGE_SIZE;
                break;
            case STOCK_TRADING_ACTION_MESSAGE:
                offset += STOCK_TRADING_ACTION_MESSAGE_SIZE;
                break;
            case REG_SHO_SHORT_SALE_PRICE_TEST_RESTRICTED_INDICATOR:
                offset += REG_SHO_SHORT_SALE_PRICE_TEST_RESTRICTED_INDICATOR_SIZE;
                break;
            case MARKET_PARTICIPANT_POSITION_MESSAGE:
                offset += MARKET_PARTICIPANT_POSITION_MESSAGE_SIZE;
                break;
            case MARKET_WIDE_CIRCUIT_BREAKER_DECLINE_LEVEL_MESSAGE:
                offset += MARKET_WIDE_CIRCUIT_BREAKER_DECLINE_LEVEL_MESSAGE_SIZE;
                break;
            case MARKET_WIDE_CIRCUIT_BREAKER_STATUS_MESSAGE:
                offset += MARKET_WIDE_CIRCUIT_BREAKER_STATUS_MESSAGE_SIZE;
                break;
            case QUOTING_PERIOD_UPDATE:
                offset += QUOTING_PERIOD_UPDATE_SIZE;
                break;
            case LULD_AUCTION_COLLAR:
                offset += LULD_AUCTION_COLLAR_SIZE;
                break;
            case OPERATIONAL_HALT:
                offset += OPERATIONAL_HALT_SIZE;
                break;
            case ADD_ORDER_NO_MPID_ATTRIBUTION_MESSAGE:
                /*res = parse_header(buffer, len, offset, msg);
                offset+=ITCH_HEADER_SIZE;*/
                msg->Type = ADD_ORDER_NO_MPID_ATTRIBUTION_MESSAGE;
                res = parse_add_message(buffer, len, offset+1, msg);
                offset += ADD_ORDER_NO_MPID_SIZE;
                break;
            case ADD_ORDER_MPID_ATTRIBUTION_MESSAGE:
                /*res = parse_header(buffer, len, offset, msg);
                offset+=ITCH_HEADER_SIZE;*/
                msg->Type = ADD_ORDER_MPID_ATTRIBUTION_MESSAGE;
                res = parse_add_message(buffer, len, offset+1, msg);
                offset += ADD_ORDER_MPID_SIZE;
                break;
            case ADD_ORDER_ORDER_EXECUTED_MESSAGE:
                offset += ADD_ORDER_ORDER_EXECUTED_MESSAGE_SIZE;
                break;
            case ADD_ORDER_ORDER_EXECUTED_WITH_PRICE_MESSAGE:
                offset += ADD_ORDER_ORDER_EXECUTED_WITH_MESSAGE_SIZE;
                break;
            case ORDER_CANCEL_MESSAGE:
                offset += ORDER_CANCEL_MESSAGE_SIZE;
                break;
            case ORDER_DELETE_MESSAGE:
                offset += ORDER_DELETE_MESSAGE_SIZE;
                break;
            case ORDER_REPLACE_MESSAGE:
                offset += ORDER_REPLACE_MESSAGE_SIZE;
                break;
            case TRADE_MESSAGE:
                offset += TRADE_MESSAGE_SIZE;
                break;
            case CROSS_TRADE_MESSAGE:
                offset += CROSS_TRADE_MESSAGE_SIZE;
                break;
            case BROKEN_TRADE_MESSAGE:
                offset += BROKEN_TRADE_MESSAGE_SIZE;
                break;
            case NOII_MESSAGE:
                offset += NOII_MESSAGE_SIZE;
                break;
            case RETAIL_INTEREST_MESSAGE:
                offset += RETAIL_INTEREST_MESSAGE_SIZE;
                break;
            case DIRECT_LISTING_WITH_CAPITAL_RAISE_MESSAGE:
                offset += DIRECT_LISTING_WITH_CAPITAL_RAISE_MESSAGE_SIZE;
                break;
            default:
                printf("Error Not a Correct Message\n");
        }
    }

    return res;
}
