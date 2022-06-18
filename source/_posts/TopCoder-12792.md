---
title: TopCoder 12792 BitwiseAnd 题解
date: 2022-04-04 22:07:46
description: 比较简单的思维题。
tags:
	- 思维
categories:
	- 题解
	- TopCoder
mathjax: true
urlname: topcoder-12792
---

## 题意

[原题链接](https://community.topcoder.com/stat?c=problem_statement&pm=12792)

给 $m$ 个数 $\{ori_i\}$，让你添加 $n - m$ 个数（设这一共 $n$ 个数为 $\{a_i\}$），使得：

1. $\forall 1 \le i, j \le n,~a_i \operatorname{bitand} a_j \ne 0$
2. $\forall 1 \le i, j, k \le n,~a_i \operatorname{bitand} a_j \operatorname{bitand} a_k = 0$

求字典序最小的 $\{a_i\}$，或者判断无解。

## 做法

先转成二进制，然后条件就等价于对于每一个二进制位， **$\{a_i\}$ 中这一位是 $1$ 的不超过 $2$ 个**。（不满足就无解，我们把 **$a_i$ 中这一位是 $1$** 记为 **这一位在 $a_i$ 中出现**）

那么我们将 $a$ 的前 $m$ 个设为 $ori$，后面先设为 $0$。

枚举 $i, j (1 \le i < j \le n)$（都按从小到大的顺序），然后强制令 $a_i \operatorname{bitand} a_j \ne 0$。具体如下：

- 如果 $a_i \operatorname{bitand} a_j$ 已经不为 $0$，就不考虑下面的了。
- 如果 $i \le m$ 且 $j \le m$，那么无解。（因为都修改不了）
- 如果 $i \le m$，那么 $a_i$ 修改不了，只有从 $a_j$ 下手。如果 $a_i$ 中有二进制位只在 $a_i$ 出现过一次，那么可以让这一位在 $a_j$ 也出现一次。如果有多个这样的二进制位，取最小的。（因为要字典序最小）
- 否则（$i > m, j > m$）分两种情况：
  - 如果 $a_i$ 有二进制位只在出现过一次（显然这一次在 $a_i$ 处），那么让它在 $a_j$ 中出现。多个取最小。（因为要字典序最小）
  - 否则，取以下两种情况中 $a_i$ 较小的：
    - 如果有二进制位只在 $a_j$ 中出现，那么让它在 $a_i$ 中出现。多个取最小。
    - 如果有一个二进制位没有出现过，那么让它在 $a_i$ 和 $b_i$ 中都出现。

{% note no-icon info 看不懂？伪代码版解释。 %}

将只在 $a_i$ 处出现一次的最小的 $k$ 记为 $occur(a_i)$（如果没有就是 $-1$）。  
将没有出现过的最小的 $k$ 记为 $occur_{min}$（如果没有就是 $-1$）。  
$\operatorname{bitand}$ 是二进制下的按位与，$\textbf{and}$ 是逻辑与（就是 **且**）。

$$
\begin{array}{ll}
1  &  \textbf{if } a_i \operatorname{bitand} b_i \ne 0&\\
2  &  \qquad \text{Skip.}&\\
3  &  \textbf{elif } i \le m \textbf{ and } j \le m&\\
4  &  \qquad \text{No Solution.}&\\
5  &  \textbf{elif } i \le m&\\
6  &  \qquad \textbf{if } occur(a_i) = -1&\\
7  &  \qquad \qquad \text{No Solution.}&\\
8  &  \qquad a_j \gets a_j \operatorname{bitor} 2^{occur(a_i)}&\\
9  &  \textbf{else}&\\
10 &  \qquad \textbf{if } occur(a_i) \ne -1&\\
11 &  \qquad \qquad a_j \gets a_j \operatorname{bitor} 2^{occur(a_i)}&\\
12 &  \qquad \textbf{else}&\\
13 &  \qquad \qquad id \gets +\infty&\\
14 &  \qquad \qquad \textbf{if } occur(a_j) \ne -1&\\
15 &  \qquad \qquad \qquad id \gets \min\left\{id, occur(a_j)\right\}&\\
16 &  \qquad \qquad \textbf{if } occur_{min} \ne -1&\\
17 &  \qquad \qquad \qquad id \gets \min\left\{id, occur_{min}\right\}&\\
18 &  \qquad \qquad \textbf{if } id = +\infty&\\
19 &  \qquad \qquad \qquad \text{No Solution.}&\\
20 &  \qquad \qquad a_i \gets a_i \operatorname{bitor} 2^{id}&\\
21 &  \qquad \qquad a_j \gets a_j \operatorname{bitor} 2^{id}&\\
\end{array}
$$

{% endnote %}

证明不难（贪心），~~当作业~~。

## 代码

{% note no-icon info 代码 %}
```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using std::vector;
typedef long long LL;
const int N = 50 + 5;
int edge[N][N];
int used[70];
vector<int> to[N];
int nw_id() {
	for(int i = 0; i < 60; i++) if(!used[i]) return i;
	return -1;
}
LL ans[N];
class BitwiseAnd {
public:
	vector<LL> lexSmallest(vector<LL> st, int n) {
		int m = st.size();
		for(int i = 1; i <= m; i++) ans[i] = st[i - 1];
		for(int i = 1; i <= m; i++)
			for(int j = 0; j < 60; j++) if(st[i - 1] >> j & 1) {
				if(!used[j]) used[j] = i;
				else if(used[j] != -1) edge[i][used[j]] = edge[used[j]][i] = true, used[j] = -1;
				else return {};
			}
		for(int i = 59; i >= 0; i--) if(used[i] > 0) to[used[i]].push_back(i);
		for(int i = 1; i <= n; i++)
			for(int j = i + 1; j <= n; j++) if(!edge[i][j]) {
				if(i <= m && j <= m) return {};
				else if(i <= m) {
					if(to[i].empty()) return {};
					int id = to[i].back();
					to[i].pop_back();
					ans[j] |= 1LL << id;
					used[id] = -1;
				} else {
					int id;
					if(!to[i].empty()) id = to[i].back(), to[i].pop_back();
					else {
						id = 100;
						if(to[i].empty() && to[j].empty()) id = std::min(id, nw_id());
						if(!to[j].empty()) id = std::min(id, to[j].back()), to[j].pop_back();
					}
					if(id == -1 || id == 100) return {};
					ans[i] |= 1LL << id, ans[j] |= 1LL << id;
					used[id] = -1;
				}
			}
		auto v = vector<LL>(ans + 1, ans + n + 1);
		std::sort(v.begin(), v.end());
		return v;
	}
};
```
{% endnote %}

