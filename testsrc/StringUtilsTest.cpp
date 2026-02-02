#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(StringUtils::Slice("", 0), "");
    EXPECT_EQ(StringUtils::Slice("ponyo", 0, 3), "pon");
    EXPECT_EQ(StringUtils::Slice("unlimited void", 0), "unlimited void");
    EXPECT_EQ(StringUtils::Slice("paladin", 3), "adin");
    EXPECT_EQ(StringUtils::Slice("shanty", -2), "ty");
    EXPECT_EQ(StringUtils::Slice("snake plant", 1, -1), "nake plan");
    EXPECT_EQ(StringUtils::Slice("aggie", -5), "aggie");
    EXPECT_EQ(StringUtils::Slice("cleave", 2, 4), "ea");
    EXPECT_EQ(StringUtils::Slice("monstera", -3, -1), "er");
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(StringUtils::Capitalize(""), "");
    EXPECT_EQ(StringUtils::Capitalize("totoro"), "Totoro");
    EXPECT_EQ(StringUtils::Capitalize("AGGIE PRIDE"), "Aggie pride");
    EXPECT_EQ(StringUtils::Capitalize("fireball"), "Fireball");
    EXPECT_EQ(StringUtils::Capitalize("snake plant"), "Snake plant");
    EXPECT_EQ(StringUtils::Capitalize("SHANTY"), "Shanty");
    EXPECT_EQ(StringUtils::Capitalize("howl's moving castle"), "Howl's moving castle");
    EXPECT_EQ(StringUtils::Capitalize("bard"), "Bard");
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(StringUtils::Upper(""), "");
    EXPECT_EQ(StringUtils::Upper("domain expansion"), "DOMAIN EXPANSION");
    EXPECT_EQ(StringUtils::Upper("Shanty"), "SHANTY");
    EXPECT_EQ(StringUtils::Upper("ryomen sukuna"), "RYOMEN SUKUNA");
    EXPECT_EQ(StringUtils::Upper("monstera deliciosa"), "MONSTERA DELICIOSA");
    EXPECT_EQ(StringUtils::Upper("gojo satoru"), "GOJO SATORU");
    EXPECT_EQ(StringUtils::Upper("nah, i'd win"), "NAH, I'D WIN");
    EXPECT_EQ(StringUtils::Upper("purple!"), "PURPLE!");
    EXPECT_EQ(StringUtils::Upper("123"), "123");
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(StringUtils::Lower(""), "");
    EXPECT_EQ(StringUtils::Lower("LIMITLESS"), "limitless");
    EXPECT_EQ(StringUtils::Lower("Shantanu"), "shantanu");
    EXPECT_EQ(StringUtils::Lower("BINDING VOW"), "binding vow");
    EXPECT_EQ(StringUtils::Lower("pothos"), "pothos");
    EXPECT_EQ(StringUtils::Lower("MALEVOLENT SHRINE"), "malevolent shrine");
    EXPECT_EQ(StringUtils::Lower("Ficus Elastica"), "ficus elastica");
    EXPECT_EQ(StringUtils::Lower("HOLLOW PURPLE!"), "hollow purple!");
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(StringUtils::LStrip(""), "");
    EXPECT_EQ(StringUtils::LStrip("spirited away"), "spirited away");
    EXPECT_EQ(StringUtils::LStrip("   UC Davis"), "UC Davis");
    EXPECT_EQ(StringUtils::LStrip("\t\nrogues are cool"), "rogues are cool");
    EXPECT_EQ(StringUtils::LStrip("   pothos   "), "pothos   ");
    EXPECT_EQ(StringUtils::LStrip("     "), "");
    EXPECT_EQ(StringUtils::LStrip(" \r\n\tShanty"), "Shanty");
}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(StringUtils::RStrip(""), "");
    EXPECT_EQ(StringUtils::RStrip("kiki's delivery service"), "kiki's delivery service");
    EXPECT_EQ(StringUtils::RStrip("wizard   "), "wizard");
    EXPECT_EQ(StringUtils::RStrip("monstera\t\n"), "monstera");
    EXPECT_EQ(StringUtils::RStrip("   aggie"), "   aggie");
    EXPECT_EQ(StringUtils::RStrip("     "), "");
    EXPECT_EQ(StringUtils::RStrip("shanty \r\n\t "), "shanty");
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(StringUtils::Strip(""), "");
    EXPECT_EQ(StringUtils::Strip("my neighbor totoro"), "my neighbor totoro");
    EXPECT_EQ(StringUtils::Strip("   cleric   "), "cleric");
    EXPECT_EQ(StringUtils::Strip("\t\nfiddle leaf fig\r\n"), "fiddle leaf fig");
    EXPECT_EQ(StringUtils::Strip("   shanty"), "shanty");
    EXPECT_EQ(StringUtils::Strip("davis   "), "davis");
    EXPECT_EQ(StringUtils::Strip("     "), "");
    EXPECT_EQ(StringUtils::Strip(" \t\ncursed technique\n\r "), "cursed technique");
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(StringUtils::Center("", 5), "     ");
    EXPECT_EQ(StringUtils::Center("hi", 5), " hi  ");
    EXPECT_EQ(StringUtils::Center("shanty", 10), "  shanty  ");
    EXPECT_EQ(StringUtils::Center("aggie", 5), "aggie");
    EXPECT_EQ(StringUtils::Center("gojo", 8, '-'), "--gojo--");
    EXPECT_EQ(StringUtils::Center("pothos", 11, '*'), "**pothos***");
    EXPECT_EQ(StringUtils::Center("xyz", 2), "xyz");
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(StringUtils::LJust("", 5), "     ");
    EXPECT_EQ(StringUtils::LJust("shanty", 10), "shanty    ");
    EXPECT_EQ(StringUtils::LJust("aggie", 5), "aggie");
    EXPECT_EQ(StringUtils::LJust("yuji", 8, '-'), "yuji----");
    EXPECT_EQ(StringUtils::LJust("monstera", 12, '*'), "monstera****");
    EXPECT_EQ(StringUtils::LJust("xyz", 2), "xyz");
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(StringUtils::RJust("", 5), "     ");
    EXPECT_EQ(StringUtils::RJust("shanty", 10), "    shanty");
    EXPECT_EQ(StringUtils::RJust("aggie", 5), "aggie");
    EXPECT_EQ(StringUtils::RJust("megumi", 10, '-'), "----megumi");
    EXPECT_EQ(StringUtils::RJust("ficus", 9, '*'), "****ficus");
    EXPECT_EQ(StringUtils::RJust("xyz", 2), "xyz");
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(StringUtils::Replace("", "a", "b"), "");
    EXPECT_EQ(StringUtils::Replace("chihiro", "hiro", "hiro-san"), "chihiro-san");
    EXPECT_EQ(StringUtils::Replace("eldritch blast", "blast", "power"), "eldritch power");
    EXPECT_EQ(StringUtils::Replace("shanty shanty", "shanty", "awesome"), "awesome awesome");
    EXPECT_EQ(StringUtils::Replace("aaaaa", "a", "b"), "bbbbb");
    EXPECT_EQ(StringUtils::Replace("aggie pride", "x", "y"), "aggie pride");
    EXPECT_EQ(StringUtils::Replace("infinite void", "void", ""), "infinite ");
    EXPECT_EQ(StringUtils::Replace("philodendron", "", "x"), "philodendron");
}

