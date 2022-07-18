---
title: 洛谷 P3953 [NOIP2017 提高组] 逛公园 题解
date: 2022-07-18 11:25:45
description: 做法复杂且容易假的图论题。
tags:
	- 图论
	- 最短路
	- 动态规划
	- 拓扑排序
categories:
	- 题解
	- 洛谷
mathjax: true
urlname: luogu-3953
---

以下都是这道题：

[洛谷 P3953](https://www.luogu.com.cn/problem/P3953)  
[LOJ 2316](https://loj.ac/p/2316)  
[UOJ 331](https://uoj.ac/problem/331)

## 题意

给一张 $n$ 个点 $m$ 条边的带权有向图，求从 $1$ 到 $n$ 的长度不超过路 $dist(1, n)+K$ 的路径条数，对 $P$ 取模（如果有无数条输出 $-1$）。不保证 $P$ 是质数（存疑，不过不影响）。

$n \le 10^5, m \le 2 \times 10^5, K \le 50, P \le 10^9, 0 \le w \le 1000$。

## 思路

首先考虑边权大于 $0$ 怎么做。

容易想到设 $f(i, j)$ 表示从 $1$ 到 $i$，路径长度比 $dist(1, i)$ 大 $j$ 的方案数。

转移就是：

$$
f(v, j) = \sum_{(u, v, w) \in E} f(u, dist(1, v) + j - w - dist(1, v))
$$

这里需要注意先枚举 $j$ 再枚举 $v$。

然后考虑加上边权为 $0$ 的边（下面称作 $0$ 边）。

首先需要判无数解的情况，即出现边权和为 $0$ 的环（下称 $0$ 环）。

当然，并不是所有 $0$ 环都能造成无数解。怎么判断呢？

对于一个点 $u$，如果 $dist(1, u) + dist(u, n) \le dist(1, n) + K$，那么我们把点 $u$ 叫做 **可行点**。为什么呢？如果一个点不满足这个条件，那么它一定不可能出现在答案中，所以我们直接抛弃掉这种点。

那么我们建一张只包含所有可行点、每条边都是从 $1$ 到 $i$ 的最短路径上的边的子图，显然它是一个 DAG。

那么我们有一些结论：（~~读者自证不难~~）：

1. 显然所有的 $0$ 边都在 DAG 上
2. 所有在 DAG 上的环都是 $0$ 环。
3. 所有 **能造成无数解的 $0$ 环** 都在 DAG 上
4. 所有在 DAG 上的 $0$ 环都能造成无数解。

所以判 $0$ 环只需要在 DAG 上跑一遍拓扑排序就好啦！

但是还有一个问题。刚刚我们 DP 的时候，边权是大于 $0$ 的。这时候边是可以任意顺序枚举的。但是现在有 $0$ 边了，我们就要保证一定的顺序。

由于所有 $0$ 边都的 DAG 上，所以我们可以先把在 DAG 上的边按拓扑序更新一遍，然后剩下的边边权都大于 $0$，就可以任意顺序枚举啦！

总结一下，我们需要：

1. 建正图和反图
2. 对于 $1$ 在正图跑最短路，对于 $n$ 在反图跑最短路
3. 建 DAG，跑拓扑，判 $0$ 环
4. DP

最后提一句，建议在 UOJ 交一下，上面有很多 hack 数据，可以卡掉很多假做法。（UOJ hack 4 是卡 SPFA 的，所以 ~~SPFA 已死~~ 要用 dijkstra 写最短路）

## 代码

{% note no-icon info 代码 %}
```cpp
#include <cstdio>
#include <algorithm>
#include <queue>
#include <vector>
#include <cstring>

typedef long long LL;

const int N = 1e5 + 5;
const int M = 2e5 + 5;
const int K_MAX = 50 + 5;
const LL LLINF = 0x3f3f3f3f3f3f3f3fLL;

int n, m, K, mod;
struct Graph {
	struct Edge { int to, nxt; LL w; } edge[M << 1];
	int head[N], ek;
	void clear() { memset(head, 0, sizeof(head)); ek = 1; }
	void add_edge(int u, int v, LL w) { edge[ek] = (Edge){v, head[u], w}, head[u] = ek++; }
} G, R, DAG;

LL dis1[N], disn[N];
void dijkstra(Graph Graph, LL *dis, int st) { // 最短路
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> q; // 强制转成小根堆
	for(int i = 1; i <= n; i++) dis[i] = LLINF;
	dis[st] = 0;
	q.push({0, st});
	while(!q.empty()) {
		auto p = q.top();
		int u = p.second, d = p.first;
		q.pop();
		if(dis[u] != d) continue;
		for(int i = Graph.head[u]; i; i = Graph.edge[i].nxt) if(dis[u] + Graph.edge[i].w < dis[Graph.edge[i].to]) {
			int v = Graph.edge[i].to;
			dis[v] = dis[u] + Graph.edge[i].w;
			q.push({dis[v], v});
		}
	}
}

bool exist[N];
void build_DAG() { // 建图（最短路生成图）
	for(int i = 1; i <= n; i++) exist[i] = true;
	for(int i = 1; i <= n; i++) if(dis1[i] + disn[i] > dis1[n] + K) exist[i] = false; // 不可能出现在答案中
	for(int u = 1; u <= n; u++) if(exist[u])
		for(int i = G.head[u]; i; i = G.edge[i].nxt) {
			int v = G.edge[i].to;
			if(!exist[v]) continue;
			if(dis1[v] == dis1[u] + G.edge[i].w) DAG.add_edge(u, v, G.edge[i].w);
		}
}

int ind[N];
std::vector<int> order;
bool check() {
	// 拓扑排序
	order.clear();
	int cnt = 0;
	for(int i = 1; i <= n; i++) ind[i] = 0; // ind - 入度
	std::vector<int> q;
	for(int u = 1; u <= n; u++) for(int i = DAG.head[u]; i; i = DAG.edge[i].nxt) {
		int v = DAG.edge[i].to;
		ind[v]++;
	}
	for(int i = 1; i <= n; i++) if(!ind[i]) q.push_back(i), cnt++, order.push_back(i);
	while(!q.empty()) {
		int u = q.back();
		q.pop_back();
		for(int i = DAG.head[u]; i; i = DAG.edge[i].nxt) {
			int v = DAG.edge[i].to;
			if(ind[v]) {
				ind[v]--;
				if(ind[v] == 0) q.push_back(v), cnt++, order.push_back(v);
			}
		}
	}
	return cnt == n;
}

int f[N][K_MAX];
int dp() {
	for(int i = 1; i <= n; i++) for(int j = 0; j <= K; j++) f[i][j] = 0;
	f[1][0] = 1;
	for(int j = 0; j <= K; j++) {
		for(int i = 0; i < n; i++) { // 在 DAG 上 DP
			int u = order[i];
			for(int k = DAG.head[u]; k; k = DAG.edge[k].nxt) {
				int v = DAG.edge[k].to;
				(f[v][j] += f[u][j]) %= mod;
			}
		}
		for(int u = 1; u <= n; u++) // DP 不在 DAG 上的边
			for(int i = G.head[u]; i; i = G.edge[i].nxt) {
				int v = G.edge[i].to;
				if(dis1[v] == dis1[u] + G.edge[i].w) continue;
				int tmp = dis1[u] + j + G.edge[i].w - dis1[v];
				if(tmp <= K) (f[v][tmp] += f[u][j]) %= mod;
			}
	}
	int ans = 0;
	for(int j = 0; j <= K; j++) (ans += f[n][j]) %= mod;
	return ans;
}

int main() {
	int T; scanf("%d", &T);
	while(T--) {
		G.clear(), R.clear(), DAG.clear();
		scanf("%d%d%d%d", &n, &m, &K, &mod);
		for(int i = 1; i <= m; i++) {
			int u, v; LL w;
			scanf("%d%d%lld", &u, &v, &w);
			G.add_edge(u, v, w);
			R.add_edge(v, u, w);
		}
		dijkstra(G, dis1, 1), dijkstra(R, disn, n);
		build_DAG();
		if(!check()) { puts("-1"); continue; }
		printf("%d\n", dp());
	}
	return 0;
}
```
{% endnote %}

