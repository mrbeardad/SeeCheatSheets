# Docker基本概念
* 镜像：相当于一个Root文件系统

* 容器：加载镜像创建的虚拟运行环境称为容器

* 仓库：在本地或远程存储并管理镜像的系统

# docker子命令
## 镜像
### 基础操作
<!-- entry begin: docker image 镜像 -->
> 所有`IMAGE`都可扩展为`REPO/IMAGE:TAG`
* docker image
    * ls
    * inspect   IMAGE
    * history   IMAGE
    * tag       IMAGE   TAG_IMAGE
    * rm        IMAGE
    * prune
<!-- entry end -->

### 远程仓库
<!-- entry begin: docker pull push search 远程仓库 -->
* docker login
* docker logout
* docker search    IMAGE
* docker pull      IMAGE
* docker push      IMAGE
<!-- entry end -->

### 制作镜像
<!-- entry begin: docker export import save load 镜像制作 -->
> save/load与export/import的区别在于，前者会保留IMAGE所有信息（包括TAG, CMD），
> 而后者只是保留容器中的文件系统。
* docker save   -o file.tar IMAGE 
* docker load   -i file.tar

* docker export -o file.tar CONT
* docker import file|URL|-  IMAGE

* docker commit CONT IMAGE
<!-- entry end -->

<!-- entry begin: docker build Dockerfile .dockerignore -->
* docker build -t IMAGE
```conf
# .dockerignoe语法与.gitignore一样

# Dockerfile
FROM        IMAGE:TAG       # 指定基础镜像
LABEL       maintainer="e@mail"
ENV         PROJECT=/app    # 设置环境变量，环境变量也能在Dockerfile命令中使用
WORKDIR     /app            # 指定工作目录
SHELL       ["bash", "-u"]  # 指定构建命令使用的默认SHELL
COPY        . /app          # 将当前目录下所有文件拷贝到镜像中的`/app`目录下
RUN         cmd             # 在基础镜像的当前layer上执行该命令以更改镜像
EXPOSE      port            # 暴露指定端口
VOLUME      /path           # 指定挂载的VOLUME
STOPSIGNAL  signal          # 指定时容器exit的signal
ONBUILD     CMD cmd         # 指定当该镜像用作FROM时执行的命令
HEALTHCHECK CMD cmd         # 周期性检测容器是否正常（cmd返回0）
ENTRYPOINT  cmd             # 设置入口命令（使用相对/绝对项目路径），`docker run IMAGE`后的参数传给该命令，包括CMD指定的参数。该入口命令一般为脚本，记得使用exec执行真正命令以防止SIGTERM被bash捕获
CMD         ["cmd", "args"] # 设置容器启动命令，以JSON-List形式写命令则默认调用exec而非通过shell执行
```
<!-- entry end -->

## 容器
### 基础操作
<!-- entry begin: docker container -->
* docker container
    * create
    * start [-ia]
    * run
    * rename
    * rm [-v]
    * prune
    * update

    * stop          CONT
    * restart       CONT
    * pause         CONT
    * unpause       CONT
    * wait          CONT
    * kill          CONT
    * attach        CONT
    * exec [-it]    CONT

    * ls [-a]
    * logs [-f]     CONT
    * inspect       CONT
    * top           CONT
    * diff          CONT

* docker stats
* docker events
<!-- entry end -->

