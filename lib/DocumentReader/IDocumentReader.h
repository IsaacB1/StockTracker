#include <iostream>
#include <string>
#include <vector>

#ifndef IDOCUMENTREADER_H
#define IDOCUMENTREADER_H

class IDocumentReader {
    public:
        void readInDocument(const std::string& filePath);
        ~IDocumentReader() = default;
};

#endif