#include <gtest/gtest.h>
#include "normal.hpp"
#include "see.hpp"

#include "mine.hpp"

// std::string str("*italic*");
// //                                                          区块边界恢复
// //                                                      区块边界清除 |
// //                                               栈顶内联块恢复  |   |
// //                           内联块头部      内联块尾部清除  |   |   |
// //                      区块边界头部 |                   |   |   |   |
// //                               V   V                   V   V   V   V
// EXPECT_EQ(hilit.highlight(str), "\e[m\e[3;38;5;255mitalic\e[m\e[m\e[m\e[m");

TEST(TestNormal, testItalicBold)
{
    auto& hilit = see::MkdHighlight::Instance_.getBlock("normal");

    EXPECT_EQ(dynamic_cast<see::NormalBlk&>(hilit).highlight("**bold**", "\e[31m"), "\e[31m\e[1;38;5;255mbold\e[m\e[31m\e[m\e[31m");
    EXPECT_EQ(hilit.highlight("**bold**"), "\e[m\e[1;38;5;255mbold\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("***italicbold***"), "\e[m\e[1;3;38;5;255mitalicbold\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("*italic***bold*****italicbold***"), "\e[m\e[3;38;5;255mitalic\e[m\e[m\e[1;38;5;255mbold\e[m\e[m\e[1;3;38;5;255mitalicbold\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("**escape*one**"), "\e[m\e[1;38;5;255mescape*one\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("not seperator*****"), "\e[mnot seperator\e[1;3;38;5;255m**\e[m\e[m"); // 内联块未闭合
}

TEST(TestNormal, testDel)
{
    auto& hilit = see::MkdHighlight::Instance_.getBlock("normal");

    EXPECT_EQ(hilit.highlight("~del~"), "\e[m~del~\e[m\e[m");
    EXPECT_EQ(hilit.highlight("~~del~~"), "\e[m\e[9mdel\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("~~~~"), "\e[m\e[9m\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("~~~"), "\e[m\e[9m~\e[m\e[m"); // 内联块未闭合
}

TEST(TestNormal, testUndAndKbd)
{
    auto& hilit = see::MkdHighlight::Instance_.getBlock("normal");

    EXPECT_EQ(hilit.highlight("<u>underline</u>"), "\e[m\e[4munderline\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("<kbd>keyboard</kbd>"), "\e[m\e[7mkeyboard\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("<u>und<kbd>kbd</kbd>erline</u>"), "\e[m\e[4mund\e[7mkbd\e[m\e[4merline\e[m\e[m\e[m\e[m");
}

TEST(TestNormal, testCode)
{
    auto& hilit = see::MkdHighlight::Instance_.getBlock("normal");

    EXPECT_EQ(hilit.highlight("`code`"), "\e[m\e[38;2;255;168;0mcode\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("``co`de``"), "\e[m\e[38;2;255;168;0mco`de\e[m\e[m\e[m\e[m");
}

TEST(TestNormal, testComment)
{
    auto& hilit = see::MkdHighlight::Instance_.getBlock("normal");

    EXPECT_EQ(hilit.highlight("<!-- <!-- comment --> -->"), "\e[m\e[38;5;245m<!-- <!-- comment -->\e[m\e[m -->\e[m\e[m");
}

TEST(TestNormal, testLink)
{
    auto& hilit = see::MkdHighlight::Instance_.getBlock("normal");

    EXPECT_EQ(hilit.highlight("[link](https://example.org)"), "\e[m\e[33m[\e[m\e[mlink\e[33m](\e[m\e[4;34mhttps://example.org\e[m\e[33m)\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("![link](https://example.org)"), "\e[m\e[33m![\e[m\e[mlink\e[33m](\e[m\e[4;34mhttps://example.org\e[m\e[33m)\e[m\e[m\e[m\e[m");
    EXPECT_EQ(hilit.highlight("[link] (https://example.org)"), "\e[m[link] (https://example.org)\e[m\e[m");
    EXPECT_EQ(hilit.highlight("[link](https://example.org"), "\e[m[link](https://example.org\e[m\e[m");
}
