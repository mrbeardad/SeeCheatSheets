/************************************************
 * see.cpp -- A program to search cheat sheet
 * Copyright (c) 2020 Heachen Bear
 * Author: Heachen Bear < mrbeardad@qq.com >
 * License: Apache
 ************************************************/

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unistd.h>
#include <utility>
#include <vector>

#define int int32_t
#define long int64_t

using std::literals::operator""s;

namespace
{
    void print_help_then_exit() noexcept
    {
        std::cout << "\033[32mDescription:\033[m\n";
        std::cout << "    \033[33msee\033[m will find cheat-sheets among ~/.cheat/notes/*.md.\n";
        std::cout << "Yeah! They are markdown files. Each entry starts with '\033[34m<!-- entry begin: keywords... -->\033[m',\n";
        std::cout << "and ends with '\033[34m<!-- entry end -->\033[m'. \033[33msee\033[m will search apposite entries and print them.\n";
        std::cout << "    For example, \033[33msee echo.*\033[m will match :\n\033[36m";
        std::cout << "        <!-- entry begin: echo -->\n";
        std::cout << "        * echo\n";
        std::cout << "            * -n        ：不自动加入换行符（zsh会将无换行结尾的输出的尾部标记`反显的%`）\n";
        std::cout << "            * -e        ：启用转义语义（zsh自动开启）\n";
        std::cout << "        <!-- entry end -->\n";
        std::cout << "\033[32mUsage:\033[m\n";
        std::cout << "    see [<options>] <keyword> [<keyword>...]\n";
        std::cout << "\033[32mOptions:\033[m\n";
        std::cout << "    \033[36m-h\033[m                : Display this help information\n";
        std::cout << "    \033[36m-f\033[m <file-prefix>  : Specify files in ~/.cheat/notes whoes file name prefix match <file-prefix> to search\n";
        std::cout << "    \033[36m-w\033[m <keyword>      : Point out that the regex you give is a complete word, not a part of a word\n";
        std::cout << std::endl;
        std::exit(EXIT_FAILURE);
    }


