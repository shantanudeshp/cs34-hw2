#include <gtest/gtest.h>

#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(XMLReader, BasicReadEntities) {
    auto src = std::make_shared<CStringDataSource>(
        "<root a=\"1\"><child>hi</child></root>"
    );
    CXMLReader reader(src);

    SXMLEntity e;

    ASSERT_TRUE(reader.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(e.DNameData, "root");
    EXPECT_TRUE(e.AttributeExists("a"));
    EXPECT_EQ(e.AttributeValue("a"), "1");

    ASSERT_TRUE(reader.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(e.DNameData, "child");

    ASSERT_TRUE(reader.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(e.DNameData, "hi");

    ASSERT_TRUE(reader.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(e.DNameData, "child");

    ASSERT_TRUE(reader.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(e.DNameData, "root");

    EXPECT_TRUE(reader.End());
}

TEST(XMLReader, SkipCharData) {
    auto src = std::make_shared<CStringDataSource>(
        "<root><child>hi</child></root>"
    );
    CXMLReader reader(src);

    SXMLEntity e;

    ASSERT_TRUE(reader.ReadEntity(e, true));
    EXPECT_EQ(e.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(e.DNameData, "root");

    ASSERT_TRUE(reader.ReadEntity(e, true));
    EXPECT_EQ(e.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(e.DNameData, "child");

    // CharData "hi" should be skipped
    ASSERT_TRUE(reader.ReadEntity(e, true));
    EXPECT_EQ(e.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(e.DNameData, "child");

    ASSERT_TRUE(reader.ReadEntity(e, true));
    EXPECT_EQ(e.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(e.DNameData, "root");
}

TEST(XMLWriter, WritesAndEscapes) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity root;
    root.DType = SXMLEntity::EType::StartElement;
    root.DNameData = "root";
    root.SetAttribute("a", "1&2");

    SXMLEntity child;
    child.DType = SXMLEntity::EType::StartElement;
    child.DNameData = "child";

    SXMLEntity text;
    text.DType = SXMLEntity::EType::CharData;
    text.DNameData = "x < y & z";

    SXMLEntity endChild;
    endChild.DType = SXMLEntity::EType::EndElement;
    endChild.DNameData = "child";

    SXMLEntity endRoot;
    endRoot.DType = SXMLEntity::EType::EndElement;
    endRoot.DNameData = "root";

    ASSERT_TRUE(writer.WriteEntity(root));
    ASSERT_TRUE(writer.WriteEntity(child));
    ASSERT_TRUE(writer.WriteEntity(text));
    ASSERT_TRUE(writer.WriteEntity(endChild));
    ASSERT_TRUE(writer.WriteEntity(endRoot));
    ASSERT_TRUE(writer.Flush());

    // Check escaping
    EXPECT_EQ(
        sink->String(),
        "<root a=\"1&amp;2\"><child>x &lt; y &amp; z</child></root>"
    );
}

TEST(XMLWriter, FlushClosesOpenTags) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity root;
    root.DType = SXMLEntity::EType::StartElement;
    root.DNameData = "root";

    SXMLEntity child;
    child.DType = SXMLEntity::EType::StartElement;
    child.DNameData = "child";

    ASSERT_TRUE(writer.WriteEntity(root));
    ASSERT_TRUE(writer.WriteEntity(child));

    ASSERT_TRUE(writer.Flush());

    EXPECT_EQ(sink->String(), "<root><child></child></root>");
}
