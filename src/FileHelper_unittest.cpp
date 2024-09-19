#include "FileHelper.h"
#include <gtest/gtest.h>
#include <endian.h>


TEST(fHelper, openFile) {
    FileHelper s;
    EXPECT_EQ(s.open("hello", "r"), false) << "open regular file";
    EXPECT_EQ(s.open("list.txt","r"), true) << "open a not exist file";

}
TEST(fHelper, readFile) {
    FileHelper s;
    s.open("list.txt", "r");
    EXPECT_EQ(s.get_file_sz(), 26) << "get file size";
    EXPECT_STREQ(s.get_file_type().c_str(), "regular file\n") << "get file type string";
}

TEST(fHelper, writeFile) {
    
    // FileHelper s;
    // s.open("list.txt", "rw+");
    // int sz_0 = s.get_file_sz();
    
    // int wval = 0x01020304;


    // s.append_one(&wval, sizeof(sz_0));
    // s.close();

    // s.open("list.txt", "r");
    
    // uint8_t val;
    // s.read(&val, sizeof(uint8_t), 1, 26);
    // EXPECT_EQ(val, 4);


}