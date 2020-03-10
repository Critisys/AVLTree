/* Copyright (C) 2018
* Course: CO2003
* Author: Rang Nguyen
* Ho Chi Minh City University of Technology
*/

#include "TreeSet.h"

TreeSet::TreeSet()
{
	root = NULL;
	count = 0;
}


TreeSet::~TreeSet()
{
	clear();
}

void TreeSet::clearRec(AVLNode* root) {
	if (root != NULL) {
		clearRec(root->left);
		clearRec(root->right);
		delete root;
	}
}
void TreeSet::clear() {
	clearRec(root);
	root = NULL;
	count = 0;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

int height(AVLNode * node) {
	if (node == NULL) return -1;
	else return node->balance;
}

int diff(AVLNode * node) {
	if (node == NULL) return 0;
	int l_height = height(node->left);
	int r_height = height(node->right);
	int diff = l_height - r_height;
	return diff;
}

AVLNode * rotateLL(AVLNode * node) {
	AVLNode * temp;
	temp = node->left;
	node->left = temp->right;
	temp->right = node;
	node->balance = max(height(node->right), height(node->left)) + 1;
	temp->balance = max(height(temp->right), height(temp->left)) + 1;
	return temp;
}

AVLNode * rotateRR(AVLNode * node) {
	AVLNode * temp;
	temp = node->right;
	node->right = temp->left;
	temp->left = node;
	node->balance = max(height(node->right), height(node->left)) + 1;
	temp->balance = max(height(temp->right), height(temp->left)) + 1;
	return temp;
}

AVLNode * rotateLR(AVLNode* node) {
	AVLNode * temp = node->left;
	node->left = rotateRR(temp);
	return rotateLL(node);
}

AVLNode * rotateRL(AVLNode * node) {
	AVLNode * temp = node->right;
	node->right = rotateLL(temp);
	return rotateRR(node);
}

AVLNode * balancing(AVLNode * root) {
	int balance = diff(root);
	if (balance > 1) {
		if (diff(root->left) > 0) root = rotateLL(root);
		else root = rotateLR(root);
	}
	else if (balance < -1) {
		if (diff(root->right) < 0) root = rotateRR(root);
		else root = rotateRL(root);
	}

	return root;
}

AVLNode * recursionAdd(AVLNode * root, int val) {
	if (root == NULL) {
		root = new AVLNode(val);

	}
	else if (val < root->key) {
		root->left = recursionAdd(root->left, val);
		root = balancing(root);
	}
	else if (val > root->key) {
		root->right = recursionAdd(root->right, val);
		root = balancing(root);
	}

	root->balance = max(height(root->left), height(root->right)) + 1;

	return root;
}


int TreeSet::add(int val) {
	// TODO

	if (contains(val)) {
		return 0;
	}

	count++;
	root = recursionAdd(root, val);
	return 1;
}

bool TreeSet::contains(int val) {
	// TODO
	if (root == NULL) return false;
	AVLNode * temp = root;
	while (temp != NULL) {
		if (val == temp->key) return true;
		else if (val < temp->key) temp = temp->left;
		else if (val > temp->key) temp = temp->right;
	}
	return false;
}

void Copy(AVLNode * &node, AVLNode * root) {
	if (root == NULL) return;

	node = recursionAdd(node, root->key);
	Copy(node, root->left);
	Copy(node, root->right);
}

void TreeSet::copy(const TreeSet& set) {
	// TODO
	clear();

	Copy(this->root, set.root);
	return;
}

int TreeSet::first() {	
	if (root == NULL) {
		throw "NoSuchElementException";
	}
	// TODO

	AVLNode * temp = root;
	while (temp->left != NULL) {
		temp = temp->left;
	}

	return temp->key;
}

int TreeSet::last() {
	if (root == NULL) {
		throw "NoSuchElementException";
	}
	// TODO

	AVLNode * temp = root;
	while (temp->right != NULL) {
		temp = temp->right;
	}

	return temp->key;
}

int TreeSet::higher(int val) {
	// TODO
	AVLNode * temp = root;
	AVLNode * out = NULL;
	while (temp != NULL) {
		if (temp->key > val) {
			out = temp;
			temp = temp->left;
		}
		else {
			temp = temp->right;
		}
	}

	if (out == NULL) return -1;
	else return out->key;
}

int TreeSet::lower(int val) {
	// TODO
	AVLNode * temp = root;
	AVLNode * out = NULL;
	while (temp != NULL) {
		if (temp->key < val) {
			out = temp;
			temp = temp->right;
		}
		else {
			temp = temp->left;
		}
	}

	if (out == NULL) return -1;
	else return out->key;
}

AVLNode * inorderSuccessor(AVLNode * root) {
	AVLNode * temp = root;
	while (temp->left != NULL) temp = temp->left;
	return temp;
}

AVLNode * del(AVLNode * node, int val) {
	if (node == NULL) return NULL;
	else if (node->key < val) {
		node->right = del(node->right, val);
		node = balancing(node);
	}
	else if (node->key > val) { 
		node->left = del(node->left, val); 
		node = balancing(node);
	}
	else {
		if (node->left == NULL) {
			AVLNode * temp = node->right;
			delete node;
			return temp;
		}
		else if (node->right == NULL) {
			AVLNode * temp = node->left;
			delete node;
			return temp;
		}
		else {
			AVLNode * temp = inorderSuccessor(node->right);
			node->key = temp->key;
			int newVal = temp->key;
			node->right = del(node->right, newVal);
			node = balancing(node);
		}
	}

	node->balance = max(height(node->right), height(node->left)) + 1;
	return node;

}

int TreeSet::remove(int val) {
	// TODO

	if (contains(val)) {
		root = del(root, val);
		count--;
		return 1;
	}
	else return 0;

}


TreeSet* TreeSet::subSet(int fromVal, int toVal) {
	// TODO
	TreeSet * newTree = new TreeSet;

	if (contains(fromVal)) {
		newTree->add(fromVal);
	}
	
	int temp = higher(fromVal);
	while (temp < toVal && temp > -1) {
		newTree->add(temp);
		temp = higher(temp);
	}

	return newTree;
}


int TreeSet::size() {
	// TODO
	return count;
}

