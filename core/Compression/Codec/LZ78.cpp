// Implementation of LZ78 looseless compression.
//
// Implemented by Arkadi Kagan
//
#include "LZ78.h"
#include <string.h>
#include "../Fatal/Fatal.h"

CLZ78::CLZ78()
{
	memset(&nill, 0, sizeof(nill));
	nill.nill_key.next = nill.nill_key.prev = &nill.nill_key;
	nill.nill_key.pnode = &nill;
	LastKey = 0;
	border = 2;
	bits_len = 1;
}
CLZ78::~CLZ78()
{
	CleanAll();
}

void CLZ78::CleanAll()
{
	while (tree.root != tree.NIL)
		tree.deleteNode(tree.root);

	while (nill.nill_key.next != &nill.nill_key)
		DeleteKey(nill.nill_key.next);
	LastKey = 0;
	border = 2;
	bits_len = 1;
}

void CLZ78::bitscpy(BYTE *target, long &toffset, BYTE *source, long &soffset, long bitslen)
{
	long i;
	for (i = 0; i < bitslen; i++)
		target[(toffset+i)/8] |= ((source[(soffset+i)/8]>>((soffset+i)%8))&1) << ((toffset+i)%8);
	toffset += bitslen;
	soffset += bitslen;
}

CLZ78::tagNode *CLZ78::FindKeyWord(DWORD key_word)
{
	tagNode tmpNode;
	tmpNode.key_word = key_word;
	RedBlackNode::Node *tnode = tree.findNode(&tmpNode);
	if (tnode == NULL)
		return NULL;
	return tnode->data;
}
CLZ78::tagNode *CLZ78::FindKey(CLZ78::tagNode *node, BYTE key)
{
	tagKey *cur;
	for (cur = node->nill_key.next; cur != &node->nill_key; cur = cur->next)
		if (cur->key == key)
			return cur->node;
	return NULL;
}
CLZ78::tagNode *CLZ78::AddKey(CLZ78::tagNode *node, DWORD key_word, BYTE key)
{
	tagKey *cur_key = new tagKey;
	tagNode *cur_node = new tagNode;
	memset(cur_key, 0, sizeof(tagKey));
	memset(cur_node, 0, sizeof(tagNode));

	cur_key->key = key;
	cur_key->node = cur_node;
	cur_key->pnode = node;

	cur_key->next = node->nill_key.next;
	cur_key->prev = &node->nill_key;
	cur_key->next->prev = cur_key;
	cur_key->prev->next = cur_key;

	cur_node->nill_key.next = cur_node->nill_key.prev = &cur_node->nill_key;
	cur_node->nill_key.pnode = cur_node;

	cur_node->key_word = key_word;
	cur_node->parent = cur_key;
	cur_node->level = node->level + 1;

	tree.insertNode(cur_node);

	return cur_node;
}
DWORD CLZ78::GenerateKey()
{
	if (LastKey >= border-1)
	{
		border = border << 1;
		bits_len++;
	}
	if (bits_len >= 8*sizeof(DWORD))
		FatalError("Dictionary is full. Source is too big or demaged");
	return ++LastKey;
}

void CLZ78::DeleteNode(CLZ78::tagNode *node)
{
	while (node->nill_key.next != &node->nill_key)
		DeleteKey(node->nill_key.next);
}
void CLZ78::DeleteKey(CLZ78::tagKey *key)
{
	DeleteNode(key->node);

	key->next->prev = key->prev;
	key->prev->next = key->next;

	delete key->node;
	delete key;
}

// slength must be length of source sequence in bytes
long CLZ78::EncodeOnce(BYTE *target, long &toffset, BYTE *source, long slength)
{
	long i, offset;
	tagNode *node = &nill, *tnode;
	for (i = 0; i < slength; i++)
	{
		tnode = FindKey(node, source[i]);
		if (tnode == NULL)
		{
			offset = 0;
			bitscpy(target, toffset, (BYTE*)&node->key_word, offset, bits_len);
			offset = i*sizeof(BYTE)*8;
			bitscpy(target, toffset, source, offset, sizeof(BYTE)*8);
			AddKey(node, GenerateKey(), source[i]);
			return i+1;
		}
		node = tnode;
	}
	if (node != &nill)
	{
		node = node->parent->pnode;
		offset = 0;
		bitscpy(target, toffset, (BYTE*)&node->key_word, offset, bits_len);
		offset = (slength-1)*sizeof(BYTE)*8;
		bitscpy(target, toffset, source, offset, sizeof(BYTE)*8);
	}
	return slength;
}
long CLZ78::DecodeOnce(BYTE *target, BYTE *source, long &soffset)
{
	tagNode *node, *fnode = NULL;
	long len = 0, offset = 0;
	DWORD key_word = 0;
	bitscpy((BYTE*)&key_word, offset, source, soffset, bits_len);
	node = FindKeyWord(key_word);
	if (node != NULL)
	{
		fnode = node;
		len = node->level;
		while (node->parent != NULL)
		{
			target[node->level-1] = node->parent->key;
			node = node->parent->pnode;
		}
	} else fnode = &nill;
	offset = 0;
	bitscpy(target + len, offset, source, soffset, 8*sizeof(BYTE));
	AddKey(fnode, GenerateKey(), target[len]);
	return len+1;
}

long CLZ78::GetMaxEncoded(long len)
{
	return len+sizeof(DWORD);
}
long CLZ78::GetMaxDecoded(BYTE *source)
{
	return ((DWORD*)source)[0];
}
void CLZ78::Encode(BYTE *target, long &tlen, BYTE *source, long slen)
{
	long toffset = 0;
	long coded = 0;
	long tmp;
	DWORD *size = (DWORD*)target;
	memset(target, 0, tlen);
	*size = slen;
	target += sizeof(DWORD);
	tlen = sizeof(DWORD);
	while ((tmp = EncodeOnce(target, toffset, source + coded, slen - coded)) != 0)
	{
		coded += tmp;
		if (toffset/8 >= slen)
		{
			tlen = 0;
			return;
		}
		OnStep();
	}
	tlen += toffset/8 + 1;

	CleanAll();
}
long CLZ78::Decode(BYTE *target, long &tlen, BYTE *source, long)
{
	long coded = 0;
	long offset = 0;
	tlen = ((DWORD*)source)[0];
	source += sizeof(DWORD);
	memset(target, 0, tlen);
	while (coded < tlen)
	{
		coded += DecodeOnce(target + coded, source, offset);
		OnStep();
	}
	CleanAll();

	return offset/8+1 + sizeof(DWORD);
}
