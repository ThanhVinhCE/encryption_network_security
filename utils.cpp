#include "utils.h"

ZZZ powerMoldulo(const ZZZ& p, ZZZ e, const ZZZ& m){
    /**
     * p^e mod m
    **/
    ZZZ res;

    if (e == 0){
        res = 1;
        return res;
    }
    if (e < 0 ){
        res = -1;
        return res;
    }

   string k = e.get_str(2);
   string s;
   res = 1;
   for (long i = 0; i < k.length(); i++) {

      res = (res*res) % m;
      s = k[i];
      if (s.compare("1") == 0){
          res = (res*p) % m;
      }
   }
   return res;
}

ZZZ euclideExtends(ZZZ m, ZZZ e){

    /**
     *  ax + by = m
     *
     **/
    ZZZ x = 0;
    ZZZ a = e;
    ZZZ y = 1;
    ZZZ b = m;
    if(a == 1) return y;
    if(a == 0) return -1;
    while (true)
    {
        ZZZ quotient = b / a;
        b = b - a*quotient;
        x = x + y*quotient;
        if (b == 1) return m - x;
        if ( b == 0) return -1;
        quotient = a /b;
        a = a - b*quotient;
        y = y +quotient*x;
    }

    return x;

}

ZZZ powerBigNumber(ZZZ e, ZZZ b){
    /**
     * return d = b^e
    **/
   ZZZ res = 1;
   ZZZ temp = b;

   while (e > 0)
   {
        if ((e & 1) == 1) res = res*temp;
        e >>= 1;
        temp = temp* temp;
   }
   return res;
}

ZZZ gcdBigNumber(ZZZ a, ZZZ b)
{
    /* GCD(0, b) == b; GCD(a, 0) == a,
       GCD(0, 0) == 0 */
    if (a == 0)
        return b;
    if (b == 0)
        return a;

    /*Finding K, where K is the
      greatest power of 2
      that divides both a and b. */

    long k;

    for (k = 0; ((a | b) & 1) == 0; ++k)
    {
        a = a/2;
        b = b/2;

    }

    /* Dividing a by 2 until a becomes odd */
    while ((a & 1)== 0)
        a = a/2;

        /* From here on, 'a' is always odd. */
    do
    {
        /* If b is even, remove all factor of 2 in b */
        while ((b & 1) == 0)
            b = b/2;

        /* Now a and b are both odd.
           Swap if necessary so a <= b,
           then set b = b - a (which is even).*/
        if (a > b)
            // Swap a and b
        {
                ZZZ temp = a;
                a = b;
                b = temp;
        }

        b = (b - a);

    }while (!(b == 0));

    /* restore common factors of 2 */
    return a*powerBigNumber(k,2);
}

ZZZ createEncryptKey(long sizeNumber, ZZZ phi){
    ZZZ e,k;
    do{
        gmp_randclass e_random(gmp_randinit_default);
        e_random.seed(time(NULL));
        e = e_random.get_z_bits(sizeNumber);
        ZZZ e_even = e&1;
        if(e_even == 0) e = e|1;
        k = gcdBigNumber(e,phi);
    }while (k!=1);
    return e;
}
