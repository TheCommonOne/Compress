#include<iostream>
#include"HuffmanTreeNode.h"
#include"minHeap.h"
#include<string>
#include<stack>
using namespace std;

template<class dataType>
class HuffmanTree {
public:
	HuffmanTreeNode<dataType> *root;
	void MergeTree(HuffmanTreeNode<dataType> *ht1,
		HuffmanTreeNode<dataType> *ht2,
		HuffmanTreeNode<dataType>* parent);
	void DeleteTree(HuffmanTreeNode<dataType> *root);
	HuffmanTree(dataType *weight, int n);
	void preFirst(HuffmanTreeNode<dataType>*);
	virtual ~HuffmanTree() { DeleteTree(root); }
	HuffmanTreeNode<dataType> * search(int i);
	void coding(string *);
};

template<class dataType>
void HuffmanTree<dataType>::MergeTree(HuffmanTreeNode<dataType> *ht1,
	HuffmanTreeNode<dataType> *ht2,
	HuffmanTreeNode<dataType>* parent) {
	parent->element = ht1->element + ht2->element;
	
	parent->left_child = ht1;
	parent->left_child->parent = parent;
	
	parent->right_child = ht2;
	parent->right_child->parent = parent;
	
	parent->data = -1;
}

template<class dataType>
void HuffmanTree<dataType>::DeleteTree(HuffmanTreeNode<dataType> *root) {
	if (root)
	{
		DeleteTree(root->left_child);
		DeleteTree(root->right_child);
		delete root;
	}
}
template<class dataType>
HuffmanTree<dataType>::HuffmanTree(dataType *weight, int n) {
	minHeap<HuffmanTreeNode<dataType>*> heap(n);
	HuffmanTreeNode<dataType> *parent, *first_child, *second_child;
	HuffmanTreeNode<dataType> *nodeList = new HuffmanTreeNode<dataType>[n];

	HuffmanTreeNode<dataType> *temp;
	for (int i = 0; i < n; i++)
	{
		nodeList[i].element = weight[i];
		nodeList[i].data = i;
		nodeList[i].parent = nodeList[i].left_child = nodeList[i].right_child = NULL;
		heap.Insert(nodeList+i);
	}
	heap.initiate();
	for (int i = 0; i < n - 1; i++)
	{
		parent = new HuffmanTreeNode<dataType>;
		/*if (temp.element == 270)
			cout << 1;
		if (temp.element == 546)
			cout << 2;*/
		MergeTree(heap.removeMin(), heap.removeMin(), parent);
		heap.Insert(parent);
	}
	temp = heap.removeMin();
	this->root = new HuffmanTreeNode<dataType>(temp);
	//delete[] node;
}

template<class dataType>
HuffmanTreeNode<dataType> * HuffmanTree<dataType>::search(int i) {
	HuffmanTreeNode<dataType>*temp = this->root;
	stack<HuffmanTreeNode<dataType>*> stk;
	stk.push(temp);
	while (temp || !stk.empty())
	{
		if (i == temp->data&&temp->left_child ==NULL&&temp->right_child==NULL)
		{
			return temp;
		}
		if (temp->right_child)
			stk.push(temp->right_child);
		temp = temp->left_child;
		if (!temp)
		{
			if (stk.empty())
				return NULL;
			temp = stk.top();
			stk.pop();
		}
	}
	return NULL;
}

template<class dataType>
void HuffmanTree<dataType>::coding(string * st) {
	int counter = 0;
	HuffmanTreeNode<dataType>*temp;
	stack<char> stk;
	char tp;
	for (; counter < 256;)
	{
		temp  =search(counter);
		while (temp->parent)
		{
			if (temp->parent->left_child == temp)
			{
				stk.push('0');
			}
			else
				stk.push('1');
			temp = temp->parent;
		}
		while (!stk.empty())
		{
			tp = stk.top();
			string tmp(&tp,1);
			st[counter].append(tmp);
			stk.pop();
		}
		counter++;
	}
}
#pragma once