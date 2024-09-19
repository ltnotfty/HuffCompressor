#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <cstdint>
#include <string>
#include <spdlog/spdlog.h>
#include <sys/stat.h>

class FileHelper
{

public:
    FileHelper();
    ~FileHelper();
    bool open(const char *file, const char *mode);
    void close();
    std::size_t get_file_sz();
    std::string get_file_type();
    int put_one_char();
    void reset();

    // void *ptr, size_t size, size_t nmemb, FILE *stream
    bool read(void *ptr, size_t sz, size_t nmemb, size_t offset);
    bool read_next_one(void *ptr, size_t sz);

    bool append(void *ptr, size_t sz, size_t nmemb);
    bool append_one(void *ptr, size_t sz);
    

    bool write(void *ptr, size_t sz, size_t nmemb, size_t offset);

    bool modify(void *ptr, size_t sz, size_t nmemb, size_t offset);
    
friend class BitBufferWriter;
friend class BitBufferReader;
private:
    std::string cur_file_name;
    FILE *fp;

    struct stat file_status;
};
#endif