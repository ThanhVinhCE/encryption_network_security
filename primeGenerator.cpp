#include "primeGenerator.h"

PrimeGenerator::PrimeGenerator(void)
{

}
void PrimeGenerator::renew()
{
    /*random prime with NTL/ZZ.h*/
    ZZ numbersite;

    //Create random bit with length range 0->pg_lenght
    NTL::RandomBits(numbersite,pg_lenght);

    stringstream buffer;
    buffer << numbersite; //Shift numbersite(bit) to the left of the buffer

    this->pg_number = buffer.str(); //Set bits in the buffer to pg_number(type mpz_class)
    /****************************/
    /*check and add bit such that bit was make have to equal size of prime was defined*/
    string pg_base2 = pg_number.get_str(2);

    for(int pg_base2_len = pg_base2.length() ; pg_base2_len < this->pg_lenght ; pg_base2_len++){
        pg_base2 += '1';
    }
    this->pg_number.set_str(pg_base2,2);
    /****************************/

    /*renew bitget,make to preprocess and pg_index*/

    pg_bit_get = new vector<bool>(this->pg_sieve_size); //Create boolt vector is false value with range of the pg_sieve_size size

    preprocess();
    this->pg_index = 0;
}

long PrimeGenerator::get_size()const
{
    return this->pg_lenght;
}


ZZZ PrimeGenerator::generate_prime()
{
    /*after preprocess, make to check prime by rabin miller test*/

    while(true)
    {
        for( ; this->pg_index < pg_sieve_size ; this->pg_index += 2)
        {
            if(pg_bit_get->at(this->pg_index) == 0)
            {
                ZZZ k=this->pg_number+this->pg_index;
                if(check_prime(k))
                {
                    this->pg_index+=2;
                    return k;
                }
            }
        }
        renew();
    }
}
void PrimeGenerator::preprocess()
{
	int j = 1;
	while (j<1000) {
		ZZZ p = pg_prime_sample[j++];
		ZZZ r = this->pg_number % p;
		for (ZZZ i = p - r; i < pg_sieve_size; i += p)
		{
			this->pg_bit_get->at(i.get_ui()) = true;
		}
	}
}
bool PrimeGenerator::prime_test(const ZZZ& n,unsigned int len)
{
    /*kiem tra gia nguyen to fermat co so */
    ZZZ tmp;
    mpz_powm(tmp.get_mpz_t(), ZZZ(2).get_mpz_t(),ZZZ((n - 1)).get_mpz_t(), n.get_mpz_t());
    if (tmp != 1) return false;
    /**************************************/
    /*let k and m such that: n-1=m*2^k*/
    ZZZ m = n^1;                    //sub(m, n, 1); because n is odd
	long k;					// MakeOdd(m);
	k = 0;
	do{
		m >>= 1;				//= > m = m / 2;
		//y = m & 1;				//check m is even number
		k++;
	} while ((m & 1) == 0);
    /*check prime number with the first 20 prime numbers*/
    gmp_randclass r(gmp_randinit_default);
    ZZZ x;
	for (int i = 0; i < 20; i++) {
		x = pg_prime_sample[i];
		if (rabin_miller_test(n, x, k, m))return false;
	}
	/*check prime number with the random 44 numbers <= sqrt(n)*/
	ZZZ qk;
	mpz_sqrt(qk.get_mpz_t(),n.get_mpz_t());
	for (int i = 0; i < len; i++)
	{
		do {
            x= r.get_z_range(qk); //RandomBnd(x, n);
		} while (x <= 73);
		if (rabin_miller_test(n, x, k, m))return false;
	}
	return true;
}

bool PrimeGenerator::rabin_miller_test(const ZZZ& n, const ZZZ& x, long &k, const ZZZ&m)
{
	ZZZ z,y;
	//z = modulo(x, m, n);
	mpz_powm(z.get_mpz_t(),x.get_mpz_t(),m.get_mpz_t(),n.get_mpz_t());
	if (z == 1 || z == (n^1)) return 0;

	long j=0;
	do {
		y = z;
		//z=y*y%n;
		mpz_powm_ui(z.get_mpz_t(),y.get_mpz_t(),2,n.get_mpz_t());
		j++;
	} while (j < k && z != 1);
	return (z != 1 || y != (n^1))?true:false;
}

ZZZ PrimeGenerator::generate_strong_prime(long sizeNumber)
{
    /*STEP1: Generate two other primes: P-- and p+*/
    ZZZ p_sub_sub,p_plus;
    long size_prime=pg_lenght;
    PrG_set_size(sizeNumber);
    renew();
    p_sub_sub = generate_prime();

    PrG_set_size(sizeNumber);
    renew();
    p_plus = generate_prime();

    pg_lenght = size_prime;
    /**************************************/
    /*STEP2: compute p- as the least prime. p-=a--*p--+1 for some integer a--=2,4,6,..*/
    ZZZ p_sub;
    ZZZ a=(p_sub_sub<<1);
    p_sub=a+1;
    while(!check_prime(p_sub))
    {
        p_sub+=a;
    }
    /**************************************/
    /*STEP3: Compute po=((p+)^(p- -1)- (p-)^(p+ -1))mode (p-*p+)*/
    ZZZ po,p1,n,d;
    n=p_sub*p_plus;
    d=p_sub^1;
    mpz_powm(po.get_mpz_t(),p_plus.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
    d=p_plus^1;
    mpz_powm(p1.get_mpz_t(),p_sub.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
    mpz_powm_ui(po.get_mpz_t(),ZZZ(po-p1).get_mpz_t(),1,n.get_mpz_t());
    /*************************************/
    /*STEP4: find a- to p as the least prime of the form
    p=po+a*p-*p+ */
    ZZZ p;
    if((po&1)==0)
    {
        po=po+p_sub*p_plus;
    }
    a=(p_sub<<1)*p_plus;
    p=po+a;
    while(!check_prime(p))
    {
        p+=a;
    }
    return p;
}
bool PrimeGenerator::check_prime(const ZZZ& n)
{
    /*check with the first 400 prime numbers*/
    if((n&1)==0)return false;
    int j=0;
    while (j<400) {
        if(n%pg_prime_sample[j++]==0)return false;
    }
    /*check prime by rabin miller test algorithm*/
    if(prime_test(n))
        return true;
    return false;
}
void PrimeGenerator::PrG_set_size(unsigned long size_prime)
{
    this->pg_lenght=size_prime;
    this->pg_sieve_size=(int)(0.693*size_prime*2);
}
PrimeGenerator::~PrimeGenerator(void)
{
    delete this->pg_bit_get;
}
