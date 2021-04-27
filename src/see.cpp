/**
 * Copyright (c) 2020 Heachen Bear & Contributors
 * File: see.cpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 09.02.2021
 * Last Modified Date: 26.04.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */

#include <fstream>
#include <iostream>
#include <regex>
#include <unistd.h>

#include "header.hpp"
#include "mine.hpp"
#include "normal.hpp"
#include "see.hpp"
#include "srchilite/sourcehighlight.h"
#include "unicode/display_width.hpp"

namespace see
{
/*************************************************************************************************/
// MkdBlock
/*************************************************************************************************/
// 注册器：其子类仅需创建一个对象实例便自动向中介者(MkdHighlight)注册
MkdBlock::MkdBlock(std::string name): registName_(std::move(name))
{
    if ( !MkdHighlight::Instance().regist(registName_, this) ) {
        throw std::logic_error{name + "regist error"};
    }
}


std::string MkdBlock::highlight(std::string&& text)
{
    return highlight(text);
}


/*************************************************************************************************/
// WinSize
/*************************************************************************************************/
WinSize::WinSize()
{
    if ( isatty(STDOUT_FILENO) ) {
        mine::handle(ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize_));
    } else {
        // 如果stdout非终端，则设置默认窗口大小供MkdBlock使用
        winsize_.ws_row = 30;
        winsize_.ws_col = 100;
    }
}


int WinSize::get_row() const noexcept
{
    return winsize_.ws_row;
}


int WinSize::get_col() const noexcept
{
    return winsize_.ws_col;
}


/*************************************************************************************************/
// MkdHighlight
/*************************************************************************************************/
MkdHighlight::MkdHighlight() {  }


int MkdHighlight::get_tty_row() const noexcept
{
    return winsize_.get_row();
}


int MkdHighlight::get_tty_col() const noexcept
{
    return winsize_.get_col();
}


bool MkdHighlight::regist(std::string name, MkdBlock* block)
{
    auto [ignore, isOK] = blocks_.emplace(std::move(name), block);
    return isOK;
}


MkdBlock& MkdHighlight::get_block(const std::string& name)
{
    return *blocks_.at(name);
}


const MkdHighlight::Register& MkdHighlight::get_all_blocks() const
{
    return blocks_;
}


std::string& MkdHighlight::highlight(std::string& text)
{
    std::stringstream textStrm{text};
    text.clear();

    for ( std::string oneline{}; std::getline(textStrm, oneline); ) {
        MkdBlock* curBlk{};
        for ( auto itr = blocks_.begin(), end = blocks_.end(); itr != end; ++itr ) {
            if ( itr->second->match_begin(oneline) ) {
                curBlk = itr->second;
                if ( getenv("OUTPUT_DEBUG_LOG") != nullptr )
                    std::cerr << "match: " << itr->first << ":" << oneline << '\n';
                break;
            }
        }

        std::string nextline{};
        while ( std::getline(textStrm, nextline) && !curBlk->match_end(nextline) )
            oneline +=  '\n' + nextline;
        // 若最后读取nextline时忽略了存在于末尾的换行，则添加上去
        oneline += textStrm.unget().peek() == '\n' ? "\n" : "";
        // 将最后读取的nextline还原回流中
        textStrm.rdbuf()->pubseekoff(-static_cast<int>(nextline.size()), io::cur, io::in);

        text += curBlk->highlight(oneline);
    }

    return text;
}


MkdHighlight& MkdHighlight::Instance()
{
    static MkdHighlight Instance;
    return Instance;
}


