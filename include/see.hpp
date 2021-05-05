/**
 * Copyright (c) 2020 Heachen Bear & Contributors
 * File: see.hpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 09.02.2021
 * Last Modified Date: 29.04.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */

#ifndef MRBEARDAD_SEE_HPP
#define MRBEARDAD_SEE_HPP

#include <sys/ioctl.h>

#include <filesystem>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "mine.hpp"


namespace see
{


// interface of markdown block highlight
class MkdBlock
{
    std::string registName_;
public:
    explicit MkdBlock(std::string name);
    virtual ~MkdBlock() =default;

    virtual bool match_begin(const std::string& oneline) =0;
    virtual bool match_end(const std::string& oneline) =0;
    virtual std::string highlight(const std::string& text) =0;
};


// catch the tty window size
class WinSize
{
    winsize winsize_;
public:
    WinSize();
    int get_row() const noexcept;
    int get_col() const noexcept;
};


// Mediator Pattern: Mediator
class MkdHighlight
{
public:
    using Register = std::unordered_map<std::string, MkdBlock*>;
    using AllBlocks = std::pair<Register::const_iterator, Register::const_iterator>;

private:
    WinSize     winsize_;
    Register    blocks_;

public:
    int get_tty_row() const noexcept;
    int get_tty_col() const noexcept;
    bool regist(std::string name, MkdBlock* block);
    MkdBlock& get_block(const std::string& name);
    AllBlocks get_all_blocks() const;
    std::string& highlight(std::string& text);

    // Singleton Pattern: Singleton
    static MkdHighlight& Instance();

private:
    MkdHighlight();
};


std::tuple<std::vector<fs::path>, std::vector<std::string>, bool>
parse_cmdline(int argc, char* argv[]);


std::string
search_entries(const std::vector<fs::path>& files, const std::vector<std::string>& keys);


} // namespace see

#endif // MRBEARDAD_SEE_HPP

