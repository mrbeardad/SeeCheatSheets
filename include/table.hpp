#ifndef MRBEARDAD_SEE_TABLE_HPP
#define MRBEARDAD_SEE_TABLE_HPP


#include "normal.hpp"


namespace see {


class TableBlk : public MkdBlock
{
    static TableBlk Instance_;

public:
    TableBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string& oneline) override;
    std::string& highlight(std::string& text) override;
};
inline TableBlk TableBlk::Instance_{};


} // namespace see


#endif // !defined(MRBEARDAD_SEE_TABLE_HPP)