/*************************************************************************************************/
// parse_cmdline
/*************************************************************************************************/
std::tuple<std::vector<fs::path>, std::vector<std::string>, bool>
parse_cmdline(int argc, char* argv[])
{
    // 从管道读取数据，仅用于渲染，不解析命令行参数
    if ( !isatty(STDIN_FILENO) )
        return {};

    auto print_help_then_exit = [] () {
        std::cout <<
            "\033[32mDescription:\033[m\n"
            "    \033[33msee\033[m will find cheat-sheets among ~/.cheat/*.md.\n"
            "Yeah! They are markdown files. Each entry starts with '\033[34m<!-- entry begin: keywords... -->\033[m',\n"
            "and ends with '\033[34m<!-- entry end -->\033[m'. \033[33msee\033[m will search apposite entries and print them.\n"
            "    For example, \033[33msee 'echo.*'\033[m will match :\n\033[36m"
            "        <!-- entry begin: echo -->\n"
            "        * echo\n"
            "            * -n        ：不自动加入换行符（zsh会将无换行结尾的输出的尾部标记`反显的%`）\n"
            "            * -e        ：启用转义语义（zsh自动开启）\n"
            "        <!-- entry end -->\n"
            "\033[32mUsage:\033[m\n"
            "    see [<options>] <keyword> [<keyword>...]\n"
            "\n"
            "    You can redirect stdin to a file(not a tty) to use only Markdown Syntax Highlight.\n"
            "    see </path/to/file"
            "\033[32mOptions:\033[m\n"
            "    \033[36m-h\033[m                : Display this help information\n"
            "    \033[36m-f\033[m <file-prefix>  : Specify file in ~/.cheat/ whoes file name prefix match <file-prefix> to search\n"
            "    \033[36m-p\033[m                : Disable to use Pager\n"
            << std::endl;
        std::exit(1);
    };

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

    // 读取命令参数，即关键字
    while ( optind < argc ) {
        keys.emplace_back(argv[optind++]);
    }

    // 处理参数无效的情况
    if ( keys.empty() ) {
        std::cerr << "\e[31m`see` Error:\e[0m Need entry-regex-keys" << std::endl;
        print_help_then_exit();
    }

    fs::path cheatDir{std::getenv("HOME")};
    cheatDir /= ".cheat";

    // 如果~/.cheat/目录不存在则报错并退出
    if ( !fs::is_directory(cheatDir) ) {
        throw std::runtime_error{"Error: No such directory ~/.cheat/"};
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

std::string search_entries(const std::vector<fs::path>& files, const std::vector<std::string>& keys)
{
    std::string entries{};

    for ( auto& thisFile : files ) {
        std::ifstream fstrm{thisFile, io::in};
        if ( !fstrm ) {
            throw std::runtime_error{"Error: Can not read file -- " + thisFile.string()};
        }

        bool onceMatch{true}; // 用于提前打印文件名
        bool inEntry{false};   // 用于判断当前匹配是否为某一entry内容

        for ( std::string oneline{}; std::getline(fstrm, oneline); ) {
            // 若未在entry中且搜索到entryBegin则尝试进入该entry
            if ( !inEntry && oneline.starts_with("<!-- entry begin:") && oneline.ends_with("-->") ) {
                ssize_t prefix{sizeof("<!-- entry begin:") - 1}, suffix{sizeof("-->") - 1};

                // 有一个key不匹配则略过
                bool isMatch{true};
                for ( auto& thisKey : keys ) {
                    if ( !std::regex_search(oneline.begin() + prefix, oneline.end() - suffix, std::regex{thisKey}) ) {
                        isMatch = false;
                        break;
                    }
                }

                if ( isMatch ) {
                    if ( onceMatch ) {
                        entries << "# FILE: " << thisFile << "\n";
                        onceMatch = false;
                    }
                    inEntry = true;
                    entries << oneline << '\n';
                }
            } else if ( inEntry ) {
                // 若在entry中且未搜索到结束标志，则将该行送入entry流中
                if ( oneline != "<!-- entry end -->" ) {
                    entries << oneline << '\n';
                // 若在entry中且搜索到结束标志，则退出inEntry状态
                } else {
                    inEntry = false;
                    entries << '\n';
                }
            }
        }
    }
    return entries;
}

/*************************************************************************************************/

class SeperatorBlk : public see::MkdBlock
{
    static inline std::regex Seperator{R"(^(\s*\*\s*){3,}$)"};
public:
    SeperatorBlk(): see::MkdBlock{"seperator"} {  }

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
        for ( int cnt{}; cnt < see::MkdHighlight::Instance().get_tty_col() - 12; ++cnt ) hyphen += "─";
        return text = space + hyphen + space;
    }

private:
    static SeperatorBlk Instance_;
};
SeperatorBlk SeperatorBlk::Instance_{};


/*************************************************************************************************/

class QuoteBlk : public see::MkdBlock
{
public:
    QuoteBlk(): see::MkdBlock{"quote"} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, Quote);
    }

    bool match_end(const std::string& oneline) override
    {
        return oneline.empty()
            || see::MkdHighlight::Instance().get_block("header").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("seperator").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("list").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("code").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("comment").match_end(oneline);
    }

    std::string& highlight(std::string& text) override
    {
        std::stringstream sstrm{text};
        text.clear();
        for ( std::string oneline{}; std::getline(sstrm, oneline); ) {
            auto quote = std::regex_replace(oneline, Quote, "\033[48;5;235m\033[38;5;240m$1\033[m");
            auto normal = std::regex_replace(oneline, Quote, "$2");
            auto& normalBlk = see::MkdHighlight::Instance().get_block("normal");
            dynamic_cast<NormalBlk&>(normalBlk).highlight(normal, "\033[38;5;245m");
            text += quote + normal + '\n';
        }
        return text;
    }
