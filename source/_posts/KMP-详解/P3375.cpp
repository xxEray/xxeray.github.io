#include <cstdio>
#include <cstring>

const int N = 1e6 + 5;

char s[N], t[N];
int nxt[N];

int n, m;

int main() {
	std::scanf("%s%s", s + 1, t + 1);
	n = std::strlen(s + 1), m = std::strlen(t + 1);
	nxt[0] = -1;
	for(int i = 1; i <= m; i++) {
		int j = nxt[i - 1];
		while(t[j + 1] != t[i] && j != -1) j = nxt[j];
		nxt[i] = j + 1;
	}
	int j = 0;
	for(int i = 1; i <= n; i++) {
		while(j && t[j + 1] != s[i]) j = nxt[j];
		if(t[j + 1] == s[i]) j++;
		if(j == m) {
			std::printf("%d\n", i - m + 1);
			j = nxt[j];
		}
	}
	for(int i = 1; i <= m; i++) std::printf("%d ", nxt[i]);
	return 0;
}