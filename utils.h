#ifndef UTILS_H
#define UTILS_H
#include <NTL/ZZ.h>
#include <iostream>
#include <gmpxx.h>
using namespace std;
using namespace NTL;

typedef mpz_class ZZZ;

ZZZ powerMoldulo(const ZZZ& p, ZZZ e, const ZZZ& m);
ZZZ euclideExtends(ZZZ m, ZZZ e);
ZZZ gcdBigNumber(ZZZ a, ZZZ b);
ZZZ powerBigNumber(ZZZ e, ZZZ b);
ZZZ createEncryptKey(long sizeNumber, ZZZ phi);
#endif