private:
    static inline std::regex Quote{R"(^(\s*>+)(\s+\S*))"};
    static QuoteBlk Instance_;
};
QuoteBlk QuoteBlk::Instance_{};

/*************************************************************************************************/

class ListBlk : public see::MkdBlock
{
public:
    ListBlk(): see::MkdBlock{"list"} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, List) && !see::MkdHighlight::Instance().get_block("seperator").match_begin(oneline);
    }

    bool match_end(const std::string &oneline) override
    {
        return oneline.empty()
            || see::MkdHighlight::Instance().get_block("header").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("seperator").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("list").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("quote").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("code").match_end(oneline)
            || see::MkdHighlight::Instance().get_block("comment").match_end(oneline);
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
            see::MkdHighlight::Instance().get_block("normal").highlight(normal);
            text = head + normal + '\n';
        }
        return text;
    }
private:
    static inline std::regex List{R"(^(\s*[-*+]\s+))"};
    static ListBlk Instance_;
};
ListBlk ListBlk::Instance_{};

/*************************************************************************************************/

class TableBlk : public see::MkdBlock
{
public:
    TableBlk(): see::MkdBlock{"table"} {  }

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
                    text += see::MkdHighlight::Instance().get_block("normal").highlight(normal) + "│";
                    normal.clear();
                }
            }
            text += '\n' + newLine3rd + '\n';

            sstrm.ignore(512, '\n');
            // 复用之前的line2nd
            for ( int cnt{}; std::getline(sstrm, line2nd); ++cnt ) {
                auto& normalBlk = see::MkdHighlight::Instance().get_block("normal");
                text += dynamic_cast<NormalBlk&>(normalBlk).highlight(line2nd,
                        cnt % 2 ? "\033[38;5;240m\033[48;2;94;175;220m" : "\033[38;5;240m\033[48;2;128;128;255m")
                    + "\033[m\n";
            }
        } else {
            see::MkdHighlight::Instance().get_block("normal").highlight(text);
        }
        return text;
    }
private:
    static inline std::regex Table1st{R"(^\s*(\|.*)+\|)"};
    static inline std::regex Table2nd{R"(^\s*(\|[: -]+)+\|)"};
    static TableBlk Instance_;
};
TableBlk TableBlk::Instance_{};

/*************************************************************************************************/

class CodeBlk : public see::MkdBlock
{
    bool needHilit_;
public:
    CodeBlk(): see::MkdBlock{"code"}, needHilit_{true} {  }

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
    static CodeBlk Instance_;
};
CodeBlk CodeBlk::Instance_{};


class CommentBlk : public MkdBlock
{
public:
    CommentBlk(): MkdBlock{"comment"} {  }

    bool match_begin(const std::string& oneline) override
    {
        return std::regex_search(oneline, Comment);
    }

    bool match_end(const std::string&) override
    {
        return true;
    }

    virtual std::string& highlight(std::string& text) override
    {
        text.clear();
        return text;
    }

private:
    static inline std::regex Comment{R"(^<!--.*-->$)"};
    static CommentBlk Instance_;
};
CommentBlk CommentBlk::Instance_{};

} // namespace see

