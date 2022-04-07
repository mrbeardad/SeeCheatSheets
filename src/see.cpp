/**
 * @file see.cpp
 * @author Heache Bear (mrbeardad@qq.com)
 * @brief
 * @version 3.1
 * @date 2022-04-07
 *
 * @copyright Copyright (c) 2020 - 2022
 *
 */
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#include "unicode/display_width.hpp"

#include "mkdhilit/mkdhilit.hpp"
#include "utils/mine.hpp"

int main(int argc, char* argv[]) {
    auto& hilit = mkdhilit::MkdHighlight::Instance();

    std::vector<fs::path> files{};
    std::vector<std::string> keys{};
    bool disablePager{};
    std::string text{};

    // 如果stdin被重定向到非终端文件则转而高亮之
    if (!::isatty(STDIN_FILENO)) {
        text.assign(std::istreambuf_iterator<char>{std::cin},
                    std::istreambuf_iterator<char>{});
    } else {
        // 可能抛出异常，如命令行参数错误、文件无法访问等
        try {
            std::tie(files, keys, disablePager) =
                mkdhilit::parse_cmdline(argc, argv);
            text = mkdhilit::search_entries(files, keys);
        } catch (...) {
            std::exit(1);
        }
    }

    text = hilit.highlight(text);

    // 是否允许使用PAGER
    if (!disablePager && ::isatty(STDOUT_FILENO)) {
        int row{1}, col{};
        size_t first{};
        for (size_t last{};
             (last = text.find('\n', last)) != std::string::npos;) {
            ++row;
            col = std::max(
                col, unicode::display_width(text.substr(first, last - first)));
            first = ++last;
        }
        if (first <= text.size()) {
            col = std::max(col, unicode::display_width(text.substr(first)));
        }
        if ((row > hilit.getTtyRow() || col > hilit.getTtyCol())) {
            const auto* pager = ::getenv("PAGER");
            pager = pager == nullptr ? "less" : pager;
            int fds[2];
            mine::handle(::pipe(fds));

            if (auto pid = mine::handle(::fork()); pid == 0) {  // 子进程
                mine::handle(::dup2(fds[0], STDIN_FILENO));
                mine::handle(::close(fds[0]));  // 关闭pipe-read-end副本
                mine::handle(::close(fds[1]));  // 关闭pipe-write-end副本
                if (pager == "less"s)
                    mine::handle(::execlp(pager, pager, "-S", nullptr));
                else
                    mine::handle(execlp(pager, pager, nullptr));
            } else {  // 父进程
                mine::handle(::dup2(fds[1], STDOUT_FILENO));
                mine::handle(::close(fds[0]));  // 关闭pipe-read-end副本
                mine::handle(::close(fds[1]));  // 关闭pipe-write-end副本
            }
        }
    }

    std::cout << text << std::endl;

    mine::handle(::close(STDOUT_FILENO));  // 关闭pipe-write-end
    ::wait(nullptr);  // 不检测返回值，如此一来，是否使用PAGER都适用

    return 0;
}