### 运行容器
<!-- entry begin: docker run -->
* docker create|run [OPT] IMAGE CMD ARGS
    > 需要输入交互则加参数`-it`，需要后台运行则加参数`-d`
    * `-it`                 ：终端交互
    * `-d`                  ：后台运行（保留stdout）
    * `-e`                  ：指定环境变量
    * `--workdir`           ：指定工作目录
    * `--name NAME`         ：指定容器的NAME
    * `--rm`                ：退出时自动清理容器
    * `--restart`           ：指定为`no|on-failure|always|unless-stopped`
    * `-c [0-1024]`         ：限制CPU使用率
    * `--cpu-sets=0,1`      ：限制CPU核心使用
    * `-m 500M`             ：限制内存使用
    * `--device=/dev/sda`   ：指定挂载设备
    * `--privileged`        ：挂载所有设备
    * `-v /h_path:/d_path`  ：挂载本机文件`/h_path`到容器`/d_path`
    * `-v v_name:/d_path`   ：冒号前不加`/`则表示指定docker在`/var/lib/docker/volumes`创建并挂载Volume`v_name`
    * `-v /docker_path`     ：指定docker`/var/lib/docker/volumes`创建并挂载Volume，volume名称随机
    * `--volumes-form`      ：挂载某个容器挂载的volumes
    * `--network NET`       ：指定Docker网络，默认提供`bridge`与`host`
    * `-P`                  ：将所有端口随机映射到宿主机
    * `-p HPORT:CPORT`      ：指定容器IP:容器端口:宿主机端口
    * `-h HOSTNAME`         ：指定容器主机名，会写入容器中的`/etc/{hostname,hosts}`
    * `--dns=IP`            ：指定容器DNS，会写入容器中的`/etc/resolv.conf`
    * `--add-host=HOST:IP`  ：指定容器hostname解析，写入`/etx/hosts`
<!-- entry end -->

<!-- entry begin: docker-compose -->
* docker-compose

```yaml
# docker-compose.yml
version: "3"    # 指定版本
services:       # 指定需要运行的容器
    mysql:              # 指定名称
        depends_on:
            - other_services
        images: "mysql:5.7"     # 指定使用的镜像，或使用下述build
        build: .                # 指定build需要的Dockerfile所在目录
        stdin_out: true         # -i
        tty: true               # -t
        workding_dir: /app      # --workdir
        environment:            # -e
            MYSQL_ALLOW_EMPTY_PASSWORD: 1
        container_name: db      # --name
        restart: "always"       # 与--start一样
        devices:                # --device
            - "/dev/sda:/dev/sda"
        volume:                 # -v
            - "/h_path:/d_path"
            - "v_name:/d_path"
            - "/d_path"
        network_mode: "none"    # --network host|bridge
        ports:
            - "3306:3306"
        hostname: "laptop"      # -h
        extra_hosts:            # --add-host
            - "googledns:8.8.8.8"
```
<!-- entry end -->

### 文件交互
<!-- entry begin: docker cp -->
> `-`表示从stdin读取tarball并解压到容器中，或将容器中的文件打包成tarball输出到stdout
* docker cp [-aL] CONT:SRC_PATH DEST_PATH|-
* docker cp [-aL] SRC_PATH|- CONT:DEST_PATH
<!-- entry end -->

# 网络模型
考虑下列情况：

&emsp;容器中运行了httpd，IP与端口为`172.17.0.2:80`；Docker0的IP为`172.17.0.1`；宿主机的IP为`192.168.0.2`。
此时从宿主机中打开浏览器，输入地址`172.17.0.2:80`即可访问容器中的httpd。
此时数据包走向为：宿主机`192.168.0.2`根据Docker创建的路由表将包送到Docker创建的虚拟网桥`172.17.0.1`，
然后然后虚拟网桥将数据包从`172.17.0.1`转发给同网段中的容器`172.17.0.2:80`。

&emsp;上述是从宿主机访问容器，接下来是从宿主机外的其他物理主机访问容器。某主机IP为`192.168.0.3`，
打开它的浏览器并输入地址`192.168.0.2:80`，数据包从`192.168.0.3`到`192.168.0.2:80`，
由于Docker会自动开启宿主机的端口，并给宿主机创建iptable来转发数据包，于是`192.168.0.2:80`被宿主机接收并转发到`172.17.0.2:80`。
根据路由策略，该转发包被送到虚拟网卡`172.17.0.1`，然后再转发给容器`172.17.0.2:80`。
自此，主机`192.168.0.3`访问主机`192.168.0.2`运行再容器中的httpd便成功了。

