/**
 * Copyright (c) 2020-2021 Heachen Bear & Contributors
 * File: normal.hpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 07.03.2021
 * Last Modified Date: 08.05.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */

#if !defined(MRBEARDAD_SEE_NORMAL_HPP)
#define MRBEARDAD_SEE_NORMAL_HPP


#include "see.hpp"

namespace see
{


class NormalBlk : public see::MkdBlock
{
    static NormalBlk Instance_;

public:
    NormalBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string&) override;
    virtual std::string& highlight(std::string& text) override;
    std::string& highlight(std::string& text, const std::string& bgColor);
};
inline NormalBlk NormalBlk::Instance_{};


} // namespace see


#endif // !defined(MRBEARDAD_SEE_NORMAL_HPP)
