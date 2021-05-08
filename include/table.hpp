#if !defined(MRBEARDAD_SEE_TABLE_HPP)
#define MRBEARDAD_SEE_TABLE_HPP


#include "normal.hpp"
#include "see.hpp"


class TableBlk : public see::MkdBlock
{
    static TableBlk Instance_;

public:
    TableBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string& oneline) override;
    std::string& highlight(std::string& text) override;
};
inline TableBlk TableBlk::Instance_{};


#endif // !defined(MRBEARDAD_SEE_TABLE_HPP)
