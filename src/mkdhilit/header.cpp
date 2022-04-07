#include "unicode/display_width.hpp"

#include "header.hpp"
#include "mkdhilit.hpp"
#include "normal.hpp"

namespace mkdhilit {

HeaderBlk::HeaderBlk() : MkdBlock{"header"} {}

bool HeaderBlk::matchBegin(const std::string& oneline) {
    return oneline.length() > 0 && oneline[0] == '#';
}

bool HeaderBlk::matchEnd(const std::string&) {
    return true;
}

class Color {
    unsigned char rgb_[3];

   public:
    Color() : rgb_{0xff, 0, 0} {}

    std::wstring operator()() {
        ssize_t ffIdx{};
        for (; ffIdx < 3; ++ffIdx)
            if (rgb_[ffIdx] == 0xff)
                break;
        ssize_t prevIdx{(ffIdx - 1 + 3) % 3}, nextIdx{(ffIdx + 1) % 3};
        if (rgb_[nextIdx] == 0xff) {
            prevIdx = ffIdx;
            nextIdx = (nextIdx + 1) % 3;
        }
        if (rgb_[prevIdx] == 0) {
            rgb_[nextIdx] += 0x11;
        } else {
            rgb_[prevIdx] -= 0x11;
        }
        return L"\e[48;2;" + std::to_wstring(rgb_[0]) + L";" +
               std::to_wstring(rgb_[1]) + L";" + std::to_wstring(rgb_[2]) +
               L"m\e[38;2;" + std::to_wstring(0xff - rgb_[0]) + L";" +
               std::to_wstring(0xff - rgb_[1]) + L";" +
               std::to_wstring(0xff - rgb_[2]) + L"m";
    }
};

std::string& HeaderBlk::highlight(std::string& text) {
    auto normal = text.substr(text.find_first_not_of("# "));
    normal.pop_back();  // 弹出换行符
    auto hashs = text.substr(0, text.find(' '));
    int nHyphen{(mediator_.getTtyCol() - unicode::display_width(normal) -
                 static_cast<int>(hashs.size()) * 2 - 4) /
                2};
    std::string hyphen{};
    for (; nHyphen > 0; --nHyphen)
        hyphen += "─";

    if (text.find("# FILE: ") == 0) {
        text.clear();
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        auto wTmpText = myconv.from_bytes(hyphen + ' ' + hashs + ' ' + normal +
                                          ' ' + hashs + ' ' + hyphen);
        std::wstring wText{};
        Color rgbColor{};
        for (wchar_t thisWChar : wTmpText) {
            wText += rgbColor();
            wText += thisWChar;
        }
        wText += L"\e[m\n";
        return text = myconv.to_bytes(wText);
    }

    text.clear();
    auto* bgColor = "\e[34;42m";
    auto* fgColor = "\e[34m";
    return text << bgColor << hyphen << "\e[m" << fgColor << ' ' << hashs << ' '
                << dynamic_cast<NormalBlk&>(mediator_.getBlock("normal"))
                       .highlight(normal, fgColor)
                << ' ' << hashs << ' ' << bgColor << hyphen << "\e[m\n";
}

}  // namespace mkdhilit
