#include "mdlintf.h"
extern const U32 ECC_p[];
extern const U32 ECC_a[];
extern const U32 ECC_b[];
extern const U32 ECC_P0x[];
extern const U32 ECC_P0y[];
extern const U32 ECC_N[];

void sha256_mod_val(char *c, BIGINT *p, BIGINT *out);

U8 ECES_gen_m(S8 *data,S32 len,BIGINT *m)
{
    int l;
    char buf[ECC_BYTENUM];
    buf[0] = 0;
    l = ECC_BYTENUM-len-2;
    if(l<0)
        return;
    if(l>0)
        memset(buf+1,1,l);
    buf[l+1] = 0;
    memcpy(buf+l+2,data,len);
    char_to_bigint(buf,m);
}

U8 ECES_get_m(BIGINT *m,U32 *len,char *data)
{
    int i;
    char temp[ECC_BYTENUM];
    bigint_to_char(m,temp);
	
        if(temp[0])
        return 1;
    for(i=1;i<ECC_BYTENUM;i++)
    {
        if(!temp[i])
            break;
    }
    i++;
    if(i<ECC_BYTENUM)
    {
        *len = ECC_BYTENUM-i;
        memcpy(data,temp+i,*len);
        return 0;
    }
    else
        return 1;
}

void get_rand_bigint(BIGINT *a)
{
    a->data[0] = 0;
    a->data[1] = Get_Rand();
    a->data[2] = Get_Rand();
    a->data[3] = Get_Rand();
    a->data[4] = Get_Rand();
    a->data[5] = Get_Rand();
    a->data[6] = Get_Rand();
    if(judge_bigint(a,(BIGINT *)ECC_N)>0)
		bigint_sub(a,(BIGINT *)ECC_N,a);
}

void ECES_encrypt_kernel(BIGINT *m, BIGINT *k, ECC_POINT *pB, ECC_POINT *pR, BIGINT *p, BIGINT *c)
{
        ECC_POINT p0,pTemp;
        memcpy(p0.x.data, &ECC_P0x, 4*ECC_MAXLONG);
        memcpy(p0.y.data, &ECC_P0y, 4*ECC_MAXLONG);
        ecc_point_mul(k,pB,&pTemp);
        ecc_point_mul(k,&p0,pR);
        bigint_mod_mul(m,&(pTemp.x),p,c);
}

void ECES_decrypt_kernel(ECC_POINT *pR, BIGINT *c, BIGINT *kB, BIGINT *p, BIGINT *m)
{
      ECC_POINT pTemp;
      BIGINT u;
      ecc_point_mul(kB,pR,&pTemp);
      bigint_EEA_algo(&(pTemp.x),p,&u);
      bigint_mod_mul(c,&u,p,m);
}

void ECES_send_char(ECC_POINT *pR, BIGINT *c, S8 *out)
{
      point_to_char(pR,out);
      bigint_to_char(c,out+49);
}

void ECES_divide_char(S8 *in, ECC_POINT *pR, BIGINT *c)
{
      char_to_point(in,pR);
      char_to_bigint(in+49,c);
}

void e_random(S8 *random,U32 rlength, S8 * pubkey, S8 *erandom, U32 *erlength)
{
    BIGINT m,k,c;
    ECC_POINT pB,pR;
    BIGINT *p;
    p = (BIGINT *)ECC_p;
    ECES_gen_m(random,rlength,&m);
#if 1
    get_rand_bigint(&k);
#else
       k.data[0] = 0;
       k.data[1] = 0x889411b4;
	k.data[2] = 0x3bcd436a;
	k.data[3] = 0xbe8e4c28;
	k.data[4] = 0xff791658;
	k.data[5] = 0xa9b72fa6;
	k.data[6] = 0x70236920;    
#endif
    char_to_bigint(pubkey+1,&(pB.x));
    char_to_bigint(pubkey+1+ECC_BYTENUM,&(pB.y));
    ECES_encrypt_kernel(&m,&k,&pB,&pR,p,&c);
    ECES_send_char(&pR,&c,erandom);
    *erlength = 3*ECC_BYTENUM+1;
}

void d_random(S8 *erandom, U32 erlength, S8 *prikey, S8 *random, U32 *rlength)
{
    BIGINT c,kB,m;
    ECC_POINT pR;
    BIGINT *p;
    p = (BIGINT *)ECC_p;    
    ECES_divide_char(erandom,&pR,&c);
    char_to_bigint(prikey,&kB);
    ECES_decrypt_kernel(&pR,&c,&kB,p,&m);
    ECES_get_m(&m,rlength,random);
}

