#include "Codec/Huffman.h"
#include "Codec/LZ77.h"
#include "Codec/LZ78.h"
#include "Codec/LZW.h"
#include "Codec/Arithmetic.h"
#include "Codec/RLE.h"
#include "Generic.h"

#include "Fatal/Fatal.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <string.h>

#define	BLOCK_SIZE	0x10000
#define SIGNATURE	"CODR"

DWORD	block_size;

void Encode(FILE *s, FILE *t, char *sequence)
{
	int i;
	long slen, tlen, scount, length, compressed = 0;
	BYTE *source, *target;
	BYTE *signature = (BYTE*)SIGNATURE;
	compressed += fwrite(signature, 1, strlen((char*)signature), t);
	compressed += fwrite(&block_size, 1, sizeof(DWORD), t);
	fseek(s, 0, SEEK_END);
	length = ftell(s);
	fseek(s, 0, SEEK_SET);
	for (scount = 0; scount < length; scount += block_size)
	{
		source = new BYTE[block_size];
		slen = fread(source, 1, block_size, s);
		if (!slen)
			break;
		for (i = 0; sequence[i]; i++)
		switch(tolower(sequence[i]))
		{
		case 'a':
			{
				printf("Encoding Arithmetic, source size = %4ld\n", slen);
				TGeneric<CArithmetic> *ar = new TGeneric<CArithmetic>;
				tlen = ar->GetMaxEncoded(slen);
				target = new BYTE[tlen+1];
				ar->Encode(target+1, tlen, source, slen);
				if (tlen)
				{
					target[0] = 'a';
					delete source;
					source = target;
					slen = tlen+1;
				} else
				{
					delete target;
					target = new BYTE[slen+1];
					target[0] = 0;
					memcpy(target+1, source, slen);
					delete source;
					source = target;
					slen++;
				}
				delete ar;
			}
			break;
		case 'h':
			{
				printf("Encoding Huffman, source size = %4ld\n", slen);
				TGeneric<CHuffman> *huf = new TGeneric<CHuffman>;
				tlen = huf->GetMaxEncoded(slen);
				target = new BYTE[tlen+1];
				huf->Encode(target+1, tlen, source, slen);
				if (tlen)
				{
					target[0] = 'h';
					delete source;
					source = target;
					slen = tlen+1;
				} else
				{
					delete target;
					target = new BYTE[slen+1];
					target[0] = 0;
					memcpy(target+1, source, slen);
					delete source;
					source = target;
					slen++;
				}
				delete huf;
			}
			break;
		case '7':
			{
				printf("Encoding LZ77, source size = %4ld\n", slen);
				TGeneric<CLZ77> *lz = new TGeneric<CLZ77>;
				tlen = lz->GetMaxEncoded(slen);
				target = new BYTE[tlen+1];
				lz->Encode(target+1, tlen, source, slen);
				if (tlen)
				{
					target[0] = '7';
					delete source;
					source = target;
					slen = tlen+1;
				} else
				{
					delete target;
					target = new BYTE[slen+1];
					target[0] = 0;
					memcpy(target+1, source, slen);
					delete source;
					source = target;
					slen++;
				}
				delete lz;
			}
			break;
		case '8':
			{
				printf("Encoding LZ78, source size = %4ld\n", slen);
				TGeneric<CLZ78> *lz = new TGeneric<CLZ78>;
				tlen = lz->GetMaxEncoded(slen);
				target = new BYTE[tlen+1];
				lz->Encode(target+1, tlen, source, slen);
				if (tlen)
				{
					target[0] = '8';
					delete source;
					source = target;
					slen = tlen+1;
				} else
				{
					delete target;
					target = new BYTE[slen+1];
					target[0] = 0;
					memcpy(target+1, source, slen);
					delete source;
					source = target;
					slen++;
				}
				delete lz;
			}
			break;
		case 'w':
			{
				printf("Encoding LZW, source size = %4ld\n", slen);
				TGeneric<CLZW> *lz = new TGeneric<CLZW>;
				tlen = lz->GetMaxEncoded(slen);
				target = new BYTE[tlen+1];
				lz->Encode(target+1, tlen, source, slen);
				if (tlen)
				{
					target[0] = 'w';
					delete source;
					source = target;
					slen = tlen+1;
				} else
				{
					delete target;
					target = new BYTE[slen+1];
					target[0] = 0;
					memcpy(target+1, source, slen);
					delete source;
					source = target;
					slen++;
				}
				delete lz;
			}
			break;
		case 'r':
			{
				printf("Encoding RLE, source size = %4ld\n", slen);
				TGeneric<CRLE> *rle = new TGeneric<CRLE>;
				tlen = rle->GetMaxEncoded(slen);
				target = new BYTE[tlen+1];
				rle->Encode(target+1, tlen, source, slen);
				if (tlen)
				{
					target[0] = 'r';
					delete source;
					source = target;
					slen = tlen+1;
				} else
				{
					delete target;
					target = new BYTE[slen+1];
					target[0] = 0;
					memcpy(target+1, source, slen);
					delete source;
					source = target;
					slen++;
				}
				delete rle;
			}
			break;
		case 'c':
			delete source;
			fclose(s);
			fclose(t);
			FatalError("DCT compression is not implemented yet");
			break;
		case 'f':
			delete source;
			fclose(s);
			fclose(t);
			FatalError("Furie Transform compression is not implemented yet");
			break;
		default:
			delete source;
			fclose(s);
			fclose(t);
			FatalError("Unrecognized method of compression");
			break;
		};
		if (!i)
		{
			delete source;
			fclose(s);
			fclose(t);
			FatalError("Compression method is not specified");
		}
		if (!scount)
			compressed += fwrite(&i, 1, 1, t);
		compressed += fwrite(source, 1, slen, t);
		delete source;
	}
	printf("Compression successfull. Compressed size = %ld\n", compressed);
}
void Decode(FILE *s, FILE *t)
{
	int i, delta, saved, count = 0;
	long slen, tlen, length, decompressed = 0;
	long last_point, temp;
	BYTE *source, *target;
	BYTE signature[20];
	fseek(s, 0, SEEK_END);
	length = ftell(s);
	fseek(s, 0, SEEK_SET);
	fread(signature, 1, strlen(SIGNATURE), s);
	if (memcmp(signature, SIGNATURE, strlen((char*)SIGNATURE)))
		FatalError("The source file is not encoded by this tool.\nDecoding impossible.\n");
	fread(&block_size, 1, sizeof(DWORD), s);
	fread(&count, 1, 1, s);
	length--;
	last_point = ftell(s);
	slen = min((DWORD)length, block_size);
	while (last_point < length)
	{
		delta = 0;
		saved = 0;
		fseek(s, last_point, SEEK_SET);
		source = new BYTE[block_size];
		slen = fread(source, 1, block_size, s);
		for (i = 0; i < count; i++)
		{
			temp = 0;
			switch(tolower(source[delta]))
			{
			case 'a':
				{
					printf("Decoding Arithmetic, compressed size = %ld\n", slen);
					TGeneric<CArithmetic> *ar = new TGeneric<CArithmetic>;
					tlen = ar->GetMaxDecoded(source+1+delta);
					target = new BYTE[tlen];
					temp = ar->Decode(target, tlen, source+1+delta, slen-1-delta);
					delete source;
					source = target;
					slen = tlen;
					delta = 0;
					delete ar;
				}
				break;
			case 'h':
				{
					printf("Decoding Huffman, compressed size = %ld\n", slen);
					TGeneric<CHuffman> *huf = new TGeneric<CHuffman>;
					tlen = huf->GetMaxDecoded(source+1+delta);
					target = new BYTE[tlen];
					temp = huf->Decode(target, tlen, source+1+delta, slen-1-delta);
					delete source;
					source = target;
					slen = tlen;
					delta = 0;
					delete huf;
				}
				break;
			case '7':
				{
					printf("Decoding LZ77, compressed size = %ld\n", slen);
					TGeneric<CLZ77> *lz = new TGeneric<CLZ77>;
					tlen = lz->GetMaxDecoded(source+1+delta);
					target = new BYTE[tlen];
					temp = lz->Decode(target, tlen, source+1+delta, slen-1-delta);
					delete source;
					source = target;
					slen = tlen;
					delta = 0;
					delete lz;
				}
				break;
			case '8':
				{
					printf("Decoding LZ78, compressed size = %ld\n", slen);
					TGeneric<CLZ78> *lz = new TGeneric<CLZ78>;
					tlen = lz->GetMaxDecoded(source+1+delta);
					target = new BYTE[tlen];
					temp = lz->Decode(target, tlen, source+1+delta, slen-1-delta);
					delete source;
					source = target;
					slen = tlen;
					delta = 0;
					delete lz;
				}
				break;
			case 'w':
				{
					printf("Decoding LZW, compressed size = %ld\n", slen);
					TGeneric<CLZW> *lz = new TGeneric<CLZW>;
					tlen = lz->GetMaxDecoded(source+1+delta);
					target = new BYTE[tlen];
					temp = lz->Decode(target, tlen, source+1+delta, slen-1-delta);
					delete source;
					source = target;
					slen = tlen;
					delta = 0;
					delete lz;
				}
				break;
			case 'r':
				{
					printf("Decoding RLE, compressed size = %ld\n", slen);
					TGeneric<CRLE> *rle = new TGeneric<CRLE>;
					tlen = rle->GetMaxDecoded(source+1+delta);
					target = new BYTE[tlen];
					temp = rle->Decode(target, tlen, source+1+delta, slen-1-delta);
					delete source;
					source = target;
					slen = tlen;
					delta = 0;
					delete rle;
				}
				break;
			case 'c':
				fclose(s);
				fclose(t);
				FatalError("DCT compression is not implemented yet");
				break;
			case 'f':
				fclose(s);
				fclose(t);
				FatalError("Furie Transform compression is not implemented yet");
				break;
			case 0:	// not coded data
				printf("Unpacked data block, size = %ld\n", slen);
				delta++;
				temp = 0;
				break;
			default:
				fclose(s);
				fclose(t);
				FatalError("Unrecognized method of compression");
				break;
			};
			if (!delta && !saved)
			{
				last_point += temp + i + 1;
				saved = 1;
			}
		}
		if (!saved)
			last_point += slen + delta;
		if (!i)
		{
			fclose(s);
			fclose(t);
			FatalError("Compression method is not specified");
		}
		decompressed += fwrite(source + delta, 1, slen - delta, t);
		delete source;
	}
	printf("Decompression successfull. Decompressed size = %ld\n", decompressed);
}
void Help(char *name)
{
	printf("Use this program like this:\n");
	printf("%s [block_size] -{d|{e{a|h|7|8|w|c|f}}} <source> <target>\n", name);
	printf("Where:\n");
	printf("\tblock_size\t- Size of sub block of data to encode\n");
	printf("\t<source>\t- Source file name\n");
	printf("\t<target>\t- Target file name\n");
	printf("Options requered:\n");
	printf("\te\t- Encode file\n");
	printf("\td\t- Decode file\n");
	printf("Encoding methods:\n");
	printf("\ta\t- Arithmetic coding\n");
	printf("\th\t- Huffman coding\n");
	printf("\t7\t- LZ77 coding\n");
	printf("\t8\t- LZ78 coding\n");
	printf("\tw\t- LZW coding\n");
	printf("\tr\t- RLE coding\n");
	printf("\tc\t- DCT encoding for graphic\n");
	printf("\tf\t- Furie transform\n");
}
int main(int carg, char **arg)
{
	FILE *s, *t;
	char *sname, *tname, *options;
	printf("Compression utility\n");
	printf("Author: Arkadi Kagan\n");
	if ((carg != 4) && (carg != 5))
	{
		Help(arg[0]);
		return 1;
	}

	block_size = atol(arg[1]);
	if (!block_size)
		block_size = BLOCK_SIZE;

	options = arg[1 + (carg == 5)];
	sname = arg[2 + (carg == 5)];
	tname = arg[3 + (carg == 5)];

	s = fopen(sname, "rb");
	if (s == NULL)
		FatalError("Can`t open source file");
	t = fopen(tname, "wb+");
	if (t == NULL)
	{
		fclose(s);
		FatalError("Can`t create target file\n");
	}

	switch(tolower(options[1]))
	{
	case 'e': Encode(s, t, options + 2);
		break;
	case 'd': Decode(s, t);
		break;
	default:
		fclose(s);
		fclose(t);
		FatalError("Not specified action to perform (Encode or Decode)");
	};

	fclose(s);
	fclose(t);
	return 0;
}
