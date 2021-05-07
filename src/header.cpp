#include "header.hpp"
#include "normal.hpp"


see::HeaderBlk::HeaderBlk()
    : see::MkdBlock{"header"} {  }


bool see::HeaderBlk::matchBegin(const std::string& oneline)
{
    return oneline.starts_with('#');
}


bool see::HeaderBlk::matchEnd(const std::string&)
{
    return true;
}


class Color
{
    char rgb_[3];
public:
    Color()
        : rgb_{static_cast<char>(255), 0, 0} {  }

    std::string operator()()
    {
        ssize_t firstFF{-1}, lastFF{-1};
        for ( ssize_t idx{}; idx < 3; ++idx ) {
            if ( rgb_[idx] == 255) {
                if ( firstFF == -1 )
                    firstFF = lastFF = 255;
                else
                    lastFF = 255;
            }
        }

    }
};


std::string& see::HeaderBlk::highlight(std::string& text)
{
    auto normal = text.substr(text.find_first_not_of("# "));
    normal.pop_back();  // 弹出换行符

    auto hashs  = text.substr(0, text.find(' '));

    int nHyphen{(see::MkdHighlight::Instance().getTtyCol()
            - unicode::display_width(normal)
            - static_cast<int>(hashs.size()) * 2 - 4) / 2};
    std::string hyphen{};
    for ( ; nHyphen > 0; --nHyphen ) hyphen += "─";

    if ( text.starts_with("# FILE: ") ) {
        std::string tmpText{hyphen + hashs + normal + hashs + hyphen};
        ;
    }

    text.clear();
    auto* bgColor = "\e[34;42m";
    auto* fgColor = "\e[34m";
    return text << bgColor << hyphen << "\e[m" << fgColor << ' ' << hashs << ' '
        <<  dynamic_cast<see::NormalBlk&>(see::MkdHighlight::Instance().getBlock("normal")).highlight(normal, fgColor)
        << ' ' << hashs << ' ' << bgColor << hyphen << "\e[m\n";
}

