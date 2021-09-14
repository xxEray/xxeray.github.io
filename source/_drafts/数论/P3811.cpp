// https://www.luogu.com.cn/problem/P3811

#include <cstdio>

const int N = 20000528 + 5;

int inv[N];

int main() {
    int n, p;
    scanf("%d%d", &n, &p);
    inv[1] = 1;
    for(int i = 2; i <= n; i++) inv[i] = (long long)(p - p / i) * inv[p % i] % p;
    for(int i = 1; i <= n; i++) printf("%d\n", inv[i]);
    return 0;
}