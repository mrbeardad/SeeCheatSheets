#ifndef MRBEARDAD_SEE_HEADER_HPP
#define MRBEARDAD_SEE_HEADER_HPP

#include "mkdblock.hpp"

namespace mkdhilit {

class HeaderBlk : public MkdBlock {
    static HeaderBlk Instance_;

   public:
    HeaderBlk();
    virtual bool matchBegin(const std::string& oneline) override;
    virtual bool matchEnd(const std::string&) override;
    virtual std::string& highlight(std::string& text) override;
};
inline HeaderBlk HeaderBlk::Instance_{};

}  // namespace mkdhilit

#endif  // MRBEARDAD_SEE_HEADER_HPP
