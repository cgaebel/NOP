#include "HMD6.h"
#include <cassert>

/* The following code confirms that the defined MD6 constants satisfy 
** some expected properties.  These tests should never fail; consider 
** these tests to be documentation. Failure of these tests would cause 
** compilation to fail.
*/

#if ( (md6_w!=8) && (md6_w!=16) && (md6_w!=32) && (md6_w!=64) )
  #error "md6_w must be one of 8,16,32, or 64."
#elif ( md6_n<=0 )
  #error "md6_n must be positive."
#elif ( md6_b<=0 )
  #error "md6_b must be positive."
#elif ( md6_c<=0 )
  #error "md6_c must be positive."
#elif ( md6_v<0 )
  #error "md6_v must be nonnegative."
#elif ( md6_u<0 )
  #error "md6_u must be nonnegative."
#elif ( md6_k<0 )
  #error "md6_k must be nonnegative."
#elif ( md6_q<0 )
  #error "md6_q must be nonnegative."
#elif ( md6_b>=md6_n )
  #error "md6_b must be smaller than md6_n."
#elif ( md6_c>=md6_b )
  #error "md6_c must be smaller than md6_b."
#elif ( (md6_b%md6_c)!=0 )
  #error "md6_b must be a multiple of md6_c."
#elif ( md6_n != md6_b + md6_v + md6_u + md6_k + md6_q )
  #error "md6_n must = md6_b + md6_v + md6_u + md6_k + md6_q."
#elif ( md6_max_stack_height < 3 )
  #error "md6_max_stack_height must be at least 3."
#elif ( md6_r * md6_c + md6_n >= 5000 )
  /* since md6_standard_compress allocates fixed-size array A[5000] */
  #error "r*c+n must be < 5000."
#if 0
  /* "sizeof" doesn't work in preprocessor, these checks don't work */
  #elif ( (md6_v != 0) && (md6_v != (sizeof(md6_control_word)/(md6_w/8))) )
	#error "md6_v must be 0 or match md6_control_word size."
  #elif ( (md6_u != 0) && (md6_u != (sizeof(md6_nodeID)/(md6_w/8))) )
	#error "md6_u must be 0 or match md6_nodeID size."
#endif
#endif

/* Variables defining lengths of various values */
#define   w   md6_w  /* # bits in a word (64) */
#define   n   md6_n  /* # words in compression input (89) */
#define   c   md6_c  /* # words in compression output (16) */
#define   b   md6_b  /* # message words per compression input block (64) */
#define   v   md6_v  /* # words in control word (1) */
#define   u   md6_u  /* # words in unique nodeID (1) */
#define   k   md6_k  /* # key words per compression input block (8) */
#define   q   md6_q  /* # Q words per compression input block (15) */


/* "Tap positions" for feedback shift-register */

#if (n==89)
#define  t0   17     /* index for linear feedback */
#define  t1   18     /* index for first input to first and */
#define  t2   21     /* index for second input to first and */
#define  t3   31     /* index for first input to second and */
#define  t4   67     /* index for second input to second and */
#define  t5   89     /* last tap */
#endif

/* Loop-unrolling setup.
**
** Define macros for loop-unrolling within compression function 
** These expand to:     loop_body(right-shift,left-shift,step)  
** These are given for word sizes 64, 32, 16, and 8, although   
** only w=64 is needed for the standard MD6 definition.         
**                                                              
** Also given for each word size are the constants S0 and Smask 
** needed in the recurrence for round constants.
*/

#if (w==64)                        /* for standard word size */
#define RL00 loop_body(10,11, 0)
#define RL01 loop_body( 5,24, 1)
#define RL02 loop_body(13, 9, 2)
#define RL03 loop_body(10,16, 3)
#define RL04 loop_body(11,15, 4)
#define RL05 loop_body(12, 9, 5)
#define RL06 loop_body( 2,27, 6)
#define RL07 loop_body( 7,15, 7)
#define RL08 loop_body(14, 6, 8)
#define RL09 loop_body(15, 2, 9)
#define RL10 loop_body( 7,29,10)
#define RL11 loop_body(13, 8,11)
#define RL12 loop_body(11,15,12)
#define RL13 loop_body( 7, 5,13)
#define RL14 loop_body( 6,31,14)
#define RL15 loop_body(12, 9,15)

