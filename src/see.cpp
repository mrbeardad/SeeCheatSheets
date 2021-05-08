/**
 * Copyright (c) 2020 Heachen Bear & Contributors
 * File: see.cpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 09.02.2021
 * Last Modified Date: 08.05.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */

#include <fstream>
#include <iostream>
#include <regex>
#include <unistd.h>

#include "mine.hpp"

#include "code.hpp"
#include "comment.hpp"
#include "header.hpp"
#include "list.hpp"
#include "normal.hpp"
#include "quote.hpp"
#include "see.hpp"
#include "seperator.hpp"
#include "table.hpp"


namespace see
{
/*************************************************************************************************/
// MkdBlock
/*************************************************************************************************/
MkdBlock::~MkdBlock() =default;


// 注册器：其子类仅需创建一个对象实例便自动向中介者(MkdHighlight)注册
MkdBlock::MkdBlock(const std::string& name)
    : mediator_{MkdHighlight::Instance()}
{
    if ( !mediator_.regist(name, this) ) {
        throw std::logic_error{name + "regist error"};
    }
}


/*************************************************************************************************/
// MkdHighlight
/*************************************************************************************************/
MkdHighlight::MkdHighlight()
    : winsize_{}, blocks_{}
{
    if ( ::isatty(STDOUT_FILENO) ) {
        mine::handle(::ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize_));
    } else {
        // 如果stdout非终端，则设置默认窗口大小供MkdBlock使用
        winsize_.ws_row = 30;
        winsize_.ws_col = 100;
    }
}


int MkdHighlight::getTtyRow() const noexcept
{
    return winsize_.ws_row;
}


int MkdHighlight::getTtyCol() const noexcept
{
    return winsize_.ws_col;
}


bool MkdHighlight::regist(const std::string& name, MkdBlock* block)
{
    auto [ignore, isOK] = blocks_.emplace(std::move(name), block);
    return isOK;
}


MkdBlock& MkdHighlight::getBlock(const std::string& name)
{
    return *blocks_.at(name);
}


MkdHighlight::AllBlocks MkdHighlight::getAllBlocks() const
{
    return {blocks_.begin(), blocks_.end()};
}


std::string& MkdHighlight::highlight(std::string& text)
{
    auto origText = std::move(text);
    text.clear();
    mine::Split getLine{origText, '\n', true};

    for ( std::string_view onelineView{}; getLine(onelineView); ) {
        std::string oneline{onelineView};
        MkdBlock* curBlk{};
        for ( auto iter = blocks_.begin(); iter != blocks_.end(); ++iter ) {
            if ( iter->second->matchBegin(oneline) ) {
                curBlk = iter->second;
                if ( std::getenv("OUTPUT_DEBUG_LOG") != nullptr )
                    std::cerr << "\e[32mmatch:\e[m " << iter->first << ":" << oneline;
                break;
            }
        }
        if ( curBlk == nullptr )
            throw std::logic_error{"this line didn't match ans block: " + oneline};

        std::string& blockText = oneline;
        for ( std::string_view nextlineView{}; getLine(nextlineView); ) {
            std::string nextline{nextlineView};
            // 若匹配到区块尾部，则将该行放回
            if ( curBlk->matchEnd(nextline) ) {
                ssize_t offset = -static_cast<ssize_t>(nextlineView.size());
                getLine.seek(offset);
                break;
            }
            blockText += nextline;
        }

        if ( std::getenv("OUTPUT_DEBUG_LOG") != nullptr ) {
            std::cerr << "============================================================\n";
            std::cerr << blockText;
            std::cerr << "============================================================\n\n";
        }
        text += curBlk->highlight(blockText);
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
    auto print_help_then_throw = [] () {
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
        throw std::runtime_error{"bad command line parameters"};
    };

    std::vector<std::string> keys{}, filePrefix{};
    bool disablePager{};
    while ( true ) {
        auto choice = ::getopt(argc, argv, "hpw:f:");

        if ( choice == -1 ) break;

        switch ( choice ) {
            case '?':
            case 'h':
                print_help_then_throw();
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
        print_help_then_throw();
    }

    // 如果~/.cheat/目录不存在则抛出异常
    fs::path cheatDir{std::getenv("HOME")};
    cheatDir /= ".cheat";
    if ( !fs::is_directory(cheatDir) ) {
        throw std::runtime_error{"Error: No such directory ~/.cheat/"};
    }

    std::vector<fs::path> files{};
    for ( const auto& thisDirEntry : fs::directory_iterator{cheatDir} ) {
        if ( !fs::is_regular_file(thisDirEntry) || thisDirEntry.path().extension() != ".md" )
            continue;

        if ( filePrefix.empty() ) {
            files.emplace_back(thisDirEntry.path());
        } else {
            std::string curFileName{thisDirEntry.path().filename()};
            for ( auto& thisFilePrefix : filePrefix ) {
                // 确认文件名前缀是否与用户指定匹配
                if ( curFileName.starts_with(thisFilePrefix) ) {
                    files.emplace_back(thisDirEntry.path());
                    break;
                }
            }
        }
    }

    return {files, keys, disablePager};
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

        bool onceMatchInThisFile{true}; // 用于提前打印文件名
        bool inEntry{false};            // 用于判断当前匹配是否为某一entry内容

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
                    if ( onceMatchInThisFile ) {
                        entries << "# FILE: " << thisFile << "\n";
                        onceMatchInThisFile = false;
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


} // namespace see
