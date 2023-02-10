#include <cstdio>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;
int main() {
    int N, M;
    int x, i, min;
    int index = 0, count = 0;
    cin >> N >> M;
    vector<int> seq(N);                               //store all number
    priority_queue<int, vector<int>, greater<int>> q; //min heap    
    vector<int> next, cur;                            //cur:current run ; next:next run;

    //initialize
    for (i = 0; i < N; i++) {                         //input all numbers and store in seq
        cin >> seq[i];
    }

    for (; index < M; index++) q.push(seq[index]);    //initialize heap

    //replacement selection
    while (count < N) {
        min = q.top();                                //queue pop
        q.pop(); count++;
        cur.push_back(min);                           //save data in current run
        if (index < N) {                              //deal with the next number(where to go)
            if (seq[index] < min) {
                next.push_back(seq[index]);           //next run
            }
            else q.push(seq[index]);                  //queue
            index++;
        }
        if (q.empty()) {                              //output result of current run
            for (i = 0; i < cur.size() - 1; i++) {
                cout << cur[i] << " ";
            }
            cout << cur[i] << endl;

            cur.clear();                              //transfer data in next run to quque and clear next
            for (i = 0; i < next.size() ; i++) {
                q.push(next[i]);
            }
            next.clear();
        }
    }
    return 0;
}
