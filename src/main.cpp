#include "see.hpp"
#include <iostream>
#include <sys/wait.h>
#include <unicode/display_width.hpp>
#include <unistd.h>

int main(int argc, char* argv[])
{
    see::MkdHighlight hilit{};
    auto [files, keys, disablePager] = see::parse_cmdline(argc, argv);

    // 如果stdin被重定向到非终端文件则转而高亮之
    if ( !isatty(STDIN_FILENO) ) {
        std::cout << hilit.highlight(std::cin);
        return 0;
    }

    auto entries = see::search_entries(files, keys);

    // 是否启动PAGER
    if ( !disablePager ) {
        int row{};
        int maxCol{91}; // unicode::display_width()不支持控制字符，文件名提示行宽为91
        for ( std::string oneline{}; std::getline(entries, oneline); ) {
            ++row;
            maxCol = std::max(maxCol, unicode::display_width(oneline));
        }
        entries.rdbuf()->pubseekpos(0);
        entries.clear();
        if ( isatty(STDOUT_FILENO) || row > see::MkdBlock::Row || maxCol > see::MkdBlock::Col ) {
            auto    pager = getenv("PAGER");
            int     fds[2];
            mine::handle(pipe(fds));
            if ( auto pid = mine::handle(fork()); pid == 0 ) {
                dup2(fds[0], STDIN_FILENO);
                close(fds[1]);
                if ( pager == "less"s ) mine::handle(execlp(pager, pager, "-S", nullptr));
                else mine::handle(execlp(pager, pager, nullptr));
            } else {
                dup2(fds[1], STDOUT_FILENO);
                close(fds[1]);
            }
        }
    }

    std::cout << hilit.highlight(entries) << std::endl;
    close(STDOUT_FILENO);
    wait(nullptr);

    return 0;
}
