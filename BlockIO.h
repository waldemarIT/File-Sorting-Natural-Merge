#ifndef BLOCKIO_H
#define BLOCKIO_H

#include "Record.h"
#include <vector>
#include <fstream>
#include <string>

class BlockIO {
public:
    BlockIO(int& reads, int& writes);
    bool openForRead(const std::string& filename);
    bool openForWrite(const std::string& filename);
    bool readRecord(Record& rec);
    void writeRecord(const Record& rec);
    void flushBuffer();
    void close();
    bool eof();

private:
    static const int BLOCK_SIZE = 10;
    std::vector<Record> buffer;
    int bufferPos;
    std::fstream file;
    bool reading;
    int& readCount;
    int& writeCount;
};

#endif // BLOCKIO_H
