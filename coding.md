# Coding Logic

## 编码逻辑

### 布尔代数

- 分配律：`A && (B || C)` 等同 `(A && B) || (A && C)`
- 德摩根定律：`!(A && B)` 等同 `!A || !B` 且 `!(A || B)` 等同 `!A && !B`

### 分支逻辑

- `if() else`：每个独立事件存在多种状态
- `if() if()`：多个独立事件不相关联（可同时发生）时各自单独处理
- `if(&& ||)`：多个独立事件相关联是则将状态组合后处理，$N_1 × N_2 × ... × N_n$（N 为一个事件的状态数）
- `if() elif() else`：若相邻分支的**分支条件**或**分支内容**部分重合，可合并到大分支以简化代码
- `multi-if` => `switch` => `hash-table`

### 循环步骤

1. 循环变量 - 维护语义 - 如何迭代
2. 条件检测 - 迭代与操作的相对位置（迭前++，++迭后）
3. 初始状态：根据相关关系调整初始状态，使第一次循环正确执行
4. 终止条件：若特殊终止条件变量在循环中构造，或由其导致退出循环时需要特殊操作，则需要使用 if-break

### 递归步骤

1. 递推公式与基准公式
2. 明确递归函数的参数、返回值、副作用
3. 尾递归转为循环

### 向量与数量

- 计算**距离**：
  - $(V_0, V_n] = V_n - V_0$
- 计算**数量**：
  - $[V_0, V_n] = V_n - V_0 + 1 = V_{n+1} - V_0$
- 计算**向量**：
  - $V_n = V_0 + D_{0n}$
  - $V_{n+1} = V_0 + N_{0n}$

### 指导方针

