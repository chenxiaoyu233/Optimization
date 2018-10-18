#ifndef __MY_BITSET_IMPLEMENT_H__
#define __MY_BITSET_IMPLEMENT_H__

#include "Common.h"
#include "BitSetImplement.h"

template <class BitSet, class Base>
class MyBitSetImplement: public BitSetImplement<BitSet, Base> {
	protected:
	virtual size_t nextBitPos(void *ptr);
	public:
	MyBitSetImplement(int n);
};

template <class Bitset, class Base>
MyBitSetImplement<Bitset, Base>::MyBitSetImplement(int n): BitSetImplement<Bitset, Base>(n) { }

template <class Bitset, class Base>
size_t MyBitSetImplement<Bitset, Base>:: nextBitPos(void *ptr) {
	Bitset *__ptr__ = (Bitset*) ptr;
	return __ptr__ -> nextBitPos();
}

#endif
