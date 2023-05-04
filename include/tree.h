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


template <typename TVal>
void TNode<TVal>::rotate() {
	TNode<TVal> *f = this->f;
	if (f == nullptr) return;
	TNode<TVal> *ff = f->f;
	bool k = (this == f->child[1]);
	bool kk = (f == ff->child[1]);

	f->child[k] = this->child[k^1];
	if (this->child[k^1] != nullptr) this->child[k^1]->f = f;
	this->f = ff;
	if (ff != nullptr) ff->child[kk] = this;
	this->child[k^1] = f;
	f->f = this;
}

template <typename TVal>
void Tree< TVal >::splay(TNode< TVal >* t, TNode< TVal >* p)
{
	while (t -> f != p) {
		TNode<TVal> *f = t->f;
		TNode<TVal> *ff = f->f;
		if (ff != nullptr && ((ff->child[1] == f) ^ (f->child[1] == t))) {
			f -> rotate();
		}
		t->rotate();
	}
}

template <typename TVal>
TNode<TVal> * Tree<TVal>::find(std::function<u8 (TVal *)> check)
{
	TNode<TVal> *pt = root;
	while (pt != nullptr) {
		switch (check(&pt->val)){
			case 0:
				pt = pt->child[0];
				break;
			case 1:
				return pt;
				break;
			case 2:
				pt = pt->child[1];
		}
	}
	return pt;
}

template<typename TVal>
void Tree<TVal>::insert(TNode<TVal>* node, std::function<bool (TVal *)> check)
{
	TNode<TVal> *pt = root, *f;
	bool k;
	while (pt != nullptr) {
		f = pt;
		if (check(&pt->val)) {
			k = 0;
			pt = pt->child[0];
		}
		else {
			k = 1;
			pt = pt->child[1];
		}
	}
	if (f == nullptr) root = node;
	else f->child[k] = node;
}

#endif
