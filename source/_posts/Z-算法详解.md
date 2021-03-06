---
title: Z 算法详解
description: Z 算法又称拓展 KMP，是一种字符串算法。那么它有什么妙用呢？
tags:
  - 算法
  - 字符串
categories:
  - 信息技术
  - 知识点
mathjax: true
urlname: z-algorithm
abbrlink: 35812
date: 2021-01-23 18:58:06
---

## 引入

先来看几个问题：

> 什么是 Z 算法呢？

Z 算法（Z-Algorithm）是一种字符串匹配算法，可以在 $O(n)$ 时间内算出一些关于字符串匹配的问题。

> 为什么要用 Z 算法？

暴力进行匹配是 $O(n^2)$ 的，但用 Z 后是 $O(n)$ 的。

> 一些约定：
>
> 1. $s[l .. r]$ 表示 $s_ls_{l + 1}s_{l + 2} ... s_r$
> 2. z 函数用 $z()$ 表示，原字符串用 s 表示
> 3. 下标从 1 开始

## Z 函数和 Z-Box

Z 算法的核心在于 Z 函数（也称 Z 数组）和 Z-Box。下面来分别讲解一下。

### 定义

#### Z 函数

**定义：** $z(i)$ 的值是满足 $s[1 .. x] = s[i .. i + x - 1]$ 的最大的 $x$。

> 例如：  
> $s = \texttt{aabbaabb}$，$z(5) = 4$，因为 $s[1 .. 4] = s[5 .. 8]$  
> $s = \texttt{aabab}$，$z(3) = 0$  
> $s = \texttt{ababa}$，$z(3) = 3$

#### Z-Box

**定义：** $s[i .. i + z(i) - 1]$ 为一个 Z-Box，共有 $n$ 个 Z-Box。  
这个概念有什么用呢？一会求 Z 函数是会用到。

### 如何求解

#### Z-Box

求 Z-Box 很简单，就是将 Z 函数求出来后就能 $O(1)$ 求。

#### Z 函数

如何求 Z 函数呢？我们知道 Z 算法的时间复杂度是 $O(n)$ 的，所以求 $z[1 .. n]$ 也必须是 $O(n)$ 的。  
于是 Z-Box 就派上用场了——我们可以用之前算出的 Z-Box 和 $z[2 .. i - 1]$ 来算出 $z(i)$。

我们维护最右的 Z-Box 的左右端点，记为 $zl$, $zr$，并设这个 Z-Box 的大小（即 $zr - zl + 1$）为 $zv$。  
设当前我们要求 $z(i)$，于是就有两种情况：

1. $zr < i$，直接暴力更新 $z(i)$，并更新 $zl$, $zr$。
2. $zr \ge i$，下面详细讨论。

![Z 函数更新](Z-算法详解/Z-函数更新.png)

设 $i' = i - zl + 1$，因为$s[zl .. zr] = s[1 .. zr-zl+1]$，所以 $i'$ 相当于将 $i$ 平移到了最开始。  
此时在 $zl .. zr$ 内的部分（红色）满足 $z(i) = z(i')$，而在 $zl .. zr$ 外的部分（绿色）就暴力拓展。  
即：

1. $zv \le zr - i + 1$，直接用 $z(i) = zv$。
2. $zv > zr - i + 1$，从 $zr$ 开始暴力匹配 $z(i)$（绿色部分），更新 $zl$, $zr$。

#### 时间复杂度

因为 $zl$, $zr$ 一直往右移动，不移动 $zl, zr$ 时的操作为常数，所以时间复杂度为 $O(n)$。

### 应用

一道例题：

> 给出两个字符串 s, t，求出所有的位置 x，满足 $s[x .. x + len(t) - 1] = t$。  
> 其中 $len(s), len(t) \le 10^5$，s, t 仅包含大小写字母。

可以设一个字符 $r = t + \texttt{!} + s$，然后对 $r$ 跑一边 Z-Algorithm。  
然后看 $r[len(t) + 2 .. len(r)]$ 中有多少个的 Z 函数等于 $len(t)$。

原理留给读者自己想。
