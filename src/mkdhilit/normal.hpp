#ifndef MRBEARDAD_SEE_NORMAL_HPP
#define MRBEARDAD_SEE_NORMAL_HPP

#include "mkdblock.hpp"

namespace mkdhilit {

class NormalBlk : public MkdBlock {
    static NormalBlk Instance_;

   public:
    NormalBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string&) override;
    virtual std::string& highlight(std::string& text) override;
    std::string& highlight(std::string& text, const std::string& bgColor);
};
inline NormalBlk NormalBlk::Instance_{};

}  // namespace mkdhilit

#endif  // MRBEARDAD_SEE_NORMAL_HPP
