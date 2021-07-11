#include <regex>

#include "mine.hpp"
#include "srchilite/sourcehighlight.h"
#include "unicode/display_width.hpp"

#include "code.hpp"


namespace see {


CodeBlk::CodeBlk()
    : see::MkdBlock{"code"}, matchedEnd_{false} {  }


bool CodeBlk::matchBegin(const std::string& oneline)
{
    static auto CodeBlkBeginRegex = std::regex{R"(\s*`{3,}\w*\s*)"};
    return std::regex_match(oneline, CodeBlkBeginRegex);
}


bool CodeBlk::matchEnd(const std::string &oneline)
{
    if ( matchedEnd_ ) {
        matchedEnd_ = false;
        return true;
    }

    static auto CodeBlkEndRegex = std::regex{R"(\s*`{3,}\s*)"};
    // 如果匹配到类末尾，则先将该行匹配进来，下行则返回false表示不要下行
    if ( std::regex_match(oneline, CodeBlkEndRegex) ) {
        matchedEnd_ = true;
    }
    return false;
}


std::string& CodeBlk::highlight(std::string& text)
{
    auto type = text.substr(0, text.find('\n') + 1);
    // 同时去掉形如```cpp头部与形如```尾部
    std::stringstream sstrm{text.substr(type.size(), text.find_last_not_of("`\n ") + 2 - type.size())};
    text.clear();
    size_t first{type.find_first_not_of('`')};
    type = type.substr(first, type.find_first_of(" \n\t") - first);

    std::vector<int> nCols{};
    int maxCol{};
    for ( std::string oneline{}; std::getline(sstrm, oneline); ) {
        auto col = unicode::display_width(oneline);
        nCols.emplace_back(col);
        maxCol = std::max(maxCol, col);
    }
    sstrm.rdbuf()->pubseekpos(0);
    sstrm.clear();

    std::string hyphen{};
    auto nHyphen = maxCol + 4 - static_cast<int>(type.size()) - 4;
    nHyphen = nHyphen < 0 ? 0 : nHyphen / 2 + 1;
    for ( int cnt{}; cnt < nHyphen; ++cnt )
        hyphen += "─";
    text = "\033[38;2;255;165;0m┌" + hyphen + " \033[32m" + type + "\033[38;2;255;165;0m " + hyphen + "┐\033[m\n";
    maxCol = nHyphen * 2 + static_cast<int>(type.size()) + 4;

    std::stringstream hilitStrm{};
    try {
        srchilite::SourceHighlight  codeHilit{"esc.outlang"};
        codeHilit.highlight(sstrm, hilitStrm, type + ".lang");
    } catch (std::exception&) {
        hilitStrm.str(sstrm.str());
    }

    size_t idx{};
    for ( std::string oneline{}; std::getline(hilitStrm, oneline); ) {
        oneline = std::regex_replace(oneline, "\033\\[01;30m"_rgx, "\033[01;34m");
        text += "\033[38;2;255;165;0m│\033[m "
            + oneline
            + std::string(maxCol - nCols.at(idx++) - 4, ' ')
            + " \033[38;2;255;165;0m│\033[m\n";
    }

    text += "\033[38;2;255;165;0m└";
    for ( int cnt{}; cnt < maxCol - 2; ++cnt ) text += "─";
    text += "┘\033[m\n";

    return text;
}


} // namespace see
