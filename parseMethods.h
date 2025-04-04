#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "commons.h"
#include "commands.h"
#include "structManip.h"
using namespace std;

void modeSwitch(struct currentConfig& cfg, char* input);
void parseCSS(struct currentConfig& cfg, char* input, struct node** data);
bool parseCommands(char* input, struct node** data);
bool detectNum(char input);
void strRightStrip(char* input, unsigned int i);