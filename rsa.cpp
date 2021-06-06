#include "utils.h"
#include "primeGenerator.h"
#include "rsa.h"

ZZ stringToNumber(string str)
{
  ZZ number = conv<ZZ>(str[0]);
  long len = str.length();
  for(long i = 1; i < len; i++)
  {
      number *= 128;
      number += conv<ZZ>(str[i]);
  }

  return number;
}

string numberToString(ZZ num)
{
    long len = ceil(log(num)/log(128));
    char str[len];
    //cout << num << endl;
    for(long i = len-1; i >= 0; i--)
    {
        //cout << num%16 << endl;
        str[i] = conv<int>(num % 128);
        num /= 128;
    }
    string s = "";
    for(int i = 0; i < len; i++){
      s = s + str[i];
    }

    return s;
}

ZZ PowerModula(const ZZ& a, const ZZ& e, const ZZ& n)
{
    // (a power e) mod n
   if (e == 0) return ZZ(1);

   long k = NumBits(e);

   ZZ res;
   res = 1;

   for (long i = k-1; i >= 0; i--) {
      res = (res*res) % n;
      if (bit(e, i) == 1) res = (res*a) % n;
   }

   if (e < 0)
      return InvMod(res, n);
   else
      return res;
}

void createKeys(char* nameFilePrivateKey, char* nameFilePublicKey, long sizeNumber){

    PrimeGenerator primeGenerator;
    ofstream privateFile(nameFilePrivateKey);
    ofstream publicFile(nameFilePublicKey);

    ZZZ p = primeGenerator.generate_strong_prime(sizeNumber);
    ZZZ q = primeGenerator.generate_strong_prime(sizeNumber);

    ZZZ n = p*q;
    ZZZ phi = (p-1)*(q-1);

    ZZZ e;
    e = createEncryptKey(sizeNumber,phi);

    ZZZ d;
    do
    {
        d = euclideExtends(phi,e);
        e = createEncryptKey(sizeNumber,phi);
    } while (d == -1);

    /* private key */
    privateFile << n.get_str() << endl;
    privateFile << d.get_str() << endl;

    privateFile.clear();
    privateFile.close();

    /* public key */

    publicFile << n.get_str() << endl;
    publicFile << e.get_str() << endl;

    publicFile.clear();
    publicFile.close();

}

void cryptRSA (char* fileMText,char* fileKey,char *fileCrypt)
{
    //string headCode = "1000010010000100"; /*head16bit*/
    ifstream filetext(fileMText);
    ifstream filekey(fileKey);
    ofstream filecrypt(fileCrypt);
    if(!filetext){
        cout<<"Fail!Filetext isn't exist";
        return;
    }
    if(!filekey){
        cout<<"Fail!Filekey isn't exist";
        return;
    }
    /*read key from fileKey*/
    string TEXT;
    ZZ n, e, M, C;

    // get n
    filekey >> TEXT;
    n = conv<ZZ>(TEXT.c_str());

    // get e
    filekey >> TEXT;
    e = conv<ZZ>(TEXT.c_str());

    //get M
    filetext >> TEXT;

    M = stringToNumber(TEXT);

    C = PowerModula(M, e, n);
    
    filecrypt << C << endl;

    filecrypt.close();
    filetext.close();
    filekey.close();
}

void decryptRSA (char* fileEncrypt,char* fileKey,char *fileDecrypt)
{
    ifstream fileEnc(fileEncrypt);
    ofstream decrypt(fileDecrypt);
    ifstream PK(fileKey);
     if(!PK){
        cout<<"Fail!publicKey.txt isn't exist";
        return;
    }
    if(!fileEnc){
        cout<<"Fail!filetext.txt isn't exist";
        return;
    }

    ZZ n, d, C, M;
    string text;

    // get C
    fileEnc >> text;
    C = conv<ZZ>(text.c_str());

    // get n
    PK >> text;
    n = conv<ZZ>(text.c_str());

    //get d
    PK >> text;
    d = conv<ZZ>(text.c_str());

    
    M = PowerModula(C, d, n);

    string ZZ_str = numberToString(M);

    decrypt << ZZ_str << endl;

    fileEnc.close();
    decrypt.close();
    PK.close();
}