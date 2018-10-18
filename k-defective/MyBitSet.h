#ifndef __MY_BITSET_H__
#define __MY_BITSET_H__

#include "Common.h"

template <size_t BitNum>
class MyBitSet {
	protected:
	size_t WordNum;
	const static size_t WordLen = sizeof(uint64_t);
	uint64_t *field;
	void mallocMemory();
	void deleteMemory();

	public:
	MyBitSet();
	~MyBitSet();
	MyBitSet(const MyBitSet& other);
	void operator = (const MyBitSet &other);
	MyBitSet operator | (const MyBitSet &other);
	MyBitSet operator & (const MyBitSet &other);
	bool operator [] (size_t idx);
	void reset();
	void reset(size_t idx);
	void set();
	void set(size_t idx);
	size_t count();
	size_t nextBitPos();
};

template <size_t BitNum>
void MyBitSet<BitNum>::mallocMemory() {
	WordNum = BitNum / sizeof(uint64_t) + (BitNum % sizeof(uint64_t) > 0);
	field = new uint64_t[WordNum];
    memset(field, 0, WordLen * WordNum);
}

template <size_t BitNum>
void MyBitSet<BitNum>::deleteMemory() {
	delete[] field;
}

template <size_t BitNum>
MyBitSet<BitNum>::MyBitSet() {
	mallocMemory();
}

template <size_t BitNum>
MyBitSet<BitNum>::~MyBitSet() {
	deleteMemory();
}

template <size_t BitNum>
MyBitSet<BitNum>::MyBitSet(const MyBitSet<BitNum> &other) {
	mallocMemory();
	memcpy(field, other.field, WordLen * WordNum);
}

template <size_t BitNum>
void MyBitSet<BitNum>::operator = (const MyBitSet<BitNum> &other) {
	memcpy(field, other.field, WordLen * WordNum);
}

template <size_t BitNum>
MyBitSet<BitNum> MyBitSet<BitNum>::operator | (const MyBitSet<BitNum> &other) {
	MyBitSet<BitNum> ret = *this;
	for (int i = 0; i < WordLen; i++) ret.field[i] |= other.field[i];
	return ret;
}

template <size_t BitNum>
MyBitSet<BitNum> MyBitSet<BitNum>::operator & (const MyBitSet<BitNum> &other) {
	MyBitSet<BitNum> ret = *this;
	for (int i = 0; i < WordLen; i++) ret.field[i] &= other.field[i];
	return ret;
}

template <size_t BitNum>
bool MyBitSet<BitNum>::operator[] (size_t idx) {
	return (field[idx / WordLen] >> (idx % WordLen)) & 1;
}

template <size_t BitNum>
void MyBitSet<BitNum>::reset() {
	memset(field, 0, WordLen * WordNum);
}

template <size_t BitNum>
void MyBitSet<BitNum>::reset(size_t idx) {
	if ((field[idx / WordLen] >> (idx % WordLen)) & 1) {
		field[idx / WordLen] &= ~(1ul << (idx % WordLen));
	}
}

template <size_t BitNum>
void MyBitSet<BitNum>::set() {
	memset(field, 0xFF, WordLen * WordNum);
}

template <size_t BitNum>
void MyBitSet<BitNum>::set(size_t idx) {
	if (!((field[idx /WordLen] >> (idx % WordLen)) & 1)) {
		field[idx / WordLen] |= (1ul << (idx % WordLen));
	}
}

template <size_t BitNum>
size_t MyBitSet<BitNum>::count() {
	size_t ans = 0;
	for (int i = 0; i < WordNum; i++) ans += __builtin_popcountll(field[i]);
	return ans;
}

// 找到下一个不为1的二进制位, 然后将其置为1
// 如果没有这样的二进制位, 返回-1
template <size_t BitNum>
size_t MyBitSet<BitNum>::nextBitPos() {
	size_t ans = -1;
	for (int i = 0; i < WordNum; i++) if (field[i] != 0) {
		ans = __builtin_ctzll(field[i]);
		field[i] &= ~(1ul << ans);
		ans += i * WordLen;
		break;
	}
	return ans;
}

#endif
