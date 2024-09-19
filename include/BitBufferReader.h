#ifndef BIT_BUFFER_READER_H
#define BIT_BUFFER_READER_H

#include <cstdint>
#include "FileHelper.h"
#include <spdlog/spdlog.h>

class BitBufferReader
{
public:
explicit BitBufferReader(FileHelper &fHelper, uint8_t lbit, uint32_t lbyt);

bool get_one_bit(bool &zo);

private:

uint8_t last_bit_pos;
uint8_t bit_buffer;
uint8_t cur_bit_offset;
uint32_t tot_read_bytes;
uint32_t last_byte_pos;
FileHelper &rFileHelper;
};

#endif