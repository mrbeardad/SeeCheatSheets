/************************************************
 * see.cpp -- A program to search cheat sheet
 * Copyright (c) 2020 Heachen Bear
 * Author: Heachen Bear < mrbeardad@qq.com >
 * License: Apache
 ************************************************/

#include <algorithm>
#include <array>
#include <boost/locale.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <srchilite/sourcehighlight.h>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <tuple>
#include <unicode/display_width.hpp>
#include <unistd.h>
#include <utility>
#include <vector>

#include "mine.hpp"

namespace
{
struct Highlight
{
    int     row_;
    int     col_;
    bool    needPager_;
public:
    Highlight(): needPager_{}
    {
        winsize termWinSize{};
        ioctl(STDERR_FILENO, TIOCGWINSZ, &termWinSize);
        row_ = termWinSize.ws_row;
        col_ = termWinSize.ws_col - 2;  // 稍微比真实宽度小一点，防止折回
    }


    std::string highlight(std::istream& entry) noexcept
    {
        std::string ret{};

        for ( std::string oneline{}; std::getline(entry, oneline); ) {
            if ( !needPager_ && unicode::display_width(oneline) > col_ ) needPager_ = 1;

            auto isStart = [] (const std::string& line, size_t idx) {
                return std::regex_search(line, Start[idx]);
            };

            if ( isStart(oneline, HEADER) ) {
#ifndef NDEBUG
                std::cout << "header: " << oneline << std::endl;
#endif // NDEBUG
                do_header(oneline);
            } else if ( isStart(oneline, SEPERATOR) ) {
#ifndef NDEBUG
                std::cout << "seperator: " << oneline << std::endl;
#endif // NDEBUG
                do_seperator(oneline);
            } else if ( isStart(oneline, QUOTE) ) {
                for ( std::string nextline{}; std::getline(entry, nextline); ) {
                    if ( nextline.empty()
                            || isStart(nextline, HEADER)
                            || isStart(nextline, SEPERATOR)
                            || isStart(nextline, LISTS)
                            || isStart(nextline, CODE1)
                            || isStart(nextline, COMMENT) ) {
                        entry.rdbuf()->pubseekoff(-static_cast<int>(nextline.size()) - 1, io::cur, io::in); // 将最后读取的nextline还原回流中
                        break;
                    }
                    oneline += "\n" + nextline;
                }
#ifndef NDEBUG
                std::cout << "quote: " << oneline << std::endl;
#endif // NDEBUG
                do_quote(oneline);
            } else if ( isStart(oneline, LISTS) ) {
                for ( std::string nextline{}; std::getline(entry, nextline); ) {
                    if ( nextline.empty()
                            || isStart(nextline, HEADER)
                            || isStart(nextline, SEPERATOR)
                            || isStart(nextline, QUOTE)
                            || isStart(nextline, LISTS)
                            || isStart(nextline, CODE1)
                            || isStart(nextline, COMMENT) ) {
                        entry.rdbuf()->pubseekoff(-static_cast<int>(nextline.size()) - 1, io::cur, io::in); // 将最后读取的nextline还原回流中
                        break;
                    }
                    oneline += "\n" + nextline;
                }
#ifndef NDEBUG
                std::cout << "lists: " << oneline << std::endl;
#endif // NDEBUG
                do_lists(oneline);
            } else if ( isStart(oneline, TABLE1) ) {
                std::string nextline{};
                std::getline(entry, nextline);
                if ( !isStart(nextline, TABLE2) ) {
                    oneline += "\n" + nextline;
#ifndef NDEBUG
                    std::cout << "normal in table: " << oneline << std::endl;
#endif // NDEBUG
                    do_normal(oneline);
                } else {
                    oneline += "\n" + nextline;
                    while ( std::getline(entry, nextline) ) {
                        if ( !isStart(nextline, TABLE1) ) {
                            entry.rdbuf()->pubseekoff(-static_cast<int>(nextline.size()) - 1, io::cur, io::in); // 将最后读取的nextline还原回流中
                            break;
                        }
                        oneline += "\n" + nextline;
                    }
#ifndef NDEBUG
                    std::cout << "table: " << oneline << std::endl;
#endif // NDEBUG
                    do_table(oneline);
                }
            } else if ( isStart(oneline, CODE1) ) {
                for ( std::string nextline{}; std::getline(entry, nextline); ) {
                    oneline += "\n" + nextline;
                    if ( isStart(nextline, CODE2) ) break;
                }
#ifndef NDEBUG
                std::cout << "code: " << oneline << std::endl;
#endif // NDEBUG
                do_code(oneline);
            } else {
#ifndef NDEBUG
                std::cout << "normal: " << oneline << std::endl;
#endif // NDEBUG
                do_normal(oneline);
            }

            ret += oneline + "\n";
        }
        return ret;
    }


private:
    void do_normal(std::string& text, const std::string& color = "\033[m")
    {
        static std::regex           Code{R"(`(.*?)`)"};
        static std::regex           Tex{R"((\${1,2})(.*?)\1)"};
        static std::regex           Italic{R"(\*(.*?)\*)"};
        static std::regex           Bold{R"(\*\*(.*?)\*\*)"};
        static std::regex           BoldItalic{R"(\*\*\*(.*?)\*\*\*)"};
        static std::regex           Link{R"(\[(.*?)\]\((.*)\))"};
        static std::regex           Comment{R"((<!--.*?-->))"};
        static std::string          hilitColor{"\033[38;5;255m"};
        static std::string          codeColor{"\033[38;2;255;168;0m"};
        text = std::regex_replace(text, Link, "\033[33m[\033[m$1\033[33m](\033[4;34m$2\033[m\033[33m)" + color);
        text = std::regex_replace(text, Code, codeColor + " $1 " + color);
        text = std::regex_replace(text, Tex, codeColor + " $1 " + color);
        text = std::regex_replace(text, Italic, hilitColor + "\033[3m $1 \033[m" + color);
        text = std::regex_replace(text, Bold, hilitColor + "\033[3m  $1  \033[m" + color);
        text = std::regex_replace(text, BoldItalic, hilitColor + "\033[3m   $1   \033[m" + color);
        text = std::regex_replace(text, Comment, "\033[38;5;245m$1\033[m" + color);
        text.insert(0, color);
    }


