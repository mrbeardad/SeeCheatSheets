#include "header.hpp"
#include "normal.hpp"


see::HeaderBlk::HeaderBlk()
    : see::MkdBlock{"header"} {  }


bool see::HeaderBlk::match_begin(const std::string& oneline)
{
    return oneline.starts_with('#');
}


bool see::HeaderBlk::match_end(const std::string&)
{
    return true;
}


std::string& see::HeaderBlk::highlight(std::string& text)
{
    auto bgColor = "\e[34;42m";
    auto fgColor = "\e[34m";
    auto normal = text.substr(text.find_first_not_of("# "));
    normal.pop_back();  // 弹出换行符

    auto hashs  = text.substr(0, text.find(" "));

    int nHyphen{(see::MkdHighlight::Instance().get_tty_col()
            - unicode::display_width(normal)
            - static_cast<int>(hashs.size()) * 2 - 4) / 2};
    std::string hyphen{};
    for ( int cnt{}; cnt < nHyphen; ++cnt ) hyphen += "─";

    text.clear();
    return text << bgColor << hyphen << "\e[m" << fgColor << ' ' << hashs << ' '
        <<  dynamic_cast<see::NormalBlk&>(see::MkdHighlight::Instance().get_block("normal")).highlight(normal, fgColor)
        << ' ' << hashs << ' ' << bgColor << hyphen << "\e[m\n";
}

