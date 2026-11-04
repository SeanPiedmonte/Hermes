#include "BigEndianConv.h"

u16 be16_conv(u8 msb, u8 lsb) {
    return static_cast<u16>((lsb << 8) | msb);
}

u32 be32_conv(u8 buf[4]) {
    return static_cast<u32>((buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) |
                            buf[0]);
}

u64 be48_conv(u8 buf[6]) {
    return (static_cast<u64>(buf[5]) << 40) | (static_cast<u64>(buf[4]) << 32) 
            | static_cast<u64>((buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) 
            | buf[0]);
}

u64 be64_conv(u8 buf[8]) {
    return (static_cast<u64>(buf[7]) << 56) | (static_cast<u64>(buf[6]) << 48) |
           (static_cast<u64>(buf[5]) << 40) | (static_cast<u64>(buf[4]) << 32) | 
           static_cast<u64>((buf[3] << 24) | (buf[2] << 16)  | (buf[1] << 8) 
           | buf[0]);
}
