#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;
//创建2-3 B+树，包含操作：初始化，插入（含分裂），查找，输出
typedef struct BTreeNode* BTree;

struct BTreeNode
{
	int size;
	int key[3];
	bool isLeaf;
	BTree left, right, middle;
};

BTree stack[10000];
int top = 0;

void push(BTree x)
{
	stack[top++] = x;
}

BTree pop()
{
	return stack[--top];
}

BTree queue[10000];
int front, rear;
void enqueue(BTree t)
{
	front = (front + 1) % 10000;
	queue[front] = t;
}

BTree dequeue()
{
	rear = (rear + 1) % 10000;
	return queue[rear];
}

void print(BTree t) //输出树
{
	if (!t)return;
	BTree tmp;
	enqueue(t);
	int cur = 1;
	int curnum = 1;
	int nextnum = 0;
	while (front != rear) {
		tmp = dequeue();
		if (!tmp->isLeaf) {
			curnum--;
			printf("[%d", tmp->key[0]);
			if (tmp->left) {
				enqueue(tmp->left);
				nextnum++;
			}
			if (tmp->key[1] != -1) {
				printf(",%d]", tmp->key[1]);
				if (tmp->middle) {
					enqueue(tmp->middle);
					nextnum++;
				}
			}
			else printf("]");
			if (tmp->right) {
				enqueue(tmp->right);
				nextnum++;
			}
			if (curnum == 0) {
				printf("\n");
				cur++;
				curnum = nextnum;
				nextnum = 0;
			}

		}
		else {
			printf("[%d", tmp->key[0]);
			if (tmp->key[1] != -1)printf(",%d", tmp->key[1]);
			if (tmp->key[2] != -1)printf(",%d]", tmp->key[2]);
			else printf("]");
		}
	}
	printf("\n");
}

int findKey(BTree t)
{
	while (!t->isLeaf) {
		t = t->left;
	}
	return t->key[0];
}

void Insert(int x, BTree& t)
{
	if (x < 0) return;
	BTree root;
	top = 0;
	if (!t) { //如果t是空的，创建新叶子节点
		t = (BTree)malloc(sizeof(struct BTreeNode));
		t->isLeaf = true;
		t->size = 1;
		t->key[0] = x;
		t->key[1] = t->key[2] = -1;
		t->left = t->middle = t->right = NULL;
		return;
	}
	BTree p = t;

	//step1:找到叶结点p
	while (!p->isLeaf) {
		push(p);//为什么要push?
		if (p->size == 2) {
			if (x < p->key[0])p = p->left;
			else if (x > p->key[0])p = p->right;
			else {
				printf("Key %d is duplicated\n", x);
				return;
			}
		}
		else if (p->size == 3) {
			if (x < p->key[0]) p = p->left;
			else if (x > p->key[0] && x < p->key[1])p = p->middle;
			else if (x > p->key[1])p = p->right;
			else {
				printf("Key %d is duplicated\n", x);
				return;
			}
		}
	}

	//step2:插入
	int i = p->size;
	//叶结点非满，直接插入  
	if (i != 3) {
		for (int j = 0; j < 2; j++) {
			if (x == p->key[j]) {
				printf("Key %d is duplicated\n", x);
				return;
			}
		}

		while (i >= 1 && x <= p->key[i - 1]) {
			p->key[i] = p->key[i - 1];
			i--;
		}
		//找到插入的位置  
		p->key[i] = x;
		p->size++;
		return;
	}
	//叶结点满
	else {
		//分裂叶结点
		//始终维护node1<node2 并且 node3<node4
		BTree node1 = p;
		BTree node2 = (BTree)malloc(sizeof(struct BTreeNode));
		BTree node3 = NULL, node4 = NULL;

		int a[4];
		a[0] = node1->key[0], a[1] = node1->key[1], a[2] = node1->key[2];
		if (x == a[0] || x == a[1] || x == a[2]) {
			printf("Key %d is duplicated\n", x);
			return;
		}
		int i = 2;
		for (; x < a[i] && i >= 0; i--) {
			a[i + 1] = a[i];
		}
		a[i + 1] = x;
		node1->key[0] = a[0], node1->key[1] = a[1], node1->key[2] = -1;
		node2->key[0] = a[2], node2->key[1] = a[3], node2->key[2] = -1;
		node2->left = node2->right = node2->middle = NULL;
		node1->size = 2, node2->size = 2;
		node1->isLeaf = true, node2->isLeaf = true;

		BTree parent = NULL, tmp = p;
		//tmp是当前结点
		while (tmp != NULL) {
			if (top) parent = pop();
			else parent = NULL;
			//case 1: 当前结点是根节点，根节点长高
			if (!parent) {
				root = (BTree)malloc(sizeof(struct BTreeNode));
				root->left = node1;
				root->right = node2;
				root->middle = NULL;
				root->isLeaf = false;
				root->size = 2;
				root->key[0] = findKey(root->right);
				root->key[1] = root->key[2] = -1;
				t = root;
				break;
			}
			//case 2 :当前结点的父节点不满
			else if (parent->size == 2) {
				if (x < parent->key[0]) {
					parent->middle = node2;
					parent->left = node1;
				}
				else {
					parent->middle = node1;
					parent->right = node2;
				}
				parent->size = 3;
				parent->key[2] = -1;
				parent->key[1] = findKey(parent->right);
				parent->key[0] = findKey(parent->middle);
				parent->isLeaf = false;
				break;
			}
			//case 3 :当前父节点已满，分裂父节点
			else if (parent->size == 3) {
				node3 = parent;
				node4 = (BTree)malloc(sizeof(struct BTreeNode));
				node4->isLeaf = node3->isLeaf = false;
				node3->size = node4->size = 2;
				if (x < parent->key[0]) {
					node4->left = parent->middle;
					node4->right = parent->right;
					node4->middle = NULL;
					node3->left = node1;
					node3->right = node2;
					node3->middle = NULL;
				}
				else if (x > parent->key[0] && x < parent->key[1]) {
					node4->left = node2;
					node4->right = parent->right;
					node4->middle = NULL;
					node3->left = parent->left;
					node3->right = node1;
					node3->middle = NULL;
				}
				else if (x > parent->key[1]) {
					node4->left = node1;
					node4->right = node2;
					node4->middle = NULL;
					node3->left = parent->left;
					node3->right = parent->middle;
					node3->middle = NULL;
				}
				node4->key[0] = findKey(node4->right);
				node4->key[1] = node4->key[2] = -1;
				node3->key[0] = findKey(node3->right);
				node3->key[1] = node3->key[2] = -1;
				node1 = node3, node2 = node4;
				//tmp = parent;
				tmp = node3;
			}
		}
	}
	return;
}

int main()
{
	front = rear = 0;
	BTree t = NULL;
	int n, x;
	int i;
	cin >> n;
	for (i = 0; i < n; i++) {
		cin >> x;
		Insert(x, t);
	}
	print(t);
	system("pause");
	return 0;
}