#ifndef __STL_SET_IMPLEMENT_H__
#define __STL_SET_IMPLEMENT_H__

#include "Common.h"

// 使用STL SET实现的集合数据结构

template <class Set, class Base>
class STLSetImplement: public Base {
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
	STLSetImplement(int n);
};

template <class Set, class Base>
STLSetImplement<Set, Base>::STLSetImplement(int n):Base(n) { }

template <class Set, class Base>
void STLSetImplement<Set, Base>::addVertexToSet(void *ptr, int idx) {
	Set *__ptr__ = (Set*) ptr;
	__ptr__ -> insert(idx);
}

template <class Set, class Base>
void STLSetImplement<Set, Base>::removeVertexFromSet(void *ptr, int idx) {
	Set *__ptr__ = (Set*) ptr;
	if (__ptr__ -> count(idx)) __ptr__ -> erase(idx);
}

template <class Set, class Base>
void* STLSetImplement<Set, Base>::setIntersection(void *A, void *B) {
	Set *__A__ = (Set*) A, *__B__ = (Set*) B, *__C__ = (Set*) newSet();
	__C__ -> clear(); 
	for (int i = 0; i < size; i++) if (__A__ -> count(i) && __B__ -> count(i)) {
		__C__ -> insert(i);
	}
	return __C__;
}

template <class Set, class Base>
void* STLSetImplement<Set, Base>::setUnion(void *A, void *B) {
	Set *__A__ = (Set*) A, *__B__ = (Set*) B, *__C__ = (Set*) newSet();
	__C__ -> clear(); 
	for (int i = 0; i < size; i++) if (__A__ -> count(i) || __B__ -> count(i)) {
		__C__ -> insert(i);
	}
	return __C__;
}

template <class Set, class Base>
void* STLSetImplement<Set, Base>::neighborSetOf(int idx) {
	Set *__ptr__ = (Set*) newSet();
	__ptr__ -> clear();
	for (auto to: from[idx]) __ptr__ -> insert(to);
	return __ptr__;
}

template <class Set, class Base>
bool STLSetImplement<Set, Base>::existsInSet(void *ptr, int idx) {
	Set *__ptr__ = (Set*) ptr;
	return __ptr__ -> count(idx);
}

template <class Set, class Base>
int STLSetImplement<Set, Base>::sizeOfSet(void *ptr) {
	Set *__ptr__ = (Set*) ptr;
	return __ptr__ -> size();
}

template <class Set, class Base>
void STLSetImplement<Set, Base>::deleteSet(void *ptr) {
	Set *__ptr__ = (Set*) ptr;
	delete __ptr__;
}

template <class Set, class Base>
void* STLSetImplement<Set, Base>::newSet() {
	return new Set();
}

template <class Set, class Base>
void STLSetImplement<Set, Base>::setCopyTo(void *src, void *dst) {
	Set *__src__ = (Set*) src, *__dst__ = (Set*) dst;
	(*__dst__) = (*__src__);
}

template <class Set, class Base>
size_t STLSetImplement<Set, Base>::nextBitPos(void *ptr) { return 0; }

template <class Set, class Base>
void STLSetImplement<Set, Base>::__init__(void *P, void *C) {
	Set *__P__ = (Set*) P, *__C__ = (Set*) C;
	__P__ -> clear();
	for (int i = 0; i < size; i++) __C__ -> insert(i);
}

#endif
