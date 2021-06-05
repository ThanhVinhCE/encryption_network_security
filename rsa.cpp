#include "utils.h"
#include "primeGenerator.h"
#include "rsa.h"

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

    decrypt << M << endl;

    // ZZZ n,d,code;
    // string a;
    // /*read key from fileKey*/
    // PK>>a; n=a;
    // PK>>a; d=a;
    // /*read text from fileText*/
    // filetext>>a;
    // /*decrypto base64 to bits*/
    // code.set_str(de_convert_base64(a),2);
    // /*M=C^d mode n*/
    // mpz_powm(code.get_mpz_t(),code.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
    // /*convert to bit and crop length of KEY{128,192,256}*/
    // a=code.get_str(2);
    // a=a.substr(2048-KEY,KEY);
    // /********************/
    // code.set_str(a,2);
    // decrypt<<code.get_str();

    fileEnc.close();
    decrypt.close();
    PK.close();
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
    M = conv<ZZ>(TEXT.c_str());
    /*linked head16bit with bit random + KEY{128,192,256}*/
    // TEXT = headCode;
    // ZZ numbersite;
    // RandomBits(numbersite,(2032-KEY));
    // stringstream buffer;
    // buffer << numbersite;
    // ZZZ code;
    // code = buffer.str();
    // int strlen = code.get_str(2).length();
    // for(;strlen < 2032-KEY; strlen++) TEXT+='1';
    // TEXT+=code.get_str(2);
    // filetext >> headCode;
    // code = headCode;
    // TEXT+=code.get_str(2);
    // /*********************/
    // /*C=M^e mod n*/
    // code.set_str(TEXT,2);
    // mpz_powm(code.get_mpz_t(),code.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
    // filecrypt << convert_base64(code.get_str(2)) << endl;

    C = PowerModula(M, e, n);

    //string encryptText = 
    
    filecrypt << C << endl;

    filecrypt.close();
    filetext.close();
    filekey.close();
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

// string convert_base64(const string &code)
// {
//     string decode;
//     ZZZ convert_number;
//     for(int i=0;i<code.length();i+=6)
//     {

//         convert_number.set_str(code.substr(i,6),2);
//         long ka=convert_number.get_ui();
//         if(ka<26) ka+=65;
//         else if(ka<52)ka+=71;
//         else if(ka<62)ka-=4;
//         else if(ka==62)ka=43;
//         else if(ka==63)ka=47;
//         decode+=(char)ka;
//     }
//     return decode;
// }
// string de_convert_base64(const string &code)
// {
//     string decode;
//     ZZZ convert_number;
//     long ka;
//     for(int i=0;i<code.length();i++)
//     {
//         ka=code[i];
//         if(ka<='Z'&& ka>='A') ka-=65;
//         else if(ka>='a'&&ka<='z')ka-=71;
//         else if(ka<='9'&&ka>='0')ka+=4;
//         else if(ka=='+')ka=62;
//         else if(ka=='/')ka=63;
//         convert_number=ka;
//         string ma=convert_number.get_str(2);
//         if(i<code.length()-1)
//         {
//             for(int j=0;j<6-ma.length();j++)
//             {
//                 decode+='0';
//             }
//         }
//         decode+=ma;
//     }
//     return decode;
// }
