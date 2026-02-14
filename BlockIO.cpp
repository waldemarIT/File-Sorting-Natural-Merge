#include "BlockIO.h"
#include <iostream>

BlockIO::BlockIO(int& reads, int& writes) : bufferPos(0), reading(true), readCount(reads), writeCount(writes) {
    buffer.reserve(BLOCK_SIZE);
}

bool BlockIO::openForRead(const std::string& filename) {
    file.open(filename, std::ios::in | std::ios::binary);
    reading = true;
    bufferPos = 0;
    buffer.clear();
    return file.is_open();
}

bool BlockIO::openForWrite(const std::string& filename) {
    file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    reading = false;
    bufferPos = 0;
    buffer.clear();
    return file.is_open();
}

bool BlockIO::readRecord(Record& rec) {
    if (bufferPos >= (int)buffer.size()) {
        buffer.clear();
        bufferPos = 0;

        for (int i = 0; i < BLOCK_SIZE; ++i) {
            Record temp;
            file.read(reinterpret_cast<char*>(&temp), sizeof(Record));
            if (file.gcount() == sizeof(Record)) {
                buffer.push_back(temp);
            } else {
                break;
            }
        }

        if (!buffer.empty()) {
            readCount++;
        }
    }

    if (bufferPos < (int)buffer.size()) {
        rec = buffer[bufferPos++];
        return true;
    }
    return false;
}

void BlockIO::writeRecord(const Record& rec) {
    buffer.push_back(rec);
    if ((int)buffer.size() >= BLOCK_SIZE) flushBuffer();
}

void BlockIO::flushBuffer() {
    if (!buffer.empty()) {
        for (const auto& rec : buffer) {
            file.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
        }
        writeCount++;
        buffer.clear();
        bufferPos = 0;
    }
}

void BlockIO::close() {
    if (!reading) flushBuffer();
    if (file.is_open()) file.close();
    buffer.clear();
    bufferPos = 0;
}

bool BlockIO::eof() {
    return bufferPos >= (int)buffer.size() && file.eof();
}
