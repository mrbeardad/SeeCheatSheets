#include "utils/mine.hpp"

#include "list.hpp"
#include "mkdhilit.hpp"

namespace mkdhilit {

ListBlk::ListBlk() : mkdhilit::MkdBlock{"list"} {}

bool ListBlk::matchBegin(const std::string& oneline) {
    size_t idx{};
    for (; idx < oneline.size() && std::isspace(oneline[idx]); ++idx)
        ;
    if (oneline[idx] != '*' && oneline[idx] != '-' && oneline[idx] != '+')
        return false;
    char headChar{oneline[idx]};
    size_t origIdx{++idx};
    for (; idx < oneline.size() && std::isspace(oneline[idx]); ++idx)
        ;
    if (origIdx == idx)
        return false;
    int headCharCount{1};
    bool anyOtherChar{};
    for (; idx < oneline.size(); ++idx) {
        if (oneline[idx] == headChar)
            ++headCharCount;
        else if (!std::isspace(oneline[idx]))
            anyOtherChar = true;
    }
    return anyOtherChar || headCharCount < 3;
}

bool ListBlk::matchEnd(const std::string& oneline) {
    return oneline.empty() ||
           mediator_.getBlock("header").matchBegin(oneline) ||
           mediator_.getBlock("seperator").matchBegin(oneline) ||
           mediator_.getBlock("quote").matchBegin(oneline) ||
           mediator_.getBlock("code").matchBegin(oneline) ||
           mediator_.getBlock("comment").matchBegin(oneline);
}

std::string& ListBlk::highlight(std::string& text) {
    std::string origText{std::move(text)};
    text.clear();
    mine::Split getLine{origText, '\n', true};
    for (std::string_view onelineView{}; getLine(onelineView);) {
        std::string head{};
        std::string normal{};
        auto headCharIdx = onelineView.find_first_of("+-*");
        if (headCharIdx == std::string::npos ||
            !std::isspace(onelineView[headCharIdx + 1])) {
            normal = onelineView;
        } else {
            head << "\033[36m" << onelineView.substr(0, headCharIdx + 1)
                 << "\033[m";
            normal = onelineView.substr(headCharIdx + 1);
        }
        mediator_.getBlock("normal").highlight(normal);
        text += head + normal;
    }
    return text;
}

}  // namespace mkdhilit
