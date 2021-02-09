#include "see.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <srchilite/sourcehighlight.h>
#include <sys/ioctl.h>
#include <unicode/display_width.hpp>
#include <unistd.h>
#include <utility>

namespace see
{

int MkdBlock::Row{50};
int MkdBlock::Col{100};

bool MkdBlock::init_winsize()
{
    // 初始化MkdBlock::Row和MkdBlock::Col
    if ( isatty(STDOUT_FILENO) ) {
        winsize termWinSize{};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &termWinSize);
        Row = termWinSize.ws_row;
        Col = termWinSize.ws_col;
        return false;
    }
    return true;
}

std::unordered_map<std::string, std::pair<std::regex, std::string> > MkdBlock::Emphasizes{
    {"code", {R"(`(.*?)`)"_rgx,
        "\033[38;2;255;168;0m $1 "s}},
    {"tex", {R"((\${1,2})(.*?)\1)"_rgx,
        "\033[38;2;255;168;0m $1 "s}},
    {"italic", {R"(\*(.*?)\*)"_rgx,
        "\033[38;5;255m\033[3m $1 \033[m"s}},
    {"bold", {R"(\*\*(.*?)\*\*)"_rgx,
        "\033[38;5;255m\033[1m  $1  \033[m"s}},
    {"bolditalic", {R"(\*\*\*(.*?)\*\*\*)"_rgx,
        "\033[38;5;255m\033[1;3m   $1   \033[m"s}},
    {"link", {R"(\[(.*?)\]\((.*)\))"_rgx,
        "\033[33m[\033[m$1\033[33m](\033[4;34m$2\033[m\033[33m)"s}},
    {"comment", {R"((<!--.*?-->))"_rgx,
        "\033[38;5;245m$1\033[m"s}}
};

MkdBlock::MkdBlock(MkdHighlight& mediator, const std::string& bgColor)
    : mediator_{mediator}, bgColor_{bgColor} {  }

MkdBlock::~MkdBlock() {  }

MkdHighlight& MkdBlock::mediator()
{
    return mediator_;
}

std::string MkdBlock::set_bg_color(const std::string& color)
{
    auto ret = bgColor_;
    bgColor_ = color;
    return ret;
}

bool MkdBlock::match_begin(const std::string&)
{
    return true;
}

bool MkdBlock::match_end(const std::string&)
{
    return true;
}

std::string& MkdBlock::highlight(std::string& text)
{
    text = std::regex_replace(text, Emphasizes.at("link").first,
            Emphasizes.at("link").second + bgColor_);
    text = std::regex_replace(text, Emphasizes.at("code").first,
            Emphasizes.at("code").second + bgColor_);
    text = std::regex_replace(text, Emphasizes.at("tex").first,
            Emphasizes.at("tex").second + bgColor_);
    text = std::regex_replace(text, Emphasizes.at("italic").first,
            Emphasizes.at("italic").second + bgColor_);
    text = std::regex_replace(text, Emphasizes.at("bold").first,
            Emphasizes.at("bold").second + bgColor_);
    text = std::regex_replace(text, Emphasizes.at("bolditalic").first,
            Emphasizes.at("bolditalic").second + bgColor_);
    text = std::regex_replace(text, Emphasizes.at("comment").first,
            Emphasizes.at("comment").second + bgColor_);
    text.insert(0, bgColor_);
    return text;
}

/*************************************************************************************************/

class HeaderBlk : public MkdBlock
{
public:
    explicit HeaderBlk(MkdHighlight& mediator): MkdBlock{mediator, "\033[34m"} {  }

    virtual bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, Header);
    }

    virtual bool match_end(const std::string&) override
    {
        return true;
    }

    std::string& highlight(std::string& text) override
    {
        auto normal = text.substr(text.find_first_not_of("# "));
        normal.pop_back();
        auto hashs  = text.substr(0, text.find(" "));
        int nHyphen{(Col - unicode::display_width(normal) - static_cast<int>(hashs.size()) * 2 - 4) / 2};
        std::string hyphen{};
        for ( int cnt{}; cnt < nHyphen; ++cnt ) hyphen += "─";

        text = "\033[34;42m" + hyphen + "\033[m\033[34m " + hashs + " "
            + MkdBlock::highlight(normal)
            +  " " + hashs + " \033[34;42m" + hyphen + "\033[m\n";
        return text;
    }
private:
    static inline std::regex Header{R"(^(#+)\s*\S+$)"};
};

/*************************************************************************************************/

