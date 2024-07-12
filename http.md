# HTTP

- [HTTP](#http)
  - [架构](#架构)
  - [协议](#协议)
    - [报文](#报文)
    - [URL](#url)
    - [方法](#方法)
    - [状态码](#状态码)
    - [连接](#连接)
  - [重定向](#重定向)
  - [认证](#认证)
  - [缓存](#缓存)
  - [安全](#安全)
    - [HTTP 严格传输安全(HSTS)](#http-严格传输安全hsts)
    - [内容安全策略(CSP)](#内容安全策略csp)
    - [关闭类型嗅探](#关闭类型嗅探)
    - [拒绝网页内嵌](#拒绝网页内嵌)
    - [Cookie 安全](#cookie-安全)
    - [跨源资源共享(CORS)](#跨源资源共享cors)

## 架构

```txt
Client ⇄ Proxy ⇄ Proxy ⇄ Server
```

- Proxy functions
  - filtering
  - load balancing
  - logging
  - authentication
  - caching

## 协议

### 报文

```yml
GET / HTTP/1.1
Host: developer.mozilla.org
Accept-Language: fr
```

```yml
HTTP/1.1 200 OK
Date: Sat, 09 Oct 2010 14:28:02 GMT
Server: Apache
Last-Modified: Tue, 01 Dec 2009 20:18:22 GMT
ETag: "51142bc1-7449-479b075b2891b"
Accept-Ranges: bytes
Content-Length: 29769
Content-Type: text/html

<!DOCTYPE html>… (here come the 29769 bytes of the requested web page)
```

- Body

  - 单资源固定长度，`Content-Type`、`Content-Length`
  - 单资源不定长度，`Transfer-Encoding: chunked`
  - 多资源，`Content-Type: multipart/form-data; boundary=----`、`Content-Disposition: form-data; name="myFile"`

### URL

```txt
        origin                                uri
v------------------------v-------------------------------------------------v
http://www.example.com:80/path/to/index.html?key1=value1&key2=value2#anchor
^------^--------------^--^------------------^-----------------------^------^
 scheme    host       port      path             query parameter    fragment
```

- 片段`#anchor`不随 http 传输，仅用于浏览器定位页面片段
- 绝对 URL 与相对 URL
- 安全字符与非安全字符的转义

### 方法

- 首先根据请求与响应是否包含主体来选择方法，若无法确定则再根据幂等或安全与否再选择。

| 方法    | 描述           | 请求包含主体 | 成功响应包含主体 | 幂等 | 安全 | 缓存                                 |
| ------- | -------------- | ------------ | ---------------- | ---- | ---- | ------------------------------------ |
| GET     | 获取资源       | no           | yes              | yes  | yes  | yes                                  |
| POST    | 发送资源       | yes          | yes              | no   | no   | include `Cache-control` or `Expires` |
| PUT     | 创建或替换资源 | yes          | no               | yes  | no   | no                                   |
| PATCH   | 部分修改资源   | yes          | no               | no   | no   | no                                   |
| DELETE  | 删除资源       | no           | no               | yes  | no   | no                                   |
| HEAD    | 预查 GET       | no           | no               | yes  | yes  | yes                                  |
| OPTION  | 预检请求       | no           | yes              | yes  | yes  | no                                   |
| CONNECT | 建立隧道       | no           | yes              | no   | no   | no                                   |
| TRACE   | 路径测试       | no           | no               | yes  | no   | no                                   |

### 状态码

| 状态码 | 状态描述              | 备注           |
| ------ | --------------------- | -------------- |
| 200    | OK                    | 请求成功       |
| 400    | Bad Request           | 客户端请求错误 |
| 500    | Internal Server Error | 服务器处理错误 |

### 连接

- HTTP/1.0
  - 可扩展性
- HTTP/1.1
  - 长连接
  - 流水线
  - chunked 编码
  - 缓存控制
- HTTP/2：
  - 多路复用
  - 二进制帧
  - 首部压缩
  - 服务端推送
- HTTP/3：
  - QUIC: 通过 UDP 实现可靠传输，且同时运行多个流，每个流的丢包检测和重传都相互独立

浏览器发起的初始协议可能是 http/1.x 或 http/2，根据浏览器实现有所不同，大致如下

- 若非 TLS 连接，则使用简单的 http 报文头部的协议字段协商协议版本，仅支持 http/1.x
- 若是 TLS 连接，则尝试使用 ALPN (Application-Layer Protocol Negotiation, as part of TLS handshake) 协商协议版本，若检测到服务器不支持则回退 http/1.x，否则使用服务器返回的版本（通常是 http/2）
- 若服务器返回 Alt-Svc 头部或 ALTSVC 帧，则升级到 http/3
- 特别的，http/1.1 支持 `Connection: Upgrade` 来升级协议到 http/2（不常用）和 Websocket

## 重定向

| 状态码 | 描述               | 处理方法                               | 应用场景                        |
| ------ | ------------------ | -------------------------------------- | ------------------------------- |
| 301    | Moved Permanently  | GET 方法不变，其他方法有可能会变为 GET | 网站重构                        |
| 308    | Permanent Redirect | 方法和消息主体都不发生变化             | 网站重构，用于非 GET 方法       |
| 302    | Found              | GET 方法不变，其他方法有可能会变为 GET | 网页意外不可用                  |
| 307    | Temporary Redirect | 方法和消息主体都不发生变化             | 网页意外不可用，用于非 GET 方法 |
| 304    | Not Modified       | 重定向到缓存                           | 缓存校验，表明缓存仍有效        |

- HTTP 重定向：`Location: URL`
- HTML 重定向：`<meta http-equiv="Refresh" content="0; URL=https://example.com/">`
- JavaScript 重定向：`window.location = "https://example.com/";`

## 认证

| 状态码 | 描述                          | 备注       |
| ------ | ----------------------------- | ---------- |
| 401    | Unauthorized                  | 未认证     |
| 407    | Proxy Authentication Required | 代理未认证 |
| 403    | Forbidden                     | 未授权     |
| 404    | Not Found                     | 资源不存在 |

| 响应首部                                   | 备注             |
| ------------------------------------------ | ---------------- |
| `WWW-Authenticate: <type> realm=<realm>`   | 指定认证方案     |
| `Proxy-Authenticate: <type> realm=<realm>` | 指定代理认证方案 |

| 请求首部                                    | 备注         |
| ------------------------------------------- | ------------ |
| `Authorization: <type> <credentials>`       | 认证信息     |
| `Proxy-Authorization: <type> <credentials>` | 认证代理信息 |

## 缓存

- 缓存类型：缓存分为 private 与 public，前者仅客户端存储，后者均可存储
- 缓存状态：缓存状态有 fresh 与 stale，过期日期 = `Date` + `max-age` - `Age`
- 缓存校验：fresh 缓存可直接复用，stale 缓存需要校验成功后才能在 1 小时内复用
- 复用旧缓存：当与源服务器断开连接时允许使用 stale 缓存
- 启发式缓存：即使未指定`Cache-Control`也可进行缓存，通常 `max-age` = (`Last-Modified` - `Date`) / 10
- 缓存折叠：缓存代理同时接收多个相同请求时，只向源服务器发送一个请求，除非指定`Cache-Control: private`
- 缓存破坏：给静态资源名添加版本号（如哈希等），修改后 URL 不同导致缓存失效

| `Cache-Control` 首部参数 | 类型 | 备注（`,`逗号分隔列表）                                                                 |
| ------------------------ | ---- | --------------------------------------------------------------------------------------- |
| `public`                 | 响应 | 允许共享缓存，即允许反向代理缓存，当响应具有`Authorization`的请求时才需要显示指定       |
| `private`                | 响应 | 仅限私有缓存，即仅限浏览器本地缓存，一般需要显示指定                                    |
| `max-age=3600`           | 响应 | 指定缓存从源服务器产生开始计算的保持 fresh 的流逝时间                                   |
| `must-revalidate`        | 响应 | 禁止“复用旧缓存”                                                                        |
| `no-cache`               | 响应 | 每次复用缓存都必须校验，且禁止“复用旧缓存”                                              |
| `no-store`               | 响应 | 禁止缓存                                                                                |
| `immutable`              | 响应 | 表明缓存在 fresh 期间绝不可能更改，浏览器重载页面时忽略校验该缓存，一般用于缓存破坏模式 |
| `max-age=0`              | 请求 | 表明仅允许使用从源服务器创建不超过指定时间的响应的缓存，一般用于浏览器刷新              |
| `no-cache`               | 请求 | 表明需要校验缓存                                                                        |
| `max-stale=3600`         | 请求 | 表明允许返回已过期的缓存，但过期时间不能超过指定时间                                    |

| 其他相关首部        | 类型 | 备注                                                                                           |
| ------------------- | ---- | ---------------------------------------------------------------------------------------------- |
| `Date`              | 响应 | 响应报文创建时间                                                                               |
| `Age`               | 响应 | 响应报文在代理中已缓存的时间                                                                   |
| `Last-Modified`     | 响应 | 资源上次修改时间                                                                               |
| `ETag`              | 响应 | 用于缓存校验，一般为哈希值                                                                     |
| `Expires`           | 响应 | 指定缓存保持 fresh 的时间点而非流逝时间                                                        |
| `Vary`              | 响应 | 表示即使 URI 相同，但指定的请求首部若不同则响应报文则不同，常见如`Origin`和`Accept-Encoding`等 |
| `If-Modified-Since` | 请求 | 缓存校验，对应`Last-Modified`                                                                  |
| `If-None-Match`     | 请求 | 缓存校验，对应`Etag`，优先于上条                                                               |

| 状态码 | 描述         | 备注               |
| ------ | ------------ | ------------------ |
| 304    | Not Modified | 作为缓存校验的响应 |

## 安全

### HTTP 严格传输安全(HSTS)

- 浏览器会阻止在 HTTP（非安全）协议下加载动态资源，如 JavaScript
- HSTS 表明当前站点仅允许在 HTTPS 安全协议下访问
- `about://net-internals#hsts`中查看/删除 HSTS 列表

| `Strict-Transport-Security`响应首部 | 备注（`;`分号分隔列表）              |
| ----------------------------------- | ------------------------------------ |
| `max-age=63072000`                  | 指定 HSTS 记忆有效时间               |
| `includeSubDomains`                 | 包括子域名也应用 HSTS                |
| `preload`                           | 使用 HSTS 预加载列表，由 google 维护 |

### 内容安全策略(CSP)

| `Content-Security-Policy`响应首部        | 备注                                                           |
| ---------------------------------------- | -------------------------------------------------------------- |
| `<policy-directive>; <policy-directive>` | 限制加载的各类型资源的来源(Origin)，这默认会禁用内联 js 与 css |

也可使用 HTML`<meta>`元素设置策略：

```html
<meta
  http-equiv="Content-Security-Policy"
  content="default-src 'self'; img-src https://*; child-src 'none';"
/>
```

### 关闭类型嗅探

| `X-Content-Type-Options`响应首部 | 备注                                                 |
| -------------------------------- | ---------------------------------------------------- |
| `nosniff`                        | 若嗅探的 MIME 类型与`Content-Type`不匹配，则阻止请求 |

### 拒绝网页内嵌

| `X-Frame-Options`响应首部 | 备注（默认允许跨源嵌套） |
| ------------------------- | ------------------------ |
| `DENY`                    | 禁止任何形式的嵌套       |
| `SAMEORIGIN`              | 允许同源请求嵌套         |

### Cookie 安全

> `Site`表示两 URL 的 secheme 相同且 host 中的可注册域名相同（表示网站来自同一组织）

| `Set-Cookie`响应首部 | 备注（`;`分号分隔列表）                                                                                         |
| -------------------- | --------------------------------------------------------------------------------------------------------------- |
| `<name>=<value>`     | name 不能包含控制字符和分隔符；value 可以用双引号来转义部分分隔符；若需多个 name=value 对则添加多个`Set-Cookie` |
| `Max-Age=`           | 指定 cookie 持久化时间，默认 session cookie                                                                     |
| `Expires=`           | 指定 cookie 持久化时间点，默认 session cookie                                                                   |
| `Domain=`            | 指定 cookie 当前域名或更高阶的域名（包括子域名），默认当前域名（不含子域名）                                    |
| `Path=`              | 指定 cookie 对应的路径且包括子路径，默认匹配所有路径                                                            |
| `Secure`             | 指定 cookie 仅在 https 协议下使用                                                                               |
| `HttpOnly`           | 指定 cookie 不能被 JS 访问                                                                                      |
| `SameSite=`          | `Strict`仅允许同源访问，`Lax（默认）`允许导航跳转的跨站请求，`None`表示允许跨站请求但必须设置`Secure`           |

| `Cookie`请求首部 | 备注（`;`分号分隔列表）             |
| ---------------- | ----------------------------------- |
| `<cookie-list>`  | 浏览器访问资源时会携带匹配的 cookie |

### 跨源资源共享(CORS)

- `XMLHttpRequest` 或 Fetch APIs 发起的跨源 HTTP 请求
- `drawImage()` 将 Images/video 画面绘制到 canvas。
- WebGL 贴图
- CSS 中通过 `@font-face` 使用跨源字体资源
- CSS Shape from images
- 可使用`crossorigin`属性的元素，如：`<link>`, `<img>`等等

由以上途径发起的跨源资源请求时，浏览器会先进行`OPTION`预检，若**目标资源服务器**允许**源服务器**跨源访问该资源后再进行正常资源访问。
脚本无法获取跨源访问出错报错信息，需要打开浏览器控制台查看。

简单请求不会触发 CORS 预检，典型的限制如下：

- 仅允许使用`HEAD`、`GET`与`POST`
- 仅允许使用简单首部（其中`Content-Type`限制为`text/plain`、`multipart/form-data`与`application/x-www-form-urlencoded`）
- 等等

| `crossorigin`属性 | 备注                                                                         |
| ----------------- | ---------------------------------------------------------------------------- |
|                   | 默认不会限制 html 标签链接的跨源访问                                         |
| `""`,`anonymous`  | 允许 CORS 预检                                                               |
| `use-credentials` | 允许跨源访问携带凭证(Cookie, Authorization Headers, TLS Client Certificates) |

| 请求首部                                                        | 备注                       |
| --------------------------------------------------------------- | -------------------------- |
| `Origin: <origin>`                                              | 指明跨源请求的源 Origin    |
| `Referer: <url>`                                                | 指明跨源请求的源 URL       |
| `Access-Control-Request-Method: <method>`                       | 预检时指明方法             |
| `Access-Control-Request-Headers: <field-name>[, <field-name>]*` | 预检时指明携带的自定义首部 |

| 响应首部                                               | 备注                                                                                           |
| ------------------------------------------------------ | ---------------------------------------------------------------------------------------------- |
| `Access-Control-Allow-Origin: <origin> 或 *`           | 允许跨源请求的源服务器，若指定为`<origin>`则应该添加首部`Vary: Origin`                         |
| `Access-Control-Allow-Methods: <method>[, <method>]*`  | 允许跨源请求的方法                                                                             |
| `Access-Control-Allow-Headers: <header>[, <header>]*`  | 允许跨源请求携带的用户自定义的非简单首部                                                       |
| `Access-Control-Max-Age: <delta-seconds>`              | 允许预检结果有效的时间，期间无需再次进行相同预检                                               |
| `Access-Control-Expose-Headers: <header>[, <header>]*` | 允许`getResponseHeader`访问获取非基本首部                                                      |
| `Access-Control-Allow-Credentials: true`               | 允许跨源请求携带凭证，否则浏览器忽略响应                                                       |
| `Referrer-Policy: strict-origin-when-cross-origin`     | 浏览器默认行为，同源请求发送 origin、path 和 query，跨源请求发送仅 origin，若非 HTTPS 则不发送 |
