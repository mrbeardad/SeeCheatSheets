#ifndef MRBEARDAD_SEE_HPP
#define MRBEARDAD_SEE_HPP

#include <sys/ioctl.h>

#include <filesystem>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "mkdblock.hpp"
#include "utils/mine.hpp"

namespace mkdhilit {

class MkdHighlight;

// Mediator Pattern: Mediator
class MkdHighlight {
   public:
    using Register = std::unordered_map<std::string, MkdBlock*>;
    using AllBlocks =
        std::pair<Register::const_iterator, Register::const_iterator>;

   private:
    winsize winsize_;
    Register blocks_;

   public:
    int getTtyRow() const noexcept;
    int getTtyCol() const noexcept;
    bool regist(const std::string& name, MkdBlock* block);
    MkdBlock& getBlock(const std::string& name);
    AllBlocks getAllBlocks() const;
    std::string& highlight(std::string& text);

    static MkdHighlight& Instance();

   private:
    MkdHighlight();
};

std::tuple<std::vector<fs::path>, std::vector<std::string>, bool> parse_cmdline(
    int argc,
    char* argv[]);

std::string search_entries(const std::vector<fs::path>& files,
                           const std::vector<std::string>& keys);

}  // namespace mkdhilit

#endif  // MRBEARDAD_SEE_HPP
