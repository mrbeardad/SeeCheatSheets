# Nginx

## 配置文件

- 配置文件：nginx.conf 为默认主配置文件名，用`include`指令导入模块化配置

- 单行指令：包含指令名与参数，用空格隔开，行尾分号结束

- 块指令：又称上下文，是指令的容器，用花括号包裹指令，不同上下文中可用的指令也不同

  - `main`: 最顶级指令的默认上下文
    - `events`: 通用连接处理
    - `stream`: TCP/UDP 流量
    - `mail`: Mail 流量
    - `http`: HTTP 流量
      - `server`: 虚拟主机，匹配 ip/port 与 host name 来处理流量
        - `location`: 路由策略，匹配 URI 来处理资源

- 继承：有些指令可在放在多种上下文中，此时子上下文的配置继承父上下文

## 路由转发

```nginx
Syntax : server { ... }
Default: —
Context: http
```

- 创建一个虚拟主机(server)

---

```nginx
Syntax : listen address[:port] [default_server] [ssl] [http2 | quic];
Default: listen *:80 | *:8000;
Context: server
```

- 指定当前 server 匹配地址与端口
- address 格式 `127.0.0.1`, `*`, `[::]`, `[::1]`
- port 默认 80
- default_server 指定该 server 为 default server，默认情况下第一个 server 为 default server；当绑定到该 ip/port 上的 servers 的 server_name 都不匹配时，则用 default server 来处理流量

---

```nginx
Syntax : server_name name ...;
Default: server_name "";
Context: server
```

- 指定当前 server 匹配的 host name
  1. 准确匹配，如 `www.example.com`
  2. 首通配符，如 `*.example.com`
  3. 尾通配符，如 `www.example.*`
  4. 正则匹配，如 `~www\..*\.com`
  5. 优先级从上到下，多个同优先级则匹配第一个出现的

---

```nginx
Syntax: location [ = | ~ | ~* | ^~ ] uri { ... }
        location @name { ... }
Default: —
Context: server, location
```

- 指定该路由策略匹配的 uri
  1. 先进行前缀匹配，并记录其中最长前缀匹配的 location；
  2. 再进行正则匹配，并记录第一个成功匹配的 location；
  3. 最后选择，正则匹配优先级高于前缀匹配
  - 默认前缀匹配
  - `=` 表示精准匹配，匹配成功则停止匹配直接选择该 location（无法嵌套）
  - `^~` 表示若当前 location 为最长前缀匹配，则停止匹配直接选择该 location
  - `~` 表示正则匹配且区分大小写
  - `~*` 表示正则匹配且忽略大小写
- `@name` location 无法嵌套与被嵌套

---

```nginx
Syntax : ssl_certificate file;
Syntax : ssl_certificate_key file;
Syntax : ssl_password_file file;
Default: —
Context: http, server
```

- ssl 协议卸载

---

```nginx
Syntax : if (condition) { ... }
Default: —
Context: server, location
```

- `=`, `~`, `~*`
- `!=`, `!~`, `!~*`,
- `-f`, `-d`, `-e`, `-x`
- `!-f`, `!-d`, `!-e`, `!-x`

---

```nginx
Syntax : add_header name value [always];
Default: —
Context: http, server, location, if in location
```

- 添加响应首部。若不加 always，则仅在响应码为 200, 201 , 204, 206, 301, 302, 303, 304, 307 , or 308 时添加。
- 仅当当前域没有`add_header`命令才从外部继承

---

```nginx
Syntax : root path;
Default: root html;
Context: http, server, location, if in location
```

- 指定 path 作为 url path 的路径前缀，再用于查找静态资源

---

```nginx
Syntax : alias path;
Default: —
Context: location
```

- 指定 path 替换 url path 中被 location uri 匹配到的部分 ，再用于查找静态资源。与 root 命令不相容，支持正则替换语法

---

```nginx
Syntax : index file ...;
Default: index index.html;
Context: http, server, location
```

