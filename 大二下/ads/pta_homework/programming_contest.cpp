#include <stdio.h>
#include <iostream>
#include<vector>
using namespace std;
#define MAXN 10
#define MAXS 1000

int need_time(const int time[], const int score[], int happy_score, int n);

int main() {
    int n, i, happy_score;
    int time[MAXN], score[MAXN];
    cin >> n >> happy_score;
    //scanf("%d %d", &n, &happy_score);
    for (i = 0; i < n; ++i)
        cin >> time[i];
        //scanf("%d", &time[i]);
    for (i = 0; i < n; ++i)
        cin >> score[i];
        //scanf("%d", &score[i]);
    printf("%d\n", need_time(time, score, happy_score, n));
    return 0;
}

/* Your function will be put here */

int min(int i, int j) {
	return i < j ? i : j;
}
int need_time(const int time[], const int score[], int happy_score, int n)
{
	int i, j, ret;
	int ma = 100000;
	int a[MAXS];
	ret = ma;
	//初始化得到分数对应的时间
	for (i = 0; i <= happy_score; i++) a[i] = ma;
	a[0] = 0;
	//每次加入一道新的题
	for (i = 0; i < n; i++) {
		//逆序遍历所有存在的值
		for (j = happy_score - 1; j >= 0; j--) {
			//如更新值大于happy_score
			if (j + score[i] >= happy_score) ret = min(ret, a[j] + time[i]);
			//如果更新值小于happy_score
			else {
				a[j + score[i]] = min(a[j + score[i]], a[j] + time[i]);
			}
		}
	}
	if (ret == ma) return -1;
	return ret;
}