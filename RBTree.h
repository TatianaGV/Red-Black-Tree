#pragma once
#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include<iterator>
#include <queue>
#include <stack>
#include <set>
#include <fstream>
#include <string>


template<typename T>
class RBTree
{
	enum Color { BLACK, RED };

	struct TreeNode {
		T data;
		Color nodeColor;
		TreeNode* parent;
		TreeNode* left;
		TreeNode* right;
		TreeNode(T d, Color c, TreeNode* p, TreeNode* l, TreeNode* r) : data(d), nodeColor(c), parent(p), left(l), right(r) {}
	};

	TreeNode* root;

	class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
		TreeNode* data;
		bool last = false;
	public:
		iterator(TreeNode* d, bool lst = false) : data(d), last(lst) {}

		TreeNode* operator*() {
			return data;
		}

		friend bool operator==(const iterator& it1, const iterator& it2) {
			return (it1.data == it2.data && it1.last == it2.last) || (it1.last && it2.last);
		}

		friend bool operator!=(const iterator& it1, const iterator& it2) {
			return !(it1 == it2);
		}

		iterator operator++() {
			if (data == nullptr) return iterator(nullptr, false);
			TreeNode* it = data;
			if (it->right) {
				it = it->right;
				while (it->left) it = it->left;
				data = it;
				last = false;
				return (*this);
			}
			if (it->parent) {
				TreeNode* tmp = new TreeNode(it->data, BLACK, it, nullptr, nullptr);
				while (it->parent && it->parent->left != it) it = it->parent;
				if (it->parent) it = it->parent;
				else {
					data = tmp;
					last = true;
					return (*this);
				}
				data = it;
				last = false;
				return (*this);
			}
		}

		iterator operator--() {
			TreeNode* it = data;
			if (last) {
				while (data->right) data = data->right;
				last = false;
				return *this;
			}
			if (it->left) {
				it = it->left;
				while (it->right) it = it->right;
				data = it;
				last = false;
				return (*this);
			}
			if (it->parent) {
				if (last) {
					data = it;
					last = false;
					return(*this);
				}
				while (it->parent && it->parent->right != it) it = it->parent;
				if (it->parent) it = it->parent;
				else {
					data = nullptr;
					last = false;
					return (*this);
				}
				data = it;
				last = false;
				return(*this);
			}
		}

		iterator prev() {
			iterator it(data, last);
			return --it;
		}