U32 ECDSA_sign_kernel(BIGINT *k, BIGINT *kA, char *hash, BIGINT *r, BIGINT *s)
{
    ECC_POINT p0,pTemp;
    BIGINT N,h,c;
    memcpy(N.data, &ECC_N, 4*ECC_MAXLONG);
    sha256_mod_val(hash,&N,&h);

        memcpy(p0.x.data, &ECC_P0x, 4*ECC_MAXLONG);
        memcpy(p0.y.data, &ECC_P0y, 4*ECC_MAXLONG);
        //ecc_point_mul2(k,kp0X,kp0Y,&pTemp);
        ecc_point_mul(k,&p0,&pTemp);
        bigint_mod_val(&(pTemp.x),&N,r);

        if(judge_bigint_null(r))
            return 1;
HCMD_CALC(ECC_N+1,ECC_NUMBITS);
        bigint_mod_mul(r,kA,&N,&c);
        bigint_mod_add(&h,&c,&N,&c);
        bigint_EEA_algo(k,&N,s);
        bigint_mod_mul(s,&c,&N,s);

        if(judge_bigint_null(s))
            return 1;
        else
            return 0;
}

U32 ECDSA_validate_kernel(BIGINT *r, BIGINT *s, char *hash, ECC_POINT *pA)
{
#if 0
    ECC_POINT p0,pTemp0,pTemp1;
    BIGINT N,h,c,u1,u2,p;
    memcpy(N.data, &ECC_N, 4*ECC_MAXLONG);
    memcpy(p0.x.data, &ECC_P0x, 4*ECC_MAXLONG);
    memcpy(p0.y.data, &ECC_P0y, 4*ECC_MAXLONG);
    memcpy(p.data, &ECC_p, 4*ECC_MAXLONG);
HCMD_CALC(ECC_N+1,ECC_NUMBITS);
    sha256_mod_val(hash,&N,&h);
    bigint_EEA_algo(s,&N,&c);
    bigint_mod_mul(&h,&c,&N,&u1);
    bigint_mod_mul(r,&c,&N,&u2);

    //ecc_point_mul2(&u1,kp0X,kp0Y,&pTemp0);
    ecc_point_mul(&u1,&p0,&pTemp0);
    ecc_point_mul(&u2,pA,&pTemp1);

    ecc_point_add(&pTemp0,&pTemp1,&p,&pTemp0);
    bigint_mod_val(&(pTemp0.x),&N,&c);
    return judge_bigint(&c,r);
#else
    ECC_POINT p0,pTemp0,pTemp1;
    BIGINT h,c,u1,u2;
    //memcpy(N.data, &ECC_N, 4*ECC_MAXLONG);
    memcpy(p0.x.data, &ECC_P0x, 4*ECC_MAXLONG);
    memcpy(p0.y.data, &ECC_P0y, 4*ECC_MAXLONG);
    //memcpy(p.data, &ECC_p, 4*ECC_MAXLONG);
HCMD_CALC(ECC_N+1,ECC_NUMBITS);
    sha256_mod_val(hash,(BIGINT *)ECC_N,&h);
    bigint_EEA_algo(s,(BIGINT *)ECC_N,&c);
    bigint_mod_mul(&h,&c,(BIGINT *)ECC_N,&u1);
    bigint_mod_mul(r,&c,(BIGINT *)ECC_N,&u2);

    ecc_point_mul(&u1,&p0,&pTemp0);
    ecc_point_mul(&u2,pA,&pTemp1);

    ecc_point_add(&pTemp0,&pTemp1,(BIGINT *)ECC_p,&pTemp0);
    bigint_mod_val(&(pTemp0.x),(BIGINT *)ECC_N,&c);
    return judge_bigint(&c,r);
#endif
}



/***************************************************************/
/***************************************************************/
/**************** my HASH(SHA256) function library *************/

// for SHA256
/* the K array */
const U32 K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
		0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
		0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
		0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
		0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
		0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
		0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
		0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
		0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
		0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
		0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
		0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
		0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Various logical functions */
#define Ch(x,y,z)	((x & y) ^ (~x & z))
#define Maj(x,y,z)  ((x & y) ^ (x & z) ^ (y & z))
#define S(x, n)		(((x)>>((n)&31))|((x)<<(32-((n)&31))))
#define R(x, n)		((x)>>(n))
#define Sigma0(x)	(S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x)	(S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x)	(S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x)	(S(x, 17) ^ S(x, 19) ^ R(x, 10))

void sha256_mod_val(char *c, BIGINT *p, BIGINT *out)
{
     int i;
     BIGINT u,v;
     char *x;
     memset(&v,0,sizeof(v));
     v.data[ECC_MAXLONG-1] = 1;
     x = c+24;
     char_to_bigint(c,&u);
     bigint_mod_val(&u,p,&u);
     i=64;
     while(i)
     {
         i--;
         bigint_mod_mul_two(&u,p,&u);
         if(x[7-(i>>3)] & (1<<(i&7)))
            bigint_mod_add(&u,&v,p,&u);
     }
     *out = u;
}

/* init the SHA state */
void sha_init(sha_state * md)
{
    md->curlen = md->length = 0;
    md->state[0] = 0x6A09E667UL;
    md->state[1] = 0xBB67AE85UL;
    md->state[2] = 0x3C6EF372UL;
    md->state[3] = 0xA54FF53AUL;
    md->state[4] = 0x510E527FUL;
    md->state[5] = 0x9B05688CUL;
    md->state[6] = 0x1F83D9ABUL;
    md->state[7] = 0x5BE0CD19UL;
}

