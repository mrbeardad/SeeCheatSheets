**Nginx的功能在于路由转发与负载均衡**
# 路由转发
* 模拟若干**server**，通过**listen**与**server_name**进行匹配
* 实现若干路由策略，通过**location**进行匹配
* 对请求包进行操作：
  * 接受并响应静态资源，或转发动态资源请求到后端服务，或拒绝请求
  * 协议转换
  * 修改报文内容
  * 缓存资源

```nginx
server {
  # listen命令指定该虚拟主机监听地址与端口
  # default_server表示若该监听地址端口无server_name匹配，则使用该server作为默认
  listen (address[:port]|port) [default_server] [ssl] [http2 | spdy];

  # server_name命令指定用于匹配的主机名（可能是ip或domain），支持通配符与正则表达式
  # 1. 准确匹配server_name，e.g. www.example.com
  # 2. 通配符在开始时匹配server_name成功，e.g. *.example.com
  # 3. 通配符在结尾时匹配server_name成功，e.g. www.example.*
  # 4. 正则表达式匹配server_name成功，e.g. ~^www.example.com$
  # 5. 多个同一优先级则匹配第一个出现的
  server_name example.com ...;

  # ssl卸载
  ssl_certificate example.com.crt
  ssl_certificate_key example.com.key;
  ssl_password_file password.txt;

  # root命令指定文件系统目录用于查找静态资源
  root /path/to/root;
  # index命令指定用于访问目录（URL末尾为/）时内部重定向（重新匹配location）的默认文件
  index index.html;

  # locatin命令用于对匹配的url进行特殊操作，支持通配符与正则表达式
  # 正常匹配规则：
  # 1. 先匹配标准uri，记录匹配前缀最长的location；
  # 2. 再匹配正则uri，若存在匹配则使用该正则location，否则使用之前记录的标准locaion
  # 前缀符号：
  # =：用于标准uri前，严格精准匹配，若匹配成功则停止
  # ^~：用于标准uri前，若该location匹配度在标准匹配中最高则直接使用该项而不进行正则匹配
  # ~：用于表示uri包含正则表达式，并且区分大小写。
  # ~*：用于表示uri包含正则表达式，并且不区分大小写
  @location [=|~|~*|^~] uri {
    add_header Header 'value'
  }

  location /file {
    # 手动指定静态资源查找，而非nginx默认利用root与index查找
    # 按try_files参数顺序查找，末尾/表示目录，若无则跳转uri或返回状态码code
    try_files file ... uri;
    try_files file ... =code;
    try_files file ... @location
  }

	location /api {
    proxy_set_header Host $http_host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_set_header X-Forwarded-Proto $scheme;
	  proxy_pass http://127.0.0.1:8080;
    proxy_http_version 1.1;
  }
  
  location /process {
    set $variable value;
    if (condition) {
      break;
    }
    # 修改匹配的url
    # last: 停止当前location并重新匹配location
    # break: 停止当前location但不重新匹配
    # redirect: 302暂时重定向
    # permanent: 301永久重定向
    rewrite regex replacement [last|break|redirect|permanent];
    return code [text];
    return code URL;
    return URL;
  }
}
```


# 负载均衡
  * 为upstream指定一组服务器
  * 选择负载均衡策略：轮询、加权轮询、ip hash、url hash、热备
  * 将匹配到的请求包通过负载均衡算法转发给upstream中的一个
  
```nginx
# 普通轮询
upstream svr0 {
    server 192.168.80.121:80;
    server 192.168.80.122:80;
    server 192.168.80.123:80;
}
# 加权轮询
upstream svr1 {
    server 192.168.80.121:80 weight=1;
    server 192.168.80.122:80 weight=2;
    server 192.168.80.123:80 weight=3;
}
# 最少连接
upstream svr2 {
    least_conn;
    server 192.168.80.121:80;
    server 192.168.80.122:80;
    server 192.168.80.123:80;
}
# 最快响应
upstream svr3 {
    fair;
    server 192.168.80.121:80;
    server 192.168.80.122:80;
    server 192.168.80.123:80;
}
# ip hash
upstream svr4 {
    ip_hash;
    server 192.168.80.121:80;
    server 192.168.80.122:80;
    server 192.168.80.123:80;
}
# url hash
upstream svr5 {
    server 192.168.80.121:80;
    server 192.168.80.122:80;
    server 192.168.80.123:80;
    hash $request_uri;
    hash_method crc32;
}
upstream keepalive {
    server 192.168.80.121:80;
    server 192.168.80.122:80;
    server 192.168.80.123:80;
    # 长连接控制
    keepalive 32;           # 最大空闲长连接数
    keepalive_requests 1000;# 每次长连接最大请求书
    keepalive_time 1h;      # 每次长连接最长保持时间
    keepalive_timeout 75s;  # 空闲长连接最长保持时间
}
# server额外参数：
# weight：weight越大，负载的权重就越大。
# down：表示当前的server暂时不参与负载
# backup： 其它所有的非backup机器down或者忙的时候，请求backup机器
# max_fails：用于健康监测，允许请求失败的次数，默认为1
# fail_timeout：用于健康监测，max_fails次失败后暂停的时间。
}
```

# 全局配置示例
```nginx
# file: nginx.conf

user www-data;
worker_processes auto;
pid /run/nginx.pid;

events {
    worker_connections 768;
    multi_accept off; # 开启后每个work进程一直轮询处理连接，而非利用kernel多路复用的事件驱动
}

http {
    # Basic Settings
    sendfile on;
    tcp_nopush on;
    tcp_nodelay on;
    keepalive_timeout 65;
    types_hash_max_size 2048;
    # server_tokens off;
    # server_names_hash_bucket_size 64;
    # server_name_in_redirect off;
    include /etc/nginx/mime.types;
    default_type application/octet-stream;

    # SSL Settings
    ssl_protocols TLSv1 TLSv1.1 TLSv1.2 TLSv1.3; # Dropping SSLv3, ref: POODLE
    ssl_prefer_server_ciphers on;

    # Logging Settings
    access_log /var/log/nginx/access.log;
    error_log /var/log/nginx/error.log;

    # Gzip Settings
    gzip on;
    # gzip_vary on;
    # gzip_proxied any;
    # gzip_comp_level 6;
    # gzip_buffers 16 8k;
    # gzip_http_version 1.1;
    # gzip_types text/plain text/css application/json application/javascript text/xml application/xml application/xml+rss text/javascript;

    # Virtual Host Configs
    include /etc/nginx/conf.d/*.conf;
    include /etc/nginx/sites-enabled/*;
}
```