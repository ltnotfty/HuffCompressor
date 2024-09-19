#include "HuffZipper.h"


bool HuffZipper::zip(const char *in_file, const char *out_zip_file)
{
    if (!zipHelper.open(in_file, "rb"))
        return false;

    HuffTree rt = build_huff_tree(zipHelper);

    // test for build huff tree
    // std::string echo;
    // print_huf_tree(rt, echo);



    
    spdlog::info("build huff tree success!");
    struct HuffFileHdr huffFileHdr;
    struct HuffEncodeTableItem enTable[UINT8_MAX + 1];
    memset(enTable, 0, sizeof(enTable));
    
    uint16_t bit_cnt = 0;
    uint32_t bit_con = 0;
    uint16_t char_type_cnt = 0;


    build_encode_table(rt, bit_cnt, bit_con, enTable);

    auto bitsLen = get_encode_bits_len(rt, enTable);
    spdlog::info("total encode bytes: {}", (bitsLen + 7) / 8);
    delete rt;



    spdlog::info("build encode table success!");
    FileHelper wHelper;
    if (!wHelper.open(out_zip_file, "wb")) {
        spdlog::warn("cant create file: {:s}", out_zip_file);
        return false;
    }

    wHelper.append_one(&huffFileHdr, sizeof(HuffFileHdr));
    BitBufferWriter bitWriter(wHelper);

    // write huffFileHdr to out_zip_file
    // for_each item.bit_cnt != 0 in enTable
    //     write item to out_zip_file
    //     char_type_cnt++;
    // wHelper.append_one(&huffFileHdr, sizeof(huffFileHdr));
    for (int i = 0; i <= UINT8_MAX; ++i) {
        if (enTable[i].bit_cnt) {

            wHelper.append_one(&enTable[i], sizeof(enTable[i]));
        
            char_type_cnt++;
        }
    }
    spdlog::info("encode table write to file {:s}", out_zip_file);

    show_encode_table(enTable);
    
    
    int ch;
    int tot_ch = 0;
    zipHelper.reset();
    spdlog::info("reset fp for {:s}", in_file);
    
    uint64_t tot_bits_cnt = 0;

    while ((ch = zipHelper.put_one_char()) != EOF) {
        tot_ch++;
    
    
        uint8_t pv = static_cast<uint8_t>(ch);
        bitWriter.write_bit_str( enTable[pv].bit_cnt, enTable[pv].bit_str);
        tot_bits_cnt += enTable[pv].bit_cnt;
        // bitWriter.write_bit_str(enTable[ch].bit_cnt, enTable[ch].bit_str);
    }
    // 
    huffFileHdr.last_bit_offset = bitWriter.last_check_and_write();
    huffFileHdr.tot_type_cnt = char_type_cnt;
    huffFileHdr.last_byte_offset = bitWriter.get_tot_write_bytes();

    spdlog::info("last_bit_offset: {}, tot_write_bytes: {}, tot_bits_cnt / 8: {}, tot_ch: {}", 
        huffFileHdr.last_bit_offset, huffFileHdr.last_byte_offset, tot_bits_cnt / 8, tot_ch);
   

    // modify file hdr
    wHelper.modify(&huffFileHdr, sizeof(huffFileHdr), 1, 0);
    wHelper.close();


    
    return true;
}

bool HuffZipper::unzip(const char *unzip_file, const char *out_unzip_file)
{
    bool open_success = this->unzipHelper.open(unzip_file, "rb");
    if ( !open_success)
        return false;


    /* read hdr*/
    HuffFileHdr fileHdr;

    unzipHelper.read_next_one(&fileHdr, sizeof(fileHdr));
    // unzipFileHdr.read(fileHdr);
    
    // struct HuffEncodeTableItem 
    /*read encode table*/
    spdlog::debug("type_cnt: {}", fileHdr.tot_type_cnt);


    auto rt = new HuffTreeNode();
    HuffEncodeItem huffItem;
    for (int i = 0;i < fileHdr.tot_type_cnt; ++i) {
        // read one HuffEncodeTableItem
        // unzipfile.read_to(HuffEncodeTableItem);
        // build_huff_tree_from(rt, huffItem);
        unzipHelper.read_next_one(&huffItem, sizeof(huffItem));

        spdlog::debug("{:c}: {} {}", huffItem.char_value, huffItem.bit_cnt, huffItem.bit_str);
        build_huff_tree_from(rt, huffItem);
    }

/* 
    test for rebuild hufftree
    std::string echo;
    print_huf_tree(rt, echo);
*/

    FileHelper wHelper;
    if (!wHelper.open(out_unzip_file, "wb"))
        return false;
    
    // for last_bit_offset == 0, do some special handle


    auto lstByte = fileHdr.last_byte_offset;
    if (0 == fileHdr.last_bit_offset)
        lstByte++;
    BitBufferReader bufferReader(unzipHelper, fileHdr.last_bit_offset, lstByte);
    spdlog::info("last bit offset: {}, last byte_offset: {}, tot_type_cnt:{}", 
        fileHdr.last_bit_offset, fileHdr.last_byte_offset, fileHdr.tot_type_cnt);

    bool one = false;
    auto root = rt;

    int tot_write_bytes = 0;
    while ( bufferReader.get_one_bit( one)) {
        if (nullptr == root ) {
            spdlog::error("something wrong happened!");
            break;
        }

        if (one)
            root = root->right;
        else 
            root = root->left;


        if ( (nullptr == root->left) &&
             (nullptr == root->right)) {
                wHelper.append_one(&root->char_value, 1);
                ++tot_write_bytes;
                root = rt;
        }
    }
    spdlog::info("total write {} bytes", tot_write_bytes);



    delete rt;
    return true;
}


