#ifndef MRBEARDAD_MKDBLOCK_HPP
#define MRBEARDAD_MKDBLOCK_HPP


#include <string>


namespace see {


class MkdHighlight;


// interface of markdown block highlight
struct MkdBlock
{
    MkdHighlight& mediator_;

    virtual ~MkdBlock();
    explicit MkdBlock(const std::string& name);

    virtual bool matchBegin(const std::string& oneline) =0;
    virtual bool matchEnd(const std::string& oneline) =0;
    virtual std::string& highlight(std::string& text) =0;
};


} // namespace see


#endif // MRBEARDAD_MKDBLOCK_HPP
