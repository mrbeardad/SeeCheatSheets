#if !defined(MRBEARDAD_SEE_CODE_HPP)
#define MRBEARDAD_SEE_CODE_HPP


#include "see.hpp"


class CodeBlk : public see::MkdBlock
{
    bool matchedEnd_;

    static CodeBlk Instance_;

public:
    CodeBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string &oneline) override;
    std::string& highlight(std::string& text) override;
};
inline CodeBlk CodeBlk::Instance_{};


#endif // !defined(MRBEARDAD_SEE_CODE_HPP)
