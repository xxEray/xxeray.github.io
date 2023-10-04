---
title: CodeForces 814E An unavoidable detour for home 题解
description: 即巧妙又麻烦的组合计数。
tags:
  - 数学
  - 组合
categories:
  - 题解
  - CodeForces
mathjax: true
urlname: codeforces-814e
date: 2023-10-04 20:19:29
---


## 题意

[题目链接](https://codeforces.com/problemset/problem/814/E)

（洛谷翻译）

- 给出 $n$ 个点，和每个点的度 $d_i$ 让你构造出一张无向图满足以下两条性质：
   - 点 $1$ 到点 $i$ 仅有 **唯一** 一条最短路。
   - 点 $1$ 到点 $i$ 的最短路长度大于等于点 $1$ 到点 $i-1$ 的最短路长度。
- 求能构成满足条件的无向图的个数？
- $n \leq 50$，$2 \leq d_i \leq 3$。
- 这篇题解提供了 $n \le 1000$ 的做法（时间复杂度 $O(n^3)$，空间复杂度 $O(n^2)$）。

## 思路

设 $d_i$ 表示 $1$ 到 $i$ 的最短路，那么根据题意 $d_i$ 一定是单调不降的。那么我们容易想到分层图，显然同一层的点的编号一定是连续的。因为这个是最短路，所以每个点（除了 $1$）一定会有一条边连向上一层，并且不可能有连向两层以前的边。又因为最短路唯一，所以一定是恰好一条边连向上一层。然后发现同层之间也可以任意连边。

因为 $2\sim n$ 的点一定要向前面连一条边，所以我们不考虑它，也就是讲 $2\sim n$ 的度数减去一。现在我们认为所有点的度数为 $[1, 2]$（先不考虑 $1$，最后再说它）。

然后容易想到 DP。设 $f(i, j)$ 表示考虑了点 $1 \sim i$，$i$ 刚好是一层的结束，第 $i$ 层有 $j$ 条边连向下一层的方案数。那么转移显然是 $f(i + j, k) \leftarrow f(i, j) + g(c_1, c_2, k)$。其中 $c_1,c_2$ 分别表示 $(i, i + j]$ 中度数为 $1,2$ 的点的个数；$g(c_1, c_2, k)$ 表示分别有 $c_1,c_2$ 个度数为 $1, 2$ 的点，并且在内部连边之后剩下了 $k$ 条边（连到下一层）。

现在的问题是 $g(c_1, c_2, k)$ 怎么转移。显然我们有一个空间为 $O(n^3)$ 的做法，就是考虑某一个点的连边方案来转移，这里不详细展开了。但是我们想要一个空间为 $O(n^2)$ 的做法，这迫使我们抛弃 DP 的想法，使用组合方式计算。

首先考虑 $c_2=k=0$，也就是 $c_1$ 个点两两配对的方案数。设其为 $u(i)$（$i$ 为偶数），那么显然 $u(i) = u(i - 2) \cdot (i - 1)$，容易得到 $u(i) = (i - 1)\times (i - 3)\times \cdots \times 3\times 1 = (i - 1)!!$。

然后考虑加上 $c_2$。首先有一个想法是把一个度数为 $2$ 的点拆成两个，记为 $i_1, i_2$，并记 $i \leftrightarrow j$ 表示 $i$ 和 $j$ 连边。但是这样会有问题三个问题：

- $i_1 \leftrightarrow i_2$，下面称为自环；
- $i_1 \leftrightarrow j_1,i_2\leftrightarrow j_2$（或者 $i_1\leftrightarrow j_2,i_2\leftrightarrow j_1$），以下称为重边；
- 我们拆出来的两个点实际上是同一个，所以 $i_1\leftrightarrow j,i_2\leftrightarrow k$ 和 $i_1\leftrightarrow k,i_2\leftrightarrow j$ 是一样的，所以要乘 $\frac 12$，但是我们并不知道要乘多少 $\frac 12$。

首先考虑解决第三个问题。假设分别和 $i_1, i_2$ 连边的点为 $j,k$。我们发现一共会有三种情况：

- $j,k$ 都和 $i$ 在同一层；
- $j$ 和 $i$ 在同一层，$k$ 连到下一层（或者对称的，$k$ 和 $i$ 在同一层，$j$ 连到下一层）；
- $j,k$ 都连到下一层。

我们发现不管是哪种情况，我们会重复计算 $i_1\leftrightarrow j,i_2\leftrightarrow k$ 和 $i_1\leftrightarrow k,i_2\leftrightarrow j$，所以都需要乘上 $\frac 12$。所以其实我们只需要整体乘上 $\left(\frac 12\right)^{c_2}$ 而不需要管具体哪些是同层的。

然后解决前两个问题。我们发现这个可以容斥解决。我们枚举 $r$ 表示钦定 **自环和重边加起来一共有 $r$ 个**，其中自环是一个点拆出来的一对点算一个，重边是两个点拆出来的两对点算一个。然后再枚举 $i\le r$ 表示重边的个数，然后令 $j=r-i$ 表示自环的个数。那么此时我们一共钦定的点数为 $2i+j=r+i$（未拆点之前），拆点之后未被钦定的点数为 $p=c_1+2(c_2-(2i+j))$。那么式子应该是：

$$
g(c_1, c_2, 0) = \frac{1}{2^{c_2}}\sum_{r=0}^{c_2}(-1)^r\sum_{\substack{i+j=r\\p=c_1+2(c_2-(2i+j))}}\binom{c_2}{2i+j}\binom{2i+j}{j}(p-1)!!(2i-1)!!2^i
$$

然后我们加上 $k$。这个很简单，就是先选 $k$ 个点出来再套用刚才的式子。注意要乘上一个 $k!$。式子如下：

$$
g(c_1, c_2, k) = \frac{k!}{2^{c_2}}\sum_{r=0}^{c_2}(-1)^r\sum_{\substack{i+j=r\\p=c_1+2(c_2-(2i+j))}}\binom{c_2}{2i+j}\binom{p}{k}\binom{2i+j}{j}(p-k-1)!!(2i-1)!!2^i
$$

考虑把这个式子化的好看一点。我们交换枚举顺序，先枚举 $x=2i+j=r+i$，再枚举 $i\le\lfloor\frac x2 \rfloor$，那么 $p=c_1+2(c_2-x)$。式子如下：

$$
g(c_1, c_2, k) = \frac{k!}{2^{c_2}}\sum_{x=0}^{c_2}\binom{c_2}{x}\binom{p}{k}(p-k-1)!!\boxed{\sum_{\substack{0\le i\le\lfloor x/2 \rfloor\\j=x-2i}}(-1)^{i+j}\binom{x}{j}(2i-1)!!2^i}
$$

我们发现框起来的部分只和 $x$ 有关，我们可以预处理它，并记为 $h(x)$。

然后现在我们把这个式子代回 $f$ 的转移：

$$
f(i+j, k)\leftarrow f(i,j)\frac{k!}{2^{c_2}}\sum_{x=0}^{c_2}\binom{p}{k}(p-k-1)!!\binom{c_2}{x}h(x)
$$

化一下可以得：

$$
f(i+j,k)\leftarrow \boxed{k!\binom{p}{k}(p-k-1)!!}f(i,j)\sum_{x=0}^{c_2}\binom{c_2}{x}h(x)
$$

容易发现框起来的部分只和 $p,k,x$ 相关，而剩下部分都和 $p,k$ 无关。所以我们可以分步转移：

$$
g(i,p=c_1+2(c_2-x))\leftarrow f(i,j)\sum_{x=0}^{c_2}\binom{c_2}{x}h(x)
$$

$$
f(i,k)\leftarrow k!\binom{p}{k}(p-k-1)!!g(i, p)
$$

然后就做完了。时间复杂度 $O(n^3)$，空间复杂度 $O(n^2)$。

## 代码

{% note no-icon info 代码 %}

```cpp
#include <bits/stdc++.h>

typedef long long LL;

const int N = 1000 + 5;
const LL MOD = 1e9 + 7;

int n;
int a[N];

LL fac[2 * N], invfac[2 * N];
LL qpow(LL x, LL y) { LL ret = 1; while(true) { if(y & 1) ret = ret * x % MOD; if(!(y >>= 1)) return ret; x = x * x % MOD; } }
LL inv(LL x) { return qpow(x, MOD - 2); }
LL C(int x, int y) { return fac[x] * invfac[y] % MOD * invfac[x - y] % MOD; }
void get_factorial(int mx) {
	fac[0] = 1;
	for(int i = 1; i <= mx; i++) fac[i] = fac[i - 1] * i % MOD;
	invfac[mx] = inv(fac[mx]);
	for(int i = mx; i >= 1; i--) invfac[i - 1] = invfac[i] * i % MOD;
}

LL match[2 * N], f[N][N], g[N][2 * N], h[2 * N];
LL powerinv2[N], power2[N];
int pre[3][N];

int main() {
	scanf("%d", &n);
	for(int i = 1; i <= n; i++) scanf("%d", &a[i]);
	for(int i = 2; i <= n; i++) a[i]--;
	for(int i = 1; i <= n; i++) pre[1][i] = pre[1][i - 1] + (a[i] == 1), pre[2][i] = pre[2][i - 1] + (a[i] == 2);
	get_factorial(2 * n + 1);
	powerinv2[0] = 1, power2[0] = 1;
	for(int i = 1; i <= n; i++) powerinv2[i] = powerinv2[i - 1] * inv(2) % MOD, power2[i] = power2[i - 1] * 2 % MOD;
	match[0] = 1;
	for(int i = 2; i <= 2 * n + 2; i += 2) match[i] = match[i - 2] * (i - 1) % MOD;
	for(int x = 0; x <= 2 * n + 1; x++)
		for(int i = 0; i <= x / 2; i++)
			(h[x] += (((x - i) & 1) ? MOD - 1 : 1) * C(x, 2 * i) % MOD * match[2 * i] % MOD * power2[i]) %= MOD;
	// for(int i = 0; i <= 2 * n + 1; i++) printf("h[%d] = %lld\n", i, h[i]);
	f[1][a[1]] = 1;
	for(int i = 1; i <= n; i++) {
		for(int p = 0; p <= 2 * i + 1; p++) if(g[i][p])
			for(int k = (p & 1); k <= std::min(p, n - i); k++)
				(f[i][k] += g[i][p] * C(p, k) % MOD * match[p - k] % MOD * fac[k]) %= MOD;
		for(int j = 1; j <= n - i; j++) if(f[i][j]) {
			int c1 = pre[1][i + j] - pre[1][i], c2 = pre[2][i + j] - pre[2][i];
			// printf("i = %d, j = %d, c1 = %d, c2 = %d\n", i, j, c1, c2);
			for(int x = 0; x <= c2; x++) {
				int p = c1 + 2 * (c2 - x);
				// printf("    x = %d, p = %d\n", x, p);
				(g[i + j][p] += f[i][j] * powerinv2[c2] % MOD * C(c2, x) % MOD * h[x]) %= MOD;
			}
		}
	}
	// for(int i = 1; i <= n; i++)  {
	// 	for(int j = 0; j <= 2 * n + 1; j++) if(g[i][j]) printf("g[%d][%d] = %lld\n", i, j, g[i][j]);
	// 	for(int j = 0; j <= n - i; j++) if(f[i][j]) printf("f[%d][%d] = %lld\n", i, j, f[i][j]);
	// }
	printf("%lld\n", f[n][0]);
	return 0;
}
```

{% endnote %}

