// https://www.luogu.com.cn/problem/P1495

#include <cstdio>

const int N = 1e5 + 5;
typedef long long LL;

LL r[N], p[N];
int n;

LL exgcd(LL a, LL b, LL &x, LL &y) {
    if(b == 0) { x = 1, y = 0; return a; }
    LL g = exgcd(b, a % b, x, y);
    LL tmp = x;
    x = y;
    y = tmp - a / b * y;
    return g;
}
LL crt() {
    LL P = 1;
    for(int i = 1; i <= n; i++) P *= p[i];
    LL x = 0;
    for(int i = 1; i <= n; i++) {
        LL ret = 1;
        ret = ret * (P / p[i]) % P;
        LL inv, tmp;
        exgcd(P / p[i], p[i], inv, tmp);
        while(inv <= 0) inv += p[i];
        inv %= p[i];
        ret = ret * inv % P;
        ret = ret * r[i] % P;
        x = (x + ret) % P;
    }
    return x;
}

int main() {
	std::scanf("%d", &n);
	for(int i = 1; i <= n; i++) std::scanf("%lld%lld", &p[i], &r[i]);
	std::printf("%lld\n", crt());
	return 0;
}