#ifndef TREE_H
#define TREE_H

#include <util.h>

template <typename TVal>
class Tree;

template <typename TVal>
class TNode {
	void rotate();
public:
	friend class Tree<TVal>;
	TNode* f;
	TNode* child[2];
	TVal val;
};

template <typename TVal>
class Tree {
	TNode<TVal>* root;
	void splay(TNode<TVal>* t, TNode<TVal>* p);
public:
	TNode<TVal>* find(u8(*check)(TVal*));
	void insert(TNode<TVal>* node, bool(*check)(TVal*));
};
#endif
