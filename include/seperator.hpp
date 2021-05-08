#if !defined(MRBEARDAD_SEE_SEPERATOR_HPP)
#define MRBEARDAD_SEE_SEPERATOR_HPP


#include "see.hpp"


class SeperatorBlk : public see::MkdBlock
{
public:
    SeperatorBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string&) override;
    std::string& highlight(std::string& text) override;

private:
    static SeperatorBlk Instance_;
};
inline SeperatorBlk SeperatorBlk::Instance_{};


#endif // !defined(MRBEARDAD_SEE_SEPERATOR_HPP)