const md6_word HMD6::S0 = (md6_word)0x0123456789abcdefULL;
const md6_word HMD6::Smask = (md6_word)0x7311c2812425cfa0ULL;

#elif (w==32)                      /* for variant word size */
#define RL00 loop_body( 5, 4, 0)
#define RL01 loop_body( 3, 7, 1)
#define RL02 loop_body( 6, 7, 2)
#define RL03 loop_body( 5, 9, 3)
#define RL04 loop_body( 4,13, 4)
#define RL05 loop_body( 6, 8, 5)
#define RL06 loop_body( 7, 4, 6)
#define RL07 loop_body( 3,14, 7)
#define RL08 loop_body( 5, 7, 8)
#define RL09 loop_body( 6, 4, 9)
#define RL10 loop_body( 5, 8,10)
#define RL11 loop_body( 5,11,11)
#define RL12 loop_body( 4, 5,12)
#define RL13 loop_body( 6, 8,13)
#define RL14 loop_body( 7, 2,14)
#define RL15 loop_body( 5,11,15)

const md6_word HMD6::S0 = (md6_word)0x01234567UL;
const md6_word HMD6::Smask = (md6_word)0x7311c281UL;

/* Loop-unrolling setup (continued).
**
*/

#elif (w==16)                      /* for variant word size */

#define RL00 loop_body( 5, 6, 0)
#define RL01 loop_body( 4, 7, 1)
#define RL02 loop_body( 3, 2, 2)
#define RL03 loop_body( 5, 4, 3)
#define RL04 loop_body( 7, 2, 4)
#define RL05 loop_body( 5, 6, 5)
#define RL06 loop_body( 5, 3, 6)
#define RL07 loop_body( 2, 7, 7)
#define RL08 loop_body( 4, 5, 8)
#define RL09 loop_body( 3, 7, 9)
#define RL10 loop_body( 4, 6,10)
#define RL11 loop_body( 3, 5,11)
#define RL12 loop_body( 4, 5,12)
#define RL13 loop_body( 7, 6,13)
#define RL14 loop_body( 7, 4,14)
#define RL15 loop_body( 2, 3,15)

const md6_word HMD6::S0 = (md6_word)0x01234;
const md6_word HMD6::Smask = (md6_word)0x7311;

#elif (w==8)                     /* for variant word size */

#define RL00 loop_body( 3, 2, 0)
#define RL01 loop_body( 3, 4, 1)
#define RL02 loop_body( 3, 2, 2)
#define RL03 loop_body( 4, 3, 3)
#define RL04 loop_body( 3, 2, 4)
#define RL05 loop_body( 3, 2, 5)
#define RL06 loop_body( 3, 2, 6)
#define RL07 loop_body( 3, 4, 7)
#define RL08 loop_body( 2, 3, 8)
#define RL09 loop_body( 2, 3, 9)
#define RL10 loop_body( 3, 2,10)
#define RL11 loop_body( 2, 3,11)
#define RL12 loop_body( 2, 3,12)
#define RL13 loop_body( 3, 4,13)
#define RL14 loop_body( 2, 3,14)
#define RL15 loop_body( 3, 4,15)

const md6_word HMD6::S0 = (md6_word)0x01;
const md6_word HMD6::Smask = (md6_word)0x73;

#endif

/* MD6 Constant Vector Q
** Q = initial 960 bits of fractional part of sqrt(6)
**
** Given here for w = 64, 32, 16, and 8, although only
** w = 64 is needed for the standard version of MD6.
*/