- 指定用于请求目录（url 结尾为`/`）时内部重定向（重新匹配 location）的默认文件

---

```nginx
Syntax : try_files file ... uri;    # 内部重定向
         try_files file ... =code;
         try_files file ... @name;
Default: —
Context: server, location
```

- 依序尝试获取指定文件（需要经 root 与 alias 处理），若都获取失败则使用最后指定方法

## 重定向

```nginx
Syntax : rewrite regex replacement [flag];
Default: —
Context: server, location, if
```

- 正则替换 uri，按顺序执行
  - last: 停止当前 ngx_http_rewrite_module 并重新匹配 location（内部重定向）
  - break: 停止当前 ngx_http_rewrite_module 但不再重新匹配 location，即该路由判定匹配当前 location（内部重定向）
  - redirect: 302 暂时重定向（外部重定向）
  - permanent: 301 永久重定向（外部重定向）
- 若 replacement 以`http://`、`https://`或`$scheme`开头则停止并返回响应
- 若 replacement 包含新的 query 部分，则默认将之前的 query 添加到后面。在 replacement 尾部添加`?`避免自动添加

```nginx
Syntax : return code [body_text];
         return code URL;
         return URL;
Default: —
Context: server, location, if
```

## 反向代理

```nginx
http {
  # /var/run/nginx-cache      - 缓存目录路径
  # levels=1:2                - 缓存子目录目录层数以及子目录名长度
  # keys_zone=PROXYCACHE:100m - 缓存唯一标识与临时内存池大小
  # inactive=6h               - 缓存不活跃时间，超过则驱逐之
  # max_size=1g               - 缓存大小
  proxy_cache_path /var/run/nginx-proxy-cache levels=1:2 keys_zone=PROXYCACHE:100m inactive=6h max_size=1g;

  server {
    location /api/ {
      proxy_cache PROXYCACHE;   # 使用对应缓存
      proxy_cache_valid 200 1d; # 覆盖HTTP Cache Control有效时间
      proxy_cache_use_stale error timeout invalid_header http_500; # 覆盖HTTP复用旧缓存策略

      proxy_set_header Host $http_host;
      proxy_set_header X-Real-IP $remote_addr;
      proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
      proxy_set_header X-Forwarded-Proto $scheme;
      proxy_http_version 1.1;
      proxy_pass http://127.0.0.1:8080; # 指定URL
      proxt_pass http://upstream_name;  # 指定upstream
    }
  }
}
```

- `proxy_pass`如果包含 URI 则被 location 匹配的部分会被替换为该 URI，否则按原请求 URL 传递

## 负载均衡

- 根据顺序与权重轮询上游服务器，若失败则连接下一个，若均失败则返回客户端最后一个失败响应
- 通过添加额外命令和添加 server 参数从而修改负载均衡的行为

```nginx
upstream name {
    server 192.168.80.121:80;
    server 192.168.80.122:80;
    server 192.168.80.123:80;
}
```

| server 命令参数     | 默认值 | 备注                                           |
| ------------------- | ------ | ---------------------------------------------- |
| `weight=number`     | 1      | 权重                                           |
| `max_conns=number`  | 0      | 限制最大连接数，0 表示无限制                   |
| `max_fails=number`  | 1      | 限制失败连接数                                 |
| `fail_timeout=time` | 10     | 连接失败后 server 在多少秒内不可用             |
| `backup`            |        | 指定 server 为备用，当主 server 不可用时才使用 |
| `down`              |        | 标记为永久不可用                               |

| 其他命令                 | 备注                                                              |
| ------------------------ | ----------------------------------------------------------------- |
| `ip_hash;`               | 请求 ip 做 hash                                                   |
| `hash key [consistent];` | key 可包含变量，consistent 表示添加或删除 server 尽量不重映射 key |
| `least_conn;`            | 最少连接优先，并考虑权重                                          |
| `least_time;`            | 最少响应时间且最少连接的 server 优先，并考虑权重                  |
