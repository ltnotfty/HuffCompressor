#ifndef HUFF_TREE_H
#define HUFF_TREE_H

#include <cstdint>

struct HuffTreeNode_
{
    uint16_t char_value;
    uint32_t char_freq;
   
    struct HuffTreeNode_ *left;
    struct HuffTreeNode_ *right;



    HuffTreeNode_(uint16_t cv, float cf,  struct HuffTreeNode_ *l,  struct HuffTreeNode_ *r):
        char_value(cv), char_freq(cf), left(l), right(r)
        {}
    HuffTreeNode_():char_value(0),char_freq(0),left(nullptr),right(nullptr)
    {}
    ~HuffTreeNode_()
    {
        delete this->left;
        delete this->right;
    }

};

using HuffTree = struct HuffTreeNode_ *;
using HuffTreeNode = struct HuffTreeNode_;

struct HuffNodeCmp {
    bool operator()(HuffTree a,  HuffTree b) const{
        return a->char_freq > b->char_freq;
    }
};


#endif