#if (w==64)                      /* for standard version */
/* 15 64-bit words */
const md6_word HMD6::Q[15] = {
	0x7311c2812425cfa0,
	0x6432286434aac8e7, 
	0xb60450e9ef68b7c1, 
	0xe8fb23908d9f06f1, 
	0xdd2e76cba691e5bf, 
	0x0cd0d63b2c30bc41, 
	0x1f8ccf6823058f8a, 
	0x54e5ed5b88e3775d, 
	0x4ad12aae0a6d6031, 
	0x3e7f16bb88222e0d, 
	0x8af8671d3fb50c2c, 
	0x995ad1178bd25c31, 
	0xc878c1dd04c4b633, 
	0x3b72066c7a1552ac, 
	0x0d6f3522631effcb, 
};
#endif

/* Data structure notes.
*/

/*
Here are some notes on the data structures used (inside state).

* The variable B[] is a stack of length-b (b-64) word records,
  each corresponding to a node in the tree.  B[ell] corresponds
  to a node at level ell.  Specifically, it represents the record which,
  when compressed, will yield the value at that level. (It only
  contains the data payload, not the auxiliary information.)
  Note that B[i] is used to store the *inputs* to the computation at
  level i, not the output for the node at that level.  
  Thus, for example, the message input is stored in B[1], not B[0].

* Level 0 is not used.  The message bytes are placed into B[1].

* top is the largest ell for which B[ell] has received data,
  or is equal to 1 in case no data has been received yet at all.

* top is never greater than L+1.  If B[L+1] is
  compressed, the result is put back into B[L+1]  (this is SEQ).

* bits[ell] says how many bits have been placed into
  B[ell].  An invariant maintained is that of the bits in B[ell], 
  only the first bits[ell] may be nonzero; the following bits must be zero.

* The B nodes may have somewhat different formats, depending on the level:
  -- Level 1 node contains a variable-length bit-string, and so
  -- Levels 2...top always receive data in c-word chunks (from
	 children), so for them bits[ell] is between 0 and b*w,
	 inclusive, but is also a multiple of cw.  We can think of these
	 nodes as have (b/c) (i.e. 4) "slots" for chunks.
  -- Level L+1 is special, in that the first c words of B are dedicated
	 to the "chaining variable" (or IV, for the first node on the level).

* When the hashing is over, B[top] will contain the 
  final hash value, in the first or second (if top = L+1) slot.
*/

/* Initialize HMD6
** Input:
**     hashLengthInBits		desired hash bit length 1 <= d <= w*(c/2)    (<=512 bits)
**     salt					key (aka salt) for this hash computation     (byte array)
**							defaults to all-zero key if key==NULL or keylen==0
**     saltLen				length of key in bytes; 0 <= keylen <= (k*8) (<=64 bytes)
**     mode					md6 mode parameter; 0 <= L <= 255
**							md6.h defines md6_default_L for when you want default
**     rounds				number of rounds; 0 <= r <= 255
*/
void HMD6::Init(int hashLengthInBits, const unsigned char *salt, int saltLen, int mode, int rounds)
{
	ZeroMemory(B, sizeof(B));
	ZeroMemory(bits, sizeof(bits));
	ZeroMemory(hashval, sizeof(hashval));
	ZeroMemory(hexhashval, sizeof(hexhashval));
	ZeroMemory(i_for_level, sizeof(i_for_level));
	ZeroMemory(K, sizeof(K));

	finalized = 0;
	d = hashLengthInBits;                       /* save hashbitlen */

	if (salt != NULL && saltLen > 0)   /* if no key given, use memset zeros*/
	{
#define BUILDNIBBLE(x) ((x) - (((x) > '9') ? ('a' - 0xa) : '0'))
#define VALIDHEX(x) ((((x) >= '0') && ((x) <= '9')) || (((x) >= 'a') && ((x) <= 'f')))
		for(int i = 0; i < saltLen; i += 2)
		{
			((BYTE*)K)[i/2] = BUILDNIBBLE(salt[i+1]) | (BUILDNIBBLE(salt[i]) << 4);
		}
#undef VALIDHEX
#undef BUILDNIBBLE

		keylen = saltLen;
		/* handle endian-ness */       /* first byte went into high end */
		ReverseLittleEndian(K, k);
	}
	else
	{
		keylen = 0;
	}

	L = mode;
	r = rounds;

	initialized = 1;  
	top = 1;

	/* if SEQ mode for level 1; use IV=0  */
	/* zero bits already there by memset; */
	/* we just need to set st->bits[1]    */
	if (mode == 0)
		bits[1] = c*w;
}