/* compress 512-bits */
static void sha_compress(sha_state * md)
{
    unsigned long S[8], W[64], t0, t1;
    int i;

    /* copy state into S */
    for (i = 0; i < 8; i++)
        S[i] = md->state[i];

    /* copy the state into 512-bits into W[0..15] */
    for (i = 0; i < 16; i++)
        W[i] = (((unsigned long) md->buf[(4 * i) + 0]) << 24) |
            (((unsigned long) md->buf[(4 * i) + 1]) << 16) |
            (((unsigned long) md->buf[(4 * i) + 2]) << 8) |
            (((unsigned long) md->buf[(4 * i) + 3]));

    /* fill W[16..63] */
    for (i = 16; i < 64; i++)
        W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];

    /* Compress */
    for (i = 0; i < 64; i++) {
        t0 = S[7] + Sigma1(S[4]) + Ch(S[4], S[5], S[6]) + K[i] + W[i];
        t1 = Sigma0(S[0]) + Maj(S[0], S[1], S[2]);
        S[7] = S[6];
        S[6] = S[5];
        S[5] = S[4];
        S[4] = S[3] + t0;
        S[3] = S[2];
        S[2] = S[1];
        S[1] = S[0];
        S[0] = t0 + t1;
    }

    /* feedback */
    for (i = 0; i < 8; i++)
        md->state[i] += S[i];
}

void sha_process(sha_state * md, unsigned char *buf, int len)
{
    while (len--) {
        /* copy byte */
        md->buf[md->curlen++] = *buf++;

        /* is 64 bytes full? */
        if (md->curlen == 64) {
            sha_compress(md);
            md->length += 512;
            md->curlen = 0;
        }
    }
}

void sha_done(sha_state * md, unsigned char *hash)
{
    int i;

    /* increase the length of the message */
    md->length += md->curlen * 8;

    /* append the '1' bit */
    md->buf[md->curlen++] = 0x80;

    /* if the length is currenlly above 56 bytes we append zeros
                               * then compress.  Then we can fall back to padding zeros and length
                               * encoding like normal.
                             */
    if (md->curlen >= 56) {
        for (; md->curlen < 64;)
            md->buf[md->curlen++] = 0;
        sha_compress(md);
        md->curlen = 0;
    }

    /* pad upto 56 bytes of zeroes */
    for (; md->curlen < 56;)
        md->buf[md->curlen++] = 0;

    /* since all messages are under 2^32 bits we mark the top bits zero */
    for (i = 56; i < 60; i++)
        md->buf[i] = 0;

    /* append length */
    for (i = 60; i < 64; i++)
        md->buf[i] = (U8)((md->length >> ((63 - i) * 8)) & 255);
    sha_compress(md);

    /* copy output */
    for (i = 0; i < 32; i++)
        hash[i] = (U8)((md->state[i >> 2] >> (((3 - i) & 3) << 3)) & 255);
}

/* sha-256 a block of memory */
void sha256_memory(unsigned char *buf, int len, unsigned char *hash)
{
    sha_state md;

    sha_init(&md);
    sha_process(&md, buf, len);
    sha_done(&md, hash);
}
/*
void sha256_memory_2(unsigned char *buf1, int len1, unsigned char *buf2, int len2, unsigned char *hash)
{
    sha_state md;

    sha_init(&md);
    sha_process(&md, buf1, len1);
    sha_process(&md, buf2, len2);
    sha_done(&md, hash);
}
*/


/***************************************************************/
/***************************************************************/
// wapi algo
/*******************************************************/
void ecdsa_signature(char *inbuf,U32 length, char *prikey, char *outbuf, U32 *slength)
{
    U8 hash[32];
    BIGINT kA,r,s,k;
    sha256_memory(inbuf,length,hash);
    char_to_bigint(prikey,&kA);
wapi_signature_1:
#if 1
    get_rand_bigint(&k);
#else
       k.data[0] = 0;
       k.data[1] = 0x2d8eec49;
	k.data[2] = 0xe2684ee6;
	k.data[3] = 0xe09ac680;
	k.data[4] = 0x8a1639ed;
	k.data[5] = 0xc48f12be;
	k.data[6] = 0xfb6ce96f;
#endif
    if(ECDSA_sign_kernel(&k,&kA,hash,&r,&s))
        goto wapi_signature_1;
    bigint_to_char(&r,outbuf);
    bigint_to_char(&s,outbuf+ECC_BYTENUM);
    *slength = ECC_BYTENUM<<1;
}


U32 ecdsa_verification(char *inbuf, U32 length ,char *hashresult, char *pubkey)
{
    BIGINT r,s;
    ECC_POINT pA;
    char_to_bigint(inbuf,&r);
    char_to_bigint(inbuf+ECC_BYTENUM,&s);
    char_to_bigint(pubkey+1,&(pA.x));
    char_to_bigint(pubkey+1+ECC_BYTENUM,&(pA.y));
    return ECDSA_validate_kernel(&r,&s,hashresult,&pA);
}



