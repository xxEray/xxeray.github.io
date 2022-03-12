// https://www.luogu.com.cn/problem/P1495

#include <cstdio>

const int N = 5e6 + 5;

typedef long long LL;

// ax + by = 1
LL exgcd(LL a, LL b, LL &x, LL &y) {
	if(b == 0) {
		x = 1, y = 0;
		return a;
	}
	LL g = exgcd(b, a % b, x, y);
	LL tmp = x;
	x = y;
	y = tmp - a / b * y;
	return g;
}

int main() {
	LL a, b;
	std::scanf("%lld%lld", &a, &b);
	LL x, y;
	exgcd(a, b, x, y);
	while(x < 0) x += b;
	x %= b;
	std::printf("%lld\n", x);
	return 0;
}