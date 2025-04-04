#include "commands.h"

int sectionCount(struct node* data)
{
	int counter = 0;
	struct node* temp = data;
	while (temp != nullptr)
	{
		counter += temp->count;
		temp = temp->next;
	}
	return counter;
}

int selectorCount(int i, struct node* data)
{
	struct block* temp = findBlock(i, data);
	if(temp != nullptr)
	{
		int selCount = 0;
		struct selector* tempSel = temp->sel;
		if (tempSel == nullptr) return 0;
		while (tempSel != nullptr)
		{
			selCount++;
			tempSel = tempSel->next;
		}
		return selCount;
	}
	return NO_BLOCK;
}

int attribCount(int i, struct node* data)
{
	struct block* temp = findBlock(i, data);
	if (temp != nullptr)
	{
		int attCount = 0;
		struct attrib* tempAtt = temp->att;
		while (tempAtt != nullptr)
		{
			attCount++;
			tempAtt = tempAtt->next;
		}
		return attCount;
	}
	return false;
}

LiteString* specificSelector(int i, int j, struct node* data)
{
	struct block* temp = findBlock(i, data);
	if (temp != nullptr && j > 0)
	{
		int selCount = 0;
		struct selector* tempSel = temp->sel;
		while (tempSel != nullptr)
		{
			if (++selCount == j) break;
			tempSel = tempSel->next;
		}
		if (tempSel != nullptr) return tempSel->name;
	}
	return nullptr;
}

LiteString* specificAttribValue(int i, char* n, struct node* data)
{
	struct block* temp = findBlock(i, data);
	if (temp != nullptr)
	{
		struct attrib* tempAtt = temp->att;
		while (tempAtt != nullptr)
		{
			if (*(tempAtt->prop) == n) return tempAtt->val;
			tempAtt = tempAtt->next;
		}
	}
	return nullptr;
}

int thisAttribCount(char* n, struct node* data)
{
	int attCount = 0;
	if (data != nullptr)
	{
		struct node* temp = data;
		struct attrib* tempAtt = nullptr;
		while (temp != nullptr)
		{
			for (int i = 0; i < T; i++)
			{
				if (temp->blocks[i] != nullptr)
				{
					tempAtt = temp->blocks[i]->att;
					while (tempAtt != nullptr)
					{
						if (*(tempAtt->prop) == n)
						{
							attCount++;
							break;
						}
						tempAtt = tempAtt->next;
					}
				}
			}
			temp = temp->next;
		}
	}
	return attCount;
}

int thisSelectorCount(char* z, struct node* data)
{
	int selCount = 0;
	if (data != nullptr)
	{
		struct node* temp = data;
		struct selector* tempSel = nullptr;
		while (temp != nullptr)
		{
			for (int i = 0; i < T; i++)
			{
				if (temp->blocks[i] != nullptr)
				{
					tempSel = temp->blocks[i]->sel;
					while (tempSel != nullptr)
					{
						if (*(tempSel->name) == z)
						{
							selCount++;
							break;
						}
						tempSel = tempSel->next;
					}
				}
			}
			temp = temp->next;
		}
	}
	return selCount;
}

LiteString* selectorsAttVal(char* n, char* z, struct node* data)
{
	if (data != nullptr)
	{
		struct node* temp = data;
		struct selector* tempSel = nullptr;
		struct attrib* tempAtt = nullptr;
		while (temp->next != nullptr)
			temp = temp->next;
		while (temp != nullptr)
		{
			for (int i = T-1; i >= 0; i--)
			{
				if (temp->blocks[i] != nullptr)
				{
					tempSel = temp->blocks[i]->sel;
					if (tempSel == nullptr)
					{
						tempAtt = temp->blocks[i]->att;
						while (tempAtt != nullptr)
						{
							if (*(tempAtt->prop) == n)
							{
								return tempAtt->val;
							}
							tempAtt = tempAtt->next;
						}
					}
					else while (tempSel != nullptr)
					{
						if (*(tempSel->name) == z)
						{
							tempAtt = temp->blocks[i]->att;
							while (tempAtt != nullptr)
							{
								if (*(tempAtt->prop) == n) 
								{
									return tempAtt->val;
								}
								tempAtt = tempAtt->next;
							}
						}
						tempSel = tempSel->next;
					}
				}
			}
			temp = temp->prev;
		}
	}
	return nullptr;
}

bool deleteSection(int i, struct node** data)
{
	if (*data != nullptr && i > 0)
	{
		int j = 0;
		struct node* temp = findBlock(i, j, *data);
		if (temp != nullptr)
		{
			deleteSelectorList(temp->blocks[j]->sel);
			deleteAttribList(temp->blocks[j]->att);
			deleteBlock(temp, data, i, j);
			return true;
		}
	}
	return false;
}

bool deleteAttrib(int i, char* n, struct node** data)
{
	if (*data != nullptr && i > 0)
	{
		int j = 0;
		struct node* temp = findBlock(i, j, *data);
		if(temp != nullptr)
		{
			if (*(temp->blocks[j]->att->prop) == n)
			{
				struct attrib* attNext = temp->blocks[j]->att->next;
				delete temp->blocks[j]->att->prop;
				delete temp->blocks[j]->att->val;
				delete temp->blocks[j]->att;
				temp->blocks[j]->att = attNext;
				if (temp->blocks[j]->att == nullptr)
				{
					deleteSelectorList(temp->blocks[j]->sel);
					deleteBlock(temp, data, i, j);
				}
				return true;
			}
			else {
				struct attrib* att = temp->blocks[j]->att;
				while (att->next != nullptr)
				{
					if (*(att->next->prop) == n)
					{
						struct attrib* attSecondNext = att->next->next;
						delete att->next->prop;
						delete att->next->val;
						delete att->next;
						att->next = attSecondNext;
						return true;
					}
					else {
						att = att->next;
					}
				}
			}
		}
	}
	return false;
}