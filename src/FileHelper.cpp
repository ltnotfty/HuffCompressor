#include "FileHelper.h"


FileHelper::FileHelper():cur_file_name(""),fp(nullptr)
{

}
FileHelper::~FileHelper()
{
   
    close();
}
bool FileHelper::open(const char *file, const char *mode)
{

    fp = fopen( file, mode);

    if ( fp == nullptr)  {
        spdlog::warn("{:s} not exists", file);
        return false;
    }
    
    cur_file_name = std::string(file);
    return true;
}
void FileHelper::close()
{
    if (fp) {
        fclose(fp);
        fp = nullptr;
        cur_file_name.clear();
    }
}
std::size_t FileHelper::get_file_sz()
{
    fseek(fp, 0, SEEK_END);
    return static_cast<std::size_t>(ftell(fp));
}
std::string FileHelper::get_file_type()
{

    stat( cur_file_name.c_str(),&file_status );

    std::string fileType;
    switch (file_status.st_mode & S_IFMT) {
    case S_IFBLK:  fileType = "block device\n";            break;
    case S_IFCHR:  fileType = "character device\n";        break;
    case S_IFDIR:  fileType = "directory\n";               break;
    case S_IFIFO:  fileType = "FIFO/pipe\n";               break;
    case S_IFLNK:  fileType = "symlink\n";                 break;
    case S_IFREG:  fileType = "regular file\n";            break;
    case S_IFSOCK: fileType = "socket\n";                  break;
    default:       fileType = "unknown?\n";                break;
    }

    return fileType;
}
int FileHelper::put_one_char()
{
    return fgetc(fp);
}
void FileHelper::reset()
{
    fseek(fp, 0, SEEK_SET);
}




bool FileHelper::read(void *ptr, size_t sz, size_t nmemb, size_t offset)
{
    fseek(fp, offset, SEEK_SET);
    return nmemb == fread(ptr, sz, nmemb, fp);
}


bool FileHelper::write(void *ptr, size_t sz, size_t nmemb, size_t offset)
{
    fseek(fp, offset, SEEK_SET);
    return nmemb == fwrite(ptr, sz, nmemb, fp);
}
    
bool FileHelper::modify(void *ptr, size_t sz, size_t nmemb, size_t offset)
{
    fseek(fp, offset, SEEK_SET);
    return nmemb == fwrite(ptr, sz, nmemb, fp);
}
    

bool FileHelper::append(void *ptr, size_t sz, size_t nmemb)
{
    return nmemb == fwrite(ptr, sz, nmemb, fp);
}
bool FileHelper::append_one(void *ptr, size_t sz)
{
    return 1 == fwrite(ptr, sz, 1, fp);
}


bool FileHelper::read_next_one(void *ptr, size_t sz)
{
    return 1 == fread(ptr, sz, 1, fp);
}
