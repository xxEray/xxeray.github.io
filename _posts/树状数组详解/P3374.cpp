#include <cstdio>
#include <cstring>
const int N = 5e5 + 5;
const int INF = 0x3f3f3f3f;
int c[N];
int n, m;
inline int lowbit(int x) { return x & -x; }
void change(int x, int d) {
	while(x <= n) {
		c[x] += d;
		x += lowbit(x);
	}
}
int total(int x) {
	int tot = 0;
	while(x) {
		tot += c[x];
		x -= lowbit(x);
	}
	return tot;
}
int main() {
	scanf("%d%d", &n, &m);
	for(int i = 1; i <= n; i++) c[i] = 0;
	for(int i = 1; i <= n; i++) {
		int x;
		scanf("%d", &x);
		change(i, x);
	}
	for(int i = 1; i <= m; i++) {
		int type, l, r;
		scanf("%d%d%d", &type, &l, &r);
		if(type == 1) change(l, r);
		else printf("%d\n", total(r) - total(l - 1));
	}
	return 0;
} /*
5 5
1 5 4 2 3
1 1 3
2 2 5
1 3 -1
1 4 2
2 1 4
*/