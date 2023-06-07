#include <lwp.h>

u64 FileTable::Open(const char *filePath) {
    open(filePath, sdFile[++processFd]);
    return processFd;
}

void FileTable::Close(u64 fd)
{
    sdFile[fd] = 0;
}

void FileTable::Read(u64 fd, u8* buf, u64 size)
{
    if (sdFile[fd] != nullptr) {
        read(sdFile[fd], buf, size);
    }
}
