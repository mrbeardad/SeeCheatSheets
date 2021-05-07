#if !defined(MRBEARDAD_SEE_HEADER_HPP)
#define MRBEARDAD_SEE_HEADER_HPP


#include "see.hpp"
#include <regex>
#include "unicode/display_width.hpp"


namespace see
{


class HeaderBlk : public see::MkdBlock
{
public:
    HeaderBlk();

    virtual bool matchBegin(const std::string& oneline) override;

    virtual bool matchEnd(const std::string&) override;

    virtual std::string& highlight(std::string& text) override;

private:
    static HeaderBlk Instance_;
};
inline HeaderBlk HeaderBlk::Instance_{};


} // namespace see


#endif // defined(MRBEARDAD_SEE_HEADER_HPP)
