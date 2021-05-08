#include "quote.hpp"

QuoteBlk::QuoteBlk()
    : see::MkdBlock{"quote"} {  }


bool QuoteBlk::matchBegin(const std::string& oneline)
{
    auto nonSpaceIdx = oneline.find_first_not_of(" \t\n");
    return nonSpaceIdx != std::string::npos && oneline[nonSpaceIdx] == '>';
}


bool QuoteBlk::matchEnd(const std::string& oneline)
{
    return oneline.empty()
        || mediator_.getBlock("header").matchBegin(oneline)
        || mediator_.getBlock("seperator").matchBegin(oneline)
        || mediator_.getBlock("list").matchBegin(oneline)
        || mediator_.getBlock("code").matchBegin(oneline)
        || mediator_.getBlock("comment").matchBegin(oneline);
}


std::string& QuoteBlk::highlight(std::string& text)
{
    auto& normalBlk = dynamic_cast<see::NormalBlk&>(mediator_.getBlock("normal"));
    auto origText = std::move(text);
    mine::Split getLine{origText, '\n', true};

    for ( std::string_view onelineView{}; getLine(onelineView); ) {
        size_t idx{};
        for ( ; idx < onelineView.size(); ++idx ) {
            if ( onelineView[idx] == '>' ) {
                text << "\033[48;5;235m\033[38;5;240m>\033[m";
            } else if ( std::isspace(onelineView[idx]) ) {
                text << onelineView[idx];
            } else {
                break;
            }
        }
        std::string normal{onelineView.substr(idx)};
        text << normalBlk.highlight(normal, "\033[38;5;245m");
    }

    return text;
}