/*
** Do processing of level ell (and higher, if necessary) blocks.
** 
** Input:
**     st         md6 state that has been accumulating message bits
**                and/or intermediate results
**     ell        level number of block to process
**     final      true if this routine called from md6_final 
**                     (no more input will come)
**                false if more input will be coming
**                (This is not same notion as "final bit" (i.e. z)
**                 indicating the last compression operation.)
** Output (by side effect on state):
**     Sets st->hashval to final chaining value on final compression.
*/
void HMD6::Process(int ell, bool final)
{
	int next_level;

	{
		md6_word C[c];

		/* not final -- more input will be coming */
		if (!final)
		{
			/* if not final and block on this level not full, nothing to do */
			if ( bits[ell] < b*w )
				return;
			/* else fall through to compress this full block, 
			**       since more input will be coming 
			*/
		}
		/* final -- no more input will be coming */
		else
		{
			if ( ell == top )
			{
				if (ell == (L + 1)) /* SEQ node */
				{
					if ( bits[ell]==c*w && i_for_level[ell] > 0 )
						return;
					/* else (bits>cw or i==0, so fall thru to compress */
				}
				else /* st->top == ell <= st->L so we are at top tree node */
				{
					if ( ell>1 && bits[ell]==c*w) return;
					/* else (ell==1 or bits>cw, so fall thru to compress */
				}
			}
			/* else (here ell < st->top so fall through to compress */
		}

		/* compress block at this level; result goes into C */
		/* first set z to 1 if this is the very last compression */
		bool z = (final && (ell == top)); 
		CompressBlock(C, ell, z);

		/* save final chaining value in st->hashval */
		if (z)
		{
			memcpy_s( hashval, sizeof(hashval), C, md6_c*(w/8) );
			return;
		}

		/* where should result go? To "next level" */
		next_level = min(ell + 1, L + 1);

		/* Start sequential mode with IV=0 at that level if necessary 
		** (All that is needed is to set bits[next_level] to c*w, 
		** since the bits themselves are already zeroed, either
		** initially, or at the end of md6_compress_block.)
		*/
		if(
			(next_level == L + 1) &&
			(i_for_level[next_level] == 0) &&
			(bits[next_level] == 0)
		)
			bits[next_level] = c*w;  

		/* now copy C onto next level */
		memcpy(
			(char*)B[next_level] + bits[next_level]/8,
			C,
			c*(w/8)
		);
		bits[next_level] += c*w;

		if (next_level > top)
			top = next_level;
	}

	Process(next_level, final);
}

/*
** Convert hashval in st->hashval into hexadecimal, and
** save result in st->hexhashval
** This will be a zero-terminated string of length ceil(d/4).
** Assumes that hashval has already been "trimmed" to correct 
** length.
** 
** Returns one of the following:
**    MD6_SUCCESS
**    MD6_NULLSTATE                      (if input state pointer was NULL)
*/
void HMD6::ComputeHexHashval()
{
	for(int i = 0; i < ((d + 7) / 8); ++i)
	{
		hexhashval[2*i  ] = "0123456789abcdef"[(hashval[i] >> 4) & 0xf];
		hexhashval[2*i+1] = "0123456789abcdef"[(hashval[i])      & 0xf];
	}

	/* insert zero string termination byte at position ceil(d/4) */
	hexhashval[(d+3)/4] = 0;
}