class SeperatorBlk : public MkdBlock
{
    static inline std::regex Seperator{R"(^(\s*\*\s*){3,}$)"};
public:
    explicit SeperatorBlk(MkdHighlight& mediator): MkdBlock{mediator} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, Seperator);
    }

    bool match_end(const std::string&) override
    {
        return true;
    }

    std::string& highlight(std::string& text) override
    {
        std::string space(6, ' ');
        std::string hyphen{};
        for ( int cnt{}; cnt < Col - 12; ++cnt ) hyphen += "─";
        return text = space + hyphen + space;
    }
};

/*************************************************************************************************/

class QuoteBlk : public MkdBlock
{
public:
    explicit QuoteBlk(MkdHighlight& mediator): MkdBlock{mediator, "\033[38;5;245m"} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, Quote);
    }

    bool match_end(const std::string& oneline) override
    {
        return oneline.empty()
            || mediator().get_block("header").match_end(oneline)
            || mediator().get_block("seperator").match_end(oneline)
            || mediator().get_block("list").match_end(oneline)
            || mediator().get_block("code").match_end(oneline)
            || mediator().get_block("comment").match_end(oneline);
    }

    std::string& highlight(std::string& text) override
    {
        std::stringstream sstrm{text};
        text.clear();
        for ( std::string oneline{}; std::getline(sstrm, oneline); ) {
            auto quote = std::regex_replace(oneline, Quote, "\033[48;5;235m\033[38;5;240m$1\033[m");
            auto normal = std::regex_replace(oneline, Quote, "$2");
            MkdBlock::highlight(normal);
            text += quote + normal + '\n';
        }
        return text;
    }
private:
    static inline std::regex Quote{R"(^(\s*>+)(\s+\S*))"};
};

/*************************************************************************************************/

class ListBlk : public MkdBlock
{
public:
    explicit ListBlk(MkdHighlight& mediator): MkdBlock{mediator} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, List) && !mediator().get_block("seperator").match_begin(oneline);
    }

    bool match_end(const std::string &oneline) override
    {
        return oneline.empty()
            || mediator().get_block("header").match_end(oneline)
            || mediator().get_block("seperator").match_end(oneline)
            || mediator().get_block("list").match_end(oneline)
            || mediator().get_block("quote").match_end(oneline)
            || mediator().get_block("code").match_end(oneline)
            || mediator().get_block("comment").match_end(oneline);
    }

    std::string& highlight(std::string& text) override
    {
        std::stringstream sstrm{text};
        text.clear();
        for ( std::string oneline{}; std::getline(sstrm, oneline); ) {
            std::smatch headMatch{};
            std::string head{};
            std::string normal{};
            if ( std::regex_search(oneline, headMatch, List) ) {
                head = headMatch.format("\033[36m$1\033[m");
                normal = headMatch.suffix().str();
            } else {
                normal = oneline;
            }
            MkdBlock::highlight(normal);
            text = head + normal + '\n';
        }
        return text;
    }
private:
    static inline std::regex List{R"(^(\s*[-*+]\s+))"};
};

/*************************************************************************************************/

class TableBlk : public MkdBlock
{
public:
    explicit TableBlk(MkdHighlight& mediator): MkdBlock{mediator} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, Table1st);
    }

    bool match_end(const std::string& oneline) override
    {
        return !std::regex_search(oneline, Table1st);
    }

    std::string& highlight(std::string& text) override
    {
        auto line2ndBegin   = text.find('\n') + 1;
        auto line2ndEnd     = text.find('\n', line2ndBegin);
        auto line2nd        = text.substr(line2ndBegin, line2ndEnd - line2ndBegin);
        if ( std::regex_search(line2nd, Table2nd) ) {
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
                    text += MkdBlock::highlight(normal) + "│";
                    normal.clear();
                }
            }
            text += '\n' + newLine3rd + '\n';

            sstrm.ignore(512, '\n');
            // 复用之前的line2nd
            for ( int cnt{}; std::getline(sstrm, line2nd); ++cnt ) {
                set_bg_color(cnt % 2 ? "\033[38;5;240m\033[48;2;94;175;220m"
                    : "\033[38;5;240m\033[48;2;128;128;255m");
                text += MkdBlock::highlight(line2nd) + "\033[m\n";
            }
            set_bg_color("\033[m");
        } else {
            MkdBlock::highlight(text);
        }
        return text;
    }
private:
    static inline std::regex Table1st{R"(^\s*(\|.*)+\|)"};
    static inline std::regex Table2nd{R"(^\s*(\|[: -]+)+\|)"};
};

/*************************************************************************************************/

