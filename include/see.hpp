/**
 * Copyright (c) 2020 Heachen Bear & Contributors
 * File: see.hpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 09.02.2021
 * Last Modified Date: 08.05.2021
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

class MkdHighlight;

// interface of markdown block highlight
struct MkdBlock
{
    MkdHighlight& mediator_;

    virtual ~MkdBlock();
    explicit MkdBlock(const std::string& name);

    virtual bool matchBegin(const std::string& oneline) =0;
    virtual bool matchEnd(const std::string& oneline) =0;
    virtual std::string& highlight(std::string& text) =0;
};


// Mediator Pattern: Mediator
class MkdHighlight
{
public:
    using Register = std::unordered_map<std::string, MkdBlock*>;
    using AllBlocks = std::pair<Register::const_iterator, Register::const_iterator>;

private:
    winsize     winsize_;
    Register    blocks_;

public:
    int getTtyRow() const noexcept;
    int getTtyCol() const noexcept;
    bool regist(const std::string& name, MkdBlock* block);
    MkdBlock& getBlock(const std::string& name);
    AllBlocks getAllBlocks() const;
    std::string& highlight(std::string& text);

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
