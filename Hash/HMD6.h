#pragma once
#include "IHash.h"

/* MD6 wordsize.
**
** Define md6 wordsize md6_w, in bits.
** Note that this is the "word size" relevant to the
** definition of md6 (it defines how many bits in an
** "md6_word");  it does *not* refer to the word size
** on the platform for which this is being compiled.
*/

#define   md6_w		64


/* Define "md6_word" appropriately for given value of md6_w.
** Also define PR_MD6_WORD to be the appropriate hex format string,
** using the format strings from inttypes.h .
** The term `word' in comments means an `md6_word'.
*/

#if (md6_w==64)                    /* standard md6 */
typedef UINT64 md6_word;
#define PR_MD6_WORD "%.16" PRIx64

#elif (md6_w==32)                  /* nonstandard variant */
typedef UINT32 md6_word;
#define PR_MD6_WORD "%.8" PRIx32

#elif (md6_w==16)                  /* nonstandard variant */
typedef UINT16 md6_word;
#define PR_MD6_WORD "%.4" PRIx16

#elif (md6_w==8)                   /* nonstandard variant */
typedef uint8_t md6_word;
#define PR_MD6_WORD "%.2" PRIx8

#endif

#define md6_c      16    /* size of compression output, in words       */
						 /* a c-word block is also called a "chunk"    */
#define md6_max_r 255    /* max allowable value for number r of rounds */

/* MD6 constants related to standard mode of operation                 */

/* These five values give lengths of the components of compression     */
/* input block; they should sum to md6_n.                              */
#define md6_q 15         /* # Q words in compression block (>=0)       */
#define md6_k  8         /* # key words per compression block (>=0)    */
#define md6_u (64/md6_w) /* # words for unique node ID (0 or 64/w)     */
#define md6_v (64/md6_w) /* # words for control word (0 or 64/w)       */
#define md6_b 64         /* # data words per compression block (>0)    */

#define md6_n      (md6_b + md6_v + md6_u + md6_k + md6_q)    /* size of compression input block, in words  */

#define md6_default_L 64    /* large so that MD6 is fully hierarchical */

#define md6_max_stack_height 29
	/* max_stack_height determines the maximum number of bits that
	** can be processed by this implementation (with default L) to be:
	**    (b*w) * ((b/c) ** (max_stack_height-3)
	**    = 2 ** 64  for b = 64, w = 64, c = 16, and  max_stack_height = 29
	** (We lose three off the height since level 0 is unused,
	** level 1 contains the input data, and C has 0-origin indexing.)
	** The smallest workable value for md6_max_stack_height is 3.
	** (To avoid stack overflow for non-default L values, 
	** we should have max_stack_height >= L + 2.)
	** (One level of storage could be saved by letting st->N[] use
	** 1-origin indexing, since st->N[0] is now unused.)
		*/

// Takes up about 15.5kb of memory. The class for an md6 hash. (HashMD6)
class HMD6 : public IHash
{
protected:
	int d;				    /* desired hash bit length. 1 <= d <= 512.			*/
	int initialized;        /* zero, then one after md6_init called				*/
	int finalized;          /* zero, then one after md6_final called			*/
	int keylen;				/* number of bytes in key K. 0<=keylen<=k*(w/8)		*/

	/* md6 mode specification parameter. 0 <= L <= 255							*/
	/* L == 0 means purely sequential (Merkle-Damgaard)							*/
	/* L >= 29 means purely tree-based											*/
	/* Default is md6_default_L = 64 (hierarchical)								*/
	int L;

	/* Number of rounds. 0 <= r <= 255											*/
	int r;

	/* index of block corresponding to top of stack								*/
	int top;

	/* bits[ell] =																*/
	/*    number of bits already placed in B[ell]								*/
	/*    for 1 <= ell < max_stack_height										*/
	/* 0 <= bits[ell] <= b*w													*/
	unsigned int bits[md6_max_stack_height];    

	/* i_for_level[ell] =														*/
	/*    index of the node B[ ell ] on this level (0,1,...)					*/
	/* when it is output														*/
	UINT64 i_for_level[md6_max_stack_height];

	/* k-word (8 word) key (aka "salt") for this instance of md6				*/
	md6_word K[md6_k];

	/* md6_word B[29][64]														*/
	/* stack of 32 64-word partial blocks waiting to be							*/
	/* completed and compressed.												*/
	/* B[1] is for compressing text data (input);								*/
	/* B[ell] corresponds to node at level ell in the tree.						*/
	md6_word B[md6_max_stack_height][md6_b];

	/* e.g. unsigned char hashval[128]											*/
	/* contains hashval after call to md6_final									*/
	/* hashval appears in first floor(d/8) bytes, with							*/
	/* remaining (d mod 8) bits (if any) appearing in							*/
	/* high-order bit positions of hashval[1+floor(d/8)].						*/
	unsigned char hashval[ md6_c * (md6_w / 8) ];

	/* e.g. unsigned char hexhashval[129];										*/
	/* zero-terminated string representing hex value of hashval					*/
	unsigned char hexhashval[(md6_c * (md6_w / 8)) + 1];

private:
	typedef UINT64 ControlWord;
	typedef UINT64 NodeID;

	static const md6_word S0;
	static const md6_word Smask;

	static const md6_word Q[15];

	static bool IsLittleEndian();
	static void ByteReverse(md6_word* x);
	static void ReverseLittleEndian(md6_word* x, size_t count);

	static void AppendBits(unsigned char* dest, unsigned int destlen, unsigned char* src, unsigned int srclen);

	static ControlWord MakeControlWord(int r, int L, int z, int p, int keylen, int d);
	static NodeID MakeNodeID(int ell, int i);

	static void Pack(md6_word* N, const md6_word* Q, md6_word* K, int ell, int i, int r, int L, int z, int p, int keylen, int d, md6_word* B);

	static void MainCompressionLoop(md6_word* A, int r);
	static void Compress(md6_word* C, md6_word* N, int r, md6_word* A);
	static void StandardCompress(md6_word* C, const md6_word* Q, md6_word* K, int ell, int i, int r, int L, bool z, int p, int keylen, int d, md6_word* B);

	static int DefaultR(int d, int keylen);

protected:
	void Init(int hashLengthInBits, const unsigned char *salt, int saltLen, int mode, int rounds);
	void CompressBlock(md6_word* C, int ell, bool lastCompression);

	void Process(int ell, bool final);
	void TrimHashval();
	void ComputeHexHashval();

public:
	HMD6(int bitLength = 512, const unsigned char* salt = NULL, int saltLength = 0);

	void Update(const BYTE* data, const size_t len);
	void Finalize();
	std::string GetHash();
	void Reset();

	~HMD6();
};