class CodeBlk : public MkdBlock
{
    bool needHilit_;
public:
    explicit CodeBlk(MkdHighlight& mediator): MkdBlock{mediator}, needHilit_{true} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, CodeBegin);
    }

    bool match_end(const std::string &oneline) override
    {
        if ( !needHilit_ ) return true;
        return std::regex_search(oneline, CodeEnd);
    }

    std::string& highlight(std::string& text) override
    {
        if ( needHilit_ ) needHilit_ = false;
        else {
            needHilit_ = true;
            text.clear();
            return text;
        }
        auto type = text.substr(0, text.find('\n') + 1);
        // 同时去掉形如```cpp头部与形如```尾部
        std::stringstream sstrm{text.substr(type.size(), text.find_last_not_of("`\n") + 2 - type.size())};
        text.clear();
        type = std::regex_replace(type, CodeBegin, "$1", std::regex_constants::format_no_copy);

        // 当前版本unicode::display_wiwdth()不支持ANSI escape
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
        } catch (std::exception& excep) {
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
private:
    static inline std::regex CodeBegin{R"(^\s*`{3,}(.*))"};
    static inline std::regex CodeEnd{R"(^\s*`{3,}$)"};
};

/*************************************************************************************************/

// 注册器
MkdHighlight::MkdHighlight()
{
    MkdBlock::init_winsize();
    blocks.insert({"header", std::make_shared<HeaderBlk>(*this)});
    blocks.insert({"seperator", std::make_shared<SeperatorBlk>(*this)});
    blocks.insert({"quote", std::make_shared<QuoteBlk>(*this)});
    blocks.insert({"list", std::make_shared<ListBlk>(*this)});
    blocks.insert({"table", std::make_shared<TableBlk>(*this)});
    blocks.insert({"code", std::make_shared<CodeBlk>(*this)});
}

MkdBlock& MkdHighlight::get_block(const std::string& name)
{
    return *blocks.at(name);
}

std::string MkdHighlight::highlight(std::istream& text)
{
    std::string hilitText{};
    for ( std::string oneline{}; std::getline(text, oneline); ) {
        auto itr = blocks.begin();
        auto end = blocks.end();
        auto curBlk = std::make_shared<MkdBlock>(*this);
        while ( itr != end ) {
            if ( itr->second->match_begin(oneline) ) {
                curBlk = itr->second;
#ifndef NDEBUG
                std::cout << "fuck: " << itr->first << ":" << oneline << std::endl;
#endif // NDEBUG
                break;
            }
            ++itr;
        }

        std::string nextline{};
        while ( std::getline(text, nextline) && !curBlk->match_end(nextline) )
            oneline +=  '\n' + nextline;
        oneline += '\n';
        // 将最后读取的nextline还原回流中
        text.rdbuf()->pubseekoff(-static_cast<int>(nextline.size()) - 1, io::cur, io::in);

        hilitText += curBlk->highlight(oneline);
    }
    return hilitText;
}

/*************************************************************************************************/

void print_help_then_exit() noexcept
{
    std::cout <<
        "\033[32mDescription:\033[m\n"
        "    \033[33msee\033[m will find cheat-sheets among ~/.cheat/*.md.\n"
        "Yeah! They are markdown files. Each entry starts with '\033[34m<!-- entry begin: keywords... -->\033[m',\n"
        "and ends with '\033[34m<!-- entry end -->\033[m'. \033[33msee\033[m will search apposite entries and print them.\n"
        "    For example, \033[33msee echo.*\033[m will match :\n\033[36m"
        "        <!-- entry begin: echo -->\n"
        "        * echo\n"
        "            * -n        ：不自动加入换行符（zsh会将无换行结尾的输出的尾部标记`反显的%`）\n"
        "            * -e        ：启用转义语义（zsh自动开启）\n"
        "        <!-- entry end -->\n"
        "\033[32mUsage:\033[m\n"
        "    see [<options>] <keyword> [<keyword>...]\n"
        "You can redirect stdin to a file(not a tty) to use only Markdown Syntax Highlight.\n"
        "\033[32mOptions:\033[m\n"
        "    \033[36m-h\033[m                : Display this help information\n"
        "    \033[36m-f\033[m <file-prefix>  : Specify file in ~/.cheat/ whoes file name prefix match <file-prefix> to search\n"
        "    \033[36m-p\033[m                : Disable to use Pager\n"
        << std::endl;
    std::exit(1);
}