    void do_lists(std::string& lists)
    {
        std::stringstream   sstrm{lists};
        lists.clear();
        for ( std::string oneline{}; std::getline(sstrm, oneline); ) {
            std::smatch headMatch{};
            std::string head{};
            std::string normal{};
            if ( std::regex_search(oneline, headMatch, Start[LISTS]) ) {
                head = headMatch.format("\033[36m$1\033[m");
                normal = headMatch.suffix().str();
            } else {
                normal = oneline;
            }
            do_normal(normal);
            lists = head + normal + "\n";
        }
        lists.erase(--lists.end());   // 最后一行的换行符在highlight函数里添加
    }


    void do_table(std::string& table)
    {
        std::stringstream   sstrm{table};
        std::string         line2nd{};
        std::string         line1st{"┌"};
        std::string         line3rd{"├"};

        auto nlPosP1 = table.find('\n') + 1;
        line2nd = table.substr(nlPosP1, table.find('\n', nlPosP1) - nlPosP1);
        table.clear();

        for ( auto pos = line2nd.begin() + 1, end = line2nd.end(); pos != end; ++pos ) {
            if ( *pos == '|' ) {
                line1st += "┬";
                line3rd += "┼";
            } else {    // '-'或':'
                line1st += "─";
                line3rd += "─";
            }
        }
        line1st.replace(line1st.size() - 3, 3, "┐\n│");
        line3rd.replace(line3rd.size() - 3, 3, "┤");
        table = line1st;
        std::getline(sstrm, line2nd);
        std::string normal{};
        for ( auto pos = line2nd.begin() + 1, end = line2nd.end(); pos != end; ++pos ) {
            if ( *pos != '|' ) {
                normal += *pos;
            } else {
                do_normal(normal);
                table += normal + "│";
                normal.clear();
            }
        }
        table += '\n' + line3rd + '\n';

        sstrm.ignore(512, '\n');
        for ( int cnt{}; std::getline(sstrm, line2nd); ++cnt ) {
            auto color = cnt % 2 ? "\033[38;5;240m\033[48;2;94;175;220m" : "\033[38;5;240m\033[48;2;128;128;255m";
            do_normal(line2nd, color);
            // std::cout << color + line2nd + "\033[m\n" << std::endl;
            table += color + line2nd + "\033[m\n";
        }
    }


