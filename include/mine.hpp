/**
 * Copyright (c) 2020-2021 Heachen Bear & Contributors
 * File: mine.hpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 06.03.2021
 * Last Modified Date: 05.05.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */


/*************************************************************************************************/
// 关闭iosteam与stdio之间的同步，同时也会关闭iostream的线程安全性
/*************************************************************************************************/
#if !defined(MRBEARDAD_MINE_HPP_CSWS) && !defined(DISABLE_CLOSE_SYNC_WITH_STDIO) \
    && (defined(_GLIBCXX_IOSTREAM) || defined(_IOSTREAM_)) \
    && __cplusplus >= 201704L
#define MRBEARDAD_MINE_HPP_CSWS
struct CloseSyncWithStdio
{
    CloseSyncWithStdio()
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);
    }
};
inline CloseSyncWithStdio CloseSyncWithStdio{};
#endif // !defined(MRBEARDAD_MINE_HPP_CSWS) && !defined(DISABLE_CLOSE_SYNC_WITH_STDIO)
// && (defined(_GLIBCXX_IOSTREAM) || defined(_IOSTREAM_))
// && __cplusplus >= 201704L


/*************************************************************************************************/
// std::filesystem别名
/*************************************************************************************************/
#if defined(_GLIBCXX_FILESYSTEM) || defined(_FILESYSTEM_)
namespace fs = std::filesystem;
#endif // defined(_GLIBCXX_FILESYSTEM) || defined(_FILESYSTEM_)


/*************************************************************************************************/
// std::ios_base别名
/*************************************************************************************************/
#if defined(_IOS_BASE_H) || defined(_XIOSBASE_)
using io = std::ios_base;
#endif // defined(_IOS_BASE_H) || defined(_XIOSBASE_)


/*************************************************************************************************/
// std::chrono别名 + std::chrono字面值inine
/*************************************************************************************************/
#if defined(_GLIBCXX_CHRONO) || defined(_CHRONO_)
namespace ch = std::chrono;
#   if __cplusplus >= 201402
using std::literals::operator""h;
using std::literals::operator""min;
using std::literals::operator""s;
using std::literals::operator""ms;
using std::literals::operator""us;
using std::literals::operator""ns;
#   endif // __cplusplus >= 201402
#endif // defined(_GLIBCXX_CHRONO) || defined(_CHRONO_)


/*************************************************************************************************/
// std::string字面值inline
/*************************************************************************************************/
#if defined(_GLIBCXX_STRING) || defined(_STRING_)
#   if __cplusplus >= 201402
using std::literals::operator""s;
#   endif // __cplusplus >= 201402
#endif // defined(_GLIBCXX_STRING) || defined(_STRING_)

/*************************************************************************************************/

// std::string_view字面值inline
/*************************************************************************************************/
#if defined(_GLIBCXX_STRING_VIEW) || defined(_STRING_VIEW_)
#   if __cplusplus >= 201703
using std::literals::operator""sv;
#   endif // __cplusplus >= 201703
#endif // defined(_GLIBCXX_STRING_VIEW) || defined(_STRING_VIEW_)


/*************************************************************************************************/
// std::regex字面值
/*************************************************************************************************/
#if !defined(MRBEARDAD_MINE_HPP_REGEX) && (defined(_GLIBCXX_REGEX) || defined(_REGEX_))
#define MRBEARDAD_MINE_HPP_REGEX
inline std::regex
operator""_rgx(const char* str, size_t len)
{
    return std::regex{str, len};
}
inline std::wregex
operator""_rgx(const wchar_t* str, size_t len)
{
    return std::wregex{str, len};
}
#endif // defined(_GLIBCXX_REGEX) || defined(_REGEX_)


/*************************************************************************************************/
// std::placeholders inline
/*************************************************************************************************/
#if defined(_GLIBCXX_FUNCTIONAL) || defined(_FUNCTIONAL_)
using namespace std::placeholders;
#endif // defined(_GLIBCXX_FUNCTIONAL) || defined(_FUNCTIONAL_)


/*************************************************************************************************/
// ASIO namespace别名
/*************************************************************************************************/
#ifdef ASIO_HPP
using asio::ip::tcp;
using asio::ip::udp;
using asio::ip::icmp;
#endif // ASIO_HPP

#ifdef ASIO_SSL_HPP
namespace ssl = asio::ssl;
#endif // ASIO_SSL_HPP

