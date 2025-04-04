#include "parseMethods.h"

void modeSwitch(struct currentConfig& cfg, char* input)
{
    if (cfg.state == parse && strcmp(input, (char*)COMMAND_START) == 0)
        cfg.state = command;
    else if (cfg.state == command && strcmp(input, (char*)PARSE_RESUME) == 0)
        cfg.state = parse;
}

void parseCSS(struct currentConfig& cfg, char* input, struct node** data)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if (cfg.isStringStarted == false)
        {
            if (input[i] == '{' && cfg.type == selector)
            {
                cfg.currentBlock = appendBlock(data);
                cfg.type = attrib;
            }
            else if (input[i] == '}' && cfg.type == attrib)
            {
                cfg.type = selector;
                cfg.currentBlock = nullptr;
            }
            else if (input[i] > ' ')
            {
                cfg.isStringStarted = true;
                cfg.stringStart = i;
            }
        }
        else
        {
            if (cfg.type == attrib)
            {
                while (input[i] != ':') i++;
                input[i] = '\0';
                strRightStrip(input, i);
                cfg.currentAtt = findAttrib(cfg.currentBlock, &input[cfg.stringStart]);
                cfg.type = attVal;
                cfg.isStringStarted = false;
            }
            else
            {
                if(cfg.type == selector)
                    while (input[i] != ',' && input[i] != '{' && input[i] != '\0') i++;
                else
                    while (input[i] != ';' && input[i] != '}' && input[i] != '\0') i++;
                char sep = input[i];
                input[i] = '\0';
                strRightStrip(input, i);
                if (cfg.tempStr == nullptr) cfg.tempStr = new LiteString(&input[cfg.stringStart]);
                else if (strlen(&input[cfg.stringStart])) (*cfg.tempStr) += &input[cfg.stringStart];
                if (cfg.type == selector)
                {
                    if (sep == ',' || sep == '{')
                    {
                        if (!findSelector(cfg.currentBlock, cfg.tempStr))
                        {
                            if (cfg.currentBlock == nullptr)
                            {
                                cfg.currentBlock = appendBlock(data);
                                cfg.currentBlock->sel = new struct selector;
                                cfg.currentSel = cfg.currentBlock->sel;
                            }
                            else {
                                cfg.currentSel->next = new struct selector;
                                cfg.currentSel = cfg.currentSel->next;
                            }
                            cfg.currentSel->name = cfg.tempStr;
                            cfg.tempStr = nullptr;
                        }
                        cfg.isStringStarted = false;
                        if (sep == '{')
                        {
                            cfg.currentSel = nullptr;
                            cfg.type = attrib;
                        }
                    }
                    else
                    {
                        cfg.stringStart = 0;
                        break;
                    }
                }
                else {
                    if (sep == ';' || sep == '}')
                    {
                        cfg.currentAtt->val = cfg.tempStr;
                        cfg.tempStr = nullptr;
                        cfg.isStringStarted = false;
                        if (sep == ';')
                            cfg.type = attrib;
                        else {
                            cfg.type = selector;
                            cfg.currentAtt = nullptr;
                            cfg.currentBlock = nullptr;
                        }
                    }
                    else
                    {
                        cfg.stringStart = 0;
                        break;
                    }
                }
            }
        }
    i++;
    }
}

bool parseCommands(char* input, struct node** data)
{
    char inputCopy[STRLEN] = { '\0' };
    strcpy(inputCopy, input);
    char* token1 = strtok(input, ",");
    if (token1 == nullptr) return 0;
    else
    {
        char* token2 = strtok(nullptr, ",");
        char* token3 = strtok(nullptr, ",");
        char* redundantTok = strtok(nullptr, ",");
        if (strcmp(token1, "?") == 0 && token2 == nullptr)
            cout << inputCopy << " == " << sectionCount(*data) << endl;
        else if (token2 == nullptr || token3 == nullptr || strlen(token2) != 1 || redundantTok) return 0;
        else {
            unsigned int i = 0;
            if (detectNum(token1[i++]))
            {
                while (detectNum(token1[i])) i++;
                if (i == strlen(token1))
                {
                    int firstNum = atoi(token1);
                    switch (token2[0])
                    {
                    case 'A':
                        if (strcmp(token3, "?") == 0)
                        {
                            int attCount = attribCount(firstNum, *data);
                            if (attCount) cout << inputCopy << " == " << attCount << endl;
                        }
                        else
                        {
                            LiteString* val = specificAttribValue(firstNum, token3, *data);
                            if (val != nullptr) cout << inputCopy << " == " << *val << endl;
                        }
                        break;
                    case 'S':
                        if (strcmp(token3, "?") == 0)
                        {
                            int selCount = selectorCount(firstNum, *data);
                            if (selCount != NO_BLOCK) cout << inputCopy << " == " << selCount << endl;
                        }
                        else
                        {
                            if (strlen(token3) < 1) return 0;
                            else
                            {
                                LiteString* jSelector = specificSelector(firstNum, atoi(token3), *data);
                                if (jSelector != nullptr) cout << inputCopy << " == " << *jSelector << endl;
                            }
                        }
                        break;
                    case 'D':
                        if (strcmp(token3, "*") == 0)
                        {
                            if (deleteSection(firstNum, data)) cout << inputCopy << " == deleted" << endl;
                        }
                        else
                        {
                            if (deleteAttrib(firstNum, token3, data)) cout << inputCopy << " == deleted" << endl;
                        }
                        break;
                    default:
                        return 0;
                        break;
                    }
                }
                else return 0;
            }
            else {
                if (strlen(token1) > 0)
                {
                    switch (token2[0])
                    {
                    case 'A':
                        if (strcmp(token3, "?") == 0)
                            cout << inputCopy << " == " << thisAttribCount(token1, *data) << endl;
                        else return 0;
                        break;
                    case 'S':
                        if (strcmp(token3, "?") == 0)
                            cout << inputCopy << " == " << thisSelectorCount(token1, *data) << endl;
                        else return 0;
                        break;
                    case 'E':
                    {
                        LiteString* val = selectorsAttVal(token3, token1, *data);
                        if (val != nullptr) cout << inputCopy << " == " << *val << endl;
                        break;
                    }
                    default:
                        return 0;
                        break;
                    }
                }
                else return 0;
            }
        }
    }
    return 1;
}

bool detectNum(char input)
{
    if (input >= '0' && input <= '9') return true;
    return false;
}

void strRightStrip(char* input, unsigned int i)
{
    if (i > 0)
    {
        input[i] = '\0';
        int j = i - 1;
        while (input[j] <= ' ')
        {
            input[j] = '\0';
            j--;
        }
    }
}