		iterator next() {
			iterator it(data, last);
			return ++it;
		}
	};

	void print_LRR_help(TreeNode* root) {
		if (root) {
			print_LRR_help(root->left);
			cout << root->data << " ";
			print_LRR_help(root->right);
		}
	}

	//for destructor
	void deleteTree(TreeNode* root) {
		if (root) {
			deleteTree(root->left);
			deleteTree(root->right);
			delete root;
		}
	}

	bool equalNodes(TreeNode* r1, TreeNode* r2) {
		if (r1 && r2) {
			if (r1->parent == nullptr && r2->parent == nullptr || r1->parent && r2->parent && r1->parent->data == r2->parent->data)
				if (r1->left == nullptr && r2->left == nullptr || r1->left && r2->left && r1->left->data == r2->left->data)
					if (r1->right == nullptr && r2->right == nullptr || r1->right && r2->right && r1->right->data == r2->right->data)
						return true;
		}
		return false;
	}

	bool isEqual_help(TreeNode* root1, TreeNode* root2) {
		if (root1 == nullptr && root2 == nullptr) return true;
		if (equalNodes(root1, root2)) {
			return isEqual_help(root1->left, root2->left) && isEqual_help(root1->right, root2->right);
		}
		else return false;
	}

	void levelPrint(TreeNode *root) {
		if (root == nullptr) return;
		std::queue<TreeNode *> q;
		q.push(root);

		while (!q.empty()) {
			TreeNode* tmp = q.front();
			q.pop();
			cout << tmp->data << " ";
			if (tmp->left != nullptr)
				q.push(tmp->left);
			if (tmp->right != nullptr)
				q.push(tmp->right);
		}
	}

	int height(TreeNode* root) {
		if (!root) return 0;
		if (root->right == nullptr && root->left == nullptr)
			return 1;
		int l = 1 + height(root->left);
		int r = 1 + height(root->right);
		return l < r ? r : l;
	}

	//левый простой поворот вокруг х
	void rotateLeft(TreeNode* x) {
		TreeNode* y = x->right;
		x->right = y->left;
		if (y->left) y->left->parent = x;
		if (y) y->parent = x->parent;
		if (x->parent) {
			if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
		}
		else {
			root = y;
		}
		y->left = x;
		if (x) x->parent = y;
	}

	//правый простой поворот вокруг х
	void rotateRight(TreeNode* x) {
		TreeNode* y = x->left;
		x->left = y->right;
		if (y->right) y->right->parent = x;
		if (y) y->parent = x->parent;
		if (x->parent) {
			if (x == x->parent->right)
				x->parent->right = y;
			else
				x->parent->left = y;
		}
		else {
			root = y;
		}
		y->right = x;
		if (x) x->parent = y;
	}


	//ребалансировка К-Ч древа
	int insertFixup(TreeNode* x) {
		int count = 0;
		while (x != root && x->parent->nodeColor == RED) {
			if (x->parent == x->parent->parent->left) {
				TreeNode* y = x->parent->parent->right;
				if (y && y->nodeColor == RED) {
					x->parent->nodeColor = BLACK;
					y->nodeColor = BLACK;
					x->parent->parent->nodeColor = RED;
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->right) {
						x = x->parent;
						rotateLeft(x);
						count++;
					}
					x->parent->nodeColor = BLACK;
					x->parent->parent->nodeColor = RED;
					rotateRight(x->parent->parent);
					count++;
				}
			}
			else {
				TreeNode* y = x->parent->parent->left;
				if (y && y->nodeColor == RED) {
					x->parent->nodeColor = BLACK;
					y->nodeColor = BLACK;
					x->parent->parent->nodeColor = RED;
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->left) {
						x = x->parent;
						rotateRight(x);
						count++;
					}
					x->parent->nodeColor = BLACK;
					x->parent->parent->nodeColor = RED;
					rotateLeft(x->parent->parent);
					count++;
				}
			}
		}
		root->nodeColor = BLACK;
		return count;
	}

	//удаление из К-Ч дерева
	int deleteFixup(TreeNode *p)
	{
		int count = 0;
		TreeNode *s;
		while (p && p != root && p->nodeColor == BLACK)	
		{
			if (p->parent->left == p) 
			{
				s = p->parent->right;
				if (s->nodeColor == RED) 
				{
					s->nodeColor = BLACK;
					p->parent->nodeColor = RED;
					rotateLeft(p->parent);
					count++;
					s = p->parent->right;
				}
				if (s->right->nodeColor == BLACK && s->left->nodeColor == BLACK)
				{
					s->nodeColor = RED;
					p = p->parent;
				}
				else
				{
					if (s->right->nodeColor == BLACK)
					{
						s->left->nodeColor = BLACK;
						s->nodeColor = RED;
						rotateRight(s);
						count++;
						s = p->parent->right;
					}
					s->nodeColor = p->parent->nodeColor;
					p->parent->nodeColor = BLACK;
					s->right->nodeColor = BLACK;
					rotateLeft(p->parent);
					count++;
					p = root;
				}
			}
			else
			{
				s = p->parent->left;
				if (s->nodeColor == RED)
				{
					s->nodeColor = BLACK;
					p->parent->nodeColor = RED;
					rotateRight(p->parent);
					count++;
					s = p->parent->left;
				}
				if (s->left->nodeColor == BLACK && s->right->nodeColor == BLACK)
				{
					s->nodeColor = RED;
					p = p->parent;
				}
				else
				{
					if (s->left->nodeColor == BLACK)
					{
						s->right->nodeColor = BLACK;
						s->nodeColor = RED;
						rotateLeft(s);
						count++;
						s = p->parent->left;
					}
					s->nodeColor = p->parent->nodeColor;
					p->parent->nodeColor = BLACK;
					s->left->nodeColor = BLACK;
					rotateRight(p->parent);
					count++;
					p = root;
				}
			}
			p->nodeColor = BLACK;
			root->nodeColor = BLACK;
		}
		return  count;
	}

	TreeNode* successor(TreeNode *p)
	{
		TreeNode *y = nullptr;
		if (p->left != nullptr)
		{
			y = p->left;
			while (y->right != nullptr)
				y = y->right;
		}
		else
		{
			y = p->right;
			while (y->left != nullptr)
				y = y->left;
		}
		return y;
	}

	void help_input(std::ofstream & out, TreeNode* root)
	{
		if (root) {
			help_input(out, root->left);
			out << root->data << "\n";
			help_input(out, root->right);
		}
	}

