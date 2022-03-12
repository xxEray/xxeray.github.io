---
title: AtCoder ABC177 F - I hate Shortest Path Problem 题解
description: 一道看似是迷宫的题目，但实际却不像看起来那么简单。
tags:
  - 数据结构
  - 动态规划
categories:
  - 信息技术
  - 题解
  - AtCoder
mathjax: true
urlname: atcoder-abc177-f
date: 2021-02-10 21:04:26
---

## 题意

题目链接：<https://atcoder.jp/contests/abc177/tasks/abc177_f>

一个 $(H + 1) \times W$ 的网格，每个位置都可以往右走一格（最右边一列不行），第 $i$ 行除了第 $A_i$ 到第 $B_i$ 个格子以外都可以往下走。  
你可以从第 $1$ 行任意一个格子开始走，对于每一个 $i \in [2, n + 1]$，求走到第 $i$ 行任意一个格子的最小步数。  
其中 $H, W \le 2 \times 10^5$，$1 \le A_i \le B_i \le W$

## 思路

### 贪心思想

容易发现，除了第 $1$ 行，每个格子肯定是尽量往下走，即只有当不能往下走时才往右走。  
那么初步想法是进行 DP。

### DP

下面用 $(i, j)$ 表示第 $i$ 行第 $j$ 列的格子。

$f(i, j)$ 表示从第一行任意一个格子走到 $(i, j)$ 时的最小步数。
那么对于 $f(i, j)$ 有以下三种情况：

1. 如果上面有挡板，则 $f(i, j) = \inf$，可以证明这样对答案没有影响。
2. 如果上面和左上（即 $(i - 1, j - 1)$）都没有挡板，则 $f(i, j) = f(i - 1, j) + 1$
3. 如果上面没有挡板，左上有挡板，则 $f(i, j) = \min_{k = 1}^j f(i - 1, k) + 1$

其中从左边往右边走的贡献全部集中在了第三种转移。

上面的转移即：

$$
f(i, j) =
\begin{cases}
    0&(i=1)\\
    \inf&(A_{i - 1} \le j \le B_{i - 1})\\
    f(i - 1, j) + 1&(B_{i - 1} < j - 1 \;\texttt{or}\; A_{i - 1} > j)\\
    \min_{k = 1}^j f(i - 1, k) + 1&(B_{i - 1} = j - 1)
\end{cases}
$$

### 数据结构优化

但是上面 DP 的复杂度是 $O(HW)$ 的，在这道题肯定是过不去的。这时候我们可以用线段树优化。

我们对于每个 $f(i, \cdots)$ 用线段树维护，直接 $O(\log H)$ 进行维护。（类似背包的思想，直接在原线段树上进行更改）

对于第一种转移，区间赋值/区间加 $\inf$。  
对于第二种转移，区间加 $1$。  
对于第三种转移，需要再维护一个线段树，值为 $f(i, j) + W - j$。此时第三种转移就用第二个线段树的值来维护。

## 代码

``` cpp
#include <cstdio>
#include <algorithm>

using namespace std;

const int N = 2e5 + 5;
const int INF = 0x3f3f3f3f;

typedef long long LL;

int a[N], b[N];
int n, m;

struct SegTree {
    LL t[N << 2], ladd[N << 2];
    inline void lazy_down(int x) {
        t[x << 1] += ladd[x];
        t[x << 1 | 1] += ladd[x];
        ladd[x << 1] += ladd[x];
        ladd[x << 1 | 1] += ladd[x];
        ladd[x] = 0;
    }
    void modify_add(int ql, int qr, LL qadd, int x = 1, int l = 1, int r = m) {
        if(ql <= l && r <= qr) { t[x] += qadd, ladd[x] += qadd; return; }
        int mid = l + ((r - l) >> 1);
        lazy_down(x);
        if(ql <= mid) modify_add(ql, qr, qadd, x << 1, l, mid);
        if(qr > mid) modify_add(ql, qr, qadd, x << 1 | 1, mid + 1, r);
        t[x] = min(t[x << 1], t[x << 1 | 1]);
    }
    LL query_min(int ql, int qr, int x = 1, int l = 1, int r = m) {
        if(ql <= l && r <= qr) return t[x];
        int mid = l + ((r - l) >> 1);
        lazy_down(x);
        if(qr <= mid) return query_min(ql, qr, x << 1, l, mid);
        else if(ql > mid) return query_min(ql, qr, x << 1 | 1, mid + 1, r);
        else return min(query_min(ql, qr, x << 1, l, mid), query_min(ql, qr, x << 1 | 1, mid + 1, r));
    }
};

SegTree ans1; // f[][i]
SegTree ans2; // f[][i] + m - i

int main() {
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; i++) scanf("%d%d", &a[i], &b[i]);
    for(int i = 1; i <= m; i++) ans2.modify_add(i, i, m - i);
    for(int i = 1; i <= n; i++) {
        LL tmp = ans2.query_min(1, b[i] + 1) - ((LL)m - b[i] - 1);
        ans1.modify_add(b[i] + 1, b[i] + 1, min(tmp - ans1.query_min(b[i] + 1, b[i] + 1), 0LL));
        ans2.modify_add(b[i] + 1, b[i] + 1, min(tmp + m - (b[i] + 1) - ans2.query_min(b[i] + 1, b[i] + 1), 0LL));
        ans1.modify_add(a[i], b[i], INF);
        ans2.modify_add(a[i], b[i], INF);
        ans1.modify_add(1, m, 1);
        ans2.modify_add(1, m, 1);
        tmp = ans1.query_min(1, m);
        if(tmp >= INF) printf("-1\n");
        else printf("%lld\n", tmp);
    }
    return 0;
}
```
