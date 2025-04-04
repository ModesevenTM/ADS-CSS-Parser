#pragma once
#include <iostream>
#include "litestring.h"

#define T                   8
#define STRLEN              128
#define BUFLEN              2048
#define COMMAND_START       "????"
#define PARSE_RESUME        "****"
#define NO_BLOCK            -1

enum inputState {
    parse,
    command
};

enum parsedType {
    selector,
    attrib,
    attVal
};

struct selector {
    LiteString* name = nullptr;
    struct selector* next = nullptr;
};

struct attrib {
    LiteString* prop = nullptr;
    LiteString* val = nullptr;
    struct attrib* next = nullptr;
};

struct block {
    struct selector* sel = nullptr;
    struct attrib* att = nullptr;
};

struct node {
    unsigned short count = 0;
    struct block* blocks[T] = { nullptr };
    struct node* prev = nullptr;
    struct node* next = nullptr;
};

struct currentConfig {
    enum inputState state = parse;
    enum parsedType type = selector;
    bool isStringStarted = false;
    struct block* currentBlock = nullptr;
    struct selector* currentSel = nullptr;
    struct attrib* currentAtt = nullptr;
    int stringStart = 0;
    LiteString* tempStr = nullptr;
};