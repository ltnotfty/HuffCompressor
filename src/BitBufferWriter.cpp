#include "BitBufferWriter.h"



BitBufferWriter::BitBufferWriter(FileHelper &fHelper):cur_bit_offset(0), tot_write_bytes(0), wFileHelper(fHelper) {
    
}




void BitBufferWriter::write_bit_str(uint16_t bit_cnt, uint32_t bit_str)
{


    for (int i = 0;i < bit_cnt; ++i, bit_str >>= 1) {

        int op_id = BIT_BUFFER_LEN  - 1 - cur_bit_offset;

        if ( bit_str & 1) {
            spdlog::debug("push: 1" );
            bufferByte |= (1 << op_id);
        }
        else {
            bufferByte &= ~(1 << op_id);
            spdlog::debug("push: 0" );
        }

        ++cur_bit_offset;

        if (cur_bit_offset == BIT_BUFFER_LEN) {
            wFileHelper.append_one(&bufferByte, 1 );
            tot_write_bytes++;

            cur_bit_offset = 0;
        }
    }
}


/* 
    last_bit_set = 0
    if cur_bit_offset \ne 0
        write it to file
        set ret = cur_bit_offset
    return ret 
*/
uint8_t BitBufferWriter::last_check_and_write()
{
    if (cur_bit_offset) {
        wFileHelper.append_one(&bufferByte, sizeof(bufferByte));
        tot_write_bytes++;
    }
    return cur_bit_offset;
}



uint32_t BitBufferWriter::get_tot_write_bytes() const
{
    return tot_write_bytes;
}