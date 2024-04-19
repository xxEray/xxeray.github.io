---
title: AtCoder ARC175F Append Same Characters 题解
date: 2024-04-18 20:23:26
description: Border 理论相关的字符串好题！
tags: 
	- 字符串
categories:
	- 题解
	- AtCoder
mathjax: true
urlname: atcoder-arc175-f
---

## 题意

给出由 $n$ 个仅含小写英文字母的字符串组成的序列，你可以做下列两种操作：

1. 选择一个小写英文字母，并在序列中 **每个** 字符串末尾添加这个字符；
2. 交换序列中两个相邻的字符串。

你需要通过若干次操作使得字符串序列的字典序 **单调不降**，求最小操作次数。

$2\le n\le 3\times 10^5$，字符串长度之和 $\le 3\times 10^5$

## 思路

首先发现我们肯定会先做完操作 1 再做操作 2，并且操作 2 的次数等于序列的逆序对数，所以问题转化为在每个串后面加一个字符串 $X$，并最小化逆序对个数加 $|X|$。

于是我们先计算出原序列的逆序对个数，然后再考虑 $X$ 的影响。

考虑怎样的两个字符串 $A,B$ 会被 $X$ 影响（不妨设 $|A|\le|B|$）。如果 $A$ 和 $B$ 在前 $\min(|A|,|B|)$ 个字符就比出结果了，那么 $X$ 显然是没用的。所以只有当 $A$ 是 $B$ 的前缀的时候 $X$ 才有用。

记 $B$ 除去 $A,B$ 的公共前缀部分后的字符串为 $Y$，那么 $AX>BX\iff X>YX$。于是其实答案只和 $X,Y$ 有关。

然后似乎就做不下去了。这时需要用到一些结论，这里先不给出证明，在文末统一给出证明（下同）。

{% note no-icon info %}

**引理 1:** $X<YX\iff X<Y^{\infty}$

{% endnote %}

有了这个之后就有了一个大致思路。我们把所有 $Y^{\infty}$ 排序，那么一个 $X$ 会影响到的 $Y$ 就是一段前缀。我们对于每个 $Y$ 统计它对逆序对的贡献，然后枚举一个前缀算答案就行了。

那么怎么对 $Y^{\infty}$ 排序呢？这里需要用到另外的结论。

{% note no-icon info %}

**引理 2:** $X^{\infty}<Y^{\infty}\iff XY<YX$

{% endnote %}

然后就可以很容易用 SA 做到 $O(n\log n)$，或者用哈希做到 $O(n\log^2 n)$。

然后考虑一个 $Y$ 对逆序对的贡献到底是什么。对于一个原串 $B$，Trie 上它到根的链上所有的（终止）节点都可以作为 $A$。不过要注意字符串可以重复，所以一个点可能有多个字符串在这里终止。

由于 $\sum |s_i|\le 3\times 10^5$，所以我们可以枚举 Trie 上每个节点上的每个字符串，然后再枚举它到根的路径上每个点，这样的总复杂度是 $\sum |s_i|$。假设我们枚举了一个字符串 $s_i=B$ 和它到根路径上一个点 $x$，那么 $x$ 上的所有字符串都可以作为 $A$ 产生贡献。对于一个 $A=s_j$，本来是 $A<B$，但如果加上 $X(X>Y^{\infty})$ 后大小关系会变成 $AX>BX$，此时如果 $j<i$ 对答案的贡献为 $+1$（增加一个逆序对），否则（$i>j$）对答案的贡献为 $-1$（减少一个逆序对）。所以对于 $x$ 上的所有 $A$ 我们只需要二分有多少个 $A$ 的下标小于 $i$ 并计算贡献就好了。

最后还一个问题就是，假如我们枚举了 $Y_1, Y_2$ 并让 $Y_1^{\infty}<X\le Y_2^{\infty}$，我们如何算出 $|X|$ 的最小值呢？考虑对于有限的字符串来说，答案是简单的，就是 $P<X\le Q \implies |X|_{\min}=\operatorname{lcp}(P,Q)+1$。所以问题转化为 $\operatorname{lcp}(Y_1^{\infty},Y_2^{\infty})$。这里又需要用一个结论。

{% note no-icon info %}

**引理 3:** 如果 $X^{\infty}\ne Y^{\infty}$，那么 $\operatorname{lcp}(X^{\infty},Y^{\infty})=\operatorname{lcp}(XY, YX)$。

{% endnote %}

然后就很容易用 SA 或哈希解决这个问题。具体来说就是把所有串拼在一起并用一个特殊字符隔开，然后就可以用 `height` 来 $O(1)$ 比较两个串以及它们的（有限）拼接。

还有一点细节需要注意。比如 $X$ 比所有 $Y^{\infty}$ 都大的时候，要特殊处理，具体就是找到第一个非 `z` 的字符（如果没有就说明不可能找到一个 $X> Y^{\infty}$）。以及每个 $Y$ 的贡献都是可能爆 int 的。

总时间复杂度为 $O(n+\sum |s_i| \log n)$，如果用哈希就是 $O(n+\sum |s_i|\log^2 n)$。

## 代码

