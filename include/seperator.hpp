#ifndef MRBEARDAD_SEE_SEPERATOR_HPP
#define MRBEARDAD_SEE_SEPERATOR_HPP


#include "see.hpp"


class SeperatorBlk : public see::MkdBlock
{
    static SeperatorBlk Instance_;

public:
    SeperatorBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string&) override;
    std::string& highlight(std::string& text) override;
};
inline SeperatorBlk SeperatorBlk::Instance_{};


#endif // MRBEARDAD_SEE_SEPERATOR_HPP
