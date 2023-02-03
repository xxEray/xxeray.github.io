---
title: AtCoder ABC172 F - Unfair Nim 题解
date: 2021-04-03 09:04:50
description: 博弈论的题目一定就是 DP 吗？
tags:
  - 思维
categories:
  - 题解
  - AtCoder
mathjax: true
urlname: atcoder-abc172-f
---

## 题意

题目链接：<https://atcoder.jp/contests/abc172/tasks/abc172_f>

有 $n$ 堆石子，第 $i$ 堆有 $a_i$ 个，两人轮流选择一堆取石子，至少取一个，至多把这堆取完。  
后手开挂了，可以在开局前将第一堆石子中拿若干个石子到第二堆（不能把第一堆拿完），问最少拿多少石子到第二堆才能使后手必胜。
$2 \le n \le 300$, $1 \le a_i \le 10^{12}$

## 思路

### 前置芝士

[Nim 游戏](https://oi-wiki.org/math/game-theory/#nim)  
[Nim 游戏解法](https://oi-wiki.org/math/game-theory/#nim_1)  
[Nim 游戏解法证明](https://oi-wiki.org/math/game-theory/#_3)

### 问题转化

令 $A = a_1$，$B = a_2$，$C = \oplus_{i=3}^n a_i$，  
则原问题转化为求最大的 $A'$，$B'$ 使得 $A' \oplus B' \oplus C = 0$ 且 $A' + B' = A + B$

### 大体确定

令 $D = \frac{A' + B' - C}{2}$，不难发现 $D = A' \operatorname{and} B'$。  
我们可以按位考虑，对于每一位：

- 如果 $A' = B' = 0$，则 $C = 0$，那么 $D = 0 = A' \operatorname{and} B'$
- 如果 $A' = B' = 1$，则 $C = 0$，那么 $D = 1 = A' \operatorname{and} B'$
- 如果 $A'$, $B'$ 一个为 $0$，另一个为 $1$，则 $C = 1$，那么 $D = 0 = A' \operatorname{and} B'$

又因为 $D = \frac{A' + B' - C}{2} = \frac{A + B - C}{2}$，所以我们可以求出 $D$。  
而我们要求 $A'$ 最大，所以 $D = A' \operatorname{and} B'$ 应该都放到 $A'$ 上。

### 细微调整

还是按位考虑。

对于每一位，如果 $C = 1$，那么 $A'$, $B'$ 中必有一个为 $1$，如果将这一位的 $1$ 加到 $A'$ 上满足题意（加上后 $A' < A$）就加到 $A'$ 上。

最后加一些特判（见代码）就完了。

## 代码

``` cpp
// 代码中的 a 是上面的 A'
#include <cstdio>

typedef long long LL;

int main() {
    int n;
    scanf("%d", &n);
    LL A = 0, B = 0, C = 0;
    scanf("%lld%lld", &A, &B);
    for(int i = 3; i <= n; i++) {
        LL x;
        scanf("%lld", &x);
        C ^= x;
    }
    LL D = (A + B - C) >> 1;
    if(((A + B - C) & 1) || D < 0 || D > A || D & C)
        { puts("-1"); return 0; }
    LL a = D;
    for(int i = 60; i >= 0; i--)
        if((C & (1LL << i)) && (a | (1LL << i)) <= A)
            a |= (1LL << i);
    printf("%lld\n", a == 0 ? -1LL : A - a);
    return 0;
}
```