/* trim hashval to desired length d bits by taking only last d bits */
/* note that high-order bit of a byte is considered its *first* bit */
void HMD6::TrimHashval()
{
	int full_or_partial_bytes = (d+7)/8;
	int bits = d % 8;                 /* bits in partial byte */
	int i;

	/* move relevant bytes to the front */
	for(i = 0; i < full_or_partial_bytes; ++i)
		hashval[i] = hashval[c*(w/8) - full_or_partial_bytes + i];

	/* zero out following bytes */
	for(i = full_or_partial_bytes; i < c*(w/8); ++i)
		hashval[i] = 0;

	/* shift result left by (8-bits) bit positions, per byte, if needed */
	if(bits > 0)
	{
		for(i = 0; i < full_or_partial_bytes; ++i)
		{
			hashval[i] = (hashval[i] << (8-bits));
			if((i+1) < c*(w/8))
				hashval[i] |= (hashval[i+1] >> bits);
		}
	}
}

/* Construct control word V for given inputs.
** Input:
**   r = number of rounds
**   L = mode parameter (maximum tree height)
**   z = 1 if this is final compression operation
**   p = number of pad bits in a block to be compressed
**   keylen = number of bytes in key
**   d = desired hash output length
**   Does not check inputs for validity.
*/
HMD6::ControlWord HMD6::MakeControlWord(int r, int L, int z, int p, int keylen, int d)
{
	return (
		(((ControlWord) 0) << 60) |  /* reserved, width  4 bits */
		(((ControlWord) r) << 48) |			/* width 12 bits */
		(((ControlWord) L) << 40) |			/* width  8 bits */
		(((ControlWord) z) << 36) |			/* width  4 bits */
		(((ControlWord) p) << 20) |			/* width 16 bits */
		(((ControlWord) keylen) << 12 ) |		/* width  8 bits */
		(((ControlWord) d))					/* width 12 bits */
	);
}

/* Make "unique nodeID" U based on level ell and position i 
** within level; place it at specified destination.
** Inputs:
**    dest = address of where nodeID U should be placed
**    ell = integer level number, 1 <= ell <= ...
**    i = index within level, i = 0, 1, 2,...
** Returns
**    U = constructed nodeID
*/
HMD6::NodeID HMD6::MakeNodeID(int ell, int i)
{
	return (
		(((NodeID)ell) << 56) | ((NodeID)i)
	);
}

/* Pack data before compression into n-word array N.
*/
void HMD6::Pack(md6_word *N, const md6_word *Q, md6_word *K, int ell, int i, int r, int L, int z, int p, int keylen, int d, md6_word *B)
{
	int ni;
	NodeID U;
	ControlWord V;

	ni = 0;

	for (int j = 0; j < q; ++j)
		N[ni++] = Q[j];       /* Q: Q in words     0--14 */

	for (int j = 0; j < k; ++j)
		N[ni++] = K[j];       /* K: key in words  15--22 */

	U = MakeNodeID(ell,i);             /* U: unique node ID in 23 */
	/* The following also works for variants 
	** in which u=0.
	*/
	memcpy(N + ni, &U,	min(u*(w/8), sizeof(NodeID)));
	ni += u;

	V = MakeControlWord(
	r,L,z,p,keylen,d);/* V: control word in   24 */
	/* The following also works for variants
	** in which v=0.
	*/
	memcpy(N + ni, &V, min(v*(w/8), sizeof(ControlWord)));
	ni += v;

	memcpy(N + ni, B, b*sizeof(md6_word));      /* B: data words    25--88 */
}

