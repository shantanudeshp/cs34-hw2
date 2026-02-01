#include <gtest/gtest.h>
#include "DSVWriter.h"
#include "DSVReader.h"
#include "StringDataSink.h"
#include "StringDataSource.h"

TEST(DSVWriter, SingleField){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    EXPECT_TRUE(Writer.WriteRow({"hello"}));
    EXPECT_EQ(Sink->String(), "hello\n");
}

TEST(DSVWriter, MultipleFields){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    EXPECT_TRUE(Writer.WriteRow({"a","b","c"}));
    EXPECT_EQ(Sink->String(), "a,b,c\n");
}

TEST(DSVWriter, EmptyRow){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    EXPECT_TRUE(Writer.WriteRow({}));
    EXPECT_EQ(Sink->String(), "\n");
}

TEST(DSVWriter, FieldWithDelimiter){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    EXPECT_TRUE(Writer.WriteRow({"a,b","c"}));
    EXPECT_EQ(Sink->String(), "\"a,b\",c\n");
}

TEST(DSVWriter, FieldWithQuote){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    EXPECT_TRUE(Writer.WriteRow({"a\"b","c"}));
    EXPECT_EQ(Sink->String(), "\"a\"\"b\",c\n");
}

TEST(DSVWriter, FieldWithNewline){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    EXPECT_TRUE(Writer.WriteRow({"a\nb","c"}));
    EXPECT_EQ(Sink->String(), "\"a\nb\",c\n");
}

TEST(DSVWriter, QuoteAll){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',', true);
    EXPECT_TRUE(Writer.WriteRow({"a","b"}));
    EXPECT_EQ(Sink->String(), "\"a\",\"b\"\n");
}

TEST(DSVWriter, QuoteAllEscape){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',', true);
    EXPECT_TRUE(Writer.WriteRow({"a\"b","c"}));
    EXPECT_EQ(Sink->String(), "\"a\"\"b\",\"c\"\n");
}

TEST(DSVWriter, TabDelimiter){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, '\t');
    EXPECT_TRUE(Writer.WriteRow({"a","b"}));
    EXPECT_EQ(Sink->String(), "a\tb\n");
}

TEST(DSVWriter, QuoteDelimiter){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, '"');
    EXPECT_TRUE(Writer.WriteRow({"a","b"}));
    EXPECT_EQ(Sink->String(), "a,b\n");
}

TEST(DSVWriter, EmptyFields){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    EXPECT_TRUE(Writer.WriteRow({"","a",""}));
    EXPECT_EQ(Sink->String(), ",a,\n");
}

TEST(DSVReader, SingleField){
    auto Source = std::make_shared<CStringDataSource>("hello\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"hello"}));
    EXPECT_TRUE(Reader.End());
}

TEST(DSVReader, MultipleFields){
    auto Source = std::make_shared<CStringDataSource>("a,b,c\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a","b","c"}));
}

TEST(DSVReader, QuotedField){
    auto Source = std::make_shared<CStringDataSource>("\"a,b\",c\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a,b","c"}));
}

TEST(DSVReader, EscapedQuote){
    auto Source = std::make_shared<CStringDataSource>("\"a\"\"b\",c\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a\"b","c"}));
}

TEST(DSVReader, NewlineInQuoted){
    auto Source = std::make_shared<CStringDataSource>("\"a\nb\",c\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a\nb","c"}));
}

TEST(DSVReader, EmptyRow){
    auto Source = std::make_shared<CStringDataSource>("\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), (size_t)0);
}

TEST(DSVReader, EmptyFields){
    auto Source = std::make_shared<CStringDataSource>(",a,\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"","a",""}));
}

TEST(DSVReader, MultipleRows){
    auto Source = std::make_shared<CStringDataSource>("a,b\nc,d\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a","b"}));
    EXPECT_FALSE(Reader.End());
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"c","d"}));
    EXPECT_TRUE(Reader.End());
}

TEST(DSVReader, NoTrailingNewline){
    auto Source = std::make_shared<CStringDataSource>("a,b");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a","b"}));
}

TEST(DSVRoundTrip, Basic){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    Writer.WriteRow({"hello","world"});
    Writer.WriteRow({"foo","bar","baz"});

    auto Source = std::make_shared<CStringDataSource>(Sink->String());
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;

    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"hello","world"}));
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"foo","bar","baz"}));
    EXPECT_TRUE(Reader.End());
}

TEST(DSVRoundTrip, SpecialChars){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    Writer.WriteRow({"a,b","c\"d","e\nf"});

    auto Source = std::make_shared<CStringDataSource>(Sink->String());
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;

    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a,b","c\"d","e\nf"}));
}

TEST(DSVReader, QuoteDelimiter){
    auto Source = std::make_shared<CStringDataSource>("a,b\n");
    CDSVReader Reader(Source, '"');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, std::vector<std::string>({"a","b"}));
}
