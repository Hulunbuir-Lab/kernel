#ifndef TREE_H
#define TREE_H

#include <util.h>
#include <functional>

template <typename TVal>
class Tree;

template <typename TVal>
class TNode {
	void rotate();
public:
	friend class Tree<TVal>;
	TNode(const TVal &x): val(x){};
	TNode<TVal>* f;
	TNode<TVal>* child[2];
	TVal val;
};

template <typename TVal>
class Tree {
	TNode<TVal>* root;
	void splay(TNode<TVal>* t, TNode<TVal>* p);
public:
	TNode<TVal>* find(std::function<u8 (TVal *)> check);
	void insert(TNode<TVal>* node, std::function<bool (TVal *)> check);
};
#endif