- 异常安全
  - 不要信任任何外部数据，必须校验其有效性
  - 检查任何可能出错的地方，并尝试恢复或报错
    - 空指针
    - 整型溢出：截断、转换、加减、除零
    - 浮点精度：有效位、非法运算、INF、NAN（[详见 INF&NAN](https://www.gnu.org/software/libc/manual/html_node/FP-Exceptions.html)）
- 异步安全
  - 任何 IO 和不可预期计算量的任务都需要异步执行
  - 注意函数对可重入性与线程安全性的保证
- 信息安全
  - 输出的敏感信息必须加密
  - 重要的数据记录必须持久化

## 解题思路

### 栈

**关键字**：层级、递归、后进先出

### 队列

**关键字**：滑动窗口、先进先出

### 单调栈

**关键字**：单调排列、最近较大/小值

特点：以单调递减栈为例

- 对出栈元素来说：
  - 入栈元素是其右侧最近的较大值
- 对入栈元素来说：
  - 栈里的元素即其左侧单调排列的元素（忽略破坏单调性的元素）
  - 出栈完成后栈顶第元素即其左侧最近的较大值

### 单调队列

**关键字**：滑动窗口最值

### 链表

**关键字**：随机插删

- 哑节点，简化边界
  > 反转链表时，哑节点为 nullptr
- 双指针，一前一后
  > 迭代更新 prev 指针时，最好使用`prev=cur; cur=cur->next_;`迭代
- 双指针，一快一慢
  > 每次循环 fast 先行两步，若成功则 slow 再后行一步，如此一来 slow 会在奇数时停在中点，偶数时停在较小中点
- 双指针，间隔距离
- 增加额外节点
- 更改链表结构

### 二叉树

**关键字**：快速搜索、元素排序

- 广度优先搜索(BFS)

- 深度优先搜索(DFS)

  - 先序遍历：序列首元素为根节点
  - 后序遍历：序列尾元素为根节点
  - 中序遍历：根节点将序列分为左右两边

  - 有序遍历：最小值在最左节点，最大值在最右节点；
    对于下个节点，若有右子树则为右子树最左节点，否则向上搜索直至根节点或某节点为其父节点的左子节点（父节点为下个节点）

  - 插入算法：利用引用语义传入父节点的子节点指针`TreeNode*& root`
    从而简化递归基准为为`if ( root == nullptr ) { root = new TreeNode{val}; return; }`

  - 删除算法：若有两个子节点则用右子树的最左节点替换该节点，否则`root = root->left ? root->left : root->right;`；
    若预期删除次数不多则可以使用“懒惰删除”从而提高性能

### 哈希表

**关键字**：极快速搜索、频度统计、重复值校验

### 二分搜索

**关键字**：搜索有序序列

```cpp
// 步骤一：设置循环条件检测
while ( left <= right ) {       // left <= right可以保证当right一直未移动时能检测到right
    mid = left + (right - left) / 2
// 步骤二：检测目标语义
    if ( Check(mid) )
        return mid;
// 步骤三：折半缩减搜索范围
    else if ( IfTargetInLeftHalf(mid) )
        right = mid - 1;        // 因为这一句则当left,right为下标时应该使用ssize_t而非size_t
    else
        left = mid + 1;
}
// 没有在循环中return说明未找到语义点
```

### 快速排序

```cpp
// 参数选择用[begin, end)而非[begin, last]从而可表示空集合
template<typename RandomIter, typename Compare=less<typename RandomIter::value_type> >
void quick_sort(RandomIter begin, RandomIter end, Compare comp=Compare{})
{
// 递归基准
    if ( end - begin < 16 ) {   // 注意快排核心算法必须满足范围[begin, end)长度至少为2
        for ( auto pos = next(begin); pos < end; ++pos )
            for ( auto swapPos = pos; swapPos > begin && comp(*swapPos, *prev(swapPos)); )
                swap(*swapPos, *--swapPos);
        return;
    }
// 选取枢纽点
    auto mid = begin + (end - begin) / 2, last = end - 1;
    if ( comp(*begin, *last) )  // 将较小值放尾部
        swap(*begin, *end);
    if ( comp(*mid, *last) )    // 将最小值放尾部
        swap(*mid, *last);
    if ( comp(*mid, *begin) )   // 将中值放首位
        swap(*mid, *begin);
    swap(*begin, *last);        // 最终使最小值在首位，中指在尾部；如此间接放置的原因是当只有两个元素时依然可行
    auto& piovt = *last;
// 划分区间
    left = begin;
    right = last;
    for ( ; ; ) {
        for ( ; comp(*++left, pivot); );     // 尾部元素为中指，不会越界
        for ( ; comp(pivot, *--right); );    // 首位元素为小值，不会越界
        if ( left >= right )
            break;
        swap(*left, *right);
    }
// 递归调用
    quick_sort(begin, left);
    quick_sort(left + 1, end);
}
```

### 堆排序

```cpp
// 下滤
template<typename RandomIter, typename Compare=less<typename RandomIter::value_type> >
void percolate_down(RandomIter begin, RandomIter end, RandomIter root, Compare comp=Compare{})
{
    for ( RandomIter child{}; (child = begin + (root - begin) * 2 + 1) < end; ) { // 子节点下标 = root * 2 - 1
        if ( child + 1 < end && comp(*root, child + 1) )
            ++child;
        if ( comp(*root, *child) ) {
            swap(*child, *root);
            root = child;
        } else {
            break;
        }
    }
}

template<typename RandomIter, typename Compare=less<typename RandomIter::value_type> >
void heap_sort(RandomIter begin, RandomIter end, Compare comp=Compare{})
{
// 建堆
    for ( auto pos = begin + (end - begin) / 2 - 1; pos >= begin; --pos ) { // 父节点下标 = (child + 1) / 2 - 1
        percolate_down(begin, end, pos, comp);
    }
// 堆排
    for ( last = end - 1; last > begin; --last ) {
        swap(*begin, *last);
        percolate_down(begin, last, begin, comp);
    }
}
```

### 归并排序

```cpp
template<typename RandomIter, typename Compare=less<typename RandomIter::value_type> >
void merge_sort(RandomIter begin, RandomIter end, RandomIter tmpBegin, RandomIter tmpEnd, Compare comp=Compare{})
{
    if ( end - begin < 2 )
        return;

    auto mid = begin + (end - begin) / 2;
    auto tmpMid = tmpBegin + (tmpEnd - tmpBegin) / 2;
    merge_sort(begin, mid, tmpBegin, tmpMid);
    merge_sort(mid, end, tmpMid, tmpEnd);
    auto left = begin, right = mid, tmpTail = tmpBegin;
    for ( ; left < mid && right < end; ) {
        if ( comp(*left, *right) ) {
            *tmpTail++ = *left++;
        } else {
            *tmpTail++ = *right++;
        }
    }
    for ( ; left < mid; )
        *tmpTail++ = *left++;
    for ( ; right < end; )
        *tmpTail++ = *right++;
    copy(tmpBegin, tmpEnd, begin);
}

template<typename RandomIter, typename Compare=less<typename RandomIter::value_type> >
void merge_sort(RandomIter begin, RandomIter end, Compare comp=Compare{})
{
    vector<typename RandomIter::value_type> tmpBuffer(end - begin);
    merge_sort(begin, end, tmpBuffer.begin(), tmpBuffer.end(), comp);
}
```

### 滑动窗口

**关键字**：子数组、跳跃遍历、滑动窗口  
**细节**：辅助表类型：覆盖型、叠加型、抵消型、语义型

### 回溯算法

**关键字**：路径搜索、路径染色、回溯算法  
**细节**：注意剪枝；注意返回前是否需要清除染色或弹出路径节点：每次都弹，全都不弹，成功则不弹

### 动态规划

“**所有的 DP 问题，本质上都是有限集中的最值问题**”——闫氏 DP 分析法

将问题模型化为：求有限集 S 中满足限制条件 C 的结果 max/min/count

- 状态表示：写出一个函数形式如`f(i, j)`用来表示一个子集合的结果

  - 集合：满足某些条件的子集合，如范围限制、长度限制、数值限制
  - 结果：通常为题解如 max/min/count

- 状态计算：写出状态表示函数的定义，即递推方程

  - 将当前集合不遗漏地划分为若干子部分求解
    - max/min 即各部分 max/min 值的 max/min（子部分划分可部分重叠）
    - count 即各部分 count 的和（子部分划分不可重叠）

- 时间优化与空间优化：都是对代码中公式的恒等变换

### 逆向思维

- 求起点的终点转为求终点的起点
- 对多个元素进行操作，相当于对其他元素进行相对逆操作
- 由结论出发，向所求条件逆推

### 位运算

- 位与(&)：0 归零 1 取值、1 不变 0 归零
- 位或(|)：0 不变 1 存值
- 异或(^)：0 不变 1 取反、同归零
- 位反(~)：~x == -x - 1
- 消除尾 1：x &= (x - 1)

### 模运算

- 同余方程
  - $(a - b) \% N = 0$
  - $a \equiv b \mod N$
  - $a+c \equiv b+c \mod N$
  - $a \times d \equiv b \times d \mod N$
- 求余方程
  - $(a + b)\% N = (a\% N + b\% N)\% N$
  - $(a \times b)\% N = (a\% N \times b\% N)\% N$
  - $(a - b)\% N = (a\% N - b\% N + N)\% N$
  - $(a \div b)\% N = (a \times b^{-1})\% N$
- 求模运算
  - 截位取值（字符转数字从前往后，数字转字符串从后往前）
  - 数值回环

### topK 算法

- 动态容器求 Top K

| 数据结构 | 描述                                                   |
| -------- | ------------------------------------------------------ |
| 数组     | 快速选择算法 O(N)                                      |
| 数组     | 插入有序数组 O(N)，随机访问 topK O(1)                  |
| 链表     | 插入有序链表 O(N)，遍历访问 topK O(1)                  |
| 红黑树   | 插入 O(log(N))，中序遍历获取 topK O(log(N))            |
| 最小堆   | 额外维护 topK 的最小堆，插入 O(log(K))，访问 topK O(1) |

- 动态容器求 No.K

| 数据结构       | 描述                                                                                  | 条件     |
| -------------- | ------------------------------------------------------------------------------------- | -------- |
| 数组           | 快速选择算法 O(N)                                                                     | 静态     |
| 数组           | 插入有序数组 O(N)，随机访问中位数 O(1)                                                | 动态插入 |
| 链表           | 维护两个指向中间节点的指针，插入有序链表 O(N)，访问中位数 O(1)                        | 动态插入 |
| 红黑树         | 每个节点维护该子树的节点数，插入 O(log(N))，访问中位数 O(log(N))                      | 动态插入 |
| AVL 树         | 用两子树的节点数做平衡因子，插入 O(log(N))，访问中位数 O(1)                           | 动态插入 |
| 最小堆与最大堆 | 使大数最小堆与小数最大堆的大小平衡，先插入小数最大堆，插入 O(log(N))，访问中位数 O(1) | 动态插入 |

### 海量数据

注：重复性问题可尝试应用 bit-map

注：重复度较高时可应用 trie-tree

- 问：找出文件 A、B 的共同数据？

  - 答：哈希取模，文件作桶，哈希统计，依序对比

- 问：统计海量数据的频度？

  - 答：哈希取模，文件作桶，哈希统计

- 问：对海量数据进行排序？

  - 答：分块，排序，归并

- 问：海量数据中找出 topK？

  - 答：维护 topK 最小堆

- 问：海量数据中找出频度 topK？

  - 答：哈希取模，文件作桶，哈希统计，维护 topK 最小堆

- 问：海量数据中找出中位数？

  - 答：截高位，文件作桶，找出中位数所在桶，应用中位数算法

- 问：分布式处理，海量数据分布在 N 台主机中，求 topK

  - 答：每台主机求 topK，然后汇总归并取前 K

- 问：分布式处理，海量数据分步在 M 台主机中，求频度 topK
  - 答：哈希取模，主机作桶，哈希统计，求 topK，汇总归并取前 K
