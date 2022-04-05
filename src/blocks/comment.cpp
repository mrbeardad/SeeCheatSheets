#include "comment.hpp"


namespace see {


CommentBlk::CommentBlk()
    : MkdBlock{"comment"} {  }


bool CommentBlk::matchBegin(const std::string& oneline)
{
    std::string endsNL{};
    if ( oneline.back() == '\n' )
        endsNL = "\n";
    return oneline.starts_with("<!--") && oneline.ends_with("-->" + endsNL);
}


bool CommentBlk::matchEnd(const std::string&)
{
    return true;
}


std::string& CommentBlk::highlight(std::string& text)
{
    text.clear();
    return text;
}


} // namespace see
