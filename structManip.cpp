#include "structManip.h"

struct block* findBlock(int i, struct node* data)
{
	if (data != nullptr && i > 0)
	{
		int blocksSoFar = 0;
		struct node* temp = data;
		while (temp != nullptr)
		{
			blocksSoFar += temp->count;
			if (i <= blocksSoFar) break;
			else temp = temp->next;
		}
		if (temp == nullptr) return nullptr;
		else {
			blocksSoFar -= temp->count;
			blocksSoFar++;
			int j = 0;
			while (blocksSoFar != i || temp->blocks[j] == nullptr)
			{
				if (temp->blocks[j] != nullptr) blocksSoFar++;
				j++;
			}
			return temp->blocks[j];
		}
	}
	return nullptr;
}

struct node* findBlock(int i, int& tPos, struct node* data)
{
	if (data != nullptr && i > 0)
	{
		int blocksSoFar = 0;
		struct node* temp = data;
		while (temp != nullptr)
		{
			blocksSoFar += temp->count;
			if (i <= blocksSoFar) break;
			else temp = temp->next;
		}
		if (temp == nullptr) return nullptr;
		else {
			blocksSoFar -= temp->count;
			blocksSoFar++;
			int j = 0;
			while (blocksSoFar != i || temp->blocks[j] == nullptr)
			{
				if (temp->blocks[j] != nullptr) blocksSoFar++;
				j++;
			}
			tPos = j;
			return temp;
		}
	}
	return nullptr;
}

void deleteBlock(struct node* thisNode, struct node** data, int i, int pos)
{
	delete thisNode->blocks[pos];
	thisNode->blocks[pos] = nullptr;
	thisNode->count--;
	if (!thisNode->count)
	{
		if (thisNode->prev != nullptr) thisNode->prev->next = thisNode->next;
		if (thisNode->next != nullptr) thisNode->next->prev = thisNode->prev;
		if (i == 1) *data = thisNode->next;
		delete thisNode;
	}
}

void deleteAttribList(struct attrib* data)
{
	struct attrib* att = data;
	struct attrib* attNext = nullptr;
	while (att != nullptr)
	{
		attNext = att->next;
		delete att->prop;
		delete att->val;
		delete att;
		att = attNext;
	}
}

void deleteSelectorList(struct selector* data)
{
	struct selector* sel = data;
	struct selector* selNext = nullptr;
	while (sel != nullptr)
	{
		selNext = sel->next;
		delete sel->name;
		delete sel;
		sel = selNext;
	}
}

struct block* appendBlock(struct node** data)
{
	if (*data == nullptr)
	{
		*data = new struct node;
		(*data)->blocks[0] = new struct block;
		(*data)->count++;
		return (*data)->blocks[0];
	}
	else {
		struct node* temp = *data;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		if (temp->count < T)
		{
			int j = T - 1;
			while (temp->blocks[j] == nullptr) j--;
			if (++j < T)
			{
				temp->blocks[j] = new struct block;
				temp->count++;
				return temp->blocks[j];
			}
		}
		temp->next = new struct node;
		temp->next->prev = temp;
		temp = temp->next;
		temp->blocks[0] = new struct block;
		temp->count++;
		return temp->blocks[0];
	}
}

struct attrib* findAttrib(struct block* block, char* name)
{
	if (block->att == nullptr)
	{
		block->att = new struct attrib;
		block->att->prop = new LiteString(name);
		return block->att;
	}
	else
	{
		struct attrib* temp = block->att;
		if (*(temp->prop) == name) return temp;
		else
		{
			while (temp->next != nullptr)
			{
				if (*(temp->next->prop) == name) return temp->next;
				else temp = temp->next;
			}
			temp->next = new struct attrib;
			temp->next->prop = new LiteString(name);
			return temp->next;
		}
	}
}

bool findSelector(struct block* block, LiteString* name)
{
	if (block == nullptr || block->sel == nullptr) return false;
	else {
		struct selector* temp = block->sel;
		while (temp != nullptr)
		{
			if (*name == *(temp->name)) return true;
			temp = temp->next;
		}
		return false;
	}
}

void clearData(struct node** data)
{
	struct node* temp = *data;
	struct node* tempNext = nullptr;
	while (temp != nullptr)
	{
		for (int i = 0; i < T; i++)
		{
			if (temp->blocks[i] != nullptr)
			{
				deleteSelectorList(temp->blocks[i]->sel);
				deleteAttribList(temp->blocks[i]->att);
				delete temp->blocks[i];
			}
		}
		tempNext = temp->next;
		delete temp;
		temp = tempNext;
	}
}