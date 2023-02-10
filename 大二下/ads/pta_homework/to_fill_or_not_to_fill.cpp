// 0.如果排序后station[0].d != 0 ,不能到达
// 1.对station升序排序
// 2.贪心算法
// case0:最大距离范围内不存在加油站，不能到达
// case1:最大距离范围内存在加油站，但是价格都比我高，那么加满油，开到价格最小的那个加油站
// case2:最大距离范围内存在加油站，且存在价格比我低的，找到最近的比我价格低的，加刚刚好的油，开过去；
#include<algorithm>
#include<iostream>
#include<vector>
#include<iomanip>

using namespace std;
struct station {
	int distance;
	double price;
};
bool cmp(station a, station b) {
	return a.distance < b.distance;
}
int main() {
	double total=0;		//合计油费
	int cur_dis=0;		//当前所在点的距离
	int cur_num = 0;	//当前所在点的编号 0，1，2....n
	double cur_tank=0;	//当前的油量
	bool flag=false;	//是否到达终点
	int C,D,D_ave,n;	//油箱容量，目的地距离，单位油能够支持行驶的距离，总共站点数量
	int max_dis;
	int i,next;
	double price;
	bool cheaper;

	
	cin >> C >> D >> D_ave >> n;
	vector<station> sta(n + 1);
	for (i = 0; i < n; i++) {
		cin >> sta[i].price >> sta[i].distance;
	}
	sta[n].price = 0; sta[n].distance = D;

	sort(sta.begin(), sta.end(), cmp);
	max_dis = C * D_ave;
	if (sta[0].distance != 0) {
		cout << "The maximum travel distance = 0.00" ;
	}
	else {
		while (!flag) {
			//寻找最大范围内是否有站点
			cheaper = false;
			price = sta[cur_num+1].price;
			next = cur_num + 1;
			for (i = cur_num+1; i <= n; i++) {
				if (sta[i].distance - sta[cur_num].distance <= max_dis) {//有站点			
					if (sta[i].price < sta[cur_num].price) {//如果存在价格更低的，直接加刚好的油开过去	
						cheaper = true;
						total += ((sta[i].distance - sta[cur_num].distance)*1.0 / D_ave- cur_tank) * sta[cur_num].price;
						cur_num = i;
						cur_tank = 0;
						if (i == n) {
							cout << fixed << setprecision(2) << total;
							return 0;
						}
						break;
					}
					if (sta[i].price < price) {//如果不存在价格更低的，找到最低的
						price = sta[i].price;
						next = i;
					}

				}
				else {//最近的站点都不在范围内
					if (i == cur_num + 1) {
						cout << "The maximum travel distance = ";
						cout << fixed << setprecision(2) << sta[cur_num].distance + D_ave *C*1.0;
						return 0;
					}
					break;
				}
			}
			
			if (!cheaper) {//如果不存在价格更低的，
				if (D - sta[cur_num].distance <= max_dis) {//如果能到终点
					total += ((D - sta[cur_num].distance) *1.0/ D_ave - cur_tank)*1.0 * sta[cur_num].price;
					cur_tank = 0;
					cout << fixed << setprecision(2) << total;
					return 0;
				}
				else {//如果不能到终点，找到最低的，加满油开过去
					total += (C - cur_tank) * sta[cur_num].price;
					cur_tank = C - (sta[next].distance - sta[cur_num].distance) *1.0/ D_ave;
					cur_num = next;
				}				
			}
		}
	}
	return 0;
}



