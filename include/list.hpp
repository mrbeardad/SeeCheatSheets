#if !defined(MRBEARDAD_SEE_LIST_HPP)
#define MRBEARDAD_SEE_LIST_HPP


#include "see.hpp"


class ListBlk : public see::MkdBlock
{
    static ListBlk Instance_;

public:
    ListBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string &oneline) override;
    std::string& highlight(std::string& text) override;
};
inline ListBlk ListBlk::Instance_{};


#endif // !defined(MRBEARDAD_SEE_LIST_HPP)
