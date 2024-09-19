#include <cstdio>
#include <gtest/gtest.h>

#include "TimeCost.h"
#include "HuffZipper.h"

int main(int argc, char *argv[])
{
    //::testing::InitGoogleTest(&argc, argv);
    //RUN_ALL_TESTS();
    spdlog::set_level(spdlog::level::err);
    TimeCost ts;

    HuffZipper zipper;
    if (false == zipper.zip(argv[1], "list.huf")) {
        spdlog::error("zip file failed");
    }
    spdlog::info("-------");
    spdlog::info("zip end");
    
    if (! zipper.unzip("list.huf", "list.nst") ) {
        spdlog::error("unzip failed");
    }


    // FileHelper helper;
    // helper.open(argv[1], "r");

    // int ch;
    // while ((ch = helper.put_one_char()) != EOF) {
    //     printf("%c", ch);
    // }

    return 0;
}