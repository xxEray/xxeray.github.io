---
title: CodeForces 605E Intergalaxy Trips 题解
description: 一道期望好题
tags:
  - 概率
  - 期望
  - 图论
  - 思维
  - 动态规划
categories:
  - 题解
  - CodeForces
mathjax: true
urlname: codeforces-605e
date: 2023-06-27 08:03:29
---

## 题意

> 有一张 $n$ 个点的有向完全图，边 $i \to j$ 有 $p_{i,j}$ 的概率出现（$p_{i,i}=1$）。你要从 $1$ 开始，每天可以走一条出边或留在原地，求最优策略下走到 $n$ 的期望天数。输出小数（不取模）。  
> $n \le 10^3$

## 思路

设 $f(i)$ 表示从 $i$ 走到 $n$ 的期望天数，那么答案就是 $f(1)$。接下来考虑怎么计算 $f$。

首先考虑怎么转移 $f$。  
考虑当前我们在 $i$，下一步要去 $j(1\le j\le n)$（$j$ 可以等于 $i$）。因为要算最优方案，所以当我们选择去 $j$ 的时候，一定满足所有优于 $j$ 的点都没有边。  
现在就可以把方程写出来了：

$$
f(i)=1+\sum_{j=1}^n f(j)p_{i,j}\prod_{1\le k\le n}^{f(k)<f(j)}(1-p_{i,k})
$$

但是这个式子需要依赖已经计算好的 $f$，所以还是没法做。  
我们观察发现，由于 $p_{i,i}=1$，所以对于所有满足 $f(j)>f(i)$ 的 $j$，后面的那个 $\prod_{1\le k\le n}^{f(k)<f(j)}(1-p_{i,k})$ 一定会包含 $k=i$ 的一项，也就是 $1-p_{i,i}=0$。换句话说，这样的 $j$ 一定对 $i$ 没有贡献。  
其实还有另一种解释，因为我们总是可以原地停留的，所以我们不可能从 $f$ 较小的走向 $f$ 较大的，这样明显不如原地停留优秀。  
所以，$f$ 总是会从较小的往较大的转移。也就是说，把原地停留的情况移项过后，**$f$ 的转移就是一个 DAG**。

先把移项过后的式子写出来：

$$
f(i)=\frac{1+\sum_{1\le j\le n}^{f(j)<f(i)} f(j)p_{i,j}\prod_{1\le k\le n}^{f(k)<f(j)}(1-p_{i,k})}{1-\prod_{1\le j\le n}^{f(j)<f(i)}(1-p_{i,j})}
$$

现在考虑怎么找出 DAG 的形态。  
首先 $f(n)=0$ 肯定是最小的 $f$。然后我们用 $f(n)$ 去转移其它的点（也就是作为式子里的 $j$）。这个时候每个点都有一个当前的 $f$（仅考虑 $u \to n$ 的边计算得来的 $f$）了。我们把当前的 $f$ 记为 $f'$。  
考虑这些 $f'$ 中最小的那一个，假设是 $f'(i)$。可以证明，此时 $f(i)=f'(i)$（证明在下面）。所以这时我们就算出了 $f(i)$。然后继续用 $f(i)$ 去转移其它的点，然后再在剩下的点里面找出当前 $f$ 最小的，假设是 $f''(k)$。按照类似的证明我们知道 $f''(k)=f(k)$。以此类推，就可以找出 DAG 形态并算出答案了。总复杂度 $O(n^2)$。

现在我们证明 $f(i)=f'(i)$。（对于其它的也类似）  
首先发现一个性质，对于任意时刻的 $f'$ 来说（不只是最终答案的 $f$），如果用 $f'(j)$ 转移到 $f'(i)$，那么一定满足 $f'(i)\ge f'(j)$。因为我们是从 $i$ 走到 $j$ 的，那 $i$ 的步数肯定要多一步，所以不可能比 $j$ 小。严谨证明就是如果 $'(j) \ge f'(i)$，那么可以继续用 $i$ 去转移到 $j$，这样 $f'(i)$ 和 $f'(j)$ 就会要么越来越小，要么相等。如果相等那么结论得证；如果越来越小那么显然矛盾。  
然后就很简单了，因为 $f'(i)$ 是除 $f(n)$（或者那些已经转移过其它点的点）以外最小的，而比 $f'(i)$ 小的已经转移到 $i$ 过了，所以不可能有其它的点转移到 $i$ 使 $f'(i)$ 更小了。如果让 $f'(i)$ 更大显然是不优的，所以 $f'(i) = f(i)$。

## 代码

代码其实很好写。

{% note no-icon info 代码 %}

```cpp
#include <bits/stdc++.h>

const int N = 1e3 + 5;

int n;
double p[N][N];

double f[N], g[N], h[N];

bool done[N];
void trans(int j, int i) { g[i] += p[i][j] * f[j] * h[i], h[i] *= 1 - p[i][j], f[i] = g[i] / (1 - h[i]); }

int main() {
	scanf("%d", &n);
	for(int i = 1; i <= n; i++) for(int j = 1; j <= n; j++) { int x; scanf("%d", &x); p[i][j] = x / 100.0; }
	for(int i = 1; i <= n; i++) f[i] = g[i] = h[i] = 1;
	done[n] = true, f[n] = 0;
	for(int i = 1; i <= n; i++) if(!done[i]) trans(n, i);
	for(int _ = 2; _ <= n; _++) {
		int now = 0;
		double val = 1e18;
		for(int i = 1; i <= n; i++) if(!done[i] && f[i] < val) now = i, val = f[i];
		done[now] = true;
		for(int i = 1; i <= n; i++) if(!done[i]) trans(now, i);
	}
	printf("%.6lf\n", f[1]);
	return 0;
}
```

{% endnote %}

## 参考资料

{% note no-icon info 参考资料 %}

<https://www.luogu.com.cn/blog/SDNetFriend/solution-cf605e>  
<https://www.luogu.com.cn/blog/LCA/solution-cf605e>  
<https://www.luogu.com.cn/blog/user33243/solution-cf605e>  
<https://codeforces.com/blog/entry/22019>

{% endnote %}
