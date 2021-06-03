#include <array>
#include <string.h>

#include "normal.hpp"
#include "see.hpp"


namespace see {


NormalBlk::NormalBlk()
    : MkdBlock{"normal"} {  }


bool NormalBlk::matchBegin(const std::string& oneline)
{
    auto [begin, end] = mediator_.getAllBlocks();
    for( ; begin != end; ++begin ) {
        if ( begin->first != "normal" && begin->second->matchBegin(oneline) ) {
            return false;
        }
    }
    return true;
}


bool NormalBlk::matchEnd(const std::string&)
{
    return true;
}


std::string& NormalBlk::highlight(std::string& text)
{
    return highlight(text, "\e[m");
}


std::string& NormalBlk::highlight(std::string& text, const std::string& bgColor)
{
    std::vector<std::string> colorStack{bgColor};
    enum class Empha {NONE, ITL, BOLD, IBOLD, DEL, UND, KBD, CODE, CODE2, CMT, LINK, LINK2};
    std::vector<Empha> emphaStack{Empha::NONE};
    auto origText = std::move(text);
    text = colorStack.back();

    std::array colors{
        "\e[m",
        "\e[3;38;5;255m",
        "\e[1;38;5;255m",
        "\e[1;3;38;5;255m",
        "\e[9m",
        "\e[4m",
        "\e[7m",
        "\e[38;2;255;168;0m",
        "\e[38;2;255;168;0m",
        "\e[38;5;245m",
        "\e[33m",
        "\e[4;34m"
    };

    auto at = [&origText] (size_t idx) {
        return idx >= origText.size() ? -1 : origText.at(idx);
    };

    auto substr = [&origText] (size_t idx, size_t len) {
        return idx + len > origText.size() ? ""s : origText.substr(idx, len);
    };

    for ( size_t idx{}, end{origText.size()}; idx != end; ) {
        switch ( origText.at(idx) ) {
            case '*':
                if ( emphaStack.back() != Empha::CODE && emphaStack.back() != Empha::CODE2
                        && emphaStack.back() != Empha::CMT
                        && (emphaStack.back() < Empha::ITL || emphaStack.back() > Empha::IBOLD) ) {

                    auto curEmpha = at(idx + 1) != '*' ? Empha::ITL
                        : at(idx + 2) != '*' ? Empha::BOLD : Empha::IBOLD;

                    auto ibIdx = static_cast<size_t>(curEmpha);
                    emphaStack.emplace_back(curEmpha);
                    colorStack.emplace_back(colors.at(ibIdx));
                    text += colorStack.back();
                    idx += ibIdx;
                } else if ( emphaStack.back() == Empha::ITL
                        || (emphaStack.back() == Empha::BOLD && at(idx + 1) == '*')
                        || (emphaStack.back() == Empha::IBOLD && substr(idx + 1, 2) == "**") ) {

                    idx += static_cast<size_t>(emphaStack.back());
                    emphaStack.pop_back();
                    colorStack.pop_back();
                    text += colors.front() + colorStack.back();
                } else {
                    goto DEF;
                }
                break;

            case '~':
                if ( emphaStack.back() != Empha::CODE && emphaStack.back() != Empha::CODE2
                        && emphaStack.back() != Empha::CMT
                        && emphaStack.back() != Empha::DEL
                        && at(idx + 1) == '~' ) {
                    emphaStack.emplace_back(Empha::DEL);
                    colorStack.emplace_back(colors[static_cast<size_t>(Empha::DEL)]);
                    text += colorStack.back();
                    idx += 2;
                } else if ( emphaStack.back() == Empha::DEL && at(idx + 1) == '~' ) {
                    emphaStack.pop_back();
                    colorStack.pop_back();
                    text += colors.front() + colorStack.back();
                    idx += 2;
                } else {
                    goto DEF;
                }
                break;

            case '<':
                if ( emphaStack.back() != Empha::CODE && emphaStack.back() != Empha::CODE2
                        && emphaStack.back() != Empha::CMT
                        && emphaStack.back() != Empha::UND
                        && substr(idx + 1, 2) == "u>" ) {
                    emphaStack.emplace_back(Empha::UND);
                    colorStack.emplace_back(colors[static_cast<size_t>(Empha::UND)]);
                    text += colorStack.back();
                    idx += 3;
                } else if ( emphaStack.back() == Empha::UND && substr(idx + 1, 3) == "/u>" ) {
                    emphaStack.pop_back();
                    colorStack.pop_back();
                    text += colors.front() + colorStack.back();
                    idx += 4;
                } else if ( emphaStack.back() != Empha::CODE && emphaStack.back() != Empha::CODE2
                        && emphaStack.back() != Empha::CMT
                        && emphaStack.back() != Empha::KBD
                        && substr(idx + 1, 4) == "kbd>" ) {
                    emphaStack.emplace_back(Empha::KBD);
                    colorStack.emplace_back(colors[static_cast<size_t>(Empha::KBD)]);
                    text += colorStack.back();
                    idx += 5;
                } else if ( emphaStack.back() == Empha::KBD && substr(idx + 1, 5) == "/kbd>" ) {
                    emphaStack.pop_back();
                    colorStack.pop_back();
                    text += colors.front() + colorStack.back();
                    idx += 6;
                } else if ( emphaStack.back() != Empha::CODE && emphaStack.back() != Empha::CODE2
                        && emphaStack.back() != Empha::CMT && substr(idx + 1, 3) == "!--" ) {
                    emphaStack.emplace_back(Empha::CMT);
                    colorStack.emplace_back(colors[static_cast<size_t>(Empha::CMT)]);
                    text += colorStack.back() + "<!--";
                    idx += 4;
                } else {
                    goto DEF;
                }
                break;

            case '-':
                if ( emphaStack.back() == Empha::CMT && substr(idx + 1, 2) == "->" ) {
                    emphaStack.pop_back();
                    colorStack.pop_back();
                    text.append("-->") += colors.front() + colorStack.back();
                    idx += 3;
                } else {
                    goto DEF;
                }
                break;

            case '[':
                if ( size_t pos{}; emphaStack.back() != Empha::CODE && emphaStack.back() != Empha::CODE2
                        && emphaStack.back() != Empha::CMT
                        && emphaStack.back() != Empha::LINK && emphaStack.back() != Empha::LINK2
                        && (pos = origText.find("](", idx + 1)) != std::string::npos
                        && origText.find(')', pos + 2) != std::string::npos ) {
                    emphaStack.emplace_back(Empha::LINK);
                    if ( at(idx - 1) == '!' ) {
                        text.insert(text.size() - 1, colors[static_cast<size_t>(Empha::LINK)]);
                    } else {
                        text += colors[static_cast<size_t>(Empha::LINK)];
                    }
                    text.append("[") += colors.front() + colorStack.back();
                    ++idx;
                } else {
                    goto DEF;
                }
                break;

            case ']':
                if ( emphaStack.back() == Empha::LINK && at(idx + 1) == '(' ) {
                    emphaStack.back() = Empha::LINK2; // BUG: 截断LINK区间中未完成empha
                    colorStack.emplace_back(colors[static_cast<size_t>(Empha::LINK2)]);
                    text += colors[static_cast<size_t>(Empha::LINK)];
                    text += "](";
                    text += colors.front() + colorStack.back();
                    idx += 2;
                } else {
                    goto DEF;
                }
                break;

            case ')':
                if ( emphaStack.back() == Empha::LINK2 ) {
                    emphaStack.pop_back();
                    colorStack.pop_back();
                    text.append(colors.front())
                        .append(colors[static_cast<size_t>(Empha::LINK)])
                        .append(")")
                        .append(colors.front())
                        .append(colorStack.back());
                    ++idx;
                } else {
                    goto DEF;
                }
                break;

            case '`':
                if ( emphaStack.back() != Empha::CODE && emphaStack.back() != Empha::CODE2 
                            && emphaStack.back() != Empha::CMT ) {
                    auto curEmpha = at(idx + 1) == '`' ? Empha::CODE2 : Empha::CODE;
                    emphaStack.emplace_back(curEmpha);
                    colorStack.emplace_back(colors.at(static_cast<size_t>(curEmpha)));
                    text += colorStack.back();
                    idx += static_cast<size_t>(curEmpha) - static_cast<size_t>(Empha::CODE) + 1;
                } else if ( emphaStack.back() == Empha::CODE
                        || emphaStack.back() == Empha::CODE2 && at(idx + 1) == '`' ) {
                    idx += static_cast<size_t>(emphaStack.back()) - static_cast<size_t>(Empha::CODE) + 1;
                    emphaStack.pop_back();
                    colorStack.pop_back();
                    text += colors.front() + colorStack.back();
                } else {
                    goto DEF;
                }
                break;

            DEF:
            default:
                text += origText[idx++];
        }
    }

    return text += "\e[m" + bgColor;
}


} // namespace see
