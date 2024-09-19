#ifndef BIT_BUFFER_WRITER_H
#define BIT_BUFFER_WRITER_H

#include <cstdint>
#include "FileHelper.h"
#include <spdlog/spdlog.h>


class BitBufferWriter
{
public:


explicit BitBufferWriter(FileHelper &);




void write_bit_str(uint16_t bit_cnt, uint32_t bit_str);

uint32_t get_tot_write_bytes() const;
/* 
    last_bit_set = 0
    if cur_bit_offset \ne 0
        write it to file
        set ret = cur_bit_offset
    return ret 
*/
uint8_t last_check_and_write();


private:
static constexpr int BIT_BUFFER_LEN = 8;


uint8_t cur_bit_offset;
uint8_t bufferByte;
uint32_t tot_write_bytes;
FileHelper &wFileHelper;

};
#endif