    void do_code(std::string& code)
    {
        std::stringstream   sstrm{};
        std::vector<int>    nCols{};
        std::string         type{code.substr(0, code.find('\n'))};
        int                 maxCol{};

        sstrm.str(code.substr(type.size() + 1, code.find_last_not_of("`\n") - type.size()));   // 同时去掉形如```cpp头部与形如```尾部
        auto typePos = type.find_first_not_of("` ");
        if ( typePos != std::string::npos ) type = type.substr(typePos);
        else type.clear();

        for ( std::string oneline{}; std::getline(sstrm, oneline); ) {
            auto col = unicode::display_width(oneline);
            nCols.emplace_back(col);
            maxCol = std::max(maxCol, col);
        }
        sstrm.rdbuf()->pubseekpos(0);
        sstrm.clear();

        std::wstring wline1st((maxCol + 4 - type.size() - 4) / 2 + 1, L'─'); // +4 = 2个空格 + 2个│；-4 = 2个空格 + ┌ + ┐，+1表示向上舍入
        auto         line1st = boost::locale::conv::utf_to_utf<char>(wline1st);
        code = "\033[38;2;255;165;0m┌" + line1st + " \033[32m" + type + "\033[38;2;255;165;0m " + line1st + "┐\033[m\n";
        maxCol = wline1st.size() * 2 + type.size() + 4;

        std::stringstream           hilitStrm{};
        try {
            srchilite::SourceHighlight  codeHilit{"esc.outlang"};
            codeHilit.highlight(sstrm, hilitStrm, type + ".lang");
        } catch (std::exception& excep) {
            hilitStrm.str(sstrm.str());
        }

        size_t idx{};
        for ( std::string oneline{}; std::getline(hilitStrm, oneline); ) {
            oneline = std::regex_replace(oneline, "\033\\[01;30m"_rgx, "\033[01;34m");
            code += "\033[38;2;255;165;0m│\033[m " + oneline + std::string(maxCol - nCols.at(idx++) - 4, ' ') + " \033[38;2;255;165;0m│\033[m\n";
        }

        std::wstring lastLine(maxCol - 2, L'─');
        code += "\033[38;2;255;165;0m└" + boost::locale::conv::utf_to_utf<char>(lastLine) + "┘\033[m";
    }


    void do_quote(std::string& quote)
    {
        std::stringstream   sstrm{quote};
        quote.clear();
        for ( std::string oneline{}; std::getline(sstrm, oneline); ) {
            auto sep    = oneline.find('>') + 1;
            auto head   = oneline.substr(0, sep);
            auto normal = oneline.substr(sep);
            head.replace(sep - 1, 1, "\033[48;5;235m\033[38;5;240m>\033[m");
            do_normal(normal, "\033[38;5;245m");
            quote += head + normal + "\n";
        }
        quote.erase(--quote.end());   // 最后一行的换行符在highlight函数里添加
    }


    void do_seperator(std::string& seperator)
    {
        std::wstring sep(col_ - 12, L'─');
        std::wstring spc(6, L' ');
        seperator = boost::locale::conv::utf_to_utf<char>(spc + sep);
    }


    void do_header(std::string& header)
    {
        auto normal = header.substr(header.find_first_not_of("# "));
        auto hashs = header.substr(0, header.find(" "));
        int numHeng{(col_ - unicode::display_width(normal) - static_cast<int>(hashs.size()) * 2 - 2) / 2};
        do_normal(normal, "\033[34m");
        std::wstring wnHeng(numHeng, L'─');
        auto nHeng = boost::locale::conv::utf_to_utf<char>(wnHeng);

        header = "\033[34;42m" + nHeng + "\033[m\033[34m " + hashs + " " + normal +  " " + hashs + " \033[34;42m" + nHeng + "\033[m";
    }


    static inline std::array Start{
        R"(^(#+)\s*\S+$)"_rgx,      // header
        R"(^(\s*\*\s*){3,}$)"_rgx,  // seperator
        R"(^\s*>+\s+\S*)"_rgx,      // quote
        R"(^(\s*[-*+]\s+))"_rgx,    // lists
        R"(^\s*`{3,}.*)"_rgx,       // code1
        R"(^\s*`{3,}$)"_rgx,        // code2
        R"(^\s*(\|.*)+\|)"_rgx,     // table1
        R"(^\s*(\|[: -]+)+\|)"_rgx, // table2
        R"(^<!--.*-->)"_rgx         // comment
    };