std::tuple<std::vector<fs::path>, std::vector<std::string>, bool>
parse_cmdline(int argc, char* argv[]) noexcept
{
    std::vector<std::string> keys{}, filePrefix{};
    bool disablePager{};
    while ( true ) {
        auto choice = getopt(argc, argv, "hpw:f:");
        if ( choice == -1 ) break;

        switch ( choice ) {
            case '?':
            case 'h':
                print_help_then_exit();
                break;
            case 'p':
                disablePager = true;
                break;
            case 'f':
                filePrefix.emplace_back(optarg);
                break;
            default:
                break;
        }
    }

    // 读取命令参数
    while ( optind < argc ) {
        keys.emplace_back(argv[optind++]);
    }

    // 处理参数无效的情况
    if ( keys.empty() ) {
        std::cerr << "\e[31m`see` Error:\e[0m Need entry-regex-keys" << std::endl;
        print_help_then_exit();
    }

    fs::path cheatDir{std::getenv("HOME")};
    cheatDir /= ".cheat/";

    // 如果~/.cheat/目录不存在则报错并退出
    if ( !fs::is_directory(cheatDir) ) {
        std::cerr << "Error: No such directory ~/.cheat/" << std::endl;
        std::exit(1);
    }

    std::vector<fs::path> files{};
    for ( const auto& thisDirEntry : fs::directory_iterator{cheatDir} ) {
        if ( !fs::is_regular_file(thisDirEntry) || thisDirEntry.path().extension() != ".md" ) {
            continue;
        }

        if ( filePrefix.empty() ) {
            files.emplace_back(thisDirEntry.path());
        } else {
            std::string curFileName{thisDirEntry.path().filename()};

            for ( auto& thisFilePrefix : filePrefix ) {
                // 确认文件名前缀是否与用户指定匹配
                if ( curFileName.find(thisFilePrefix, 0) != std::string::npos )
                    files.emplace_back(thisDirEntry.path());
            }
        }
    }

    return std::tuple{files, keys, disablePager};
}

/*************************************************************************************************/

std::stringstream search_entries(const std::vector<fs::path>& files, const std::vector<std::string>& keys) noexcept
{
    std::stringstream entries{};
    std::regex entryBegin{R"(^<!-- entry begin:.*-->$)", std::regex_constants::icase};
    std::regex entryEnd{R"(^<!-- entry end -->$)", std::regex_constants::icase};

    for ( auto& thisFile : files ) {
        std::ifstream fstrm{thisFile, io::in};
        if ( !fstrm ) {
            std::cerr << "Error: Can not read file -- " << thisFile << std::endl;
            exit(1);
        }

        bool onceMatch{false}; // 用于提前打印文件名
        bool inEntry{false};   // 用于判断当前匹配是否为某一entry内容

        for ( std::string oneline{}; std::getline(fstrm, oneline); ) {
            // 若未在entry中且搜索到entryBegin则尝试进入该entry
            if ( !inEntry && std::regex_search(oneline, entryBegin) ) {
                auto curEntryKeys = oneline.substr(sizeof("<!-- entry begin:"),
                        oneline.size() - sizeof("<!-- entry begin:-->"));

                // 有一个key不匹配则略过
                bool isMatch{true};
                for ( auto& thisKey : keys ) {
                    if ( !std::regex_search(curEntryKeys, std::regex{thisKey, std::regex_constants::icase}) ) {
                        isMatch = false;
                        break;
                    }
                }

                if ( isMatch ) {
                    if ( !onceMatch ) {
                        entries <<
                        "\033[38;2;255;0;0m\033[38;2;255;34;0m \033[38;2;255;85;0m \033[38;2;255;136;0m "
                        "\033[38;2;255;170;0m \033[38;2;255;204;0m \033[38;2;255;255;0m \033[38;2;204;255;0m "
                        "\033[38;2;170;255;0m \033[38;2;136;255;0m \033[38;2;85;255;0m \033[38;2;34;255;0m "
                        "\033[38;2;0;255;0m \033[38;2;0;255;34m \033[38;2;0;255;85m \033[38;2;0;255;136m "
                        "\033[38;2;0;255;170m \033[38;2;0;255;204m \033[38;2;0;255;255m \033[38;2;0;204;255m "
                        "\033[38;2;0;170;255m \033[38;2;0;136;255m \033[38;2;0;85;255m \033[38;2;0;51;255m "
                        "\033[38;2;0;0;255m \033[38;2;51;0;255m \033[38;2;85;0;255m \033[38;2;136;0;255m "
                        "\033[38;2;170;0;255m \033[38;2;204;0;255m \033[38;2;255;0;255m \033[m "
                        << thisFile << "\e[m\n";
                        onceMatch = true;
                    }
                    inEntry = true;
                    entries << oneline << '\n';
                }
            } else if ( inEntry ) {
                // 若在entry中且搜索到结束标志，则退出inEntry状态
                if ( std::regex_search(oneline, entryEnd) ) {
                    inEntry = false;
                    entries << '\n';
                // 若在entry中且未搜索到结束标志，则将该行送入entry流中
                } else {
                    entries << oneline << '\n';
                }
            }
        }
    }
    return entries;
}

} // namespace see

