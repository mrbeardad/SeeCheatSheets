#ifndef MRBEARDAD_SEE_CODE_HPP
#define MRBEARDAD_SEE_CODE_HPP

#include "mkdblock.hpp"

namespace mkdhilit {

class CodeBlk : public MkdBlock {
    bool matchedEnd_;

    static CodeBlk Instance_;

   public:
    CodeBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string& oneline) override;
    std::string& highlight(std::string& text) override;
};
inline CodeBlk CodeBlk::Instance_{};

}  // namespace mkdhilit

#endif  // MRBEARDAD_SEE_CODE_HPP