public:

	RBTree() : root(nullptr) {}

	~RBTree() {
		deleteTree(root);
	}

	int insert(T elem) {
		TreeNode* tmp = new TreeNode(elem, RED, nullptr, nullptr, nullptr);
		TreeNode* parent = nullptr;
		if (!root) root = tmp;
		else {
			TreeNode* current;
			current = root;
			while (current) {
				parent = current;
				if (tmp->data > current->data) current = current->right;
				else current = current->left;
			}
			if (tmp->data > parent->data) parent->right = tmp;
			else parent->left = tmp;
			tmp->parent = parent;
		}
		return insertFixup(tmp);
	}

	void printLRR() { print_LRR_help(root); }

	iterator min() {
		TreeNode* curr = root;
		while (curr->left) curr = curr->left;
		return iterator(curr, false);
	}

	iterator max() {
		TreeNode* curr = root;
		while (curr->right) curr = curr->right;
		return iterator(curr, false);
	}

	iterator begin() { return min(); }

	iterator end() { return iterator(root, true); }

	iterator find(T elem) {
		if (!root) return end();
		TreeNode* curr = root;
		while (curr) {
			if (curr->data > elem) curr = curr->left;
			else if (curr->data < elem) curr = curr->right;
			else return iterator(curr, false);
		}
		return end();
	}

	iterator upper_bound(T x) {
		TreeNode* tmp = nullptr;
		TreeNode* curr = root;
		while (curr) {
			if (x > curr->data)
				curr = curr->right;
			else if (x < curr->data) {
				tmp = curr;
				curr = curr->left;
			}
			else {
				tmp = curr;
				curr = nullptr;
			}
		}
		if (tmp) return iterator(tmp, false);
		else return end();
	}

	iterator lower_bound(T x) {
		TreeNode* tmp = nullptr;
		TreeNode* curr = root;
		while (curr) {
			if (x < curr->data)
				curr = curr->left;
			else if (x > curr->data) {
				tmp = curr;
				curr = curr->right;
			}
			else {
				tmp = curr;
				curr = nullptr;
			}
		}
		if (tmp) return iterator(tmp, false);
		else return end();
	}

	int deleteNode(T elem) 
	{
		TreeNode *z = *find(elem);
		TreeNode *x, *y;

		if (!z || z == nullptr) return 0;


		if (z->left == nullptr || z->right == nullptr) {
			/* y has a NIL node as a child */
			y = z;
		}
		else {
			/* find tree successor with a NIL node as a child */
			y = z->right;
			while (y->left != nullptr) y = y->left;
		}

		/* x is y's only child */
		if (y->left != nullptr)
			x = y->left;
		else
			x = y->right;

		/* remove y from the parent chain */
		if (x != nullptr)
			x->parent = y->parent;
		if (y->parent)
			if (y == y->parent->left)
				y->parent->left = x;
			else
				y->parent->right = x;
		else
			root = x;

		if (y != z) z->data = y->data;


		if (y->nodeColor == BLACK) {
			free(y);
			return deleteFixup(x);
		}

		free(y);
		return 0;
	}


	int remove(T elem)
	{
		int count = 0;
		if (root == nullptr) return count;
		TreeNode *y = nullptr, *q = nullptr;
		iterator it = find(elem);
		TreeNode *p = *it;
		if (it == end()) return count;
		else {
			if (p->left == nullptr || p->right == nullptr) y = p;
			else y = successor(p);
			if (y->left != nullptr)
				q = y->left;
			else
			{
				if (y->right != nullptr)
					q = y->right;
				else
					q = nullptr;
			}
			if (q != nullptr)
				q->parent = y->parent;
			if (y->parent == nullptr)
				root = q;
			else
			{
				if (y == y->parent->left)
					y->parent->left = q;
				else
					y->parent->right = q;
			}
			if (y != p)
			{
				p->nodeColor = y->nodeColor;
				p->data = y->data;
			}
			if (y->nodeColor == RED)
				return deleteFixup(y);
		}
		return count;
	}

	void print_stack() {
		std::set<TreeNode*> vert;
		std::stack<TreeNode*> st;
		st.push(root);
		vert.insert(root);
		while (!st.empty()) {
			TreeNode* node = st.top();
			if (node->right && vert.find(node->right) == vert.end()) {
				st.push(node->right);
				continue;
			}
			TreeNode* curr = st.top();
			if (curr) {
				cout << curr->data << " ";
				vert.insert(curr);
			}
			st.pop();
			if (node->left && vert.find(node->left) == vert.end()) st.push(node->left);
		}
	}

	bool isEqual(const RBTree* tree) { 

		return isEqual_help(root, tree->root); 

	}

	void printQueue() { levelPrint(root); }

	int tree_height() {
		return height(root);
	}

};


#endif