/*
 * Author: Heachen Bear
 * Email: mrbeardad@qq.com
 *
 * TODO:
 * 1. entry区块内列表查询
 * 2. 管道取消颜色
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <unistd.h>
#include <vector>

namespace {
    std::vector<bool> Opts{}; // 表示是否存在选项"hl"
    std::vector<std::string> Files{}; // 选项"f"的参数
    std::vector<std::string> Keys{}; // 命令参数

    void print_help()
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

    void get_cmd(int argc, char* argv[])
    {
        std::string optstring{"hwf:"}; // 方便未来添加参数
        Opts.resize(optstring.size());
        while ( true )
        {
            auto choice = getopt(argc, argv, optstring.data());
            if ( choice == -1 ) {
                break;
            }
            switch ( choice ) {
                case '?':
                case 'h':
                    Opts.at(0) = true;
                    break;
                case 'w':
                    Opts.at(1) = true;
                    break;
                case 'f':
                    Files.emplace_back(optarg);
                    break;
                default:
                    break;
            }
        }
        while ( optind < argc ) {
            Keys.emplace_back(argv[optind++]);
        }
        if ( Opts[0] || Keys.empty() ) {
            print_help();
            exit(1);
        }
    }

    void parse_and_print(std::string& str, bool isCpp)
    {
        static auto inCode = false;
        if ( std::regex_search(str, std::regex{R"((\s*`{3,}\w*$))"}) ) { // 解析代码块头部或尾部
            if ( inCode ) {
                inCode = false;
            } else {
                inCode = true;
            }
            str = std::regex_replace(str, std::regex{R"((\s*`{3,}\w*))"}, "\e[38;2;181;137;0m$1\e[m"); // 代码区块
        }
        if ( std::regex_search(str, std::regex{R"(^(\s*)> (.*))"})  ) {
            str = std::regex_replace(str, std::regex{R"(^(\s*)> (.*))"}, "$1\e[48;2;0;0;0m\e[38;5;245m>\e[m\e[38;5;243m $2\e[m"); // 区块标签
        } else if ( !inCode ) { // 若不在代码块中且不再引用中则解析
            str = std::regex_replace(str, std::regex{R"((<!--.*-->))"}, "\e[38;5;240m$1\e[m"); // 注释标签
            if ( isCpp ) {
                str = std::regex_replace(str, std::regex{R"((::|\.)(\w+))"}, "\e[32m$1\e[m$2"); // C++代码中领域符`::`与成员符`.`
            }
            str = std::regex_replace(str, std::regex{R"((\*{1,3})(?!\s)([^*]+)\1)"}, "\e[1;3m$2\e[m"); // Italic Bold
            str = std::regex_replace(str, std::regex{R"(<u>(.+)</u>)"}, "\e[4m$1\e[m"); // 下划线
            str = std::regex_replace(str, std::regex{R"(<kbd>(.+)</kbd>)"}, "\e[7m$1\e[m"); // 按键
            str = std::regex_replace(str, std::regex{R"(~~([^~]+)~~)"}, "\e[9m$1\e[m"); // 删除线
            str = std::regex_replace(str, std::regex{R"(^[*+-] (.+))"}, "\e[1;33m* \e[36m$1\e[m"); // 一级列表
            str = std::regex_replace(str, std::regex{R"(^\s{4,7}[*+-] (.+))"}, "    \e[1;35m*\e[m $1"); // 二级列表
            str = std::regex_replace(str, std::regex{R"(^\s{8,11}[*+-] (.+))"}, "        \e[1;34m*\e[m $1"); // 三级列表
            str = std::regex_replace(str, std::regex{R"(`([^`]+)`)"}, "\e[48;5;238m$1\e[m"); // 行内代码
        }
        std::cout << str << '\n';
    }

    void search_and_print(const std::filesystem::path& file)
    {
        // 需要多次使用该ifstream与string
        static std::ifstream Fstrm{};
        static std::string OneLine{};
        Fstrm.open(file);

        auto isCpp = file.filename() == "cppman.md" ? true : false;

        // 一行一行地操作
        for ( auto inEntry = false, onceFile = true; getline(Fstrm, OneLine); ) {
            if ( inEntry ) { // 如果当前行已在entry区块中，则匹配到区块尾则退出entry区块，而对其他区块内的内容解析后输出
                if ( OneLine == "<!-- entry end -->" ) {
                    inEntry = false;
                    parse_and_print(OneLine, isCpp);
                    std::cout << std::endl; // 每个entry区块后添加空行
                } else {
                    parse_and_print(OneLine, isCpp);
                }
            // 如果当前行未在entry区块中，则匹配到符合的entry头便进入entry区块
            } else if ( OneLine.find("<!-- entry begin:", 0) != std::string::npos && OneLine.rfind("-->") != std::string::npos ) {
                std::string_view keywords{OneLine}; // 制作entry头部的关键字视图
                keywords.remove_prefix(17);
                keywords.remove_suffix(3);

                // 需要匹配“所有”用户指定的关键字，即“任意一个”不匹配则失败
                auto isMatch = true;
                for ( auto& thisKey : Keys ) {
                    std::regex pattern{}; // 制作正则模式串
                    if ( Opts[1] ) {
                        pattern.assign("\\b" + thisKey + "\\b"); // 为选项w添加单词边界
                    } else {
                        pattern.assign(thisKey);
                    }
                    if ( !std::regex_search(keywords.begin(), keywords.end(), pattern) ) { // 任意一个不匹配则失败
                        isMatch = false;
                        break;
                    }
                }
                if ( isMatch ) { //匹配成功则
                    inEntry = true; // 进入entry区块
                    if ( onceFile ) {
                        std::cout << "\e[32m 📖️ " << file << "\e[m\n";
                        onceFile = false; // 打印文件名，只打印一次
                    }
                    parse_and_print(OneLine, isCpp);
                }
            }
        }

        Fstrm.close();
        Fstrm.clear();
    }

} // namespace

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    get_cmd(argc, argv); // 解析命令行参数

    std::filesystem::path cheatDir{getenv("HOME")}; // 获取用户家目录
    cheatDir /= ".cheat/notes";
    if ( !std::filesystem::exists(cheatDir) ) { // 判断cheat-sheets所在目录是否存在
        std::cerr << "Error: directory " << cheatDir << " doesn't exist";
        exit(1);
    }

    // 对cheat-sheets目录下的每个文件进行操作
    for ( auto& entry : std::filesystem::directory_iterator{cheatDir} ) {
        auto& curFile = entry.path().filename().native();

        // 排除不符合条件的文件
        auto isMatch = false;
        for ( auto& specifile : Files ) { // 需要匹配“任意一个”用户指定的文件
            if ( curFile.find(specifile, 0) != std::string::npos && curFile.rfind(".md") != std::string::npos ) {
                isMatch = true;
                break;
            }
        }
        if ( isMatch || Files.empty() ) { // 若匹配成功则搜索entry并打印
            search_and_print(entry.path());
        }
    }

    return 0;
}

