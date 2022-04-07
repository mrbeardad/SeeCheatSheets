#include "comment.hpp"

namespace mkdhilit {

CommentBlk::CommentBlk() : MkdBlock{"comment"} {}

bool CommentBlk::matchBegin(const std::string& oneline) {
    std::string endsNL{};
    if (oneline.back() == '\n')
        endsNL = "\n";
    return oneline.find("<!--") == 0 &&
           oneline.rfind("-->" + endsNL) ==
               oneline.length() - std::string{"-->" + endsNL}.length() + 1;
}

bool CommentBlk::matchEnd(const std::string&) {
    return true;
}

std::string& CommentBlk::highlight(std::string& text) {
    text.clear();
    return text;
}

}  // namespace mkdhilit