/* Assumes n-word input array N has been fully set up.
** Input:
**   N               input array of n w-bit words (n=89)
**   A               working array of a = rc+n w-bit words
**                   A is OPTIONAL, may be given as NULL 
**                   (then md6_compress allocates and uses its own A).
**   r               number of rounds            
** Modifies:
**   C               output array of c w-bit words (c=16)
*/
void HMD6::Compress(md6_word* C, md6_word* N, int r, md6_word* A)
{
	bool A_is_NULL;
	if (A_is_NULL = (A == NULL))
		A = new md6_word[r*c+n];

	memcpy_s(A, r*c+n, N, n*sizeof(md6_word));							/* copy N to front of A */
	MainCompressionLoop(A, r);											/* do all the work		*/
	memcpy_s(C, c*sizeof(md6_word), A+(r-1)*c+n, c*sizeof(md6_word));	/* output into C		*/

	if (A_is_NULL)           /* zero and free A if nec. */
	{
		ZeroMemory(A, (r*c+n)*sizeof(md6_word));	/* contains key info */
		delete[] A;
	}
}

/* Perform md6 block compression using all the "standard" inputs.
** Input:
**     Q              q-word (q=15) approximation to (sqrt(6)-2)
**     K              k-word key input (k=8)
**     ell            level number
**     i              index within level
**     r              number of rounds in this compression operation
**     L              mode parameter (max tree height)
**     z              1 iff this is the very last compression
**     p              number of padding bits of input in payload B
**     keylen         number of bytes in key
**     d              desired output hash bit length
**     B              b-word (64-word) data input block (with zero padding)
** Modifies:
**     C              c-word output array (c=16)
*/
void HMD6::StandardCompress(md6_word* C, const md6_word* Q, md6_word* K, int ell, int i, int r, int L, bool z, int p, int keylen, int d, md6_word* B)
{
	md6_word N[md6_n];
	md6_word A[5000];       /* MS VS can't handle variable size here */

	/* pack components into N for compression */
	Pack(N, Q, K, ell, i, r, L, z, p, keylen, d, B);

	Compress(C, N, r, A);
}

int HMD6::DefaultR(int d, int keylen)
{
	/* Default number of rounds is forty plus floor(d/4) */
	/* unless keylen > 0, in which case it must be >= 80 as well */
	return (keylen > 0) ? max(80, 40 + (d / 4)) : (40 + (d / 4));
}

/*
** Perform the md6 "main compression loop" on the array A.
** This is where most of the computation occurs; it is the "heart"
** of the md6 compression algorithm.
** Input:
**     A                  input array of length t+n already set up
**                        with input in the first n words.
**     r                  number of rounds to run (178); each is c steps
** Modifies:
**     A                  A[n..r*c+n-1] filled in.
*/
void HMD6::MainCompressionLoop(md6_word* A, int r)
{
	md6_word S = S0;
	md6_word x;

	for (int j = 0, i = n; j < (r*c); j += c)
	{

		/* ***************************************************************** */
		#define loop_body(rs, ls, step)                                   \
		x = S;                                /* feedback constant     */ \
		x ^= A[i+step-t5];                    /* end-around feedback   */ \
		x ^= A[i+step-t0];                    /* linear feedback       */ \
		x ^= ( A[i+step-t1] & A[i+step-t2] ); /* first quadratic term  */ \
		x ^= ( A[i+step-t3] & A[i+step-t4] ); /* second quadratic term */ \
		x ^= (x >> rs);                       /* right-shift           */ \
		A[i+step] = x ^ (x << ls);            /* left-shift            */   
		/* ***************************************************************** */

		/*
		** Unroll loop c=16 times. (One "round" of computation.)
		** Shift amounts are embedded in macros RLnn.
		*/
		RL00 RL01 RL02 RL03 RL04 RL05 RL06 RL07
		RL08 RL09 RL10 RL11 RL12 RL13 RL14 RL15

		/* Advance round constant S to the next round constant. */
		S = (S << 1) ^ (S >> (w-1)) ^ (S & Smask);
		i += 16;

		#undef loop_body
	}
}

