#include "DSVWriter.h"

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink;
    char DDelimiter;
    bool DQuoteAll;
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->DSink = sink;
    DImplementation->DDelimiter = (delimiter == '"') ? ',' : delimiter;
    DImplementation->DQuoteAll = quoteall;
}

CDSVWriter::~CDSVWriter() = default;

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    return false;
}
