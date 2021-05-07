/**
 * Copyright (c) 2020 Heachen Bear & Contributors
 * File: main.cpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 09.02.2021
 * Last Modified Date: 07.05.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */

#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#include "unicode/display_width.hpp"

#include "mine.hpp"
#include "see.hpp"

int main(int argc, char* argv[])
{
    auto& hilit = see::MkdHighlight::Instance();

    std::vector<fs::path>       files{};
    std::vector<std::string>    keys{};
    bool                        disablePager{};
    std::string                 text{};

    // 如果stdin被重定向到非终端文件则转而高亮之
    if ( !::isatty(STDIN_FILENO) ) {
        text.assign(std::istreambuf_iterator<char>{std::cin}, std::istreambuf_iterator<char>{});
    } else {
        // 可能抛出异常，如命令行参数错误、文件无法访问等
        try {
            std::tie(files, keys, disablePager) = see::parse_cmdline(argc, argv);
            text = see::search_entries(files, keys);
        } catch (...) {
            std::exit(1);
        }
    }

    // 是否允许使用PAGER
    if ( !disablePager && ::isatty(STDOUT_FILENO) ) {
        int row{1}, col{};
        for ( size_t first{}, last{}; ; ) {
            last = text.find('\n', last);

            if ( first >= text.size() )
                break;

            // 最后一行可能不存在，从而row比真实行数多1
            ++row;
            col = std::max(col, unicode::display_width(text.substr(first, last)));
            first = ++last;
        }
        if ( (row > hilit.getTtyRow() || col > hilit.getTtyCol()) ) {
            const auto* pager = ::getenv("PAGER");
            pager = pager == nullptr ? "less" : pager;
            int fds[2];
            mine::handle(pipe(fds));

            if ( auto pid = mine::handle(fork()); pid == 0 ) {  // 子进程
                mine::handle(dup2(fds[0], STDIN_FILENO));
                mine::handle(close(fds[0]));    // 关闭pipe-read-end副本
                mine::handle(close(fds[1]));    // 关闭pipe-write-end副本
                if ( pager == "less"s ) mine::handle(execlp(pager, pager, "-S", nullptr));
                else mine::handle(execlp(pager, pager, nullptr));
            } else {    // 父进程
                mine::handle(dup2(fds[1], STDOUT_FILENO));
                mine::handle(close(fds[0]));    // 关闭pipe-read-end副本
                mine::handle(close(fds[1]));    // 关闭pipe-write-end副本
            }
        }
    }

    std::cout << hilit.highlight(text) << std::endl;

    mine::handle(close(STDOUT_FILENO));   // 关闭pipe-write-end
    wait(nullptr);  // 不检测返回值，如此一来，是否使用PAGER都适用

    return 0;
}
