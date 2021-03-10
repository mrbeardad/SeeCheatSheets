/**
 * Copyright (c) 2020-2021 Heachen Bear & Contributors
 * File: normal.hpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 07.03.2021
 * Last Modified Date: 09.03.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */

#if !defined(MRBEARDAD_SEE_NORMAL_HPP)
#define MRBEARDAD_SEE_NORMAL_HPP


#include "see.hpp"

namespace see
{


class NormalBlk : public see::MkdBlock
{
public:
    NormalBlk(): see::MkdBlock{"normal"} {  }


    bool match_begin(const std::string& oneline) override
    {
        const auto& allBlk = see::MkdHighlight::Instance_.get_all_blocks();
        for( auto itr = allBlk.begin(), end = allBlk.end(); itr != end; ++itr ) {
            if ( itr->first != "normal" && itr->second->match_begin(oneline) ) {
                return false;
            }
        }
        return true;
    }


    bool match_end(const std::string&) override
    {
        return true;
    }


    using MkdBlock::highlight;

    virtual std::string& highlight(std::string& text) override
    {
        return highlight(text, "\e[m");
    }


    std::string& highlight(std::string& text, const std::string& bgColor);

    std::string highlight(std::string&& text, const std::string& bgColor)
    {
        return highlight(text, bgColor);
    }

private:
    static NormalBlk Instance_;
};


} // namespace see

#endif // !defined(MRBEARDAD_SEE_NORMAL_HPP)