TEST(StringUtilsTest, Split){
    EXPECT_EQ(StringUtils::Split("", ","), std::vector<std::string>({""}));
    EXPECT_EQ(StringUtils::Split("a,b,c", ","), std::vector<std::string>({"a", "b", "c"}));
    EXPECT_EQ(StringUtils::Split("hello world", " "), std::vector<std::string>({"hello", "world"}));
    EXPECT_EQ(StringUtils::Split("one  two", ""), std::vector<std::string>({"one", "two"}));
    EXPECT_EQ(StringUtils::Split("  shanty  ", ""), std::vector<std::string>({"shanty"}));
    EXPECT_EQ(StringUtils::Split("aggie pride forever", ""), std::vector<std::string>({"aggie", "pride", "forever"}));
    EXPECT_EQ(StringUtils::Split("a::b::c", "::"), std::vector<std::string>({"a", "b", "c"}));
}

TEST(StringUtilsTest, Join){
    EXPECT_EQ(StringUtils::Join("", {}), "");
    EXPECT_EQ(StringUtils::Join(", ", {"gojo", "yuji", "megumi"}), "gojo, yuji, megumi");
    EXPECT_EQ(StringUtils::Join(" ", {"snake", "plant"}), "snake plant");
    EXPECT_EQ(StringUtils::Join("", {"a", "b", "c"}), "abc");
    EXPECT_EQ(StringUtils::Join("-", {"uc", "davis"}), "uc-davis");
    EXPECT_EQ(StringUtils::Join(", ", {"shanty"}), "shanty");
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("", 4), "");
    EXPECT_EQ(StringUtils::ExpandTabs("hello\tworld", 4), "hello   world");
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb\tc", 8), "a       b       c");
    EXPECT_EQ(StringUtils::ExpandTabs("shanty\t!", 4), "shanty  !");
    EXPECT_EQ(StringUtils::ExpandTabs("\taggie", 4), "    aggie");
    EXPECT_EQ(StringUtils::ExpandTabs("ab\tcd", 4), "ab  cd");
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("", ""), 0);
    EXPECT_EQ(StringUtils::EditDistance("same", "same"), 0);
    EXPECT_EQ(StringUtils::EditDistance("cat", "hat"), 1);
    EXPECT_EQ(StringUtils::EditDistance("sitting", "kitten"), 3);
    EXPECT_EQ(StringUtils::EditDistance("shanty", "shanty"), 0);
    EXPECT_EQ(StringUtils::EditDistance("aggie", "AGGIE", true), 0);
    EXPECT_EQ(StringUtils::EditDistance("aggie", "AGGIE", false), 5);
    EXPECT_EQ(StringUtils::EditDistance("frog", "from"), 1);
}