    void search_files(std::vector<std::string>& files) noexcept
    {
        std::filesystem::path cheatDir{std::getenv("HOME")};
        cheatDir /= ".cheat/notes";
        if ( !std::filesystem::is_directory(cheatDir) ) {
            std::cerr << "Error: No such directory ~/.cheat/notes" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        auto origFiles = files;
        files.clear();
        for ( std::filesystem::directory_iterator itr{cheatDir}, end{}; itr != end; ++itr ) {
            if ( itr->path().extension() != ".md" ) {
                continue;
            }

            if ( origFiles.empty() ) {
                files.emplace_back(itr->path());
            } else {
                std::string curFileName{itr->path().filename()};
                for ( auto& thisFilePrefix : origFiles ) {
                    if ( curFileName.find(thisFilePrefix) != std::string::npos ) {
                        files.emplace_back(itr->path());
                        break;
                    }
                }
            }
        }
    }


    void highlight(std::istream& entry);


    int main_v1(int argc, char* argv[]);


    auto parse_cmdline(int argc, char* argv[]) noexcept
    {
        std::vector<std::string> keys{}, words{}, files{};
        auto needOutput = false, needRepl = false;
        while ( true )
        {
            auto choice = getopt(argc, argv, "ORhw:f:");
            if ( choice == -1 ) {
                break;
            }
            switch ( choice ) {
                case '?':
                case 'h':
                    print_help_then_exit();
                    break;
                case 'w':
                    words.emplace_back(optarg);
                    break;
                case 'f':
                    files.emplace_back(optarg);
                    break;
                case 'R':
                    needRepl = true;
                    break;
                case 'O':
                    needOutput = true;
                    break;
            }
        }

        if ( needRepl ) {
            highlight(std::cin);
            std::exit(EXIT_SUCCESS);
        } else if ( !needOutput ) {
            std::string cmd;
            for ( int i{}; i < argc; ++i ) {
                cmd += argv[i];
                cmd += ' ';
            }
            cmd = "bash -o pipefail -c \"miro <(" + cmd + "-O) | " + argv[0] + " -R\" 2> /dev/null";
            if ( std::system(cmd.data()) ) {
                main_v1(argc, argv);
            }
            std::exit(EXIT_SUCCESS);
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

        std::for_each(words.begin(), words.end(), [&keys](auto&& e){keys.emplace_back(R"(\b)" + e + R"(\b)");});
        search_files(files);

        return std::tuple{keys, files};
    }


    std::regex operator""_regex(const char* cstr, size_t len)
    {
        return std::regex{cstr};
    }


    void highlight(std::istream& entry) {
        static std::array RegexAndRepl{
            std::pair{R"(<!--(.*?)-->)"_regex, "\033[38;2;128;160;225m<!--$1-->\033[m"}, // comment
            std::pair{R"(<!-- entry begin:(.*)-->)"_regex, "<!-- entry begin:\033[38;2;255;160;160m$1\033[38;2;128;160;255m-->"}, // comment
            std::pair{"\033\\[48;5;244m \033\\[0m(.*)"_regex, "\033[48;2;0;0;0m\033[38;5;245m>\033[m\033[38;5;243m $1\033[m"}
            // std::pair{R"(^(#+) (.*))"_regex, "\033[38;2;255;165;0m$1 $2"}, // header
            // std::pair{R"(^([-+*]) (.*))"_regex, "\033[36m$1\033[m $2"}, // list1
            // std::pair{R"(^(\s{4,})([-+*]) (.*))"_regex, "$1\033[35m$2\033[m $3"}, // list2
            // std::pair{R"(^(\s{8,})([-+*]) (.*))"_regex, "$1\033[34m$2\033[m $3"}, // list3
            // std::pair{R"(^(\s{12,})([-+*]) (.*))"_regex, "$1\033[33m$2\033[m $3"} // list4
        };

        std::string oneline{};
        while ( std::getline(entry, oneline) ) {
            for ( auto& thisRAR : RegexAndRepl ) {
                oneline = std::regex_replace(oneline, thisRAR.first, thisRAR.second);
            }
            std::cout << oneline << '\n';
        }
        std::cout << '\n';
        entry.clear();
    }


    void search_entries(const std::string& file, const std::vector<std::string>& keys) noexcept
    {
        static std::stringstream Entry{};
        static std::regex Regex4Beg{R"(^<!-- entry begin:.*-->)", std::regex_constants::icase},
            Regex4end{R"(^<!-- entry end -->$)", std::regex_constants::icase};
        static std::string Oneline{};

        std::fstream fstrm{file, std::ios_base::in};
        if ( !fstrm ) {
            std::cerr << "Error: Can not read file -- " << file << std::endl;
            return;
        }

        auto anyMatch = false; // 用于提前打印文件名
        auto inEntry = false;
        while ( std::getline(fstrm, Oneline) ) {
            // 若未在entry中，则搜索合适entry并进入
            if ( !inEntry && std::regex_match(Oneline, Regex4Beg) ) {
                std::string_view onelineView{Oneline};
                onelineView.remove_prefix(16);
                onelineView.remove_suffix(onelineView.size() - onelineView.find("-->"));

                auto isMatch = true;
                for ( auto& thisKey : keys ) {
                    if ( !std::regex_search(onelineView.begin(),onelineView.end(), std::regex{thisKey, std::regex_constants::icase}) ) {
                        isMatch = false;
                        break;
                    }
                }
                if ( isMatch ) {
                    if ( !anyMatch ) {
                        std::cout << "\e[32m  " << file << "\e[m\n";
                        anyMatch = true;
                    }
                    inEntry = true;
                    Entry << Oneline << '\n';
                }
            } else if ( inEntry ) {
                // 若在entry中且搜索到结束标志，则退出inEntry状态
                if ( std::regex_match(Oneline, Regex4end) ) {
                    inEntry = false;
                    std::cout << Entry.rdbuf() << std::endl;
                // 若在entry中且未搜索到结束标志，则将改行送入entry流中
                } else {
                    Entry << Oneline << '\n';
                }
            }
        }
        fstrm.clear();
    }
}


int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // 获取 keys, files
    auto [keys, files] = parse_cmdline(argc, argv);

    for ( auto&& thisFile : files ) {
        search_entries(thisFile, keys);
    }

    return 0;
}

namespace {
    std::vector<bool> Opts{}; // 表示是否存在选项
    std::vector<std::string> Files{}; // 选项"f"的参数（用户指定文件）
    std::vector<std::string> Keys{}; // 剩下的命令参数（用户指定关键字）

