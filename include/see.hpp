/**
 * Copyright (c) 2020 Heachen Bear & Contributors
 * File: see.hpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 09.02.2021
 * Last Modified Date: 07.05.2021
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

    virtual bool matchBegin(const std::string& oneline) =0;
    virtual bool matchEnd(const std::string& oneline) =0;
    virtual std::string& highlight(std::string& text) =0;

    std::string highlight(std::string&& text)
    {
        return std::move(highlight(text));
    }
};


// catch the tty window size
class WinSize
{
    winsize winsize_;
public:
    WinSize();

    int getRow() const noexcept
    {
        return winsize_.ws_row;
    }

    int getCol() const noexcept
    {
        return winsize_.ws_col;
    }
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
    int getTtyRow() const noexcept
    {
        return winsize_.getRow();
    }

    int getTtyCol() const noexcept
    {
        return winsize_.getCol();
    }

    bool regist(std::string name, MkdBlock* block);
    MkdBlock& getBlock(const std::string& name);
    AllBlocks getAllBlocks() const;
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

