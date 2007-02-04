// Implementation of pure LZ77 looseless compression.
// Modification of this algorithm was used for NTFS compression.
//
// Implemented by Arkadi Kagan.
//
#include "LZ77.h"

#define	BITS_LEN	4

CLZ77::CLZ77()
{
}
CLZ77::~CLZ77()
{
}

long CLZ77::LZComp(BYTE *s1, BYTE *s2, long maxlen)
{
	long i;
	for (i = 0; i < maxlen; i++)
		if (s1[i] != s2[i])
			return i;
	return maxlen;
}
BYTE *CLZ77::FindLZ(BYTE *source, BYTE *s, long slen, long border, long mlen, long &len)
{
	register long maxlen = 0;
	register long limit = slen - (s-source);
	BYTE *maxp = s-1;
	BYTE *p;
	len = 0;
	for (p = s-1; p >= source; p--)
	{
		len = LZComp(p, s, limit);
		if (len > maxlen)
		{
			maxp = p;
			maxlen = len;
		}
		if (s-p >= border-1) break;
		if (len >= mlen-1) break;
	}
	len = min(maxlen, mlen-1);
	return maxp;
}

long CLZ77::GetMaxEncoded(long len)
{
	return len + sizeof(DWORD);
}
long CLZ77::GetMaxDecoded(BYTE *source)
{
	return ((DWORD*)source)[0];
}
void CLZ77::Encode(BYTE *target, long &tlen, BYTE *source, long slen)
{
	long len, block;
	long shift, border;
	BYTE *s, *t, *p;
	BYTE *flag;
	WORD *ptmp;

	((DWORD*)target)[0] = slen;		// save source size
	target += sizeof(DWORD);
	tlen = sizeof(DWORD);

	block = 0;				// block - bit in single flag byte
	shift = 16;				// shift offset to most significant bits
	border = 1;				// offset can`t be more then border
	flag = target;			// flag for every 8 entities
	tlen++;					// byte for first flag
	*flag = 0;
	s = source;
	t = target + 1;
	for (s = source; s-source < slen; )
	{
		if (shift > BITS_LEN)
			while (s-source >= border)
			{
				if (shift <= BITS_LEN) break;
				border = border << 1;
				shift--;
			}
		p = FindLZ(source, s, slen, border, (1<<shift), len);
		if (len <= 2) len = 1;
		if (len <= 1)
		{
			*t++ = *s++;
			tlen++;
		} else
		{
			ptmp = (WORD*)t;
			*ptmp = (WORD)(((s-p-1)<<shift) + len);

			*flag |= 1<<block;
			t += 2;
			tlen += 2;
			s += len;
		}
		if (++block >= 8)
		{
			flag = t++;
			*flag = 0;
			block = 0;
			tlen++;
		}
		if (tlen >= slen)
		{
			tlen = 0;
			return;
		}
		OnStep();
	}
}
long CLZ77::Decode(BYTE *target, long &tlen, BYTE *source, long slen)
{
	long i;
	long block, len;
	long shift, border;
	BYTE *s, *t, *p;
	BYTE *flag;
	WORD *ptmp;

	tlen = ((DWORD*)source)[0];
	source += sizeof(DWORD);			// read/remove target size
	slen -= sizeof(DWORD);

	t = target;
	flag = source;
	block = 0;				// block - bit in single flag byte
	shift = 16;				// shift offset to most significant bits
	border = 1;				// offset can`t be more then border
	for (s = source+1; (s < source+slen) && (t-target < tlen); )
	{
		if (shift > BITS_LEN)
			while (t-target >= border)
			{
				if (shift <= BITS_LEN) break;
				border = border << 1;
				shift--;
			}
		if (flag[0]&(1<<block))
		{
			ptmp = (WORD*)s;
			len = ((1<<shift)-1)&ptmp[0];
			p = t - (ptmp[0]>>shift) - 1;
			for (i = 0; i < len; i++)
				t[i] = p[i];
			t += len;
			s += 2;
		} else
		{
			*t++ = *s++;
			len = 1;
		}
		if (++block >= 8)
		{
			flag = s++;
			block = 0;
		}
		OnStep();
	}
	return (s-source) + sizeof(DWORD);
}
