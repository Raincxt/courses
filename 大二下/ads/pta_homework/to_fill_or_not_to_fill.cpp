// 0.��������station[0].d != 0 ,���ܵ���
// 1.��station��������
// 2.̰���㷨
// case0:�����뷶Χ�ڲ����ڼ���վ�����ܵ���
// case1:�����뷶Χ�ڴ��ڼ���վ�����Ǽ۸񶼱��Ҹߣ���ô�����ͣ������۸���С���Ǹ�����վ
// case2:�����뷶Χ�ڴ��ڼ���վ���Ҵ��ڼ۸���ҵ͵ģ��ҵ�����ı��Ҽ۸�͵ģ��Ӹոպõ��ͣ�����ȥ��
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
	double total=0;		//�ϼ��ͷ�
	int cur_dis=0;		//��ǰ���ڵ�ľ���
	int cur_num = 0;	//��ǰ���ڵ�ı�� 0��1��2....n
	double cur_tank=0;	//��ǰ������
	bool flag=false;	//�Ƿ񵽴��յ�
	int C,D,D_ave,n;	//����������Ŀ�ĵؾ��룬��λ���ܹ�֧����ʻ�ľ��룬�ܹ�վ������
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
			//Ѱ�����Χ���Ƿ���վ��
			cheaper = false;
			price = sta[cur_num+1].price;
			next = cur_num + 1;
			for (i = cur_num+1; i <= n; i++) {
				if (sta[i].distance - sta[cur_num].distance <= max_dis) {//��վ��			
					if (sta[i].price < sta[cur_num].price) {//������ڼ۸���͵ģ�ֱ�ӼӸպõ��Ϳ���ȥ	
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
					if (sta[i].price < price) {//��������ڼ۸���͵ģ��ҵ���͵�
						price = sta[i].price;
						next = i;
					}

				}
				else {//�����վ�㶼���ڷ�Χ��
					if (i == cur_num + 1) {
						cout << "The maximum travel distance = ";
						cout << fixed << setprecision(2) << sta[cur_num].distance + D_ave *C*1.0;
						return 0;
					}
					break;
				}
			}
			
			if (!cheaper) {//��������ڼ۸���͵ģ�
				if (D - sta[cur_num].distance <= max_dis) {//����ܵ��յ�
					total += ((D - sta[cur_num].distance) *1.0/ D_ave - cur_tank)*1.0 * sta[cur_num].price;
					cur_tank = 0;
					cout << fixed << setprecision(2) << total;
					return 0;
				}
				else {//������ܵ��յ㣬�ҵ���͵ģ������Ϳ���ȥ
					total += (C - cur_tank) * sta[cur_num].price;
					cur_tank = C - (sta[next].distance - sta[cur_num].distance) *1.0/ D_ave;
					cur_num = next;
				}				
			}
		}
	}
	return 0;
}



