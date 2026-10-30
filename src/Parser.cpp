#include "StockType.h"
#include "Parser.h"

int ProcessMessage(unsigned char *buffer, int len) {
    int offset = 0;
    while (offset < len) {
        switch (buffer[offset]) {
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
            case ADD_ORDER_MPID_ATTRIBUTION_MESSAGE:
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

    return 0;
}
