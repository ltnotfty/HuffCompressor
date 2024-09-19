#ifndef HUFF_FILE_H
#define HUFF_FILE_H

#include <cstdint>

struct HuffFileHdr {
    uint16_t tot_type_cnt;
    uint16_t last_bit_offset;
    uint32_t last_byte_offset;
};

struct HuffEncodeTableItem {
    uint16_t char_value;
    uint16_t bit_cnt;
    uint32_t bit_str;
};
using HuffEncodeItem = struct HuffEncodeTableItem;
using HuffEncodeTable = HuffEncodeItem *;

#endif