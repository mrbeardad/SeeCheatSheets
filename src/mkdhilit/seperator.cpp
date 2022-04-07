#include <algorithm>

#include "mkdhilit.hpp"
#include "seperator.hpp"

namespace mkdhilit {

SeperatorBlk::SeperatorBlk() : MkdBlock{"seperator"} {}

bool SeperatorBlk::matchBegin(const std::string& oneline) {
    auto cnt1 = std::count(oneline.begin(), oneline.end(), '*');
    auto cnt2 = std::count(oneline.begin(), oneline.end(), '+');
    auto cnt3 = std::count(oneline.begin(), oneline.end(), '_');
    auto cnt123 = cnt1 + cnt2 + cnt3;
    if (cnt123 < 3 || cnt123 != cnt1 && cnt123 != cnt2 && cnt123 != cnt3)
        return false;

    auto cnt4 = std::count_if(oneline.begin(), oneline.end(),
                              [](char elem) { return std::isspace(elem); });
    return cnt123 + cnt4 == oneline.size();
}

bool SeperatorBlk::matchEnd(const std::string&) {
    return true;
}

std::string& SeperatorBlk::highlight(std::string& text) {
    std::string space(6, ' ');
    std::string hyphen{};
    for (int cnt{}; cnt < mediator_.getTtyCol() - 12; ++cnt)
        hyphen += "─";
    return text = space + hyphen + space;
}

}  // namespace mkdhilit
