# 目录
<!-- vim-markdown-toc GFM -->

- [算法设计](#算法设计)
  - [回溯算法](#回溯算法)
  - [分治算法](#分治算法)
  - [动态规划](#动态规划)
  - [贪心算法](#贪心算法)
  - [算法优化](#算法优化)
- [时空复杂度](#时空复杂度)
  - [复杂度计算](#复杂度计算)
  - [经典复杂度](#经典复杂度)
- [数论](#数论)
  - [指数](#指数)
  - [对数](#对数)
  - [级数](#级数)
  - [模运算](#模运算)
  - [排列组合](#排列组合)
- [排序算法](#排序算法)
  - [冒泡排序](#冒泡排序)
  - [选择排序](#选择排序)
  - [插入排序](#插入排序)
  - [希尔排序](#希尔排序)
  - [归并排序](#归并排序)
  - [快速排序](#快速排序)
  - [堆排序](#堆排序)
  - [基数排序](#基数排序)
  - [计数排序](#计数排序)
- [C++标准库容器](#c标准库容器)
- [数据结构](#数据结构)
  - [栈](#栈)
  - [队列](#队列)
  - [堆](#堆)
  - [树状数组](#树状数组)
  - [ST表](#st表)
  - [并查集](#并查集)
  - [Trie树](#trie树)
  - [跳跃表](#跳跃表)
- [匹配与排序](#匹配与排序)
  - [KMP](#kmp)
  - [AC自动机](#ac自动机)
  - [计数排序](#计数排序-1)
- [图论](#图论)
  - [拓扑排序有向无圈图](#拓扑排序有向无圈图)
  - [单源无权路径](#单源无权路径)
  - [单源赋权路径](#单源赋权路径)
  - [关键路径分析有向无圈图](#关键路径分析有向无圈图)
  - [网络流最大值有向图](#网络流最大值有向图)
  - [无向图最小生成树无向连通图](#无向图最小生成树无向连通图)
  - [连通性](#连通性)
  - [无圈性](#无圈性)
  - [无向图割点无向图](#无向图割点无向图)
  - [欧拉回路无向连通图](#欧拉回路无向连通图)
  - [强分支有向图](#强分支有向图)
- [其它算法](#其它算法)
  - [哈夫曼编码贪婪](#哈夫曼编码贪婪)
  - [平面最近点分治](#平面最近点分治)
  - [最优二叉搜索树DP](#最优二叉搜索树dp)
  - [所有点对最短路DP](#所有点对最短路dp)
  - [收费公路重建回溯](#收费公路重建回溯)
  - [井字棋博弈回溯](#井字棋博弈回溯)
  - [单词代表](#单词代表)
  - [子序列最大值(max为负则0)](#子序列最大值max为负则0)
  - [最大公因数](#最大公因数)
  - [快速幂运算](#快速幂运算)
  - [厄拉多塞筛](#厄拉多塞筛)
  - [唯一分解定理](#唯一分解定理)

<!-- vim-markdown-toc -->

# 算法设计
## 回溯算法
* 利用***DFS***进行多路分叉选择，若选择错误或无选择则函数返回(撤销)重新选择
* 注意是否可以裁剪掉一些情况

## 分治算法
> 分治一般需要至少两次递归调用
* 将大问题分解为各类型子问题，各部分递归求解，然后综合结果
* 子问题的解可以用来裁剪掉一些情况

## 动态规划
**“所有的DP问题，本质上都是有限集中的最值问题”——闫氏DP分析法**

将问题模型化为：求有限集S中满足限制条件C的结果max/min/count

* 状态表示：写出一个函数形式如`f(i, j)`用来表示一个子集合的结果
    * 集合：满足某些条件的子集合，如范围限制、长度限制、数值限制
    * 结果：通常为题解如max/min/count

* 状态计算：写出状态表示函数的定义，即递推方程
    * 将当前集合不遗漏地划分为若干子部分求解
        * max/min即各部分max/min值的max/min（子部分划分可部分重叠）
        * count即各部分count的和（子部分划分不可重叠）

* 时间优化与空间优化：都是对代码中公式的恒等变换

## 贪心算法
* 一个问题的整体最优解可通过一系列局部的最优解的选择达到（即每个阶段做出对当前而言最好的选择而不考虑将来的后果）
* 并且每次的选择可以依赖以前作出的选择，但不依赖于后面要作出的选择，这就是贪心选择性质
* 必须证明每一步所作的贪心选择最终导致问题的整体最优解


## 算法优化
* 找出重复的计算
* 减少多余的拷贝
* 避免重复的访存
* 高速缓存命中率
* 分支预测命中率

# 时空复杂度
## 复杂度计算
* `T ≤ O `
* `T ≥ Ω `
* `T = Θ `
* `T < o `

* 复杂度合并：
    * $T_1+T_2=\max(O_1,O_2)$

    * $T_1\times T_2=O_1\times O_2$

## 经典复杂度
![fzd](images/fzd1.jpg)
![fzd2](images/fzd2.png)
![fzd3](images/fzd3.png)

# 数论
## 指数
* $X^AX^B=X^{A+B}$
* $X^A/X^B=X^{A-B}$
* $(X^A)^B=X^{AB}$

## 对数
* $\ln AB=\ln A+\ln B$
* $\ln A/B=\ln A-\ln B$
* $\ln A^B=B\ln A$
* $\log_AB=\frac{\log_CB}{\log_CA}$

## 级数
* $\sum_{i=0}^{N}A^i=\frac{1-A^{N+1}}{1-A}\qquad(|A|>1)$

* $\sum_{i=0}^{N}A^i\le\frac{1}{1-A}\qquad(|A|<1)$

* $\sum_{i=1}^{N}i^k\approx \frac{N^{k+1}}{|k+1|}\qquad(k \ne -1)$

* $\sum_{i=1}^Ni^k\approx \ln{N}\qquad(k=-1)$

* $F_{k+1}\lt(5/3)^{k+1}\qquad(F_{k+1}=F_k+F_{k-1})$

## 模运算
* 同余方程：
    > 若$(a-b)\\\% N=0$
    * 则$a\equiv b\mod N$

    * 则$a+c\equiv b+c\mod N$

    * 则$a\times d\equiv b\times d\mod N$

* 求余方程：
    * $(a+b)\\\% N = (a\\\% N + b\\\% N)\\\% N$

    * $(a\times b)\\\% N = (a\\\% N \times b\\\% N)\\\% N$

    * $(a-b)\\\% N = (a\\\% N - b\\\% N + N)\\\% N$

    * $(a\div b)\\\% N = (a\times b^{-1})\\\% N$
        > 公式推导：
        > $$
        > (a\div b)\mod N \Rightarrow (a\div b)\times 1\mod N
        > \Rightarrow(a\div b)\times(b\times b^{-1})\mod N
        > \Rightarrow(a\times b^{-1})\mod N
        > $$
        > **$b^{-1}$是$b$ 的逆元。因为计算机整数运算中`a/b`会丢弃小数，违反了上述证明必要的数学运算逻辑，故需要 $a$ 可以被 $b$ 整除**

* 模算术逆元：
    * 定义：$b\times b^{ -1 } \equiv 1 \mod N$  
        > 所有$x=b^{-1}+k\times N(k\in Z)$ 都是 $b$ 在模$N$意义下的逆元；  
        > **整数 $b$ 存在逆元的充要条件是 $b$ 与 $N$ 互素**
    * 求解：
        * 费马小定理：  
            > 若$b$为整数，$N$为素数，  
            > 则$b^{N-1} \equiv 1 \mod N\Rightarrow b\times b^{N-2} \equiv 1 \mod N$，  
            > 即$b^{N-2}$就是$b$在模$N$意义下的逆元
        * 扩展欧几里得算法
            > 前提条件：a能被b整除
            <details>
                <summary><b>Code ...</b></summary>
            
            ```cpp
            int ext_gcd(int a, int b, int& x, int& y)
            {
                if ( b == 0 ) {
                    x = 1;
                    y = 0;
                    return a;
                }
                auto gcd = ext_gcd(b, a % b, y, x);

                //   因 ans == b * y + (a % b) * x
                //   又 a % b == a - (a / b) * b
                //   故 ans == b * y + (a - (a / b) * b) * x == x * a + (y - (a / b) * x) * b
                y -= a / b * x;

                return gcd;
            }
            ```
            </details>
        * 穷举法
            <details>
                <summary><b>Code ...</b></summary>
            
            ```python
            def inverse(b, mod):
                for i in range(1, mod):
                    if i * b % mod == 1:
                        return i
                return 0
            ```
            </details>

## 排列组合
* 加法原理：完成一个过程有N类方法

* 乘法原理：完成一个过程有N个步骤

* 排列数
$$
A^m_n=\frac{n!}{(n-m)!}
$$
* 多重集的全排列数：
    > 对于每个元素都存在$n_i!$次相同排列
$$
\binom{n}{n_1,n_2,\cdots,n_k}=\frac{n!}{\prod_{i=1}^kn_i!}
$$
* 组合数
    > 抽出的组合存在$m!$种排列，除去这些重复组合
$$
C^m_n=\binom{n}{m}=\frac{n!}{m!(n-m)!}
$$
* 多重集的组合数：
    > 即求$x_1+x_2+\cdots+x_k=r$的解的个数，  
    > 利用插板法得$(x_1+1)+(x_2+1)+\cdots+(x_k+1)=r+k$，  
    > 需要在$r+k-1$个位置中插入$k-1$个隔板
$$
得\binom{r+k-1}{k-1}
$$

# 排序算法
## 冒泡排序
```cpp
int* bubbleSort(int* A, int n) {
    for ( ssize_t right{n - 1}; right > 0; --right )
        for ( ssize_t idx{}; idx < right; ++idx )
            if ( A[idx] > A[idx + 1] )
                std::swap(A[idx], A[idx + 1]);
    return A;
}
```
## 选择排序
```cpp
int* selectionSort(int* A, int n) {
    for ( ssize_t right{n - 1}; right > 0; --right ) {
        int maxIdx{};
        for ( ssize_t idx{1}; idx <= right; ++idx )
            maxIdx = A[idx] > A[maxIdx] ? idx : maxIdx;
        std::swap(A[maxIdx], A[right]);
    }
    return A;
}
```
## 插入排序
```cpp
int* insertionSort(int* A, int n) {
    for ( ssize_t idx{1}; idx < n; ++idx )
        for ( ssize_t swapIdx{idx}; swapIdx >= 1 && A[swapIdx - 1] > A[swapIdx]; --swapIdx )
            std::swap(A[swapIdx - 1], A[swapIdx]);
    return A;
}
```
## 希尔排序
```cpp
int* shellSort(int* A, int n)
{
    std::array<size_t, 5> shellGaps{1, 5, 19, 41, 109}; // 增量序列：9 * 4**i - 9 与 4**i - 3 * 2**i + 1
    for ( size_t gap : shellGaps )
        for ( size_t idx{gap}; idx < n; ++idx )
            for ( size_t prev{idx}; prev >= gap && A[prev - gap] > A[prev]; prev -= gap )
                std::swap(A[prev - gap], A[prev]);
    return A;
}
```

## 归并排序
```cpp
void mergeSort(int* A, int* B, size_t begin, size_t end)
{
    if ( end - begin <= 1 )
        return;

    size_t middle{(begin + end) / 2};
    mergeSort(A, B, begin, middle);
    mergeSort(A, B, middle, end);
    size_t leftIdx{begin}, rightIdx{middle}, mergeIdx{begin};
    for ( ; leftIdx < middle && rightIdx < end; ++mergeIdx ) {
        if ( A[leftIdx] <= A[rightIdx] )
            B[mergeIdx] = A[leftIdx++];
        else
            B[mergeIdx] = A[rightIdx++];
    }
    for ( ; leftIdx < middle; ++leftIdx )
        B[mergeIdx++] = A[leftIdx];
    for ( ; rightIdx < end; ++rightIdx )
        B[mergeIdx++] = A[rightIdx];
    for ( size_t idx{begin}; idx < end; ++idx )
        A[idx] = B[idx];
}
```
## 快速排序
```cpp
void quickSort(int* A, ssize_t begin, ssize_t end)
{
    // 小数组使用插入排序优化
    if ( end - begin <= 10 ) {
        for ( ssize_t idx{begin + 1}; idx < end; ++idx )
            for ( ssize_t swapIdx{idx}; swapIdx > 0 && A[swapIdx - 1] > A[swapIdx]; --swapIdx )
                std::swap(A[swapIdx - 1], A[swapIdx]);
        return;
    }

    // 选取枢纽点
    ssize_t middle{(begin + end) / 2}, last{end - 1};
    if ( A[middle] < A[begin] )
        std::swap(A[middle], A[begin]);
    if ( A[last] < A[begin] )
        std::swap(A[last], A[begin]);
    if ( A[middle] < A[last] )
        std::swap(A[middle], A[last]);
    int& pivot{A[last]};

    ssize_t left{begin}, right{last};
    for ( ; ; ) {
        // 有[begin] <= pivot 与 [last] == pivot作边界，不可能越界
        for ( ; A[++left] < pivot; );
        for ( ; A[--right] > pivot; );
        if ( left <= right )
            std::swap(A[left], A[right]);
        else
            break;
    }
    std::swap(A[left], pivot);

    quickSort(A, begin, left);
    quickSort(A, left + 1, end);
}
```

## 堆排序
```cpp
void percolate_down(int* A, ssize_t n, ssize_t root)
{
    int holeVal{A[root]};
    auto leftChild = [] (ssize_t rootIdx) {
        return rootIdx * 2 + 1;
    };

    for ( ; leftChild(root) < n; ) {
        ssize_t child{leftChild(root)};
        if ( child + 1 < n && A[child + 1] > A[child] )
            ++child;
        if ( A[child] > holeVal )
            A[root] = A[child];
        else
            break;
        root = child;
    }
    A[root] = holeVal;
}
int* heapSort(int* A, int n) {
    for ( ssize_t idx{n / 2 - 1}; idx >= 0; --idx )
        percolate_down(A, n, idx);

    for ( ssize_t right{n - 1}; right > 0; --right ) {
        std::swap(A[0], A[right]);
        percolate_down(A, right, 0);
    }
    return A;
}
```

## 基数排序
```cpp
int* radixSort(int* A, int n) {
    std::vector<std::vector<int> > buckets(10);
    for ( int times{1}; buckets.front().size() < n; times *= 10 ) {
        std::for_each(buckets.begin(), buckets.end(), [](std::vector<int>& thisB){thisB.clear();});
        std::for_each(A, A + n, [&buckets, &times](int elem){buckets[elem / times % 10].emplace_back(elem);});
        size_t idx{};
        for ( auto& thisB : buckets ) {
            for ( int thisElem : thisB ) {
                A[idx++] = thisElem;
            }
        }
    }
    return A;
}
```
## 计数排序
```cpp
int* countingSort(int* A, size_t n)
{
    auto minmax = std::minmax_element(A, A + n);
    int min{*minmax.first}, max{*minmax.second};
    std::vector<std::vector<int> > buckets(max - min + 1);
    std::for_each(A, A + n, [&buckets, min](int elem){buckets[elem - min].emplace_back(elem);});
    size_t idx{};
    for ( auto& thisBkt : buckets ) {
        for ( auto thisElem : thisBkt ) {
            A[idx++] = thisElem;
        }
    }
    return A;
}
```

# C++标准库容器
* vector
    * 无其他需求一般选择vector
* array
    * 固定大小
    * 或容量需求大且时间效率优于空间效率
* deque
    * 需要头部增删元素
    * 或容量需求大且空间效率优于时间效率
    * 或容量需求过大
* list
    * 需要大量地随处插删
* Pqueue
    * 只需快速找出最值而无需完全排序
* Associated
    * 快速搜索且需要自动排序
    > 若只是为了排序（不需要动态地使插入元素自动排序），则考虑线性数组+排序算法
* Unordered
    * 快速搜索且无需排序
    > 若元素的比较为整数的比较，且该整数的值域可接受作为下标索引，则考虑线性数组作极限哈希表
* Map
    * 在set的基础上提供更方便的下标操作
    * 只比较key而进行映射集合的分类

# 数据结构

## 栈
* LIFO表：后进先出

* 单调栈：
    > 从栈顶弹出破坏单调性的元素（以单调**递增**栈为例）
    * 对出栈元素：
        * 入栈元素是其右侧第一个 **小值**
    * 对入栈元素：
        * 栈中元素即其左侧单调排列的元素（去除了破坏单调性的元素）
        * 弹出所有破坏单调性的元素后，栈顶元素即其左侧第一个 **小值**


## 队列
* FIFO表：先进先出

* 多级反馈队列：
    * 优先级高的任务未完成则放入下级队列尾部（操作系统的调度算法）

* 单调队列：
    * 从 **队尾** 弹出破坏单调性的元素
    * 从队首弹出不属于滑动窗口内的元素后，队首元素即窗口内的最小值（单调升序）


## 堆
* 节点值大于其子节点值
* 子节点索引为`i`，其父节点索引为`(i + 1) / 2 - 1`
    > 若索引0空缺则为`i / 2`
* 从最后节点往前进行堆序化

## 树状数组
* 特点：快速进行任意范围区间的可加性操作与查询
* 细节：
    * 父节点管理前面`lowbit(x & -x)`个元素
    * 0索引空缺(0管理0个节点)，数组多开一格
    * 更新节点需要更新所有管理该元素的父节点
    * 询问总和只需获取每个区间的管理者并取和
* 单点修改+区间查询：
    > 数组元素维护自己管理区间的总和
    * `sum_tool(idx)`：询问区间`[1,idx]`的总和，利用`-lowbit(x)`迭代
    * `point_add(idx, val)`：更新所有管理该点的父节点，利用`+lowbit(x)`迭代
    * `range_query(lefy, right)`：`sum_tool(right) - sum_tool(left - 1)`
* 区间修改+单点查询：
    > 数组元素维护该点与前一点的差分
    * `range_add(left, right, val)`：`point_add(left, val); point_add(right + 1, -val)`
    * `point_query(idx)`：`sum_tool(idx)`
* 区间修改+区间查询：
    > 相对上一种类型，额外用一数组`diff`，其元素维护差分`(idx - 1) * BITree[idx]`
    * `range_add(left, right, val, diff)`：BITree数组同上，
        diff数组`point_add(left, (left - 1) * val); point_add(right + 1, (right - 1) * -val)`
    * `range_query(left, right, diff)`：`_sum_tool(right, diff) - _sum_tool(left - 1, diff)`，
        其中`_sum_tool(originIdx, val)`利用`sum = originIdx * BITree[idx] - diff[idx]`求和
<details>
    <summary><b>Code...</b></summary>

```cpp
template <typename T>
struct BITree
{
    vector<T> tree_m;

    explicit BITree(size_t size): tree_m(size + 1) {}

    T sum_tool(size_t idx)
    {
        T sum{};
        for ( int curIdx = idx; curIdx > 0; curIdx -= curIdx & -curIdx ) {
            sum += tree_m[curIdx];
        }
        return sum;
    }

    void point_add(size_t idx, const T& val) // point_change&range_query
    {
        int size = tree_m.size();
        for ( int curIdx = idx; curIdx < size; curIdx += curIdx & -curIdx ) {
            tree_m[curIdx] += val;
        }
    }

    T range_query(size_t left, size_t right) // point_change&range_query
    {
        return sum_tool(right) - sum_tool(left - 1);
    }

    void range_add(size_t left, size_t right, T val) // range_change&point_query
    {
        point_add(left, val);
        point_add(right + 1, -val);
    }

    T point_query(size_t idx) // range_change&point_query
    {
        return sum_tool(idx);
    }

    void range_add(size_t left, size_t right, T val, vector<T>& diff) // range_change&range_query
    {
        range_add(left, right, val);

        int size = tree_m.size();
        auto pointAdd = [&] (int idx, T val) {
            while ( idx < size ) {
                diff[idx] += val;
                idx += idx & -idx;
            }
        };

        pointAdd(left, val * (left - 1));
        pointAdd(right + 1, val * -(right - 1));
    }

    T range_query(size_t left, size_t right, vector<T>& diff) // range_change&range_query
    {
        auto pointQuery = [&] (int idx) {
            T sum{};
            int orgIdx{idx};
            while ( idx > 0 ) {
                sum += orgIdx * this->tree_m[idx] - diff[idx];
                idx -= idx & -idx;
            }
            return sum;
        };

        return pointQuery(right) - pointQuery(left - 1);
    }
};

```
</details>

## ST表
* 特点：对于重叠OP无影响的结果进行快速地任意区间查寻
* 细节：
    * `dp[i][j]`代表区间$[i, i^j-1]$的OP结果
* build()：
    * 初始状态：`dp[i][0]`=`a[i]`
    * 递推方程：`dp[i][j]`=$OP(dp[i][j-1], dp[i+2^{j-1}][j-1]$)
    * 外迭代$j$直到最后状态为$k$，内迭代$i$直到$i+2^j\le n$
* query()：
    * $k = \log(n)/\log(2)$，$n$表示数量
    * $OP(dp[i][k]，dp[j-2^k+1][k])$
<details>
    <summary><b>Code...</b></summary>

```cpp
template <typename T>
struct STable
{
    vector<vector<T> > table_m;

    template <typename ITER>
    void build(ITER begin, ITER end)
    {
        while ( begin != end ) {
            table_m.push_back({});
            table_m.back().push_back(*begin++);
        }

        int tableSize = table_m.size();
        for ( int maxPow = log(tableSize) / log(2), curPow{1}; curPow <= maxPow; ++curPow ) {
            for ( int idx{}; idx + (1 << curPow) <= tableSize; ++idx ) {
                table_m[idx].push_back(max(table_m[idx][curPow - 1], table_m[idx + (1 << (curPow - 1))][curPow - 1]));
            }
        }
    }

    T query(size_t left, size_t right)
    {
        int maxPow = log1p(right - left) / log(2);
        return max(table_m[left][maxPow], table_m[right - (1 << maxPow) + 1][maxPow]);
    }
};

```
</details>

## 并查集
* 特点：快速查询与合并类型集合
* 细节：
    * 数组索引作为元素标识，存储内容为上级元素的索引，根的存储内容为负且其绝对值代表高度
* root()：查找目标所在集合的根，并进行路径压缩
* merge()：合并两集合的根，并更新高度
<details>
    <summary><b>Code...</b></summary>

```cpp
struct DisSet
{
    vector<int> vec_m;

    explicit DisSet(size_t size): vec_m(size, -1) {}

    size_t root(size_t idx)
    {
        if ( vec_m[idx] < 0 ) {
            return idx;
        }

        size_t itsRoot{root(vec_m[idx])};
        return vec_m[idx] = itsRoot;
    }

    void merge(size_t root1, size_t root2)
    {
        int& dep1{vec_m[root1]};
        int& dep2{vec_m[root2]};
        if ( dep1 < dep2 ) {
            dep2 = root1;
        } else {
            if ( dep1 == dep2 ) {
                --dep2;
            }
            dep1 = root2;
        }
    }
};

```
</details>

## Trie树
* 特点：快速匹配字符串前缀
* 细节：
可以使用数组实现
    * 每个节点存储flag以判定此节点是否为单词尾部
    * 每个节点存储Umap或vector以判断后续节点是否存在
* 插入：
    * 按前缀一路查询，无匹配则添加节点，尾部节点flag置1
* 删除：
    * 从尾部开始，若无后续字符串清除Umap中的ptr，并销毁节点
* 查询：
    * 按前缀查询完后，flag为true则匹配
<details>
    <summary><b>Code...</b></summary>

```cpp
struct Vertex
{
    array<size_t, 26> next_m;
    bool isEnd_m;
};

struct Trie
{
    vector<Vertex> tree_m;

    explicit Trie(): tree_m{Vertex{}} {}

    void insert(const string& insetStr)
    {
        int trieIdx{};
        for ( char thisChar : insetStr ) {
            size_t& nextIdx{tree_m[trieIdx].next_m[thisChar - 'a']};
            if ( nextIdx == 0 ) {
                trieIdx = nextIdx = tree_m.size();
                tree_m.push_back({});
            } else {
                trieIdx = nextIdx;
            }
        }
        tree_m[trieIdx].isEnd_m = true;
    }

    bool match(const string& matchStr)
    {
        int strIdx{}, strEnd = matchStr.size(), trieIdx{};
        int nextIdx{};
        while ( strIdx < strEnd && (nextIdx = tree_m[trieIdx].next_m[matchStr[strIdx] - 'a']) != 0 ) {
            ++strIdx; 
            trieIdx = nextIdx;
        }
        return strIdx == nextIdx && tree_m[trieIdx].isEnd_m;
    }
};

```
</details>

## 跳跃表
* 特点：快速搜索元素并自动排序
* 细节：
    * 每个元素包含一个多级索引表，其为N级的概率为1/2ⁿ，第N级索引指向后面第2ⁿ个元素
* 搜索：
    * 从每个节点的最高级索引开始搜索
* 插入与删除：
    * 利用0-1分布，决定索引表是否提升
    * 保留每级索引最近的节点并更新
<details>
    <summary><b>noCode...</b></summary>

```cpp
//真的没有o(=·ェ·=)m
```
</details>

# 匹配与排序

## KMP
* 特点：快速匹配模式子串
* 细节：
    * 失配指针next：指向该位置所代表的字符串的一个前缀的后一个位置，该前缀与字符串后缀相等且是所有可能中最长的
    * 计算next：依赖前一个位置的next值，注意前缀的前缀等于后缀的后缀
    * 模式匹配：某个位置匹配失败，则直接将模式串前缀右移到next记录的后缀的位置上
<details>
    <summary><b>Code...</b></summary>

```cpp
struct KMP
{
    vector<int> next_m;
    string pat_m;

    explicit KMP(const string& patStr): pat_m{patStr}
    {
        int lenth = patStr.length();
        next_m.resize(lenth);
        for ( int idx{1}; idx < lenth; ++idx ) {
            int pubLen{next_m[idx - 1]};
            while ( pubLen >= 1 && patStr[pubLen] != patStr[idx] ) {
                pubLen = next_m[pubLen - 1];
            }
            if ( patStr[pubLen] == patStr[idx] ) {
                next_m[idx] = pubLen + 1;
            } else {
                next_m[idx] = 0;
            }
        }
    }

    size_t operator()(const string& cmpStr, size_t begin = 0)
    {
        int cmpIdx{}, cmpEnd = cmpStr.size(), patIdx{}, patEnd = next_m.size();
        while ( cmpIdx != cmpEnd && patIdx != patEnd ) {
            if ( cmpStr[cmpIdx] == pat_m[patIdx] ) {
                ++cmpIdx;
                ++patIdx;
            } else if ( patIdx != 0 ) {
                patIdx = next_m[patIdx - 1];
            } else {
                ++cmpIdx;
            }
        }

        if ( patIdx == patEnd ) {
            return cmpIdx - patEnd;
        } else {
            return UINT64_MAX;
        }
    }
};
```
</details>


## AC自动机
* 特点：快速进行一对多模式匹配
* 细节：
    * 失配指针：指向这样一个节点，它代表的字符串是当前已匹配的字符串的后缀，且是所有可能中最长的
    * 计算fail：依赖父节点的fail，注意将不存在的子节点赋值为其fail的同一子节点的值
    * 模式匹配：失配时跳转到fail指向的节点
<details>
    <summary><b>Code...</b></summary>

```cpp
struct Vertex
{
    array<size_t, 26> children_m;
    int fail_m;
    bool isEnd_m;
};

struct Trie
{
    vector<Vertex> trie_m;

    void insert(const string& str)
    {
        size_t trieIdx{};
        for ( size_t strIdx{}, strLen{str.size()}; strIdx < strLen; ++strIdx ) { // 循环：插入每个字符直到全部插完。迭代：strIdx，trieId
            auto& nextIdx{trie_m[trieIdx].children_m[str[strIdx] - 'a']}; // 附：子节点的索引
            if ( nextIdx == 0 ) { // 如果不存在该子节点，则添加并更新该节点
                nextIdx = trie_m.size();
                trieIdx = nextIdx; // 附：在push_back()前赋值，因为在之后的话容器重新分配导致nextIdx失效
                trie_m.push_back({});
            } else {
                trieIdx = nextIdx;
            }
        }
        trie_m[trieIdx].isEnd_m = true; // 设置该节点为字符串尾部
    }

    template<typename ...T>
    void build(const T&... strs)
    {
        // 初始化根节点，并插入所有模式串
        trie_m.push_back({});
        (insert(strs), ...);

        // 将第一层节点入队，它们的fail_m都指向根节点，代表最长公共前后缀为0
        queue<size_t> bfsQue;
        for ( size_t childIdx : trie_m[0].children_m ) {
            if ( childIdx != 0 ) {
                bfsQue.push(childIdx);
            }
        }

        while ( !bfsQue.empty() ) { // 层序遍历，保证高层节点先处理
            auto& parent{trie_m[bfsQue.front()]};
            bfsQue.pop();
            for ( size_t idx{}; idx < 26; ++idx ) { // 循环：为所有子节点设置fail_m。迭代：idx
                size_t& childIdx{parent.children_m[idx]};
                if ( childIdx == 0 ) { // 若不存在，为避免匹配时递归跳转fail_m，提前将该点设置为fail_m跳转的终点
                    childIdx = trie_m[parent.fail_m].children_m[idx];
                } else {               // 如果存在该子节点，则将它的fail_m指向其父节点的fail_m节点的相同子节点
                    trie_m[childIdx].fail_m = trie_m[parent.fail_m].children_m[idx];
                    bfsQue.push(childIdx);
                }
            }
        }
    }

    int operator()(const string& str)
    {
        size_t matchCnt{};
        vector<bool> isOnce(trie_m.size(), true); // 记录一个子串是否已被匹配过了
        for ( size_t trieIdx{}, strIdx{}, strLen{str.size()}; strIdx < strLen; ) { // 匹配字符串的每个位置
            auto& childIdx{trie_m[trieIdx].children_m[str[strIdx] - 'a']};
            if ( childIdx != 0 ) { // 如果当前位置代表的子串匹配，则计算所有其自身与其所有后缀是否为字符串结尾
                for ( size_t needCntIdx{childIdx}; needCntIdx != 0 && trie_m[needCntIdx].isEnd_m && isOnce[needCntIdx]; needCntIdx = trie_m[needCntIdx].fail_m ) {
                    matchCnt += 1;
                    isOnce[needCntIdx] = false; // 该子串可能是多个长串的尾缀，避免重复计算
                }
                trieIdx = childIdx;
                ++strIdx;
            } else if ( trieIdx != 0 ) { // 如果不匹配，则跳转到一个最长公共前后缀
                trieIdx = trie_m[trieIdx].fail_m;
            } else { // 已经到根节点了且未匹配，表示不存在尾缀，此时应该从头开始比较
                ++strIdx;
            }
        }
        return matchCnt;
    }
};
```
</details>


## 计数排序
* 特点：时间效率高，空间效率低
<details>
    <summary><b>Code...</b></summary>

```cpp
template <typename ITER>
vector<int> cnt_sort(ITER begin, ITER end, int minNum, int maxNum)
{
    vector<int> cnt(maxNum - minNum + 1);
    for ( auto pos{begin}; pos != end; ++pos ) {    // 每个元素先记录该桶中的元素个数
        ++cnt[*pos - minNum];
    }
    for ( int idx{1}, end = cnt.size(); idx < end; ++idx ) {    // 记录该通最后一个元素的序数，1开始
        cnt[idx] += cnt[idx - 1];
    }
    vector<int> ret(cnt.back());
    for ( auto pos{--end}; pos != begin; ++pos ) {    // 遍历原数组 
        ret[--cnt[*pos - minNum]] = *pos;
    }
    ret[--cnt[*begin - minNum]] = *begin;
    return ret;
}

```
</details>


# 图论
[Code...](https://github.com/mrbeardad/Practices/tree/master/ACM/BOOK/dsaa:c%2B%2B)
## 拓扑排序有向无圈图
* 数据结构：
Vertex{numr; adj;}
Umap<numr, indegree>
queue<idx>
* 步骤：
    * online算法记录indegree
    * 遍历将indegree为0的入队
    * 层序遍历处理出队元素的邻接点：
将adj的indegree-1，若变为0则入队
## 单源无权路径
* 数据结构：
Vertex{dist;  path; adj;}
queue<idx>
* 步骤：
    * 层级遍历处理出队元素的邻接点：
若adj的dist=-1则更新dist与path，并将其入队
## 单源赋权路径
* 数据结构：
Vertex{isInQueue; dist;  path; adj;}
queue<idx>
* 步骤：
    * 层序遍历处理出队元素的邻接点：
若当前dist比adj的dist更优则更新其dist与path
若更新了其dist且未在队列则更新isInQueue且入队
## 关键路径分析有向无圈图
* 数据结构：
Vertex{early; late; prevAdj; nextAdj}
Umap<numr, Vertex> Graph
* 步骤：
    * 添加初始节点0
    * 将动作节点图转换为事件节点图：
0个prev，连到0节点
1个prev，连到那个节点
多个prev，添加哑节点，哑边，真节点，真边
    * 正向bfs最长路(early)
    * 逆向bfs最短路(late)，此时权为相反数
    * dfs计算松弛时间并用stack存储一路都为0的路
## 网络流最大值有向图
* 数据结构：
Vertex{path; pathFlow; nthVisit; umapGf; umapGr}
* 步骤：
    * selectPath(nth)：对Gr图bfs无权最短路
层序遍历处理出队元素的邻接点：
若nthVisit小于nth且到该邻接点的权不为0，则更新path与pathFlow与nthVisit并将其入队
    * 遍历该路径并更新umapGr(target的nthVisit是否未被更新用于判断是否结束，target的pathFlow代表该增长通路的flow)
    * 计算umapGf(find区分Gf中有无反向流)
    * 注：source节点pathFlow置INT32_MAX
## 无向图最小生成树无向连通图
* 数据结构：
Pqueue<{eageWeight, idx1, idx2}>
DisjointSet
* 步骤：
    * 将权重最小的边依次尝试加入(优先队列)
    * 当且仅当该边两节点未在同一集合中(并查集)
    * 直到所有节点连通(eages==vertexs-1)
## 连通性
* 数据结构：
Vertex{isKnown; adj}
* 步骤：
一次dfs可遍历所有节点则图为连通的
## 无圈性
* 数据结构：
Vertex{isKnown; adj}
(stack<idx>)
* 步骤：
    * 多次dfs递归，更新isKnown
    * 递归前设置stack记录路径，返回前弹出，用于判断非树边(无向图无需)
    * 无背向边则无圈
## 无向图割点无向图
* 数据结构：
Vertex{num; low; adj;}
* 步骤：
    * low(v)=min(num(v), min(low(w)), min(num(b)))
    * dfs递归，设置num与low，返回low
    * 对于根节点，有多个儿子则为割点；对于其他节点，num(v) ≤ low(w)则为割点
    * 注：循环adj时区分树边与非树边
## 欧拉回路无向连通图
* 数据结果：
Vertex{umapAdj}
list<idx> result
* 必要条件：
每个节点的边为偶数的连通无向图
* 步骤：
    * 循环一，换一条回路直到pos2insert到头
    * 循环二，list插入、删除边、反向删除边
## 强分支有向图
* 数据结构：
Vertex{numr; adj; revAdj}
map<numr, idx> maxNumr
vector<vector<idx>> branch
* 步骤：
    * 多次dfs，后序遍历设置numr，插入(numr, idx)对到map，反转边
    * 多次dfs，添加branch，从map删除该节点，每次dfs从numr最大者开始
    * 每个branch为强分支
    * 注：第二种dfs需提前为branch提供空位
# 其它算法

## 哈夫曼编码贪婪
* 数据结构：
Node{char, freq, left, right}
Pqueue<NodePtr>
vector<string>
* 步骤：
合并频数最小的两棵trie树(后合并的深度小)
* 证明：
d1*p1+d2*p2 < d1*p2+d2*p1 (d1<d2, p1>p2)
trie树代表的d*p带入上式仍成立
## 平面最近点分治
* 步骤：
    * 功能：接受两个容器，返回范围内最近点距离
    * 基准：一个点返回INF，两个点返回距离
    * 一般：
为递归准备参数容器，一个x排序，一个y排序
为mid准备容器，限定x范围按y排序
计算mid，注意控制转移
## 最优二叉搜索树DP
* 步骤：
    * 最优子结构：
三个节点的三种排列
    * 状态转移方程：
多个节点DP根节点，计算子范围最优排列
    * 记忆表推进：
由下至上扩大范围进行DP，结果用二叉堆表示
## 所有点对最短路DP
* 步骤：
    * 最优子结构：
最初即邻接边的值
两点最短距离由下次循环的更优者代替
    * 状态转移方程：
每对点DP中间节点，计算子范围距离合，若更优则更新该点对的距离
    * 记忆表推进
二维数组记录并用于更新最优值
## 收费公路重建回溯
* 步骤：
    * 由大到小地选择距离，插入在左或在右
    * 插入节点后删除新增的距离
    * 出错则回溯
## 井字棋博弈回溯
* 步骤：
    * 选择所有可走的位置，计算每个位置胜数
    * 预测对方所有可走的位置
    * 递归博弈直到得出结果(胜，败，平)
    * 记忆表记录选择情况(博弈)
    * 裁剪

## 单词代表
* 数据结构：
array (array) UMmap
* 步骤：
    * 替换版：需要区分不同删除位置的代表
    * 删增版：只需区分单词长度
    * 注：同代表的单词中其自身会出现多次

## 子序列最大值(max为负则0)
* 数据结构：
vector<int>
* 步骤：
    * 若前缀为负则可选择下一个非负前缀进行优化
    * 注：记录maxBeg，maxEnd，maxVal，thisBeg，thisEnd，thisMax，nowSum

## 最大公因数
* 最大公因数(gcd)的性质
    * $\gcd(a, b) = \gcd(b, a)$
    * $\gcd(a, b) = \gcd(a-b, b)\qquad(a ≥ b)$
    * $\gcd(a, b) = \gcd(a\\\% b, b)$
    * $\gcd(a, b, c) = \gcd(gcd(a, b), c)$
* 最小公倍数(lcm)：$lcm(a, b) = a\times b \div gcd(a, b)$
<details>
    <summary><b>Code...</b></summary>

```cpp
int gcd(int m, int n)
{
    while ( n != 0 ) {
        auto tmp = m % n;
        m = n;
        n = tmp;
    }
    return m;
}
```
</details>


## 快速幂运算
* 描述：$a^{23}=a^{0x17}=a^{0b00010111}=a^{0x10}\times a^{0x4}\times a^{0x2}\times a^{0x1}$
<details>
    <summary><b>Code...</b></summary>

```cpp
int quick_pow(int num, int pow) { // 利用指数的二进制分解与指数加法，简化整数的幂运算
    int ret{1};
    while ( pow != 0 ) {
        if ( pow & 0x1 ) { // 如果thisBit为1，则乘以x(现在的x表示当前的指数的x的幂)
            ret *= num;
        }
        num *= num; // 每次循环都更新x，需要时才乘入ret
        pow >>= 1;
    }
    return ret;
}
```
</details>


## 厄拉多塞筛
* 描述：素数的倍数均为非素数，筛掉这些倍数
<details>
    <summary><b>Code...</b></summary>

```cpp
vector<int> prime_sieve(int maxNum) // 筛掉初始值的所有倍数
{
    const int SQRT_N = sqrt(maxNum);
    vector<bool> prime(maxNum + 1, true), ret; // 数组索引标识元素
    for ( size_t num{2}; num <= SQRT_N; ++num ) {
        if ( prime[num] ) { // 跳过已经被排除的元素
            ret.push_back(num); // 半online算法
            for ( size_t needErase{num + num}; needErase <= maxNum; needErase += num ) {
                prime[needErase] = false; // MEM_OPT
            }
        }
    }
    for ( size_t idx{SQRT_N + 1}; idx <= maxNum; ++idx ) {
        if ( prime[idx] ) {
            ret.push_back(idx);
        }
    }
    return ret;
}
```
</details>


## 唯一分解定理
* 描述：每个整数可以被分解为质数的幂的乘积，如$20=2^2\times 3^0\times 5^1$
* 拓展： 从多个数的质数分解式中，选取每个质数的指数的最小值则得到gcd的质数分解式，
    选取最大值则得到lcm的质数分解式
<details>
    <summary><b>Code...</b></summary>

```cpp
vector<int> udt(int num) // 每个正整数都可以分解为唯一的素数的幂的积
{
    vector<int> ret(Primes.size()); // Primes为素数表
    for ( size_t idx{}; num > 0; ++idx ) {
        int thisPrime{Primes[idx]}, pows{};
        auto divd{div(num, thisPrime)};
        while ( divd.rem == 0 ) { // 如果当前的素数为因子，则计算其最大的指数
            ++pows;
            num = divd.quot;
            divd = div(num, thisPrime);
        }
        ret[idx] = pows;
    }
    return ret;
}
```
</details>


