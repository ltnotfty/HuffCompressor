#include "BitBufferReader.h"

BitBufferReader::BitBufferReader(FileHelper &fHelper, uint8_t lbi, uint32_t lby):
    last_bit_pos(lbi),cur_bit_offset(0),tot_read_bytes(0),last_byte_pos(lby),rFileHelper(fHelper)
{
    rFileHelper.read_next_one(&bit_buffer, sizeof(bit_buffer));
    tot_read_bytes++;
}

bool BitBufferReader::get_one_bit(bool &v)
{
    if (last_byte_pos == tot_read_bytes  && cur_bit_offset  == last_bit_pos) {
        spdlog::info("have read all bits");
        return false;
    }


    v = bit_buffer & (1 << (7 - cur_bit_offset));
    cur_bit_offset++;
    
    if ( cur_bit_offset == 8) {
        tot_read_bytes++;
        cur_bit_offset = 0;
        rFileHelper.read_next_one(&bit_buffer, sizeof(bit_buffer));
    }

    return true;
}