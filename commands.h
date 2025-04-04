#pragma once
#include "litestring.h"
#include "commons.h"
#include "structManip.h"

// Command parser functions

// i - section number
// j - selector number
// n - attribute name
// z - selector name

int sectionCount(struct node* data);								// ?
int selectorCount(int i, struct node* data);						// i,S,?
int attribCount(int i, struct node* data);							// i,A,?
LiteString* specificSelector(int i, int j, struct node* data);		// i,S,j
LiteString* specificAttribValue(int i, char* n, struct node* data);	// i,A,n
int thisAttribCount(char* n, struct node* data);					// n,A,?
int thisSelectorCount(char* z, struct node* data);					// z,S,?
LiteString* selectorsAttVal(char* n, char* z, struct node* data);	// z,E,n
bool deleteSection(int i, struct node** data);						// i,D,*
bool deleteAttrib(int i, char* n, struct node** data);				// i,D,n