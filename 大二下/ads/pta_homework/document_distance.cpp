#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

class doc {
public:
	string name;//文件名
	struct words {
		string word;
		int frequency;
	};
	vector<words> content;
	void set_name(string str);
	void set_words(string str);
};
void doc::set_name(string str) {
	name = str;
}
void doc::set_words(string str) {
	words ptr1;
	string s1;
	int fre=1;
	if (content.size() == 0) {//如果该文件还没有输入过word
		ptr1.frequency = 1;
		ptr1.word = str;
		content.push_back(ptr1);
	}
	else {//寻找该word是否出现过
		for (vector<words>::iterator iter = content.begin(); iter != content.end(); iter++)
		{
			s1 = (*iter).word;
			fre = (*iter).frequency;
			//cout << s1 << fre << endl;
			if (s1 == str) {
				(*iter).frequency++;
				return;
			} 
		}
		//没有出现过则添加记录
		ptr1.frequency = 1;
		ptr1.word = str;
		content.push_back(ptr1);
	}

}
int main() {

	vector<doc*> documents;

	int N, M;
	int i = 0,j;
	string str;
	string str2;
	doc* ptr1;
	doc* ptr2;

	string s = "#";//结束符
	string name;//文件名
	string words;//单词
	string w1, w2;
	int s1, s2, cont;
	int f1, f2, c1, c2;
	double result;

	cin >> N;
	for (i = 0; i < N; i++) {
		cin >> str;
		doc* p = new doc;
		if (str != s) {
			p->set_name(str);
		}
		cin >> str;
		while (str != s) {
			p->set_words(str);
			cin >> str;
		}
		if (p) documents.push_back(p);
	}
	//M次查询，首先找到对应的文件，然后对里面的word进行比较计算
	cin >> M;
	for (i = 0; i < M; i++) {
		s1 = 0; s2 = 0; cont = 0; result = 0;
		f1 = 0; f2 = 0; c1 = 0; c2 = 0;
		ptr1 = NULL; ptr2 = NULL;
		cin >> str;
		cin >> str2;
		for (vector<doc*>::iterator iter = documents.begin(); iter != documents.end(); iter++) {
			if (ptr1 && ptr2)break;
			if ((*iter)->name == str)  ptr1 = ( * iter);
			if ((*iter)->name == str2) ptr2 = (*iter);

		}
		if (ptr1 && ptr2) {
			//cout << ptr1->name << endl;
			//cout << ptr2->name << endl;
			s1 = ptr1->content.size();
			s2 = ptr2->content.size();

			for (j = 0; j < s1; j++) {
				w1 = (ptr1->content[j].word);
				f1 = ptr1->content[j].frequency;
				c1 += f1*f1;
				for (int k = 0; k < s2; k++) {
					w2 = (ptr2->content[k].word);
					f2 = ptr2->content[k].frequency;
					c2 += f2*f2;
					if (w1 == w2)	cont += f1 * f2;
				}	
			}
			c2 /= s1;
			result = 1.0 * cont / pow(c1, 0.5) / pow(c2, 0.5);
			result = acos(result);
		
			cout << "Case " << i+1 << ": ";
			cout.setf(ios::fixed);
			cout.precision(3);
			cout<< result << endl;
		}

	}
	return 0;
}




