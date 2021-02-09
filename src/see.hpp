/************************************************
 * see.cpp -- A program to search cheat sheet
 * Copyright (c) 2020 Heachen Bear
 * Author: Heachen Bear < mrbeardad@qq.com >
 * License: GPLv3
 ************************************************/

#pragma once
#ifndef MRBEARDAD_SEE_HPP
#define MRBEARDAD_SEE_HPP

#include <filesystem>
#include <iosfwd>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "mine.hpp"

namespace see
{

class MkdHighlight;

// 职责：markdown区块接口、普通段落的语法高亮、终端窗口大小
class MkdBlock
{
    MkdHighlight&   mediator_;
    std::string     bgColor_;
public:
    explicit MkdBlock(MkdHighlight& mediator, const std::string& bgColor = "\033[m");
    virtual ~MkdBlock();
    MkdHighlight& mediator();
    std::string set_bg_color(const std::string& color);
    virtual bool match_begin(const std::string& oneline);
    virtual bool match_end(const std::string& oneline);
    virtual std::string& highlight(std::string& text);

    static int Row;
    static int Col;
    static bool init_winsize();
    static std::unordered_map<std::string, std::pair<std::regex, std::string> > Emphasizes;
};

// 表观模式 + 中介者模式
class MkdHighlight
{
    std::unordered_map<std::string, std::shared_ptr<MkdBlock> > blocks;
public:
    MkdHighlight();
    std::string highlight(std::istream& text);
    MkdBlock& get_block(const std::string& name);
};

void print_help_then_exit() noexcept;

std::tuple<std::vector<fs::path>, std::vector<std::string>, bool>
parse_cmdline(int argc, char* argv[]) noexcept;

std::stringstream search_entries(const std::vector<fs::path>& files, const std::vector<std::string>& keys) noexcept;

} // namespace see

#endif // MRBEARDAD_SEE_HPP
