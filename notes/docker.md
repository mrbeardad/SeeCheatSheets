# Docker基本概念
* 镜像：相当于一个Root文件系统

* 容器：加载镜像创建的虚拟运行环境称为容器

* 仓库：在本地或远程存储并管理镜像的系统

# docker子命令
## 镜像
### 基础操作
<!-- entry begin: docker image 镜像 -->
> 所有`IMAGE`都可扩展为`REPO/IMAGE:TAG`
* image
    * ls
    * inspect   IMAGE
    * history   IMAGE
    * prune/rm  [IMAGE]
    * tag       IMAGE   TAG_IMAGE
    * save      -o file.tar IMAGE 
    * load      -i file.tar
<!-- entry end -->

### 远程仓库
<!-- entry begin: docker pull push search 远程仓库 -->
* login/logout
* pull      IMAGE
* push      IMAGE
* search    IMAGE
<!-- entry end -->

### 制作镜像
<!-- entry begin: docker image build export import 镜像制作 -->
* 导入导出
    > save/load与export/import的区别在于，前者会保留IMAGE所有信息（包括TAG, CMD），
    > 而后者只是保留容器中的文件系统。
    * export    -o file.tar CONT
    * import    file|URL|-  IMAGE

* image build -t IMAGE
```conf
# Dockerfile
FROM    IMAGE   # 指定基础镜像
LABEL   maintainer="email"
COPY    . /app  # 将当前目录下所有文件拷贝到镜像中的`/app`目录下
WORKDIR /app    # 指定工作目录
RUN     cmd     # 在基础镜像中运行cmd并将容器export到该镜像
EXPOSE  port    # 暴露指定端口
CMD     cmd     # 设置容器启动命令
```
<!-- entry end -->

## 容器
### 基础操作
<!-- entry begin: docker container -->
* container
    * ls [-a]
    * inspect       CONT
    * prune/rm      [CONT]
    * top           CONT
    * logs [-f]     CONT
<!-- entry end -->

### 运行容器
* container
<!-- entry begin: docker container run -->
    * create/run [OPT] IMAGE CMD ARGS
        > 需要输入交互则加参数`-it`，需要后台运行则加参数`-d`
        * `-it`                 ：终端交互
        * `-d`                  ：后台运行（保留stdout）
        * `-e`                  ：指定环境变量
        * `--rm`                ：退出时自动清理容器
        * `--name NAME`         ：指定容器的NAME
        * `--network NET`       ：指定Docker网络，默认提供`bridge`与`host`
        * `-P`                  ：将所有端口随机映射到宿主机
        * `-p HPORT:CPORT`      ：指定容器IP:容器端口:宿主机端口
        * `-h HOSTNAME`         ：指定容器主机名，会写入容器中的`/etc/{hostname,hosts}`
        * `-dns=IP`             ：指定容器DNS，会写入容器中的`/etc/resolv.conf`
    * exec [OPT] IMAGE CMD ARGS
        * `-it`                 ：终端交互
        * `-d`                  ：后台运行（保留stdout）
        * `-e`                  ：指定环境变量
    * attach        CONT
    * pause/unpause CONT
    * stop/kill     CONT
    * restart       CONT
    * start [-ia]   CONT
<!-- entry end -->

### 文件交互
<!-- entry begin: docker cp -->
* container cp [-aL] CONT:SRC_PATH DEST_PATH|-

* cp [-aL] SRC_PATH|- CONT:DEST_PATH

> `-`表示从stdin读取tarball并解压到容器中，或将容器中的文件打包成tarball输出到stdout
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