代码有点长，给个 [提交记录](https://atcoder.jp/contests/arc175/submissions/52482271) 吧。

## 证明

~~下面的题解其实基本上是[官方题解](https://atcoder.jp/contests/arc175/editorial/9662)的翻译。~~

### 前置

可能需要了解一下 [Border 理论和 period 相关的东西](https://www.luogu.com/article/d4y3zqqv)。（不用了解太多，知道 Border 和 period 的定义以及弱周期定理就行了，也可以先往下看，看到不懂的再到这篇文章里找）

### 引理 1

{% note no-icon info %}

**引理 1:** $X<YX\iff X<Y^{\infty}$

{% endnote %}

对于 $Y$ 不是 $X$ 的前缀的情况，此时 $X>YX\iff X>Y\iff X>Y^{\infty}$。

然后对于 $Y$ 是 $X$ 的前缀，则令 $X'=YX$，那么 $X>YX\iff YX'>Y^2X'\iff X'>YX'$，根据归纳法 $X'>YX'\iff X'>Y^{\infty}\iff YX'>Y^{\infty}\iff X>Y^{\infty}$。

### 引理 2

{% note no-icon info %}

**引理 2:** $X^{\infty}<Y^{\infty}\iff XY<YX$

{% endnote %}

对于字符串 $X,Y$ 和 $g=\gcd(|X|, |Y|)$，下面三个结论都是等价的：

1. $X^{\infty}=Y^{\infty}$
2. $g$ 同时是 $X,Y$ 的循环节（且 $X[0..g)=Y[0..g)）$
3. $XY=YX$

其中 $2 \Rightarrow 1$ 以及 $2 \Rightarrow 3$ 是显然的。

{% note no-icon success %}

$1\Rightarrow 2$：

根据中国剩余定理，只要 $i\equiv j \pmod g$，就一定可以找到 $k$ 满足 $k\equiv i \pmod{|X|}$ 且 $k\equiv j \pmod{|Y|}$，因为 $\gcd(|X|,|Y|)=g$。所以 $X_i = (X^{\infty})_k = (Y^{\infty})_k = Y_j$，又因为 $i+g\equiv j\pmod g$ 也成立，所以同理可得 $X_{i+g} = Y_j$，所以 $X_i = X_{i+g}$，即 $g$ 是 $X$ 的周期。$Y$ 同理。

{% endnote %}

{% note no-icon success %}

$3\Rightarrow 2$：

不妨设 $|X|\le|Y|$。由于 $XY=YX$，所以 $Y[0, |X|)=X=Y[|Y|-|X|,|Y|)$，故 $X$ 是 $Y$ 的 Border，即 $|Y|-|X|$ 是 $Y$ 的循环节。同理，由于 $XY=YX$，所以 $Y[|X|,|Y|)=Y[0,|Y|-|X|)$，故 $Y[0,|Y|-|X|)$ 是 $Y$ 的 Border，即 $|X|$ 是 $Y$ 的循环节。所以 $|X|$ 和 $|Y|-|X|$ 都是 $Y$ 的循环节，根据 **弱周期定理**，$g=\gcd(|X|, |Y|-|X|)$ 也是 $Y$ 的循环节。

{% endnote %}

### 引理 3

{% note no-icon info %}

**引理 3:** 如果 $X^{\infty}\ne Y^{\infty}$，那么 $\operatorname{lcp}(X^{\infty},Y^{\infty})=\operatorname{lcp}(XY, YX)$。

{% endnote %}

由于 $X^{\infty}\ne Y^{\infty}$，由 **引理 1** 可得 $XY\ne YX$。不妨设 $|X|\le |Y|$。

考虑 $Y$ 是 $X^{\infty}$ 的前缀的情况。因为 $Y$ 是 $X^{\infty}$ 的前缀，所以 $XY$ 也是 $X^{\infty}$ 的前缀。又由于 $|X|\le |Y|$，所以 $X$ 也是 $Y$ 的前缀，然后可以得到 $YX$ 是 $Y^2$ 的前缀，继而可以得到 $YX$ 是 $Y^{\infty}$ 的前缀。由于我们知道 $XY\ne YX$，而 $|XY|=|YX|$，所以 $XY$ 和 $YX$ 一定会在第 $|X|+|Y|$ 位或之前比较出结果。而 $X^{\infty}$ 和 $Y^{\infty}$ 分别是它们两个的前缀，所以也一定会在同一个位置比较出结果。故 $\operatorname{lcp}(XY,YX)=\operatorname{lcp}(X^{\infty},Y^{\infty})$。

然后考虑 $Y$ 不是 $X^{\infty}$ 的前缀的情况。我们不断把 $Y$ 开头的 $X$ 提出来，即找到最大的 $n$ 使得 $Y=X^nY'$，那么此时 $X$ 不是 $Y'$ 的前缀。又由于 $Y=X^nY'$ 不是 $X^{\infty}$ 的前缀，所以 $Y'$ 也不是 $X$ 的前缀。因此 $X$ 和 $Y'$ 一定会在第 $\min(|X|,|Y'|)$ 位前比出结果，用于上面引理 1 类似的方法我们可以得到 $\operatorname{lcp}(X^{\infty},Y'Y^{\infty})=\operatorname{lcp}(X,Y')$，同理可得 $\operatorname{lcp}(X,Y')=\operatorname{lcp}(XY',Y'X)$。于是：

$$
\begin{aligned}
	&\operatorname{lcp}(X^{\infty},Y^{\infty})\\
	=&\operatorname{lcp}(X^nX^{\infty},X^nY'Y^{\infty})\\
	=&n|X|+\operatorname{lcp}(X^{\infty},Y'Y^{\infty})\\
	=&n|X|+\operatorname{lcp}(XY',Y'X)\\
	=&\operatorname{lcp}(X^{n+1}Y',X^nY'X)\\
	=&\operatorname{lcp}(XY,YX)
\end{aligned}
$$

## 参考资料

{% note no-icon info %}

<https://www.luogu.com/article/d4y3zqqv>  
<https://atcoder.jp/contests/arc175/editorial/9662>

{% endnote %}
