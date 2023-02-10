//back_tracking
//werewolf game
//N : Player; N:wolves; L:Liers;
//output the wolves in decsending order
#include <iostream>
#include<vector>
using namespace std;

int N, M, L;
int num_lier;
int num_wolf;
static int answer[102];
static int assersion[102];
static int wolf[102];
static int people[102];

bool check(int k,int d){ 
    int p = 0;
    int x = num_wolf;
    int y = num_lier;
    int count = 0;
    int id;
    if (k == 1) { //�ߵ����һ���ڵ㣬�ж��Ƿ����L,M������
        if (x != M) return false;
        if (y != L) return false;
        //����Ƕ�˵����
        for(p = 0; p <= N; p++){
            if (answer[p] == 2) {
                id = assersion[p];
                if (id < 0) {//��Ϊ���ǵ�����ƽ��
                    id = -id;
                    if (answer[id] == 1)count++;
                }
                else if (id > 0) {//��Ϊ��ƽ��������
                    if (answer[id] == 2) count++;
                }
            }
        }
        if (count == M) return false;
        //����Ƕ�û��˵��
        count = 0;
        for (p = 0; p <= N; p++) {
            if (answer[p] == 2) {
                id = assersion[p];
                if (id < 0) {//��Ϊ����
                    id = -id;
                    if (answer[id] == 2)count++;
                }
                else if (id > 0) {//��Ϊ��ƽ��������
                    if (answer[id] == 1) count++;
                }
            }
        }
        if (count == M) return false;

        return true;
    }
    else if (k > 1 && k <= N+1) {//�жϸ÷����Ƿ�����
        if (d == 0) { //������ ��������
            if (x >= M) return false;
            if (people[k - 1] + y > L) return false;
            return true;
        }
        else if (d == 1) {//�����ߣ�������ƽ��
            if (wolf[k - 1] + y > L) return false;
            return true;
        }
    }
    return false;
}

bool Backtracking(int i)
{
    bool Found = false;
    bool OK;
    int max = i-1;
    if (max<0)  return true; /* solved with (x1, ��, xN) */
    if (max == 0) {//����Ѿ��ߵ����һ����
        OK = check(max + 1, 0);
        if (OK) return true;
        return false;
    }
    //�����û�ߵ����
    //�ж�ǰһ���������Ƿ�Υ������
    OK = check(max+1, 0);
    if (OK) {
        answer[max] = 2; //2���ǣ�1��ƽ��
        num_wolf++;
        if (people[max] > 0) num_lier+=people[max];
        Found = Backtracking(max);
        if (!Found) {
            num_wolf--;
            if (people[max] > 0) num_lier -= people[max];
            answer[max] = 0;
        }
    }
    //�ж�ǰһ������ƽ���Ƿ�Υ������
    if (!Found) {
        OK = check(max+1, 1);
        if (OK) {
            answer[max] = 1; //2���ǣ�1��ƽ��
            if (wolf[max] > 0) num_lier += wolf[max];
            Found = Backtracking(max);
            if (!Found) {
                if (wolf[max] > 0) num_lier -= wolf[max];
                answer[max] = 0;
            }
        }
    }   
    return Found;
};


int main() {
    int i,j;
    num_lier = 0;
    num_wolf = 0;
    
    cin >> N >> M >> L;
    for (i = 0; i < N; i++) {//����ÿ���˷��ԣ���¼ÿ�����ǡ�ƽ��������������ͳ��lier����
        cin >> j;
        assersion[i + 1] = j;
        if (j > 0) people[j]++;
        else if (j < 0) {
            j = -j;
            wolf[j]++;
        } 
    }
    i = N;
    if (Backtracking(N+1)) {//��������ȷ����������ǵ����
        while (answer[i] != 2)i--;
        cout << i;
        i--;
        while (i) {
            if (answer[i] == 2) cout << " " << i;
            i--;
        }
        
    }
    else cout << "No Solution";
}