void HuffZipper::build_encode_table(HuffTree rt, uint16_t bit_cnt, uint32_t &bit_con, HuffEncodeTable het)
{
    if (rt == nullptr)
        return;
    
    if (rt->left == nullptr && rt->right == nullptr) {
        int idx = rt->char_value;
        het[idx].bit_cnt = bit_cnt;
        het[idx].bit_str = bit_con;
        het[idx].char_value = rt->char_value; 

        return;
    }

    if ( rt->left ) {
        bit_con &= ~(1 << bit_cnt);
        build_encode_table(rt->left, bit_cnt + 1, bit_con, het);
    }
    if ( rt-> right ){
        bit_con ^= (1 << bit_cnt);
        build_encode_table(rt->right, bit_cnt + 1, bit_con, het);
    }
}

HuffTree HuffZipper::build_huff_tree(FileHelper &fHelper)
{
    int ch;
    uint16_t char_type_cnt = 0;
    
    uint32_t char_freq[UINT8_MAX + 1];
    memset(char_freq, 0,sizeof(char_freq));
    uint32_t tot = 0;

    while ((ch = fHelper.put_one_char()) != EOF) {
        if ( !char_freq[ch])
            char_type_cnt++;
        char_freq[ch]++;
        ++tot;
    }
    spdlog::info("input file's total char: {}", tot);    

    std::priority_queue<HuffTree, std::vector< HuffTree>, HuffNodeCmp> pq;
    for (int i = 0; i <= UINT8_MAX; i++) {
        if ( char_freq[i] ) {
            auto node = new HuffTreeNode(static_cast<uint8_t>(i), char_freq[i], nullptr, nullptr);
            pq.push(node);
        }
    }


    HuffTree rt = nullptr;
    while (!pq.empty()) {

        auto f = pq.top();pq.pop();
        if (pq.empty()) {
            rt = f;
            break;
        }
        auto s = pq.top(); pq.pop();

        auto mergeNode = new HuffTreeNode( static_cast<uint16_t>(256), f->char_freq + s->char_freq, s, f);
        pq.push( mergeNode );
    }


    // handle just one type char
    if ( (nullptr == rt->left) && (nullptr == rt->right)) {
        auto node = new HuffTreeNode();
        node->left = rt;
        return node;
    }



    return rt;
}


void HuffZipper::build_huff_tree_from(HuffTree root,const HuffEncodeItem &item)
{
    auto bit_cnt = item.bit_cnt;
    auto bit_con = item.bit_str;
    auto char_val = item.char_value;

    for (int i = 0;i < bit_cnt; ++i, bit_con >>= 1) {

        if (bit_con & 1) {
            if (nullptr == root->right) {
                auto node = new  HuffTreeNode(UINT8_MAX + 1, 0, nullptr, nullptr);
                root->right = node;
                if (i == (bit_cnt - 1)) {
                    node->char_value = char_val;
                }
            }
            root = root->right;
        }
        else {
            if (nullptr == root->left) {
                auto node = new  HuffTreeNode(UINT8_MAX + 1, 0, nullptr, nullptr);
                root->left = node;
                if (i == (bit_cnt - 1)) {
                    node->char_value = char_val;
                }
            }
            root = root->left;
        }
    }
}



void HuffZipper::print_huf_tree(HuffTree rt,std::string &str)
{
    if ( rt == nullptr)
        return ;
    
    if ( (nullptr == rt->left) && (nullptr == rt->right)) {
        printf("%c: %s\n", rt->char_value, str.c_str());
        return ;
    }
    if ( rt->left) {
        str.push_back('0');
        print_huf_tree(rt->left, str);
        str.pop_back();
    }
    if ( rt->right ) {
        str.push_back('1');
        print_huf_tree(rt->right, str);
        str.pop_back();    
    }

}

void HuffZipper::show_encode_table(const HuffEncodeTable r) 
{
    std::string p;
    for (int i = 0; i <= UINT8_MAX; ++i) {
        if ( r[i].bit_cnt) {
            auto cnt = r[i].bit_cnt;
            auto str = r[i].bit_str;
            for (int i = 0;i < cnt; ++i, str >>= 1) {
                if ( str & 1)
                    p.push_back('1');
                else
                    p.push_back('0');
            }
        
            spdlog::info("char: {:c}, bit_cnt: {:s}", i, p.c_str());
            p.clear();
        }
    }    
}


uint64_t HuffZipper::get_encode_bits_len(const HuffTree r, const HuffEncodeTable t)
{
    if (r == nullptr)
        return 0;
    if ( (nullptr == r->left) && (nullptr == r->right)) {
        return (uint64_t) r->char_freq * (uint64_t) t[r->char_value].bit_cnt;
    }
    return get_encode_bits_len(r->left, t) + get_encode_bits_len(r->right, t); 
}