#ifdef ASIO_HAS_LOCAL_SOCKETS
using uds = asio::local::stream_protocol;
using udd = asio::local::datagram_protocol;
#endif // ASIO_HAS_LOCAL_SOCKETS


/*************************************************************************************************/
// namespace mine
/*************************************************************************************************/

#ifndef MRBEARDAD_MINE_HPP
#define MRBEARDAD_MINE_HPP 1


#if __cplusplus >= 201703L
#   include <string_view>
#endif // __cplusplus >= 201703L
#include <ios>
#include <system_error>
#include <type_traits>
#include <utility>

/*************************************************************************************************/
// Use string instead of stringstream in most time
/*************************************************************************************************/
inline std::string&
operator<<(std::string& output, const std::string& str)
{
    return output.append(str);
}

inline std::string&
operator<<(std::string& output, char oneChar)
{
    output.push_back(oneChar);
    return output;
}

inline std::string&
operator<<(std::string& output, const char* cstr)
{
    return output.append(cstr);
}

#if __cplusplus >= 201703L
inline std::string&
operator<<(std::string& output, const std::string_view& strv)
{
    return output.append(strv);
}
#endif // __cplusplus >= 201703L


namespace mine
{

#if __cplusplus >= 201703L

// 默认以连续的空白符作为分隔符
class Split
{
    const std::string*  view_;
    size_t              curIdx_;
    char                sep_;
public:
    explicit Split(const std::string& str, char sep=-1)
        : view_{&str}, curIdx_{}, sep_{sep} {  }

    const std::string& reset(const std::string& newView, char sep=-1)
    {
        curIdx_ = 0;
        sep_ = sep;
        auto ret = view_;
        view_ = &newView;
        return *ret;
    }

    size_t seek(ssize_t offset, std::ios_base::seekdir dir=std::ios_base::cur)
    {
        if ( dir == std::ios_base::beg )
            return curIdx_ = offset;
        if ( dir == std::ios_base::end )
            return curIdx_ = view_->size() + offset;
        if ( dir == std::ios_base::cur )
            return curIdx_ += offset;
        return -1;
    }

    bool operator()(std::string_view& oneline)
    {
        if ( sep_ == -1 ) {
            auto& viewStr = *view_;
            for ( ; curIdx_ < viewStr.size() && std::isspace(viewStr[curIdx_]); ++curIdx_ );
            if ( curIdx_ == viewStr.size() )
                return false;
            size_t nextSpace{curIdx_};
            for ( ; nextSpace < viewStr.size() && !std::isspace(viewStr[nextSpace]); ++nextSpace );
            oneline = std::string_view{viewStr.data() + curIdx_, nextSpace - curIdx_};
            curIdx_ = nextSpace;
            return true;
        }

        auto sepPos = view_->find(sep_, curIdx_);
        if ( sepPos != std::string::npos ) {
            oneline = std::string_view{view_->data() + curIdx_, sepPos - curIdx_};
            curIdx_ = sepPos + 1;
            return true;
        } else if ( curIdx_ < view_->size() ) {
            oneline = std::string_view{view_->data() + curIdx_, view_->size() - curIdx_};
            curIdx_ = sepPos;
            return true;
        } else {
            return false;
        }
    }
};

#endif // __cplusplus >= 201703L

/*************************************************************************************************/
// Y Combinator
/*************************************************************************************************/
template <typename Functor>
struct YCombinator
{
    Functor func_;

    template <typename... Args>
    decltype(auto) operator()(Args&&... args)
    {
        return func_(*this, std::forward<Args>(args)...);
    }
};


template <typename Functor>
YCombinator<std::decay_t<Functor> > make_y_conbinator(Functor&& f)
{
    return {std::forward<Functor>(f)};
}


/*************************************************************************************************/
// Handle system call
/*************************************************************************************************/
template <typename T>
T handle(T sysCallRet)
{
    if ( sysCallRet < 0 ) {
        throw std::system_error{errno, std::generic_category()};
    } else {
        return sysCallRet;
    }
}


template <typename T>
T* handle(T* sysCallRet)
{
    if ( sysCallRet == nullptr ) {
        throw std::system_error{errno, std::generic_category()};
    } else {
        return sysCallRet;
    }
}


} // namespace mine


#endif  // MRBEARDAD_MINE_HPP

