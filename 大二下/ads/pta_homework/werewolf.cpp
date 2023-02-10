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
    if (k == 1) { //走到最后一个节点，判断是否符合L,M的数量
        if (x != M) return false;
        if (y != L) return false;
        //如果狼都说谎了
        for(p = 0; p <= N; p++){
            if (answer[p] == 2) {
                id = assersion[p];
                if (id < 0) {//认为是狼但是是平民
                    id = -id;
                    if (answer[id] == 1)count++;
                }
                else if (id > 0) {//认为是平民但是是狼
                    if (answer[id] == 2) count++;
                }
            }
        }
        if (count == M) return false;
        //如果狼都没有说谎
        count = 0;
        for (p = 0; p <= N; p++) {
            if (answer[p] == 2) {
                id = assersion[p];
                if (id < 0) {//认为是狼
                    id = -id;
                    if (answer[id] == 2)count++;
                }
                else if (id > 0) {//认为是平民但是是狼
                    if (answer[id] == 1) count++;
                }
            }
        }
        if (count == M) return false;

        return true;
    }
    else if (k > 1 && k <= N+1) {//判断该方向是否能走
        if (d == 0) { //往左走 孩子是狼
            if (x >= M) return false;
            if (people[k - 1] + y > L) return false;
            return true;
        }
        else if (d == 1) {//往右走，孩子是平民
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
    if (max<0)  return true; /* solved with (x1, …, xN) */
    if (max == 0) {//如果已经走到最后一个点
        OK = check(max + 1, 0);
        if (OK) return true;
        return false;
    }
    //如果还没走到最后
    //判断前一个人是狼是否违反限制
    OK = check(max+1, 0);
    if (OK) {
        answer[max] = 2; //2是狼，1是平民
        num_wolf++;
        if (people[max] > 0) num_lier+=people[max];
        Found = Backtracking(max);
        if (!Found) {
            num_wolf--;
            if (people[max] > 0) num_lier -= people[max];
            answer[max] = 0;
        }
    }
    //判断前一个人是平民是否违反限制
    if (!Found) {
        OK = check(max+1, 1);
        if (OK) {
            answer[max] = 1; //2是狼，1是平民
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
    for (i = 0; i < N; i++) {//输入每个人发言，记录每个人狼、平民的身份数，用于统计lier数量
        cin >> j;
        assersion[i + 1] = j;
        if (j > 0) people[j]++;
        else if (j < 0) {
            j = -j;
            wolf[j]++;
        } 
    }
    i = N;
    if (Backtracking(N+1)) {//如果结果正确，降序输出狼的序号
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
