---
title: 一个生成函数的 trick
date: 2024-04-24 21:38:06
description: 很妙的做法。
tags:
	- 数学
	- 生成函数
categories:
	- 题解
	- CatOJ
mathjax: true
urlname: a-trick-for-generating-function
---

## 题意

原题是 [这个](https://local.cwoi.com.cn:8443/contest/C0462/problem/B)。

前置知识需要生成函数和单位根反演。（都不需要掌握得很牢固 ~~因为我掌握的就不牢固~~）

## 做法

### 简单转化

首先那个 $F(x,a,b)$ 就是 $x^{\gcd(a, b)}$（$ab=0$ 除外），证明不难 ~~我不会证~~。

然后设 $F(x)$ 为子集和的生成函数，那么 $F(x) = \prod\limits_{i=L+1}^R (x^i+1)$。所以答案就是 $\sum\limits_{m|j} [x^j]F(x)$。

然后就是单位根反演：

$$
\begin{aligned}
	 &\sum_{m\mid i} [x^i]F(x)\\
	&=\sum_{i\ge 0} ([x^i]F(x)) \cdot [m \mid i]\\
	&=\sum_{i\ge 0} ([x^i]F(x)) \sum_{j=0}^{m-1} \omega_m^{ij}\\
	&=\sum_{j=0}^{m-1}\sum_{i\ge 0} \omega_m^{ij}\cdot ([x^i]F(x))\\
	&=\sum_{j=0}^{m-1}\sum_{i\ge 0} [x^i]F(\omega_m^jx)\\
	&=\sum_{j=0}^{m-1} F(\omega_m^j)
\end{aligned}
$$

那么现在的问题是求 $F(\omega_m^i)$。

### 推式子

首先考虑把下界去掉。$F(x) = \dfrac{\prod_{i=1}^R (x^i + 1)}{\prod_{i=1}^L (x^i + 1)}$，所以我们设 $G_{T}(x) = \prod\limits_{i=1}^{T}(x^i+1)$，那么 $F(x) = \dfrac{G_R(x)}{G_L(x)}$。

然后，由于 $L$ 和 $R$ 都是 $m^k$ 的形式，所以我们只需要计算 $G_{m^k}=\prod\limits_{i=1}^{m^k} (x^i + 1)$。

由于 $x$ 是 $\omega_m^j$ 的形式，所以 $x^i=\omega_m^{ij}=\omega_m^{j(i\bmod m)}$，也即 $x^i$ 只和 $i\bmod m$ 有关。正好求和上界是 $m^k$，所以容易得到：

$$
\begin{aligned}
	G_{m^k}(\omega_m^j)
	&=\prod_{i=1}^{m^k}(\omega_m^{ij} + 1)\\
	&=\prod_{p=0}^{m^{k-1}-1}\prod_{i=1}^m(\omega_m^{(pm+i)j} + 1)\\
	&=\prod_{p=0}^{m^{k-1}-1}\prod_{i=1}^m(\omega_m^{ij} + 1)\\
	&=\left(\prod_{i=1}^m(\omega_m^{ij} + 1)\right)^{m^{k-1}}
\end{aligned}
$$

### 巧妙的 trick

前面都很套路，这里就到这题最妙的地方了。

现在就是要算 $\prod_{i=1}^m(\omega_m^{ij}+1)$。

首先考虑 $j=1$，也就是 $\prod_{i=1}^m (\omega_m^i + 1) = \prod_{i=0}^{m-1} (\omega_m^i + 1)$。

我们知道 $\{\omega_m^i\}_{0\le i < m}$ 是方程 $x^m-1=0$ 的 $m$ 个复数解，所以 $x^m-1$ 可以因式分解为 $\prod\limits_{0\le i < m} (x - \omega_m^i)$，也即 $x^m - 1 = \prod\limits_{0\le i < m} (x - \omega_m^i)$。  
代入 $x=-1$ 可得：

$$
\begin{aligned}
	(-1)^m - 1 &= \prod_{0\le i < m} (-1 - \omega_m^i)\\
	(-1)^m - 1 &= (-1)^m \prod_{0\le i < m} (\omega_m^i + 1)\\
	\frac{(-1)^m - 1}{(-1)^m} &= \prod_{0\le i < m} (\omega_m^i + 1)\\
	1 - (-1)^m &= \prod_{0\le i < m} (\omega_m^i + 1)
\end{aligned}
$$

于是这个问题就解决了。

然后考虑 $j$ 为任意值怎么做。  
由于 $\omega_m^{ij} = \omega_m^{ij\bmod m}$，所以我们考虑多重集 $\{ij\bmod m\}_{0\le i < m} = \{0, j \bmod m, 2j \bmod m, \cdots, (m - 1)j \bmod m\}$ 是长什么样子的。简单手玩可以发现 $\gcd(j, m) \mid (ij \bmod m)$，并且每个 $\gcd(j, m)$ （在 $[0, m)$ 以内的）倍数刚好出现 $\gcd(j, m)$ 次。这个跟数论相关，可以证明的，这里就不详细展开了。然后就比较简单了。考虑推一下式子：（令 $g=\gcd(j, m)$）

$$
\begin{aligned}
	 &\prod_{i=0}^{m-1} (\omega_m^{ij} + 1)\\
	=&\left(\prod_{\gcd(j, m)\mid i}(\omega_m^i + 1)\right)^g\\
	=&\left(\prod_{0\le j < m/g}(\omega_{m/g}^j + 1)\right)^g\\
	=&\left(1 - (-1)^{m/g}\right)^g
\end{aligned}
$$

然后就是上面那个问题了。

至此，$G_{m^k}(\omega_m^{ij})$ 就求出来了，那么 $F(\omega_m^{ij})$ 也基本上求出来了。时间复杂度 $O(mT)$。

### 优化

但是这样还不足以通过。最后一个 Subtask 是 $m\le 10^7,T\le 10^4$，我们还需要更加优秀的做法。

考虑把前面推的式子代入到答案里。

首先 $G_{m^k}(\omega_m^j) = \left(1 - (-1)^{m/g}\right)^{m/g\cdot m^{k-1}}$。设 $L=m^l$，$R=m^r$，那么：

$$
\begin{aligned}
	F(\omega_m^j)
	&=\frac{G_R(\omega_m^j)}{G_L(\omega_m^j)}\\
	&=\frac{\left(1-(-1)^{m/g}\right)^{m/g\cdot m^{r-1}}}{\left(1-(-1)^{m/g}\right)^{g\cdot m^{l-1}}}\\
	&=\left(1-(-1)^{m/g}\right)^{g\cdot (m^{r-1}-m^{l-1})}
\end{aligned}
$$

特别地，如果 $L=0$，那么 $F(\omega_m^j) = \left(1-(-1)^{m/g}\right)^{g\cdot m^{r-1}}$。于是 $F(\omega_m^j)$ 就有一个 $\left(1-(-1)^{m/g}\right)^{g\cdot v}$ 的形式，其中 $v$ 是只和 $L,R$ 相关的常数。

从上面我们也可以看出 $F(\omega_m^j)$ 的值只和 $g=\gcd(j, m)$ 有关，于是可以将 $F$ 代入最开始的式子：

$$
\begin{aligned}
	 &\sum_{j=0}^{m-1} F(\omega_m^j)\\
	=&\sum_{j=0}^{m-1} F(\omega_m^{\gcd(j, m)})\\
	=&\sum_{g\mid m} \varphi(\frac mg) F(\omega_m^g)\\
	=&\sum_{g\mid m} \varphi(\frac mg) \left(1 - (-1)^{m/g}\right)^{g\cdot v}
\end{aligned}
$$

枚举因数是 $O(\sqrt m)$ 的，由于需要线性筛 $\varphi$，所以复杂度是 $O(m+T(\sqrt m + \sigma_0(m)\log n))$。

## 代码

{% note no-icon info 代码 %}

```cpp
#include <bits/stdc++.h>

typedef long long LL;

const int N = 1e7 + 5;
const LL MOD = 998244353;

int m;
LL k1, k2;

bool isprm[N];
std::vector<int> prime;
int phi[N];
void sieve(int mx) {
	for(int i = 2; i <= mx; i++) isprm[i] = true;
	phi[1] = 1;
	for(int i = 2; i <= mx; i++) {
		if(isprm[i]) prime.emplace_back(i), phi[i] = i - 1;
		for(int p : prime) {
			if((LL)p * i > mx) break;
			isprm[p * i] = false;
			if(i % p == 0) {
				phi[i * p] = phi[i] * p;
				break;
			} else phi[i * p] = phi[i] * (p - 1);
		}
	}
}

LL gcd(LL x, LL y) { return y == 0 ? x : gcd(y, x % y); }

inline LL qpow(LL x, LL y, LL mod = MOD) { LL ret = 1; while(true) { if(y & 1) ret = ret * x % mod; if(!(y >>= 1)) return ret; x = x * x % mod; } }
inline LL inv(LL x) { return qpow(x, MOD - 2, MOD); }

int main() {
	sieve(1e7);
	int T; scanf("%d", &T);
	while(T--) {
		LL a1, b1, a2, b2;
		scanf("%d%lld%lld%lld%lld", &m, &a1, &b1, &a2, &b2);
		k1 = gcd(a1, b1), k2 = gcd(a2, b2);
		LL v;
		if(a1 == 0 || b1 == 0) v = qpow(m, k2 - 1, MOD - 1);
		else v = (qpow(m, k2 - 1, MOD - 1) - qpow(m, k1 - 1, MOD - 1) + MOD - 1) % (MOD - 1);
		// printf("v = %lld\n", v);
		std::vector<int> factors;
		for(int i = 1; i * i <= m; i++) if(m % i == 0) {
			factors.emplace_back(i);
			if(i * i != m) factors.emplace_back(m / i);
		}
		// printf("factors: "); for(int d : factors) printf("%d ", d); puts("");
		LL ans = 0;
		for(int d : factors) (ans += phi[m / d] * qpow(1 - ((m / d & 1) ? -1 : 1), d * v % (MOD - 1)) % MOD) %= MOD;
		printf("%lld\n", ans * inv(m) % MOD);
	}
	return 0;
}
```

{% endnote %}
