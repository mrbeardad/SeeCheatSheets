#ifndef MRBEARDAD_SEE_COMMENT_HPP
#define MRBEARDAD_SEE_COMMENT_HPP


#include "mkdblock.hpp"


namespace see {


class CommentBlk : public MkdBlock
{
    static CommentBlk Instance_;

public:
    CommentBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string&) override;
    std::string& highlight(std::string& text) override;
};
inline CommentBlk CommentBlk::Instance_{};


} // namespace see


#endif // MRBEARDAD_SEE_COMMENT_HPP
