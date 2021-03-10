/**
 * Copyright (c) 2020-2021 Heachen Bear & Contributors
 * File: mine.hpp
 * License: GPLv3
 * Author: Heachen Bear <mrbeardad@qq.com>
 * Date: 06.03.2021
 * Last Modified Date: 10.03.2021
 * Last Modified By: Heachen Bear <mrbeardad@qq.com>
 */


/*************************************************************************************************/
// 关闭iosteam与stdio之间的同步，同时也会关闭iostream的线程安全性
/*************************************************************************************************/
#if !defined(MRBEARDAD_MINE_HPP_CSWS) && !defined(DISABLE_CLOSE_SYNC_WITH_STDIO) && (defined(_GLIBCXX_IOSTREAM) || defined(_IOSTREAM_))
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
#endif // !defined(MRBEARDAD_MINE_HPP_CSWS) && !defined(DISABLE_CLOSE_SYNC_WITH_STDIO) && (defined(_GLIBCXX_IOSTREAM) || defined(_IOSTREAM_))


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
inline
std::regex operator""_rgx(const char* str, size_t len)
{
    return std::regex{str, len};
}
inline
std::wregex operator""_rgx(const wchar_t* str, size_t len)
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


#if __has_include(<ext/stdio_filebuf.h>)
#   include <ext/stdio_filebuf.h>
#endif // __has_include(<ext/stdio_filebuf.h>)
#include <chrono>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

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


namespace mine
{


class GetLineView
{
    const std::string* view_;
    size_t curIdx_;
public:
    explicit GetLineView(const std::string& str)
        : view_{&str}, curIdx_{} {  }

    const std::string* reset(const std::string* newPtr)
    {
        std::swap(newPtr, view_);
        return newPtr;
    }

    bool operator()(std::string_view& oneline)
    {
        auto posNL = view_->find('\n', curIdx_);
        if ( posNL != std::string::npos ) {
            oneline = std::string_view{view_->data() + curIdx_, view_->data() + posNL};
            curIdx_ = posNL + 1;
            return true;
        } else if ( curIdx_ < view_->size() ) {
            oneline = std::string_view{view_->data() + curIdx_, view_->data() + view_->size()};
            curIdx_ = posNL; // 防止下次再次进行搜索
            return false;
        } else {
            return false;
        }
    }
};


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


/*************************************************************************************************/
// Construct streambuf from posix file descriptor
/*************************************************************************************************/
#if __has_include(<ext/stdio_filebuf.h>)
inline __gnu_cxx::stdio_filebuf<char>
make_strmbuf_from_fd(int fd, std::ios_base::openmode oflag) {
    return {fd, oflag};
}
#endif // __has_include(<ext/stdio_filebuf.h>)



} // namespace mine


#endif  // MRBEARDAD_MINE_HPP


