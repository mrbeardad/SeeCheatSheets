#ifndef MRBEARDAD_SEE_QUOTE_HPP
#define MRBEARDAD_SEE_QUOTE_HPP


#include "normal.hpp"


namespace see {


class QuoteBlk : public MkdBlock
{
    static QuoteBlk Instance_;

public:
    QuoteBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string& oneline) override;
    std::string& highlight(std::string& text) override;
};
inline QuoteBlk QuoteBlk::Instance_{};


} // namespace see


#endif // MRBEARDAD_SEE_QUOTE_HPP