    void print_help() noexcept
    {
        std::cout << "\e[32mDescription:\e[m\n";
        std::cout << "    \e[33msee\e[m will find cheat-sheets among ~/.cheat/notes/*.md.\n";
        std::cout << "Yeah! They are markdown files. Each entry starts with '\e[34m<!-- entry begin: .* -->\e[m',\n";
        std::cout << "and ends with '\e[34m<!-- entry end -->\e[m'. \e[33msee\e[m will search apposite entries and print them.\n";
        std::cout << "    For example, \e[33msee echo\e[m will match :\n\e[36m";
        std::cout << "        <!-- entry begin: echo -->\n";
        std::cout << "        * echo\n";
        std::cout << "            * -n        ：不自动加入换行符（zsh会将无换行结尾的输出的尾部标记`反显的%`）\n";
        std::cout << "            * -e        ：启用转义语义（zsh自动开启）\n";
        std::cout << "        <!-- entry end -->\n";
        std::cout << "\e[32mUsage:\e[m\n";
        std::cout << "    see [<options>] <regex>\n";
        std::cout << "\e[32mOptions:\e[m\n";
        std::cout << "    \e[36m-h\e[m                : Display this help information\n";
        std::cout << "    \e[36m-f\e[m <file-prefix>  : Specify files in ~/.cheat/notes whoes file name prefix match <file-prefix> to search\n";
        std::cout << "    \e[36m-w\e[m                : Point out that the regex you give is a complete word, not a part of a word\n";
        std::cout << std::endl;
    }

    void parse_cmdline_v1(int argc, char* argv[]) noexcept
    {
        std::string optstring{"hwf:"}; // 方便未来添加参数
        Opts.resize(optstring.size());
        while ( true ) // 读取命令行参数
        {
            auto choice = getopt(argc, argv, optstring.data());
            if ( choice == -1 ) {
                break;
            }
            switch ( choice ) {
                case '?':
                case 'h':
                    Opts[0] = true;
                    break;
                case 'w':
                    Opts[1] = true;
                    break;
                case 'f':
                    Files.emplace_back(optarg);
                    break;
                default:
                    break;
            }
        }
        while ( optind < argc ) { // 读取命令参数
            Keys.emplace_back(argv[optind++]);
        }
        if ( Opts[0] || Keys.empty() ) { // 处理参数无效的情况
            if ( Keys.empty() ) {
                std::cerr << "\e[31m`see` Error:\e[0m Need entry-regex-keys" << std::endl;
            }
            print_help();
            std::exit(EXIT_FAILURE);
        }
    }

    std::string& parse(std::string& str) noexcept
    {
        static auto inCode = false;
        if ( std::regex_search(str, std::regex{R"((\s*`{3,}\w*$))"}) ) { // 判断当前是否在代码块中
            inCode = !inCode;
            str = std::regex_replace(str, std::regex{R"((\s*`{3,}\w*))"}, "\e[38;2;181;137;0m$1\e[m"); // 代码区块
        }

        if ( std::regex_search(str, std::regex{R"(^(\s*)> (.*))"})  ) { // 判断是否在引用标签中
            str = std::regex_replace(str, std::regex{R"(^(\s*)> (.*))"}, "$1\e[48;2;0;0;0m\e[38;5;245m>\e[m\e[38;5;243m $2\e[m");
        } else if ( !inCode ) { // 若不在代码块中且不再引用中则解析
            str = std::regex_replace(str, std::regex{R"((<!--.*-->))"}, "\e[38;5;240m$1\e[m"); // 注释标签
            str = std::regex_replace(str, std::regex{R"((\*{1,3})(?!\s)([^*]+)\1)"}, "\e[1;3m$2\e[m"); // Italic Bold
            str = std::regex_replace(str, std::regex{R"(<u>(.+)</u>)"}, "\e[4m$1\e[m"); // 下划线
            str = std::regex_replace(str, std::regex{R"(<kbd>(.+)</kbd>)"}, "\e[7m$1\e[m"); // 按键
            str = std::regex_replace(str, std::regex{R"(~~([^~]+)~~)"}, "\e[9m$1\e[m"); // 删除线
            str = std::regex_replace(str, std::regex{R"(^[*+-] (.+))"}, "\e[1;33m* \e[36m$1\e[m"); // 一级列表
            str = std::regex_replace(str, std::regex{R"(^\s{4,7}[*+-] (.+))"}, "    \e[1;35m*\e[m $1"); // 二级列表
            str = std::regex_replace(str, std::regex{R"(^\s{8,11}[*+-] (.+))"}, "        \e[1;34m*\e[m $1"); // 三级列表
            str = std::regex_replace(str, std::regex{R"(`([^`]+)`)"}, "\e[48;5;238m$1\e[m"); // 行内代码
        }
        return str;
    }

