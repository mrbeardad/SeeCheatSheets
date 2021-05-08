#ifndef MRBEARDAD_SEE_HEADER_HPP
#define MRBEARDAD_SEE_HEADER_HPP


#include "see.hpp"


namespace see
{


class HeaderBlk : public see::MkdBlock
{
    static HeaderBlk Instance_;

public:
    HeaderBlk();
    virtual bool matchBegin(const std::string& oneline) override;
    virtual bool matchEnd(const std::string&) override;
    virtual std::string& highlight(std::string& text) override;
};
inline HeaderBlk HeaderBlk::Instance_{};


} // namespace see


#endif // MRBEARDAD_SEE_HEADER_HPP
