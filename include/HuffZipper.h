#ifndef HUFF_ZIPPER_H
#define HUFF_ZIPPER_H

#include "HuffFile.h"
#include "HuffTree.h"
#include "FileHelper.h"
#include "BitBufferWriter.h"
#include "BitBufferReader.h"


#include <cstring>
#include <queue>
#include <spdlog/spdlog.h>


class HuffZipper
{

public:

    bool zip(const char *in_file, const char *out_zip_file);

    bool unzip(const char *unzip_file, const char *out_unzip_file);
    void print_huf_tree(HuffTree rt, std::string &str);

private:

    void build_encode_table(HuffTree tr, uint16_t bit_cnt, uint32_t &bit_con, HuffEncodeTable het);
    HuffTree build_huff_tree(FileHelper &fHelper);
    void build_huff_tree_from(HuffTree root,const HuffEncodeItem &item);


    void show_encode_table(const HuffEncodeTable p);
    uint64_t get_encode_bits_len(const HuffTree, const HuffEncodeTable);

    FileHelper zipHelper;
    FileHelper unzipHelper;

};
#endif