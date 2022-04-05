#ifndef MRBEARDAD_SEE_LIST_HPP
#define MRBEARDAD_SEE_LIST_HPP


#include "mkdblock.hpp"


namespace see {


class ListBlk : public MkdBlock
{
    static ListBlk Instance_;

public:
    ListBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string &oneline) override;
    std::string& highlight(std::string& text) override;
};
inline ListBlk ListBlk::Instance_{};


} // namespace see


#endif // MRBEARDAD_SEE_LIST_HPP