    void search_and_print(const std::filesystem::path& file) noexcept
    {
        // 需要多次使用该ifstream与string
        static std::ifstream Fstrm{};
        static std::string OneLine{};

        Fstrm.open(file); // 打开文件

        auto isCpp = file.filename() == "cppman.md"; // 判断是否为cppman.md

        // 对该文件中的内容一行一行地操作，每行又四种可能：区块头、区块尾、匹配的区块内容、非匹配的区块内容
        for ( auto inEntry = false, onceFile = true; std::getline(Fstrm, OneLine); ) {
            if ( inEntry ) { // 如果当前行已在entry区块中
                if ( OneLine == "<!-- entry end -->" ) { // 如果匹配到区块尾则退出entry区块
                    inEntry = false;
                    std::cout << '\n'; // 每个entry区块后添加空行
                } else { // 如果是区块内的内容则解析后输出
                    if ( isCpp ) {
                        OneLine = std::regex_replace(OneLine, std::regex{R"((::|\.)(\w+))"}, "\e[32m$1\e[m$2"); // C++代码中领域符`::`与成员符`.`
                    }
                    std::cout << parse(OneLine) << '\n';
                }
            } else if ( OneLine.find("<!-- entry begin:", 0) != std::string::npos && OneLine.rfind("-->") != std::string::npos ) { // 如果当前行未在entry区块中
                std::string_view keywords{OneLine}; // 制作entry头部的关键字视图
                keywords.remove_prefix(17);
                keywords.remove_suffix(3);

                // 需要匹配“所有”用户指定的关键字（“任意一个”不匹配则失败），匹配成功则进入entry区块
                auto isMatch = true;
                for ( auto& thisKey : Keys ) {
                    std::regex pattern{}; // 制作正则模式串
                    if ( Opts[1] ) {
                        pattern.assign("\\b" + thisKey + "\\b");
                    } else {
                        pattern.assign(thisKey);
                    }
                    if ( !std::regex_search(keywords.begin(), keywords.end(), pattern) ) { // 如果任意一个不匹配则失败，换下一行
                        isMatch = false;
                        break;
                    }
                }
                if ( isMatch ) { // 如果匹配成功则
                    inEntry = true; // 进入entry区块
                    if ( onceFile ) {
                        std::cout << "\e[32m 📖️ " << file << "\e[m\n";
                        onceFile = false; // 打印文件名（只打印一次）
                    }
                    std::cout << parse(OneLine) << '\n';
                }
            }
        }

        Fstrm.close();
        Fstrm.clear();
    }

    int main_v1(int argc, char* argv[])
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);

        parse_cmdline_v1(argc, argv); // 解析命令行参数

        std::filesystem::path cheatDir{std::getenv("HOME")}; // 获取用户家目录
        cheatDir /= ".cheat/notes";
        if ( !std::filesystem::exists(cheatDir) ) { // 判断cheat-sheets所在目录是否存在
            std::cerr << "\e[31m`see` Error:\e[0m directory " << cheatDir << " doesn't exist";
            std::exit(EXIT_FAILURE);
        }

        // 对cheat-sheets目录下的每个文件进行操作
        auto anyFileMatch = false;
        for ( auto itr = std::filesystem::directory_iterator{cheatDir}, end = std::filesystem::directory_iterator{}; itr != end; ++itr ) {
            auto& curFile = itr->path().filename().native();

            // 排除不符合条件的文件
            auto isMatch = false;
            if ( Files.empty() ) {
                anyFileMatch = isMatch = true;
            } else {
                for ( auto& specifile : Files ) { // 匹配“任意一个”用户指定的文件即可
                    if ( curFile.rfind(".md") != std::string::npos && curFile.find(specifile, 0) != std::string::npos ) {
                        anyFileMatch = isMatch = true;
                        break;
                    }
                }
            }

            if ( isMatch ) { // 若匹配成功则搜索entry并打印
                search_and_print(itr->path());
            }
        }
        if ( !anyFileMatch ) {
            std::cerr << "\e[31m`see` Error:\e[0m No matching files";
            print_help();
        }

        return 0;
    }

} // namespace

