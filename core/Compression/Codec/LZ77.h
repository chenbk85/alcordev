#ifndef __LZ77_H
#define __LZ77_H

#include "Gendef.h"

class CLZ77
{
private:
	long LZComp(BYTE *s1, BYTE *s2, long maxlen);
	BYTE *FindLZ(BYTE *source, BYTE *s, long slen, long border, long mlen, long &len);
public:
	CLZ77();
	virtual ~CLZ77();

	void Encode(BYTE *target, long &tlen, BYTE *source, long slen);
	long Decode(BYTE *target, long &tlen, BYTE *source, long slen);
	long GetMaxEncoded(long len);
	long GetMaxDecoded(BYTE *source);

	virtual void OnStep() = 0;
};

#endif
