---
title: TopCoder 12875 PalindromeMatrix 题解
description: 一道中等难度的思维题，想通了就很简单。
tags:
  - 思维
  - 枚举
categories:
  - 题解
  - TopCoder
mathjax: true
urlname: topcoder-12875
date: 2022-04-04 16:12:09
---


{% note no-icon info %}
[本题链接](https://community.topcoder.com/stat?c=problem_statement&pm=12875)  
本题与 [TopCoder 12876 PalindromeMatrixDiv2](https://community.topcoder.com/stat?c=problem_statement&pm=12876) 是同一道题。
{% endnote %}

## 题意

给一个 $n \times m$ 的 $01$ 矩阵，求至少将多少个格子取反才能使得至少有 $r$ 行和 $c$ 列是回文串。  
$2 \le n, m \le 14$，$0 \le r \le n$，$0 \le c \le m$

## 做法

首先枚举有哪些行是回文串，然后考虑对列进行 DP。

容易发现一列一列地转移是不好转移的（因为你在考虑前面的时候无法知道后面填什么），所以考虑两列两列地转移。

设在当前枚举的条件下（有一些行是回文的情况下），

- $cost(i, j)$ 表示 **第 $i$ 列和第 $m - i + 1$ 列中有 $j$ 列是回文的最小代价**（其中 $0 \le j \le 2$）
- $f(i, j)$ 表示 **前 $i$ 列和后 $i$ 列一共有 $j$ 列是回文的最小代价**。

那么 $f(i, j) = \min_{0 \le k \le 2} \left\{f(i - 1, j - k) + cost(i, k)\right\}$。现在的问题是怎么算 $cost$。

容易发现 $s$ 是否回文只与 **$s_i$ 是否等于 $s_{n - i + 1}$** 有关，所以在一个串中我们只用考虑 $\frac n2$ 组对应字符内部的相等关系，而组与组之间是相互独立的。类似的，在一个矩阵中我们也只需要考虑 $\frac{nm}{4}$ 组内部的关系。

所以我们枚举 $i, j (1 \le i \le \frac n2, 1 \le j \le \frac m2)$，然后枚举 $(i, j)$ 这一组里 $4$ 个格子每一个的状态，然后维护到对应的 $cost$ 里就好了。

## 细节

在算 $cost$ 时，由于 **左边是回文** 和 **右边是回文** 都是 $cost(i, 1)$，所以 **可能** 会出问题（看具体写法），可以考虑将其分开，状态变成 $cost(i, 0/1/2/3)$

另外，在维护到 $cost$ 时，有一些细节，不过样例能查出来，留给读者自己调试。

## 代码

{% note no-icon info 代码 %}
```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
using std::string;
using std::vector;
const int N = 14 + 5;
const int INF = 0x3f3f3f3f;
int a[N][N];
int n, m;
int f[N][N], cost[N][5];
void chkmin(int &x, int y) { if(y < x) x = y; }
class PalindromeMatrix {
public:
	int minChange(vector<string> A, int rowCount, int columnCount) {
		n = A.size(), m = A[0].size();
		for(int i = 1; i <= n; i++) for(int j = 1; j <= m; j++) a[i][j] = A[i - 1][j - 1] - '0';
		int U = (1 << n) - 1;
		int ans = INF;
		for(int s = 0; s <= U; s++) {
			if(__builtin_popcount(s) < rowCount) continue;
			memset(cost, 0, sizeof(cost));
			memset(f, 0x3f, sizeof(f));
			for(int i = 1; i <= n / 2; i++)
				for(int j = 1; j <= m / 2; j++) {
					int i_ = n - i + 1, j_ = m - j + 1;
					int ri = s >> (i - 1) & 1, ri_ = s >> (i_ - 1) & 1;
					int tmp[5] = {INF, INF, INF, INF};
					for(int t = 0; t < 16; t++) {
						int x = t & 1, y = t >> 1 & 1, z = t >> 2 & 1, w = t >> 3 & 1;
						if(ri && (x != y)) continue;
						if(ri_ && (z != w)) continue;
						chkmin(tmp[(x == z) | ((y == w) << 1)], (x != a[i][j]) + (y != a[i][j_]) + (z != a[i_][j]) + (w != a[i_][j_]));
						chkmin(tmp[((y == w) << 1)], (x != a[i][j]) + (y != a[i][j_]) + (z != a[i_][j]) + (w != a[i_][j_]));
						chkmin(tmp[(x == z)], (x != a[i][j]) + (y != a[i][j_]) + (z != a[i_][j]) + (w != a[i_][j_]));
						chkmin(tmp[0], (x != a[i][j]) + (y != a[i][j_]) + (z != a[i_][j]) + (w != a[i_][j_]));
					}
					cost[j][0] += tmp[0], cost[j][1] += tmp[1], cost[j][2] += tmp[2], cost[j][3] += tmp[3];
					chkmin(cost[j][0], INF), chkmin(cost[j][1], INF), chkmin(cost[j][2], INF), chkmin(cost[j][3], INF);
				}
			f[0][0] = 0;
			for(int i = 1; i <= m / 2; i++) for(int k = 0; k <= 3; k++)
				for(int j = __builtin_popcount(k); j <= m; j++) chkmin(f[i][j], f[i - 1][j - __builtin_popcount(k)] + cost[i][k]);
			for(int i = columnCount; i <= m; i++) chkmin(ans, f[m / 2][i]);
		}
		return ans;
	}
};
```
{% endnote %}

