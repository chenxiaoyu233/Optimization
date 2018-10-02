#ifndef __K_DEFECTIVE_BITSET_H__
#define __K_DEFECTIVE_BITSET_H__

#include "Common.h"

template <class Bitset>
class KDefectiveBitset : public KDefectiveBase {
	protected:
	virtual void addVertexToSet(void *ptr, int idx);
	virtual void removeVertexFromSet(void *ptr, int idx);
	virtual void* setIntersection(void *A, void *B);
	virtual void* neighborSetOf(int idx);
	virtual bool existsInSet(void *ptr, int idx);
	virtual int sizeOfSet(void *ptr);
	virtual void deleteSet(void *ptr);
	virtual void* newSet();
	virtual void setCopyTo(void *src, void *dst);
	public:
	KDefectiveBitset(int n);
};

template <class Bitset>
KDefectiveBitset::KDefectiveBitset(int n) : KDefectiveBase(n) { }

template <class Bitset>
void KDefectiveBitset::addVertexToSet(void *ptr, int idx) {
	Bitset *__ptr__ = (Bitset*) ptr;
	__ptr__ -> set(idx);
}

template <class Bitset>
void KDefectiveBitset::removeVertexFromSet(void *ptr, int idx) {
	Bitset *__ptr__ = (Bitset*) ptr;
	__ptr__ -> reset(idx);
}

template <class Bitset>
void* KDefectiveBitset::setIntersection(void *A, void *B) {
	Bitset *__A__ = (Bitset*) A, *__B__ = (Bitset*) B, *__C__ = new Bitset();
	*__C__ = (*__A__) & (*__B__);
	return __C__;
}

template <class Bitset>
void* KDefectiveBitset::neighborSetOf(int idx) {
	Bitset *__Nei__ = new Bitset();
	for (int i = 0; i < from[idx].size(); i++) {
		int to = from[idx][i];
		__Nei__ -> set(to);
	}
	return __Nei__;
}

template <class Bitset>
bool KDefectiveBitset::existsInSet(void *ptr, int idx) {
	Bitset *__ptr__ = (Bitset*) ptr;
	return (*__ptr__)[idx];
}

template <class Bitset>
void KDefectiveBitset::deleteSet(void *ptr) {
	Bitset *__ptr__ = (Bitset*) ptr;
	delete __ptr__;
}

template <class Bitset>
void* KDefectiveBitset::newSet() {
	return new Bitset();
}

template <class Bitset>
void KDefectiveBitset::setCopyTo(void *src, void *dst) {
	Bitset *__src__ = (Bitset*) src, *__dst__ = (Bitset*) dst;
	(*__dst__) = (*__src__);
}

#endif
