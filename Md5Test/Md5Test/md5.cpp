
/*   
* RFC 1321 compliant MD5 implementation,   
* by Christophe Devine <devine@cr0.net>;   
* this program is licensed under the GPL.   
*/    
#include <stdio.h>
#include <string.h>
#include "md5.h"

#define UINT4 unsigned int


/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits. */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation. */
#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

// Constants for MD5 Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21



//MD5摘要
MD5VAL md5( const char * pc_abstract_str, unsigned int n_size )
{
    if( n_size == 0 ) {
         n_size = strlen( pc_abstract_str );
	}
    unsigned int n_m = n_size % 64;
    unsigned int n_lm = n_size - n_m;  //数据整块长度
    unsigned int n_ln;  //数据补位后长度
    if( n_m < 56 ) {
        n_ln = n_lm + 64;
	}
	else {
        n_ln = n_lm + 128;
	}
    char * pc_strw = new char[n_ln];
	unsigned int i;
	//复制原字串到缓冲区strw
    for( i = 0; i < n_size; i++ ) {
        pc_strw[i] = pc_abstract_str[i];
	}
	//补位
    pc_strw[i++] = 0x80;
    for( ; i < n_ln-8; i++ ) {
        pc_strw[i] = 0x00;
	}
	//补长度
    unsigned int * pn_len = ( unsigned int * )( pc_strw+i );
    *(pn_len++) = n_size<<3;
    *(pn_len++) = n_size>>29;
	//初始化MD5参数
	MD5VAL val = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
    unsigned int &n_a = val.n_a, &n_b = val.n_b, &n_c = val.n_c, &n_d = val.n_d;
    unsigned int n_aa, n_bb, n_cc, n_dd;
    for( i = 0; i < n_ln; i += 64)
	{
        pn_len = ( unsigned int * )( pc_strw + i );
		// Save the values
        n_aa = n_a; n_bb = n_b; n_cc = n_c; n_dd = n_d;
		// Round 1
        FF ( n_a, n_b, n_c, n_d, pn_len[ 0], S11, 0xd76aa478); /* 1 */
        FF ( n_d, n_a, n_b, n_c, pn_len[ 1], S12, 0xe8c7b756); /* 2 */
        FF ( n_c, n_d, n_a, n_b, pn_len[ 2], S13, 0x242070db); /* 3 */
        FF ( n_b, n_c, n_d, n_a, pn_len[ 3], S14, 0xc1bdceee); /* 4 */
        FF ( n_a, n_b, n_c, n_d, pn_len[ 4], S11, 0xf57c0faf); /* 5 */
        FF ( n_d, n_a, n_b, n_c, pn_len[ 5], S12, 0x4787c62a); /* 6 */
        FF ( n_c, n_d, n_a, n_b, pn_len[ 6], S13, 0xa8304613); /* 7 */
        FF ( n_b, n_c, n_d, n_a, pn_len[ 7], S14, 0xfd469501); /* 8 */
        FF ( n_a, n_b, n_c, n_d, pn_len[ 8], S11, 0x698098d8); /* 9 */
        FF ( n_d, n_a, n_b, n_c, pn_len[ 9], S12, 0x8b44f7af); /* 10 */
        FF ( n_c, n_d, n_a, n_b, pn_len[10], S13, 0xffff5bb1); /* 11 */
        FF ( n_b, n_c, n_d, n_a, pn_len[11], S14, 0x895cd7be); /* 12 */
        FF ( n_a, n_b, n_c, n_d, pn_len[12], S11, 0x6b901122); /* 13 */
        FF ( n_d, n_a, n_b, n_c, pn_len[13], S12, 0xfd987193); /* 14 */
        FF ( n_c, n_d, n_a, n_b, pn_len[14], S13, 0xa679438e); /* 15 */
        FF ( n_b, n_c, n_d, n_a, pn_len[15], S14, 0x49b40821); /* 16 */
		// Round 2
        GG ( n_a, n_b, n_c, n_d, pn_len[ 1], S21, 0xf61e2562); /* 17 */
        GG ( n_d, n_a, n_b, n_c, pn_len[ 6], S22, 0xc040b340); /* 18 */
        GG ( n_c, n_d, n_a, n_b, pn_len[11], S23, 0x265e5a51); /* 19 */
        GG ( n_b, n_c, n_d, n_a, pn_len[ 0], S24, 0xe9b6c7aa); /* 20 */
        GG ( n_a, n_b, n_c, n_d, pn_len[ 5], S21, 0xd62f105d); /* 21 */
        GG ( n_d, n_a, n_b, n_c, pn_len[10], S22,  0x2441453); /* 22 */
        GG ( n_c, n_d, n_a, n_b, pn_len[15], S23, 0xd8a1e681); /* 23 */
        GG ( n_b, n_c, n_d, n_a, pn_len[ 4], S24, 0xe7d3fbc8); /* 24 */
        GG ( n_a, n_b, n_c, n_d, pn_len[ 9], S21, 0x21e1cde6); /* 25 */
        GG ( n_d, n_a, n_b, n_c, pn_len[14], S22, 0xc33707d6); /* 26 */
        GG ( n_c, n_d, n_a, n_b, pn_len[ 3], S23, 0xf4d50d87); /* 27 */
        GG ( n_b, n_c, n_d, n_a, pn_len[ 8], S24, 0x455a14ed); /* 28 */
        GG ( n_a, n_b, n_c, n_d, pn_len[13], S21, 0xa9e3e905); /* 29 */
        GG ( n_d, n_a, n_b, n_c, pn_len[ 2], S22, 0xfcefa3f8); /* 30 */
        GG ( n_c, n_d, n_a, n_b, pn_len[ 7], S23, 0x676f02d9); /* 31 */
        GG ( n_b, n_c, n_d, n_a, pn_len[12], S24, 0x8d2a4c8a); /* 32 */
		// Round 3
        HH ( n_a, n_b, n_c, n_d, pn_len[ 5], S31, 0xfffa3942); /* 33 */
        HH ( n_d, n_a, n_b, n_c, pn_len[ 8], S32, 0x8771f681); /* 34 */
        HH ( n_c, n_d, n_a, n_b, pn_len[11], S33, 0x6d9d6122); /* 35 */
        HH ( n_b, n_c, n_d, n_a, pn_len[14], S34, 0xfde5380c); /* 36 */
        HH ( n_a, n_b, n_c, n_d, pn_len[ 1], S31, 0xa4beea44); /* 37 */
        HH ( n_d, n_a, n_b, n_c, pn_len[ 4], S32, 0x4bdecfa9); /* 38 */
        HH ( n_c, n_d, n_a, n_b, pn_len[ 7], S33, 0xf6bb4b60); /* 39 */
        HH ( n_b, n_c, n_d, n_a, pn_len[10], S34, 0xbebfbc70); /* 40 */
        HH ( n_a, n_b, n_c, n_d, pn_len[13], S31, 0x289b7ec6); /* 41 */
        HH ( n_d, n_a, n_b, n_c, pn_len[ 0], S32, 0xeaa127fa); /* 42 */
        HH ( n_c, n_d, n_a, n_b, pn_len[ 3], S33, 0xd4ef3085); /* 43 */
        HH ( n_b, n_c, n_d, n_a, pn_len[ 6], S34,  0x4881d05); /* 44 */
        HH ( n_a, n_b, n_c, n_d, pn_len[ 9], S31, 0xd9d4d039); /* 45 */
        HH ( n_d, n_a, n_b, n_c, pn_len[12], S32, 0xe6db99e5); /* 46 */
        HH ( n_c, n_d, n_a, n_b, pn_len[15], S33, 0x1fa27cf8); /* 47 */
        HH ( n_b, n_c, n_d, n_a, pn_len[ 2], S34, 0xc4ac5665); /* 48 */
		// Round 4 */
        II ( n_a, n_b, n_c, n_d, pn_len[ 0], S41, 0xf4292244); /* 49 */
        II ( n_d, n_a, n_b, n_c, pn_len[ 7], S42, 0x432aff97); /* 50 */
        II ( n_c, n_d, n_a, n_b, pn_len[14], S43, 0xab9423a7); /* 51 */
        II ( n_b, n_c, n_d, n_a, pn_len[ 5], S44, 0xfc93a039); /* 52 */
        II ( n_a, n_b, n_c, n_d, pn_len[12], S41, 0x655b59c3); /* 53 */
        II ( n_d, n_a, n_b, n_c, pn_len[ 3], S42, 0x8f0ccc92); /* 54 */
        II ( n_c, n_d, n_a, n_b, pn_len[10], S43, 0xffeff47d); /* 55 */
        II ( n_b, n_c, n_d, n_a, pn_len[ 1], S44, 0x85845dd1); /* 56 */
        II ( n_a, n_b, n_c, n_d, pn_len[ 8], S41, 0x6fa87e4f); /* 57 */
        II ( n_d, n_a, n_b, n_c, pn_len[15], S42, 0xfe2ce6e0); /* 58 */
        II ( n_c, n_d, n_a, n_b, pn_len[ 6], S43, 0xa3014314); /* 59 */
        II ( n_b, n_c, n_d, n_a, pn_len[13], S44, 0x4e0811a1); /* 60 */
        II ( n_a, n_b, n_c, n_d, pn_len[ 4], S41, 0xf7537e82); /* 61 */
        II ( n_d, n_a, n_b, n_c, pn_len[11], S42, 0xbd3af235); /* 62 */
        II ( n_c, n_d, n_a, n_b, pn_len[ 2], S43, 0x2ad7d2bb); /* 63 */
        II ( n_b, n_c, n_d, n_a, pn_len[ 9], S44, 0xeb86d391); /* 64 */
		// Add the original values
        n_a += n_aa;
        n_b += n_bb;
        n_c += n_cc;
        n_d += n_dd;
	}
    delete[] pc_strw;
	return val;
}

//Endian convert: big <-> little
unsigned int conv( unsigned int n_a )
{
    unsigned int n_b = 0;
    n_b |= ( n_a<<24 )&0xff000000;
    n_b |= ( n_a<<8 )&0x00ff0000;
    n_b |= ( n_a>>8 )&0x0000ff00;
    n_b |= ( n_a>>24 )&0x000000ff;
    return n_b;
}

//puc_md5是二进制 Abstract
int get_md5( const char *pc_abstract_str, char *pc_md5_ascii, unsigned char *puc_md5, unsigned int n_size )
{
    MD5VAL val;

    val = md5( pc_abstract_str, n_size );

    if ( pc_md5_ascii != NULL ) {
        sprintf( pc_md5_ascii, "%08x%08x%08x%08x", conv( val.n_a ), conv( val.n_b ), conv( val.n_c ), conv( val.n_d ) );
    }
    
    if ( puc_md5 != NULL ) {
        memcpy( puc_md5, (void *)&val, sizeof( val ) );
    }

    return 0;
}


//end of file


