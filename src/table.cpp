#include <regex>

#include "see.hpp"
#include "table.hpp"


namespace see {


static std::regex g_Table1st{R"(^\s*(\|.*)+\|)"};

static std::regex g_Table2nd{R"(^\s*(\|[: -]+)+\|)"};


TableBlk::TableBlk()
    : MkdBlock{"table"} {  }


bool TableBlk::matchBegin(const std::string& oneline)
{
    return std::regex_search(oneline, g_Table1st);
}


bool TableBlk::matchEnd(const std::string& oneline)
{
    return !std::regex_search(oneline, g_Table1st);
}


std::string& TableBlk::highlight(std::string& text)
{
    auto line2ndBegin   = text.find('\n') + 1;
    auto line2ndEnd     = text.find('\n', line2ndBegin);
    auto line2nd        = text.substr(line2ndBegin, line2ndEnd - line2ndBegin);
    if ( std::regex_search(line2nd, g_Table2nd) ) {
        std::stringstream   sstrm{text};
        std::string         newLine1st{"┌"};
        std::string         newLine2nd{};
        std::string         newLine3rd{"├"};

        for ( auto pos = line2nd.begin() + 1, end = line2nd.end(); pos != end; ++pos ) {
            if ( *pos == '|' ) {
                newLine1st += "┬";
                newLine3rd += "┼";
            } else {    // '-'或':'
                newLine1st += "─";
                newLine3rd += "─";
            }
        }
        newLine1st.replace(newLine1st.size() - 3, 3, "┐\n│");
        newLine3rd.replace(newLine3rd.size() - 3, 3, "┤");

        text = newLine1st;

        std::getline(sstrm, newLine2nd);
        std::string normal{};
        for ( auto pos = newLine2nd.begin() + 1, end = newLine2nd.end(); pos != end; ++pos ) {
            if ( *pos != '|' ) {
                normal += *pos;
            } else {
                text += MkdHighlight::Instance().getBlock("normal").highlight(normal) + "│";
                normal.clear();
            }
        }
        text += '\n' + newLine3rd + '\n';

        sstrm.ignore(512, '\n');
        // 复用之前的line2nd
        for ( int cnt{}; std::getline(sstrm, line2nd); ++cnt ) {
            auto& normalBlk = MkdHighlight::Instance().getBlock("normal");
            text += dynamic_cast<NormalBlk&>(normalBlk).highlight(line2nd,
                    cnt % 2 ? "\033[38;5;240m\033[48;2;94;175;220m" : "\033[38;5;240m\033[48;2;128;128;255m")
                + "\033[m\n";
        }
    } else {
        MkdHighlight::Instance().getBlock("normal").highlight(text);
    }
    return text;
}


} // namespace see
