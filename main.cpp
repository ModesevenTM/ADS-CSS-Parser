#define _CRT_SECURE_NO_WARNINGS
#include "includes.h"
using namespace std;


int main()
{
    char inputBuf[BUFLEN] = "";
    struct node* cssData = nullptr;
    struct currentConfig config;

    while (cin.getline(inputBuf, STRLEN))
    {
        modeSwitch(config, inputBuf);
        if (config.state == parse && strcmp(inputBuf, (char*)PARSE_RESUME))
            parseCSS(config, inputBuf, &cssData);
        else parseCommands(inputBuf, &cssData);
    }
    clearData(&cssData);
    return 0;
}