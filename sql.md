# 目录
<!-- vim-markdown-toc GFM -->

- [安装与使用](#安装与使用)
- [SQL基础](#sql基础)
  - [系统信息](#系统信息)
  - [用户管理](#用户管理)
  - [数据库](#数据库)
  - [数据表](#数据表)
  - [数据列](#数据列)
  - [行记录](#行记录)
  - [数据类型](#数据类型)
    - [整数类型](#整数类型)
    - [实数类型](#实数类型)
    - [时间日期](#时间日期)
    - [字符类型](#字符类型)
    - [枚举类型](#枚举类型)
    - [位类型](#位类型)
  - [类型约束](#类型约束)
- [SQL查询](#sql查询)
  - [查询语句](#查询语句)
  - [运算符](#运算符)
  - [函数](#函数)

<!-- vim-markdown-toc -->
# 安装与使用
<!-- entry begin: mysql mysqld initialize install -->
**初始化mysql**
* `mariadb-install-db --user=mysql --basedir=/usr --datadir=/var/lib/mysql`
* `mysqld --initialize --user=mysql --basedir=/usr --datadir=/var/lib/mysql`

**连接mysql**
* `mysql -h 主机名 -P 端口号 -D 数据库名 -u 用户名 -p密码`
<!-- entry end -->

# SQL基础
## 系统信息
<!-- entry begin: sql variable status engines charset collation names -->
```sql
-- 默认变量作用域为SESSION
SET  [GLOBAL] var = val;
SHOW [GLOBAL] VARAIABLES [LIKE 'pattern'];
SHOW [GLOBAL] STATUS     [LIKE 'pattern'];
SHOW PROCESSLIST;
SHOW ENGINES    [LIKE 'pattern'];
SHOW CHARSET    [LIKE 'pattern'];
SHOW COLLATION  [LIKE 'pattern'];

SET  NAMES charset;
-- SET character_set_client     = charset;  -- 客户端使用的字符集
-- SET character_set_connection = charset;  -- 服务器处理请求时使用的字符集
-- SET character_set_results    = charset;  -- 回传给客户端时使用的字符集
```
<!-- entry end -->

## 用户管理
<!-- entry begin: sql user password grant -->
* 用户名
    * `user`                ：默认host为`'%'`
    * `user@'%'`            ：允许从任意主机登录
    * `user@localhost`      ：只允许本机登录
    * `user@192.168.0.1`    ：只允许从指定IP的主机登录
```sql
SELECT * FROM mysql.user;                       -- 查询用户
CREATE USER username [IDENTIFIED BY 'passwd'];  -- 创建用户
DROP   USER username;                           -- 删除用户
SET PASSWORD FOR username = PASSWORD('passwd'); -- 修改密码

SHOW GRANTS                                     -- 查询授权
GRANT  privcode ON db_name.tbl_name TO username;-- 授权用户
FLUSH  PRIVILEGES;                              -- 刷新权限
    -- 权限码包括：
    -- ALL PRIVILEGES
    -- SELECT
    -- INSERT
    -- 等等...
```
<!-- entry end -->

## 数据库
<!-- entry begin: sql database db -->
```sql
SHOW   DATABASES [LIKE 'pattern'];          -- 查询已有数据库
SHOW   CREATE    DATABASE        db_name;   -- 查询数据库详情
CREATE DATABASE  [IF NOT EXISTS] db_name;   -- 创建数据库
DROP   DATABASE  [IF EXISTS]     db_name;   -- 删除数据库
```
<!-- entry end -->

## 数据表
```sql
SHOW   TABLES [FROM db_name ] [LIKE 'pattern']; -- 查询已有数据表
SHOW   CREATE TABLE tbl_name;                   -- 查询数据表详情
CREATE TABLE tbl_name (                         -- 创建数据表
        fd_name type [const],
        ...,
        [PRIMARY KEY (fd_name),]
        [KEY         (fd_name),]
        [FOREIGN KEY (fd_name) REFERENCES tbl_name(fd_name),]
        [CHECK       (clause)]
    ) ENGINE=engine CHARSET=charset ROW_FORMAT=format;
DROP  TABLE tbl_name;                           -- 删除数据表
ALTER TABLE tbl_name RENAME TO new_tbl_name;    -- 修改数据表

-- 视图
CREATE VIEW view_name AS SELECT 语句;
DROP   VIEW view_name;

-- 导入与导出
LOAD DATA LOCAL INFILE '<file_name>' INTO TABLE tbl_name;

SELECT * FROM tbl_name INTO OUTFILE '<file_name>';
```
<!-- entry end -->

## 数据列
<!-- entry begin: sql table index metadata -->
* 操作元数据
```sql
-- field
SHOW COLUMNS FROM tbl_name;
ALTER TABLE tbl_name ADD fd_name <type> [<const>] [FIRST| AFTER fd_name];
ALTER TABLE tbl_name DROP fd_name;
ALTER TABLE tbl_name CHANGE fd_name <new_fd_name> <type> [<const>];

-- index
SHOW  INDEX FROM tbl_name;
ALTER TABLE tbl_name ADD INDEX <idx_name>(fd_name);
ALTER TABLE tbl_name DROP INDEX <idx_name>;

-- PRIMARY KEY
    -- 目标域必须为`NOT NULL UNIQUE`
ALTER TABLE tbl_name ADD PRIMARY KEY (fd_name);
ALTER TABLE tbl_name DROP PRIMARY KEY;

-- FOREIGN KEY
    -- 两域类型必须相同，且被引用的域必须为`UNIQUE`
ALTER TABLE tbl_name ADD CONSTRAINT <fk_name> FOREIGN KEY (fd_name) REFERENCES tbl_name(fd_name);
ALTER TABLE tbl_name DROP FOREIGN KEY <fk_name>;

-- CHECK
ALTER TABLE tbl_name ADD CONSTRAINT <ck_name> CHECK (<Clause>);
ALTER TABLE tbl_name DROP CHECK <ck_name>;
```
<!-- entry end -->

## 行记录
<!-- entry begin: sql insert delete 事务 -->
* 操作行数据
```sql
-- 插入
INSERT INTO tbl_name [(fd_name, ...)] VALUES    -- 外键列需要指定为该键所指向的列中已存在的值以进行关联
(value, ...), ... ;                             -- 若类型为字符串或日期时间，则`value`必须使用单引号

-- 复制
INSERT INTO tbl_name (fd_name, ...)
SELECT 语句 ;                            -- 两语句中的fd_name部分两两对应

-- 删除
DELETE FROM tbl_name
[WHERE <Clause>] ;                      -- 默认删除所有数据

-- 修改
UPDATE tbl_name SET fd_name=<value>, ...
[WHERE <Clause>] ;                      -- 默认修改所有行

-- 事务
BEGIN;                                  -- 开启事务
...
SAVEPOINT <point>;                      -- 设置保存点
...
RELEASE SAVEPOINT <point>;              -- 删除保存点
...
ROLLBACK TO <point>;                    -- 撤销至保存点
...
COMMIT; 或 ROLLBACK;                    -- 提交事务 或 撤销此次事务
```
<!-- entry end -->
## 数据类型
### 整数类型
> * 支持在其后添加`(N)`，指定十进制显示时的最小显示位数，最大255（不影响存储与计算） **deprecated** 
* 支持在其后添加`UNSIGNED`，指定为无符型整数
* 计算时统一使用`BIGINT`类型

| 类型      | 字节 |
|-----------|------|
| TINYINT   | 1    |
| SMALLINT  | 2    |
| MEDIUMINT | 3    |
| INT       | 4    |
| BIGINT    | 8    |

### 实数类型
> * 对于FLOAT与DOUBLE，支持在其后添加`(N, P)`，分别指定十进制显示时的最大有效位数与小数位数精度（影响存储大小限制） **deprecated** 
* 对于DECIMAL，支持在其后添加`(N, P)`，分别指定十进制显示时的最大有效位数与小数位数精度（默认`(10, 0)`）
* 计算时统一使用`DOUBLE`类型

| 类型    | 字节 |
|---------|------|
| FLOAT   | 4    |
| DOUBLE  | 8    |
| DECIMAL | ∞    |

### 时间日期
* 对于TIMESTAMP
    * 存储格林尼治时间而显示服务器或客户端本地时间
    * 默认`NOT NULL`
    * 默认插入或更新时会更新第一个TIMESTAMP列
* 除DATE外，其余类型支持在其后添加`(fps)`表示秒级小数位数

| 类型      | 字节 | 格式                |
|-----------|------|---------------------|
| DATE      | 3    | YYYY-mm-dd          |
| TIME      | 3    | HH:MM:SS            |
| TIMESTAMP | 4    | YYYY-mm-dd HH:MM:SS |
| DATETIME  | 8    | YYYY-mm-dd HH:MM:SS |

### 字符类型
* 对于CHAR，支持在其后添加`(N)`，指定固定字符数（字节数最小为N）
* 对于VARCHAY，支持在其后添加`(N)`，指定最大字符数
* 对于BINARY，支持在其后添加`(N)`，指定固定字节数
* 对于VARBINAY，支持在其后添加`(N)`，指定最大字节数
* 对于TEXT族与BLOB族，排序时仅比较前`max_sort_length`字节，且可能使用外部空间存储数据而在表中存储指针

| 类型            | 长度             | 说明                                 |
|-----------------|------------------|--------------------------------------|
| CHAR            | 0~255       字符 | 固定字符数，忽略输入的尾后空格       |
| VARCHAR         | 0~65535     字节 | 可变字符数                           |
| BINARY          | 0~255       字节 | 类似CHAR，但用`\0`填充且索引时不忽略 |
| VARBINARY       | 0~65535     字节 | 类似VARCHAR                          |
| TINYTEXT        | 0~255       字符 | 大字符串数据                         |
| SMALLTEXT(TEXT) | 0~65535     字符 | 大字符串数据                         |
| MEDIUMTEXT      | 0~16777215  字符 | 大字符串数据                         |
| LONGTEXT        | 0~4294967286字符 | 大字符串数据                         |
| TINYBLOB        | 0~255       字节 | 大二进制数据                         |
| SMALLBLOB(BLOB) | 0~65535     字节 | 大二进制数据                         |
| MEDIUMBLOB      | 0~16777215  字节 | 大二进制数据                         |
| LONGBLOB        | 0~4294967286字节 | 大二进制数据                         |

### 枚举类型
* 支持在其后添加`('str1', 'str2')`来设置枚举字符
    * 对于ENUM，枚举字符映射为1个整数
    * 对于SET，枚举字符映射为1个bit
* 对于ENUM，存储、计算、索引时使用数字，显示时使用映射的字符。
特别注意排序时也是按数字大小来，所以最好映射的字符顺序与数字顺序相对应

| 类型 | 说明                       |
|------|----------------------------|
| ENUM | 数字与字符串间的双射       |
| SET  | 给每一位取个可读性高的名称 |

### 位类型
> **deprecated**
> * 对于BIT，字面量语法位`b'1010'`。根据上下文语境可转换位字符或数字
> * 对于BIT，InnoDB默认使用最小的可以完整表示位的整数表示，故无法节省空间
> * 对于BIT，支持在其后添加`(N)`表示位数


## 类型约束
| 约束             | 说明         | 备注                                           |
|------------------|--------------|------------------------------------------------|
| `NOT NULL`       | 不能为空     | PRIMARY KEY的必要条件                          |
| `UNIQUE`         | 不能重复     | PRIMARY KEY的必要条件，UNIQUE KEY的充分条件    |
| `AUTO_INCREMENT` | 自动增加数值 | 其后可选添加`=n`设置初始值，默认为1，每次递增1 |
| `DEFAULT val`    | 设置默认值   |                                                |


# SQL查询
## 查询语句
<!-- entry begin: sql select group order -->
```sql
SELECT [DISTINCT] expression [AS fd_alias], ... -- expression表示选择列或表达式列
[FROM tbl_name [AS tbl_alias], ...]             -- 若有多个表则expression中列名必须加上表名

[[INNER|LEFT|RIGHT] JOIN tbl_name [tbl_alias]]  -- 连接多表，与在FROM子句中指定多表区别在于此处可外连接，首选INNER JOIN而非FROM联结
[ON fd_name = fd_name]                          -- 类似WHERE，但对于外连接不会过滤掉驱动表的行

[WHERE clause]                                  -- WHERE子句，用于条件过滤索引

[GROUP BY expression, ...]                      -- 将指定expression相等的列聚合为一行，且SELECT后的选择列只能使用此处出现过的
[WITH ROLLUP]                                   -- 会将所有聚合后的行再聚合成一行并添加到最后
[HAVING clause]                                 -- 类似WHERR，但可使用聚合函数，在分组后进行过滤

[ORDER BY expression [DESC], ...]               -- 按指定顺序的列进行（升序）排序，DESC指定该列按降序排列，可使用fd_alias

[LIMIT N] [OFFSET M]

UNION [ALL]                                     -- 合并两查询结果（上下连接）。ALL表示允许重复，默认删掉重复值
SELECT 语句;                                    -- 列数要相同，且类型可互相转换。组合查询仅能在最后一条SELECT语句有ORDER BY子句
```
![JOIN](https://www.runoob.com/wp-content/uploads/2019/01/sql-join.png)

```sql
-- 嵌套子查询 --

SELECT (SELECT语句) AS expr

WHERE fd_name IN (SELECT 语句)

-- 自联结 --
SELECT ...
FROM tbl AS t1, tbl AS t2
WHERE t1.id = t2.id AND t2.grade > 60   -- 查询grade大于60的id对应的信息
```
<!-- entry end -->


## 运算符
<!-- entry begin: sql operator -->
* sql运算符
    * 逻辑：`NOT` `AND` `OR` `XOR`          ：注意运算符优先级问题
    * 算术：`+` `-` `*` `/` `%`             ：可以用`()`来改变优先级
    * 比较：
        * `=` `!=` `<` `<=` `>` `=>`        ：若有一边为NULL则会返回false
        * `<=>`                             ：当两边相等或均为NULL返回true
        * `IS [NOT] NULL value`
    * 关系：
        * `[NOT] BETWEEN value1 AND value2`
        * `[NOT] LIKE   'wildchar'`         ：支持通配符`_`和`%`，匹配完整字符串
        * `[NOT] REGEXP 'pattern'`          ：转义序列使用类似`\\.`，匹配子串
        * `[NOT] IN     (val, ...)`
        * `[NOT] EXISTS (val, ...)`
        * `<OP> ALL     (val, ...)`         ：列表中所有行都符合`<OP>`
        * `<OP> ANY     (val, ...)`         ：列表中有一行符合`<OP>`
<!-- entry end -->

## 函数
<!-- entry begin: sql function -->
| 字符处理函数             | 描述                |
|--------------------------|---------------------|
| CONCAT(s, s)             | 拼接两数据          |
| LEFT(s, l)               | 左边l个字符         |
| RIGHT(s, l)              | 右边l个字符         |
| SUBSTRING(s, i, l)       | 返回子串，下标1开始 |
| LOCATE(substr, s[, pos]) | 返回子串起始下标    |
| LENGTH(s)                | 返回字符串长度      |
| UPPER(s)                 | 转为大写            |
| LOWER(s)                 | 转为小写            |
| LTRIM(s)                 | 删除左侧空白符      |
| RTRIM(s)                 | 删除右侧空白符      |
| TRIM(s)                  | 删除两侧空白符      |

| 时间处理函数                     | 描述                   |
|----------------------------------|------------------------|
| NOW()                            | 返回当前日期时间       |
| CURDATE()                        | 返回当前日期           |
| CURTIME()                        | 返回当前时间           |
| DATE(date)                       | 返回对应日期           |
| YEAR(date)                       | 返回对应年份           |
| MONTH(date)                      | 返回对应月份           |
| DAY(date)                        | 返回对应天数           |
| DAYOFWEEK(date)                  | 返回对应礼拜           |
| TIME(time)                       | 返回对应时间           |
| HOUR(time)                       | 返回对应小时           |
| MINUTE(time)                     | 返回对应分钟           |
| SECOND(time)                     | 返回对应秒数           |
| DATE_ADD(dt, INTERVAL expr UNIT) | 增加指定日期/时间      |
| DATE_SUB(dt, INTERVAL expr UNIT) | 增加指定日期/时间      |
| DATE_FORMAT(dt, format)          | 按指定格式打印日期时间 |

| 数值处理函数 | 描述   |
|--------------|--------|
| ABS(v)       | 绝对值 |
| SIN(v)       | 正弦   |
| COS(v)       | 余弦   |
| TAN(v)       | 正切   |
| SQRT(v)      | 平方根 |
| EXP(b, e)    | 幂     |
| Rand()       | 随机数 |
| PI()         | 圆周率 |

| 聚合函数 | 描述                      |
|----------|---------------------------|
| FIRST(f) | 返回第一个值              |
| LAST(f)  | 返回最后一个值            |
| MAX(f)   | 返回最大值                |
| MIN(f)   | 返回最小值                |
| SUM(f)   | 返回总和                  |
| COUNT(f) | 返回行数，`*`会计算NULL行 |
| AVG(f)   | 返回平均值                |

<!-- entry end -->

