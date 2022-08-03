---
title: TopCoder 1684 Posters 题解
date: 2022-08-03 22:38:04
description: 大力爆搜题，但是有很多优化和剪枝技巧。
tags:
	- 枚举
	- 搜索
	- 剪枝
categories:
	- 题解
	- TopCoder
mathjax: true
urlname: topcoder-1684
---

## 前言

[原题链接](https://community.topcoder.com/stat?c=problem_statement&pm=1684)

网上关于这道题的题解不多（我只找到了[这篇](https://www.cnblogs.com/yzxverygood/p/9360251.html)，而且好像都被下面这组数据卡掉了），而且目前 VJudge 上很少有小于 3000ms 的代码，这是我写这篇题解的原因。

我这份代码暂时还 **没被任何数据卡掉**，而且目前是 VJudge 上的 **最优解**（676 ms，链接在[这里](https://vjudge.net/solution/37566473)）

{% note no-icon info 数据 %}

```plain
8,18,
{2,2,5,4,1},
{2,17,12,15,6}
```

答案是 `136`，给出一组构造方案：

```plain
AACCCCC.
AACCCCC.
AACCCCC.
AACCDDDD
AACCDDDD
AACCDDDD
AACCDDDD
AACCDDDD
AACCDDDD
AACCDDDD
AACCDDDD
AACCDDDD
AA.EDDDD
AA.EDDDD
AA.EDDDD
AA.EDDDD
ABBEDDDD
.BBEDDDD
```

{% endnote %}


## 题意

给一个 $n \times m$ 的网格，和 $K$ 个矩形，每个矩形大小 $r_i \times c_i$，要把这 $K$ 个矩形放到网格上，不允许出格，允许重叠，求覆盖面积最大值。

$n, m \le 100, K \le 5$

## 思路

$K$ 只有 $5$，一看就是爆搜。直接搜肯定是过不去的，我们要考虑剪枝。

为了后面方便，我们先枚举 $K$ 的排列，表示放矩形的顺序。

首先发现一个贪心的思路，即第一个矩形一定是放在一个角上的。由于矩形具有对称性，所以我们不妨 **钦定** 它在左上角。

那第二个矩形呢？接着我们的贪心思路，它一定在右下角。（~~我不会证，所以不证~~）

然后第三个矩形是不是也会放在一个角上呢？答案是不一定。这个反例可以很简单地举出。

{% note no-icon info 反例 %}

```plain
2,3,
{2,2,2},
{1,1,1}
```

最优方法显然是

```plain
ABC
ABC
```

{% endnote %}

所以我们可以先放前两个矩形。

然后现在考虑 dfs。假如我们要放置一个新的矩形，显然它肯定是贴着某条边的（可能是矩形的边，也可能是边界）。所以我们可以枚举这条边，这样就可以把 $100 \times 100$ 的枚举量优化到 $5 \times 5$。

还有一个显然的优化是可行性剪枝。如果当前答案加上后面所有矩形的面积之和都小于最优答案，那么直接返回。但是问题又来了：当前答案怎么维护呢？

如果开一个 $100 \times 100$ 的数组，那么我们的枚举量就又上去了。

我们考虑容斥。用之前的答案，加上当前矩形面积，减去当前矩形和之前一个矩形的面积交，加上当前矩形和之前两个矩形面积交……就可以得到现在的答案了。

然后还有一个对称性剪枝。容易发现前两个矩形的顺序其实不重要，所以我们可以只处理排列中第一个数小于第二个数的情况。

最后，由于我们需要先填两个矩形，所以 $K \le 1$ 的情况要特判。

然后就可以得到一份 ~~吊打这篇题解~~ 跑得极快的代码啦！

## 代码

{% note no-icon info 代码 %}

```cpp
// TopCoder 1684 Posters
// 676 ms  https://vjudge.net/solution/37566473
#include <cstdio>
#include <algorithm>
#include <vector>
using std::vector;

const int N = 5 + 2;
const int M = 100 + 5;

int n, m, K;
struct Node { int r, c; } a[M], tmp[M]; // tmp 输入矩形，a 按照排列重排后的矩形

// up bound, down bound, left bound, right bound（原谅我英语不好）
// 分别记录第 i 个矩形的上下左右边界（i=0 时记录网格边框）
int ub[N], db[N], lb[N], rb[N];

int p[N], sum[N]; // p 排列，sum 面积后缀和

int ans = 0; // 答案
void dfs(int id, int ret) {
	if(id == K + 1) { ans = std::max(ans, ret); return; } // 更新答案
	if(ret + sum[id] <= ans) return; // 可行性剪枝
	for(int i = 0; i < id; i++) for(int ki = 0; ki <= 1; ki++) { // ki=0 贴着矩形 i 的下面；ki=1 贴着矩形 i 的上面
		// 计算 id 的上下边界
		if(ki == 0) ub[id] = db[i] + 1;
		else ub[id] = ub[i] - a[id].r;
		db[id] = ub[id] + a[id].r - 1;
		if(ub[id] < 1 || db[id] > n) continue;
		for(int j = 0; j < id; j++)  for(int kj = 0; kj <= 1; kj++) { // kj=0 贴着矩形 i 的右边；kj=1 贴着矩形 i 的左边
			// 计算 id 的左右边界
			if(kj == 0) lb[id] = rb[j] + 1;
			else lb[id] = lb[j] - a[id].c;
			rb[id] = lb[id] + a[id].c - 1;
			if(lb[id] < 1 || rb[id] > m) continue;
			// 容斥原理计算答案
			int oldret = ret, U = (1 << (id - 1)) - 1; // U 全集
			for(int s = 0; s <= U; s++) {
				int l = lb[id], r = rb[id], u = ub[id], d = db[id];
				for(int o = 1; o < id; o++) if(s >> (o - 1) & 1)
					l = std::max(l, lb[o]), r = std::min(r, rb[o]), u = std::max(u, ub[o]), d = std::min(d, db[o]); // 求交集的上下左右边界
				ret += (__builtin_popcount(s) & 1 ? -1 : 1) * std::max(r - l + 1, 0) * std::max(d - u + 1, 0);
			}
			// 递归搜索
			dfs(id + 1, ret);
			ret = oldret;
		}
	}
}

class Posters {
public:
	int maxCover(int width, int height, vector<int> pWidth, vector<int> pHeight) {
		n = height, m = width, K = pWidth.size();
		for(int i = 1; i <= K; i++) tmp[i].r = pHeight[i - 1], tmp[i].c = pWidth[i - 1];
		// 特判 K<=1
		if(K == 0) return 0;
		if(K == 1) return tmp[1].r * tmp[1].c;
		// 特殊处理网格边框，这里的初值与上面「计算 id 上下/左右边界」部分对应
		ub[0] = n + 1, db[0] = 0, lb[0] = m + 1, rb[0] = 0;
		// 枚举排列
		for(int i = 1; i <= K; i++) p[i] = i;
		do {
			if(p[1] > p[2]) continue; // 对称性剪枝
			for(int i = 1; i <= K; i++) a[i] = tmp[p[i]]; // 处理重排后的矩形
			for(int i = K; i >= 1; i--) sum[i] = sum[i + 1] + a[i].r * a[i].c; // 计算面积后缀和
			ub[1] = 1, db[1] = a[1].r, lb[1] = 1, rb[1] = a[1].c; // 放置第一个矩形
			ub[2] = n - a[2].r + 1, db[2] = n, lb[2] = m - a[2].c + 1, rb[2] = m; // 放置第二个矩形
			dfs(3, a[1].r * a[1].c + a[2].r * a[2].c -  std::max(db[1] - ub[2] + 1, 0) * std::max(rb[1] - lb[2] + 1, 0)); // 计算前两个矩形覆盖的面积，并递归
		} while(std::next_permutation(p + 1, p + K + 1));
		return ans; // 完结撒花~
	}
};
```

{% endnote %}

