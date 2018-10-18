#ifndef __BITSET_IMPLEMENT_H__
#define __BITSET_IMPLEMENT_H__

#include "Common.h"

template <class BitSet, class Base>
class BitSetImplement: public Base {
	protected:
    using Base::size;
    using Base::from;
    
	virtual void addVertexToSet(void *ptr, int idx);
	virtual void removeVertexFromSet(void *ptr, int idx);
	virtual void* setIntersection(void *A, void *B);
	virtual void* setUnion(void *A, void *B);
	virtual void* neighborSetOf(int idx);
	virtual bool existsInSet(void *ptr, int idx);
	virtual int sizeOfSet(void *ptr);
	virtual void deleteSet(void *ptr);
	virtual void* newSet();
	virtual void setCopyTo(void *src, void *dst);
	virtual size_t nextBitPos(void *ptr);
	virtual void __init__(void *P, void *C);
	public:
	BitSetImplement(int n);
};

// 这个函数在这个类型中没有实际用处, 在子类中具体实现
template <class Bitset, class Base>
size_t BitSetImplement<Bitset, Base>::nextBitPos(void *ptr) { return 0; }

template <class Bitset, class Base>
BitSetImplement<Bitset, Base>::BitSetImplement(int n) : Base(n) { }

template <class Bitset, class Base>
void BitSetImplement<Bitset, Base>::__init__(void *P, void *C) {
	Bitset *__P__ = (Bitset*) P, *__C__ = (Bitset*) C;
	__P__ -> reset(); 
	for (int i = 0; i < size; i++) __C__ -> set(i);
}

template <class Bitset, class Base>
void BitSetImplement<Bitset, Base>::addVertexToSet(void *ptr, int idx) {
	Bitset *__ptr__ = (Bitset*) ptr;
	__ptr__ -> set(idx);
}

template <class Bitset, class Base>
void BitSetImplement<Bitset, Base>::removeVertexFromSet(void *ptr, int idx) {
	Bitset *__ptr__ = (Bitset*) ptr;
	__ptr__ -> reset(idx);
}

template <class Bitset, class Base>
void* BitSetImplement<Bitset, Base>::setIntersection(void *A, void *B) {
	Bitset *__A__ = (Bitset*) A, *__B__ = (Bitset*) B, *__C__ = new Bitset();
	*__C__ = (*__A__) & (*__B__);
	return __C__;
}

template <class Bitset, class Base>
void* BitSetImplement<Bitset, Base>::setUnion(void *A, void *B) {
	Bitset *__A__ = (Bitset*) A, *__B__ = (Bitset*) B, *__C__ = new Bitset();
	*__C__ = (*__A__) | (*__B__);
	return __C__;
}

template <class Bitset, class Base>
void* BitSetImplement<Bitset, Base>::neighborSetOf(int idx) {
	Bitset *__Nei__ = new Bitset();
	for (int i = 0; i < from[idx].size(); i++) {
		int to = from[idx][i];
		__Nei__ -> set(to);
	}
	return __Nei__;
}

template <class Bitset, class Base>
bool BitSetImplement<Bitset, Base>::existsInSet(void *ptr, int idx) {
	Bitset *__ptr__ = (Bitset*) ptr;
	return (*__ptr__)[idx];
}

template <class Bitset, class Base>
int BitSetImplement<Bitset, Base>::sizeOfSet(void *ptr) {
	Bitset *__ptr__ = (Bitset*) ptr;
	return __ptr__ -> count();
}

template <class Bitset, class Base>
void BitSetImplement<Bitset, Base>::deleteSet(void *ptr) {
	Bitset *__ptr__ = (Bitset*) ptr;
	delete __ptr__;
}

template <class Bitset, class Base>
void* BitSetImplement<Bitset, Base>::newSet() {
	return new Bitset();
}

template <class Bitset, class Base>
void BitSetImplement<Bitset, Base>::setCopyTo(void *src, void *dst) {
	Bitset *__src__ = (Bitset*) src, *__dst__ = (Bitset*) dst;
	(*__dst__) = (*__src__);
}

#endif