/* compress block at level ell, and put c-word result into C.
** Input:
**     ell				0 <= ell < max_stack_height-1
**     lastCompression  lastCompression = true if this is very last compression; else false
** Output:
**     C          c-word array to put result in
** Modifies:
**     st->bits[ell]  (zeroed)
**     st->i_for_level[ell] (incremented)  
**     st->B[ell] (zeroed)
*/
void HMD6::CompressBlock(md6_word* C, int ell, bool lastCompression)
{
	int p;

	if (ell == 1) /* leaf; hashing data; reverse bytes if nec. */
	{
		if (ell < (L + 1)) /* PAR (tree) node */
			ReverseLittleEndian(&(B[ell][0]), b);
		else /* SEQ (sequential) node; don't reverse chaining vars */
			ReverseLittleEndian(&(B[ell][c]), b - c);
	}

	p = b*w - bits[ell];          /* number of pad bits */

	StandardCompress(C, Q, K, ell, (int)i_for_level[ell], r, L, lastCompression, p, keylen, d, B[ell]);

	bits[ell] = 0; /* clear bits used count this level */
	++i_for_level[ell];

	ZeroMemory(B[ell], sizeof(B[ell]));	/* clear B[ell] */
}

bool HMD6::IsLittleEndian()
{
	md6_word x = 1 | (((md6_word)2) << (w - 8));
	unsigned char *cp = (unsigned char*)&x;

	return (*cp == 1);
}

/* return byte-reversal of md6_word x.
** Written to work for any w, w=8,16,32,64.
*/
void HMD6::ByteReverse(md6_word* x)
{
	#define mask8  md6_word(0x00ff00ff00ff00ff)
	#define mask16 md6_word(0x0000ffff0000ffff)

	#if (w == 64)
	  *x = (*x << 32) | (*x >> 32);
	#endif
	#if (w >= 32)
	  *x = ((*x & mask16) << 16) | ((*x & ~mask16) >> 16);
	#endif
	#if (w >= 16)
	  *x = ((*x & mask8) << 8) | ((*x & ~mask8) >> 8);
	#endif

	#undef mask16
	#undef mask8
}

/* Append bit string src to the end of bit string dest
** Input:
**     dest         a bit string of destlen bits, starting in dest[0]
**                  if destlen is not a multiple of 8, the high-order
**                  bits are used first
**     src          a bit string of srclen bits, starting in src[0]
**                  if srclen is not a multiple of 8, the high-order
**                  bits are used first
** Modifies:
**     dest         when append_bits returns, dest will be modified to
**                  be a bit-string of length (destlen+srclen).
**                  zeros will fill any unused bit positions in the 
**                  last byte.
*/
void HMD6::AppendBits(unsigned char* dest, unsigned int destlen, unsigned char* src, unsigned int srclen)
{
	if (srclen == 0)
		return;

	/* Initialize accum, accumlen, and di */
	UINT16 accum = 0;    /* accumulates bits waiting to be moved, right-justified */
	int accumlen = 0; /* number of bits in accumulator */
	if ((destlen % 8) != 0)
	{
		accumlen = destlen%8;
		accum = dest[destlen/8];        /* grab partial byte from dest     */
		accum = accum >> (8-accumlen);  /* right-justify it in accumulator */
	}

	int di = destlen/8;        /* index of where next byte will go within dest */

	/* Now process each byte of src */
	int srcbytes = (srclen+7)/8;   /* number of bytes (full or partial) in src */

	/* shift good bits from src[i] into accum */
	for(int i = 0; i < srcbytes; ++i)
	{
		/* not last byte */
		if (i != srcbytes-1)
		{
			accum = (accum << 8) ^ src[i];  
			accumlen += 8;
		}
		/* last byte */
		else
		{
			int newbits = ((srclen % 8 == 0) ? 8 : (srclen % 8));
			accum = (accum << newbits) | (src[i] >> (8 - newbits));
			accumlen += newbits;
		}

		/* do as many high-order bits of accum as you can (or need to) */
		while(((i != srcbytes-1) & (accumlen >= 8)) || ((i == srcbytes-1) & (accumlen > 0)))
		{
			int numbits = min(8, accumlen);
			unsigned char bits;
			bits = accum >> (accumlen - numbits);    /* right justified */
			bits = bits << (8 - numbits);            /* left justified  */
			bits &= (0xff00 >> numbits);             /* mask            */
			dest[di++] = bits;                       /* save            */
			accumlen -= numbits; 
		}
	}
}

