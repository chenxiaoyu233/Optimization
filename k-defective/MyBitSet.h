#ifndef __MY_BITSET_H__
#define __MY_BITSET_H__

#include "Common.h"

template <size_t BitNum>
class MyBitSet {
	protected:
	size_t WordNum;
	const static size_t WordLen = sizeof(uint64_t);
	static bool initFlag;
	static size_t pos[1000];
	uint64_t *field;
	const static uint64_t mod_mask = 63;
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
bool MyBitSet<BitNum>::initFlag = false;

template <size_t BitNum>
size_t MyBitSet<BitNum>::pos[1000];

template <size_t BitNum>
void MyBitSet<BitNum>::mallocMemory() {
	if( !initFlag ) {
		for (uint64_t i = 0; i < 64; i++) {
			pos[(1ul << i) % 1000] = i;
		}
		initFlag = true;
	}
	WordNum = (BitNum >> 6) + ((BitNum & mod_mask) > 0);
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
	return (field[idx >> 6] >> (idx & mod_mask)) & 1;
}

template <size_t BitNum>
void MyBitSet<BitNum>::reset() {
	memset(field, 0, WordLen * WordNum);
}

template <size_t BitNum>
void MyBitSet<BitNum>::reset(size_t idx) {
	field[idx >> 6] &= ~(1ul << (idx & mod_mask));
}

template <size_t BitNum>
void MyBitSet<BitNum>::set() {
	memset(field, 0xFF, WordLen * WordNum);
}

template <size_t BitNum>
void MyBitSet<BitNum>::set(size_t idx) {
	field[idx >> 6] |= (1ul << (idx & mod_mask));
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
		//ans = __builtin_ctzll(field[i]);
		ans = pos[(field[i] & (-field[i])) % 1000];
		field[i] &= ~(1ul << ans);
		ans += i * WordLen;
		break;
	}
	return ans;
}

#endif
