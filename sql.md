# SQL

- [SQL](#sql)
  - [数据类型](#数据类型)
  - [类型限制](#类型限制)
  - [表达式](#表达式)
  - [CRUD](#crud)
    - [表](#表)
    - [列](#列)
    - [行](#行)
    - [查询](#查询)
    - [事务](#事务)

## 数据类型

| 数据类型           | 备注                              | 示例                                                            |
| ------------------ | --------------------------------- | --------------------------------------------------------------- |
| `BOOLEAN`          | 1 字节                            | `TRUE` `FALSE`                                                  |
| `INTEGER`          | 32 位整数                         | `32`                                                            |
| `BIGINT`           | 64 位整数                         | `32`                                                            |
| `DOUBLE PRECISION` | 64 位浮点数                       | `3.14`                                                          |
| `NUMERIC(p, s)`    | 大数，p 位有效数字，s 位最小精度  | `3.1415926`                                                     |
| `CHAR(n)`          | 固定 n 个字符，长度不足用空白填充 | `'char'`                                                        |
| `VARCHAR(n)`       | 最多 n 个字符                     | `'varchar'`                                                     |
| `TEXT`             | 无限制字符                        | `'text'`                                                        |
| `DATE`             | 日期                              | `DATE '2000-07-27'` `CURRENT_DATE`                              |
| `TIME`             | 时间                              | `TIME '21:30:59.789'` `CURRENT_TIME`                            |
| `TIMESTAMP`        | 日期时间                          | `TIMESTAMP '2000-07-27 21:30:59.789'` `CURRENT_DATETIME`        |
| `INTERVAL`         | 时间间隔                          | `INTERVAL '1 year 2 months 3 days 4 hours 5 minutes 6 seconds'` |
| `ENUM(...)`        | 枚举                              |                                                                 |
|                    | 结构                              | `(1, 2.5, 'str')` `(item).fd` `(item).*`                        |
| `datatype[][]`     | 数组                              | `ARRAY[[1,2],[3,4]]` `arr[1]` `arr[:][1]`                       |

## 类型限制

- `DEFAULT`
- `NOT NULL`
- `CHECK (expression)`
- `UNIQUE [NULLS NOT DISTINCT]`
- `PRIMARY KEY`
- `REFERENCES foreign_table(ref_column)`

## 表达式

- 运算符

  - 单前：`+` `-`
  - 算术：`+` `-` `*` `/` `%`
  - 关系：
    - `=` `<>` `<` `<=` `>` `=>` ：任意一边为 NULL 则会返回 false
    - `IS [NOT] NULL`
    - `~` `!~` `~*` `!~*`：正则匹配，`!`表示反选，`*`表示忽略大小写
    - `[NOT] LIKE 'pattern'` ：支持通配符`_`和`%`
    - `[NOT] ILIKE 'pattern'` ：忽略大小写
    - `[NOT] IN (value, ...)`
    - `[NOT] BETWEEN minval AND maxval`
    - `<OP> ALL (value, ...)`
    - `<OP> ANY (value, ...)`
    - `[NOT] EXISTS (SELECT...)`：若 SELECT 字句返回结果至少一行则为 true
  - 逻辑：`NOT` `AND` `OR`

- 类型转换

```sql
CAST (expression AS datatype)
```

- 条件表达式：

```sql
CASE expression
    WHEN value THEN result
    [WHEN...]
    [ELSE result]
END

COALESCE(value, ...)  -- 返回第一个非 NULL 参数，若没有则返回 NULL
NULLIF(value1, value2)-- 若两值相等则返回 NULL
GREATEST(value, ...)  -- 返回最大值，若全为 NULL 则返回 NULL
LEAST(value, ...)     -- 返回最小值，若全为 NULL 则返回 NULL
```

- 聚合表达式：应用于多行，结果为一行

```sql
aggregate_func ([DISTINCT] expression, ... [ORDER BY...] ) [FILTER (WHERE...)]
aggregate_func (expression, ...) WITHIN GROUP (ORDER BY...) [FILTER (WHERE...)]
```

- 窗口表达式：应用于多行，结果为多行

```sql
window_func (expression, ...) [FILTER (WHERE...)] OVER ([PARTITION BY...] [ORDER BY...])
```

- 标量子查询

```sql
SELECT name, (SELECT max(pop) FROM cities WHERE cities.state = states.name) FROM states;
```

## CRUD

> 注意列表尾部多余逗号`,`会报错

### 表

- 数据表

```sql
CREATE TABLE table_name (column_name datatype [constraints], ...);
DROP TABLE table_name;
```

- 索引：加速搜索筛选目标

```sql
CREATE INDEX index_name ON table_name (expression, ...);
CREATE UNIQUE INDEX index_name ON table_name (expression, ...) [NULLS NOT DISTINCT];
```

- 分区：将逻辑表分成若干物理表，以加速搜索筛选目标

```sql
-- range
CREATE TABLE... PARTITION BY RANGE (expression, ...)
CREATE TABLE part_name PARTITION OF table_name FOR VALUES FROM (begin_include) TO (end_exclude);
-- list
CREATE TABLE... PARTITION BY LIST (expression, ...)
CREATE TABLE part_name PARTITION OF table_name FOR VALUES IN (value, ...)
CREATE TABLE part_name PARTITION OF table_name DEFAULT;
-- hash
CREATE TABLE... PARTITION BY hash (expression, ...)
CREATE TABLE part_name PARTITION OF table_name FOR VALUES WITH (modulus 3, remainder 0);
CREATE TABLE part_name PARTITION OF table_name FOR VALUES WITH (modulus 3, remainder 1);
CREATE TABLE part_name PARTITION OF table_name FOR VALUES WITH (modulus 3, remainder 2);
```

- 继承：子表继承父表的列同时拥有自己的列，访问父表会包含子表（除非`ONLY`）

```sql
CREATE TABLE sub_table (...) INHERITS (parent_table);
```

- 视图

```sql
CREATE VIEW view_name AS SELECT...;
DROP   VIEW view_name;
```

### 列

```sql
ALTER TABLE table_name ADD    COLUMN column_name datatype constraints;
ALTER TABLE table_name RENAME COLUMN column_name TO new_name;
ALTER TABLE table_name ALTER  COLUMN column_name TYPE datatype;
ALTER TABLE table_name ALTER  COLUMN column_name SET DEFAULT value;
ALTER TABLE table_name ALTER  COLUMN column_name SET NOT NULL;
ALTER TABLE table_name ALTER  COLUMN column_name DROP DEFAULT;
ALTER TABLE table_name ALTER  COLUMN column_name DROP NOT NULL;
ALTER TABLE table_name ADD    constraints;
ALTER TABLE table_name DROP   CONSTRAINT constraints;
ALTER TABLE table_name DROP   COLUMN column_name;
```

### 行

```sql
INSERT INTO table_name (column_name, ...) VALUES (value, ...), ... [RETURNING column, ...];
INSERT INTO table_name (column_name, ...) SELECT... [RETURNING column, ...];
UPDATE table_name SET column_name=column_value, ... [WHERE...] [RETURNING column, ...];
DELETE FROM table_name [WHERE...] [RETURNING column, ...];
```

### 查询

```sql
-- SELECT 语句返回虚拟表

-- 循环处理结果表的每一行
SELECT [DISTINCT [ON (expression)]] expression [AS alias], ...  -- DISTINCT 表示根据 ON 表达式去重

-- 将多张表横向连接（笛卡尔积）
[FROM table_ref [AS alias], ...]              -- table_ref 可以是 table_name, joined_table 或 SELECT...

-- 筛选结果
[WHERE expression]                            -- expression 返回布尔值

-- 按 expression 是否相等来分组，并将同组的所有行聚合成一行
[GROUP BY expression, ...]                    -- expression 只能使用 SELECT 中出现的 expression 或 alias
-- 筛选分组后的结果
[HAVING expression]                           -- expression 必须使用聚合函数

-- 将两查询结果表纵向连接，列数必须相同且类型可互换；ALL 表示允许重复，默认删掉完全重复的行；
[UNION [ALL] SELECT...]                       -- 取并集
[INTERSECT [ALL] SELECT...]                   -- 取交集
[EXCEPT [ALL] SELECT...]                      -- 取左差集

-- 排序 SELECT 结果
[ORDER BY expression [DESC], ...]             -- expression 可以使用 SELECT 中出现的 alias，DESC 表示降序（默认升序）

-- 结果丢弃前面 M 条
[OFFSET M]

-- 结果最多输出 N 条
[LIMIT N]
```

执行顺序：

1. FROM 连接多表
2. WHERE 筛选记录
3. GROUP BY 分组聚合
4. HAVING 筛选分组
5. SELECT 表达式
6. UNION/INTERSECT/EXCEPT 集合
7. ORDER BY 排序
8. LIMIT 限选
9. DISTINCT 去重

> JOIN 通常，表 A 中的一行表示一个对象，表 B 中的行表示对象关联的数据
>
> - LEFT JOIN: 将所有 A 保留，若其无关联 B 则为 NULL
> - RIGHT JOIN: 将所有 B 保留，若其无关联 A 则为 NULL
> - INNER JOIN: 仅将具有关联的 A 和 B 保留
> - FULL JOIN: 将所有 A 和 B 保留，若其无关联 A 或 B 则为 NULL
>
> ![JOIN](images/sql-join.png)

### 事务

- 原子性
- 隔离性
- 持久性

```sql
BEGIN;                      -- 开启事务
...
SAVEPOINT mypoint;          -- 设置保存点
...
RELEASE SAVEPOINT mypoint;  -- 删除保存点
...
ROLLBACK TO mypoint;        -- 撤销至保存点
...
COMMIT 或 ROLLBACK ;         -- 提交事务 或 撤销此次事务
```
