#pragma once
#include "commons.h"
#include "commands.h"

struct block* findBlock(int i, struct node* data);							// returns the i-section pointer directly
struct node* findBlock(int i, int& tPos, struct node* data);				// returns the node where i-section is (for deletion only)
void deleteBlock(struct node* thisNode, struct node** data, int i, int pos);// deletes the block from the node and, if there aren't any blocks left, also the node
void deleteAttribList(struct attrib* data);									// deletes the attribute list from the block
void deleteSelectorList(struct selector* data);								// deletes the selector list from the block
struct block* appendBlock(struct node** data);								// creates a new block (and a node if needed)
struct attrib* findAttrib(struct block* block, char* name);					// returns a new attribute list entry or an existing one if a name matches
bool findSelector(struct block* block, LiteString* name);					// checks if selector with this name exists in a list
void clearData(struct node** data);											// clears all the CSS data after the parsing ends