#include "Protocol.h"
#include "StockType.h"
#include "Parser.h"
#include "Reader.h"

void print_message(Message *msg) {
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
                printf("ADD ORDER NO MPID: \n");
                printf("   TrackingNumber: %d\n", msg->TrackingNumber);
                printf("   Timestamp     : %llu\n", msg->Timestamp);
                printf("   Stock         : %s\n", msg->Stock);
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
    u16          TrackingNumber;
    u64          Timestamp;
    u8           Stock[9];
    MessageType  Type;
    u16          StockLocate;
    InterestFlag IF; 
    union {
        SystemEvent               EventCode;
        StockDirectory            Directory;
        StockTradingAction        STA;
        ShortSalePriceTestRI      SSPTRI;
        MarketParticipantPosition MPP;
        MWCBDeclineLevelMessage   MWCBDecLevMsg;
        BreachedLevel             BL;
        IPOQuotingPeriodUpdate    IPO;
        LULDAuctionCollar         LULDAucCol;
        OperationalHalt           Halt;
        AddOrder                  AddOrder;
        ModifyOrderMessages       ModifyOrder;
        TradeMessage              TradeMessage;
        NOIIMessage               NOIIMessage;
        DirectListing             DirectListing;
    };
}

int main(int argc, char **argv) {
    u64 pos = 0, res;
    u8 buffer[HOT_BUFFER_SIZE];
    char *infile_str;
    FILE *infile;
    Message msg = {0};
    if (argc != 2) {
        printf("USAGE ./hermes <stream>\n");
        return 1;
    }

    infile = fopen(infile_str, "r");
    if (infile == NULL) {
        return -1;
    }

    infile_str = argv[1];
    while (pos) {
        pos = read_to_memory_from_file(infile, buffer, pos, HOT_BUFFER_SIZE);
        res = process_message(buffer, pos, &msg);
        if (res == -1) {
            return 1;
        }

        print_message(&msg);
    }
}