/* Byte-reverse words x[0...count-1] if machine is little_endian */
void HMD6::ReverseLittleEndian(md6_word* x, size_t count)
{
	if(IsLittleEndian())
		for(size_t i = 0; i < count; ++i)
			ByteReverse(x + i);
}

HMD6::HMD6(int bitLength, const unsigned char* salt, int saltLength)
{
	if(salt && (saltLength > 0))
	{
		std::string strSaltHash;
		{
			HMD6 saltHash(bitLength);
			strSaltHash = dynamic_cast<IHash*>(&saltHash)->GetHashAndReset((const BYTE*)salt, saltLength);
		}

		Init(bitLength, (const unsigned char*)strSaltHash.c_str(), (int)strSaltHash.length(), md6_default_L, DefaultR(bitLength, (int)(strSaltHash.length())));
	}
	else
	{
		Init(bitLength, NULL, 0, md6_default_L, DefaultR(bitLength, 0));
	}
}

void HMD6::Update(const BYTE *data, const size_t len)
{
	size_t databitlen = len*8;

	unsigned int j, portion_size;

	j = 0; /* j = number of bits processed so far with this update */
	while (j < databitlen)
	{
		/* handle input string in portions (portion_size in bits)
		** portion_size may be zero (level 1 data block might be full, 
		** having size b*w bits) */
		portion_size = (unsigned int)min(databitlen-j, (unsigned int)(b * w - (bits[1])));

		ASSERTUME(
			(portion_size % 8 == 0) &&
			(bits[1] % 8 == 0) &&
			(j % 8 == 0)
		);

		if(
			(portion_size % 8 == 0) &&
			(bits[1] % 8 == 0) &&
			(j % 8 == 0)
		)
		{
			/* use mempy to handle easy, but most common, case */
			memcpy((char *)B[1] + bits[1]/8, data + (j / 8), portion_size / 8);
		}
		else /* handle messy case where shifting is needed */
		{
			AppendBits((unsigned char *)B[1], /* dest */
			bits[1],		 /* dest current bit size */
			(unsigned char*)(data + j/8),	   /* src */
			portion_size);		 /* src size in bits  */
		}

		j += portion_size;
		bits[1] += portion_size;

		/* compress level-1 block if it is now full but we're not done yet */
		if ((bits[1] == b*w) && (j < databitlen))
			Process(1, false);

	} /* end of loop body handling input portion */
}


/* Final -- no more data; finish up and produce hash value.
*/
void HMD6::Finalize()
{
	/* md6_final was previously called */
	if (finalized == 1)
		return;

	/* force any processing that needs doing */
	int ell = 1;

	if(top != 1)
		for (; ell <= top; ++ell)
			if (bits[ell] > 0)
				break;
	/* process starting at level ell, up to root */
	Process(ell, true);

	/* md6_process has saved final chaining value in st->hashval */

	ReverseLittleEndian((md6_word*)hashval, c);

	TrimHashval();
	ComputeHexHashval();

	finalized = 1;
}

std::string HMD6::GetHash()
{
	return (const char*)hexhashval;
}

void HMD6::Reset()
{
	if(!initialized)
		return;

	finalized = 0;

	ZeroMemory(B, sizeof(B));
	ZeroMemory(bits, sizeof(bits));
	ZeroMemory(hashval, sizeof(hashval));
	ZeroMemory(hexhashval, sizeof(hexhashval));
	ZeroMemory(i_for_level, sizeof(i_for_level));

	initialized = 1;
	top = 1;
	/* if SEQ mode for level 1; use IV=0  */
	/* zero bits already there by memset; */
	/* we just need to set st->bits[1]    */
	if (L == 0)
		bits[1] = c*w;
}

HMD6::~HMD6()
{
	// Clear the class for security reasons.
	Reset();
}