    enum BlockType {
        HEADER,
        SEPERATOR,
        QUOTE,
        LISTS,
        CODE1,
        CODE2,
        TABLE1,
        TABLE2,
        COMMENT
    };
};

/*************************************************************************************************/

std::stringstream   Output{};
Highlight           Hilit{};

// 打印错误提示（或者叫帮助信息）并退出
void print_help_then_exit() noexcept;

// 搜索用户指定查询的文件，并将结果返回到引用形参files
void search_files(std::vector<std::string>& files) noexcept;

// 解析命令行参数，返回需要搜索的keys和files
std::tuple<std::vector<std::string>, std::vector<std::string>, bool>
    parse_cmdline(int argc, char* argv[]) noexcept;

// 在文件中搜索entry
void search_entries_print(const std::string& file, const std::vector<std::string>& keys) noexcept;

} // namespace



int main(int argc, char* argv[])
{
    if ( !isatty(STDIN_FILENO) ) {  // if redirect stdin to a file, then read entry from this file
        std::cout << Hilit.highlight(std::cin);
        return 0;
    }
    auto [keys, files, disablePager] = parse_cmdline(argc, argv);

    for ( auto&& thisFile : files ) {
        search_entries_print(thisFile, keys);
    }

    if ( !disablePager ) {
        auto lineNum = std::count(std::istreambuf_iterator<char>{Output}, std::istreambuf_iterator<char>{}, '\n');
        Output.rdbuf()->pubseekpos(0);
        Hilit.needPager_ = lineNum > Hilit.row_;
        if ( Hilit.needPager_ ) {
            auto    pager = getenv("PAGER");
            int     fds[2];
            mine::handle(pipe(fds));
            if ( auto pid = mine::handle(fork()); pid == 0 ) {
                dup2(fds[0], STDIN_FILENO);
                close(fds[1]);
                mine::handle(execlp(pager, pager, nullptr));
            } else {
                dup2(fds[1], STDOUT_FILENO);
                close(fds[1]);
            }
        }
    }
    std::cout << Output.rdbuf() << std::endl;
    close(STDOUT_FILENO);
    wait(nullptr);

    return 0;
}

namespace
{
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
        "    \033[36m-f\033[m <file-prefix>  : Specify files in ~/.cheat/ whoes file name prefix match <file-prefix> to search\n"
        "    \033[36m-w\033[m <keyword>      : Point out that the regex you give is a complete word, not a part of a word\n"
        "    \033[36m-p\033[m                : Disable to use Pager\n"
        << std::endl;
    std::exit(1);
}

void search_files(std::vector<std::string>& files) noexcept
{
    fs::path cheatDir{std::getenv("HOME")};
    cheatDir /= ".cheat/";

    // 如果~/.cheat/目录不存在则报错并退出
    if ( !fs::is_directory(cheatDir) ) {
        std::cerr << "Error: No such directory ~/.cheat/" << std::endl;
        std::exit(1);
    }

    auto origFiles = files;
    files.clear();

    for ( const auto& thisDirEntry : fs::directory_iterator{cheatDir} ) {
        if ( !fs::is_regular_file(thisDirEntry) || thisDirEntry.path().extension() != ".md" ) {
            continue;
        }

        if ( origFiles.empty() ) {
            files.emplace_back(thisDirEntry.path());

        } else {
            std::string curFileName{thisDirEntry.path().filename()};

            for ( auto& thisFilePrefix : origFiles ) {
                // 确认文件名前缀是否与用户指定匹配
                if ( curFileName.find(thisFilePrefix) != std::string::npos ) {
                    files.emplace_back(thisDirEntry.path());
                    break;
                }
            }
        }
    }
}


std::tuple<std::vector<std::string>, std::vector<std::string>, bool>
    parse_cmdline(int argc, char* argv[]) noexcept
{
    std::vector<std::string> keys{}, words{}, files{};
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
            case 'w':
                words.emplace_back(optarg);
                break;
            case 'f':
                files.emplace_back(optarg);
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
    if ( keys.empty() && words.empty() ) {
        std::cerr << "\e[31m`see` Error:\e[0m Need entry-regex-keys" << std::endl;
        print_help_then_exit();
    }

    // 将words转换成keys
    std::for_each(words.begin(), words.end(), [&keys](auto&& e){keys.emplace_back(R"(\b)" + e + R"(\b)");});

    search_files(files);

    return std::tuple{keys, files, disablePager};
}

/*************************************************************************************************/

