#ifndef MRBEARDAD_SEE_COMMENT_HPP
#define MRBEARDAD_SEE_COMMENT_HPP


#include "see.hpp"


class CommentBlk : public see::MkdBlock
{
    static CommentBlk Instance_;

public:
    CommentBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string&) override;
    std::string& highlight(std::string& text) override;
};
inline CommentBlk CommentBlk::Instance_{};


#endif // MRBEARDAD_SEE_COMMENT_HPP
