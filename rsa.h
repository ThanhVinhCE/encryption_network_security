#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <NTL/ZZ.h>
#include <gmp.h>
#include <gmpxx.h>
#include <string.h>

using namespace std;
using namespace NTL;

#define KEY 128

void createKeys(char* nameFilePrivateKey, char* nameFilePublicKey,long sizeNumber);
void cryptRSA (char* fileMText,char* fileKey,char *fileCrypt);
void decryptRSA (char* fileText,char* fileKey,char *fileDecrypt);
// string convert_base64(const string &code);
// string de_convert_base64(const string &code);
ZZ PowerModula(const ZZ& a, const ZZ& e, const ZZ& n);
#endif