// void highlight_print(std::istream& entry) noexcept
// {
//     static std::array RegexAndRepl{
//         std::pair{R"(<!--(.*?)-->)"_rgx, "\033[38;2;128;160;225m<!--$1-->\033[m"}, // comment
//         std::pair{R"(<!-- entry begin:(.*)-->)"_rgx, "<!-- entry begin:\033[38;2;255;160;160m$1\033[38;2;128;160;255m-->"}, // comment
//         std::pair{"\033\\[48;5;244m \033\\[0m(.*)"_rgx, "\033[48;2;0;0;0m\033[38;5;245m>\033[m\033[38;5;243m $1\033[m"},
//         std::pair{R"(^(#+) (.*))"_rgx, "\033[38;2;255;165;0m$1 $2"}, // header
//         std::pair{R"(^([-+*]) (.*))"_rgx, "\033[36m$1\033[m $2"}, // list1
//         std::pair{R"(^(\s{4,})([-+*]) (.*))"_rgx, "$1\033[35m$2\033[m $3"}, // list2
//         std::pair{R"(^(\s{8,})([-+*]) (.*))"_rgx, "$1\033[34m$2\033[m $3"}, // list3
//         std::pair{R"(^(\s{12,})([-+*]) (.*))"_rgx, "$1\033[33m$2\033[m $3"} // list4
//     };
//
//     std::string oneline{};
//     while ( std::getline(entry, oneline) ) {
//         for ( auto& thisRAR : RegexAndRepl ) {
//             oneline = std::regex_replace(oneline, thisRAR.first, thisRAR.second);
//         }
//         Output << oneline << '\n';
//     }
//     Output << '\n';
//     entry.clear();
// }


void search_entries_print(const std::string& file, const std::vector<std::string>& keys) noexcept
{
    // 该函数会多次调用，将不变的对象静态存储
    static std::stringstream    Entry{};
    static std::regex           Regex4Beg{R"(^<!-- entry begin:.*-->$)", std::regex_constants::icase};
    static std::regex           Regex4end{R"(^<!-- entry end -->$)", std::regex_constants::icase};
    static std::string          Oneline{};

    std::fstream    fstrm{file, io::in};
    if ( !fstrm ) {
        std::cerr << "Error: Can not read file -- " << file << std::endl;
        return;
    }

    bool onceMatch{false}; // 用于提前打印文件名
    bool inEntry{false};   // 用于判断当前匹配是否为某一entry内容

    while ( std::getline(fstrm, Oneline) ) {
        // 若未在entry中，则搜索合适entry并进入
        if ( !inEntry && std::regex_match(Oneline, Regex4Beg) ) {
            auto curEntryKeys = Oneline.substr(sizeof("<!-- entry begin:"), Oneline.size() - sizeof("-->"));

            // 有一个key不匹配则略过
            auto isMatch = true;

            for ( auto& thisKey : keys ) {
                if ( !std::regex_search(curEntryKeys.begin(), curEntryKeys.end(), std::regex{thisKey, std::regex_constants::icase}) ) {
                    isMatch = false;
                    break;
                }
            }

            if ( isMatch ) {
                if ( !onceMatch ) {
                    Output <<
                    "\033[38;2;255;0;0m\033[38;2;255;34;0m \033[38;2;255;85;0m \033[38;2;255;136;0m "
                    "\033[38;2;255;170;0m \033[38;2;255;204;0m \033[38;2;255;255;0m \033[38;2;204;255;0m "
                    "\033[38;2;170;255;0m \033[38;2;136;255;0m \033[38;2;85;255;0m \033[38;2;34;255;0m "
                    "\033[38;2;0;255;0m \033[38;2;0;255;34m \033[38;2;0;255;85m \033[38;2;0;255;136m "
                    "\033[38;2;0;255;170m \033[38;2;0;255;204m \033[38;2;0;255;255m \033[38;2;0;204;255m "
                    "\033[38;2;0;170;255m \033[38;2;0;136;255m \033[38;2;0;85;255m \033[38;2;0;51;255m "
                    "\033[38;2;0;0;255m \033[38;2;51;0;255m \033[38;2;85;0;255m \033[38;2;136;0;255m "
                    "\033[38;2;170;0;255m \033[38;2;204;0;255m \033[38;2;255;0;255m \033[m "
                    << file << "\e[m\n";
                    onceMatch = true;
                }
                inEntry = true;
                Entry << Oneline << '\n';
            }
        } else if ( inEntry ) {
            // 若在entry中且搜索到结束标志，则退出inEntry状态
            if ( std::regex_match(Oneline, Regex4end) ) {
                inEntry = false;

                auto outputStr = Hilit.highlight(Entry);
                Output << outputStr << std::endl;

                Entry.clear();
            // 若在entry中且未搜索到结束标志，则将该行送入entry流中
            } else {
                Entry << Oneline << '\n';
            }
        }
    }
}

} // namespace

