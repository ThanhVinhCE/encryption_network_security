#include "rsa.h"
#include "utils.h"

using namespace std;

#define _TIME int starts,finishs;
#define STARTS_TIME starts=clock();
#define FINISHS_TIME finishs=clock(); cout<<(double)(finishs-starts)/CLOCKS_PER_SEC<<endl;

int main()
{

    createKeys("privatekey.txt","publickey.txt", 500);
    cryptRSA("plainText.txt","publickey.txt","encryptFile.txt");
    decryptRSA("encryptFile.txt","privatekey.txt","decryptFile.txt");
    // cout<<powerBigNumber(2034,1234);
    return 0;
}
