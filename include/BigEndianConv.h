#ifndef BIG_ENDIAN_CONV_H
#define BIG_ENDIAN_CONV_H

#include "Protocol.h"

u16 be16_conv(u8 msb, u8 lsb);
u32 be32_conv(u8 buf[4]);
u64 be48_conv(u8 buf[6]);
u64 be64_conv(u8 buf[8]);


#endif
