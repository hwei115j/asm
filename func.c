#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "asm.h"


static char *instr[] = {"AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ", "CLA" ,"CLE", "CMA",
                        "CME", "CIR", "CIL", "INC", "SPA", "SNA", "SZA", "SZE", "HLT", "INP"
                        "OUT", "SKI", "SKO", "ION", "IOF", "HEX", "DEC", "\0"};

static int instr_code[] = {0, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7800, 0x7400, 
                            0x7200, 0x7100, 0x7080, 0x7040, 0x7020, 0x7010, 0x7008, 0x7004,
                            0x7002,0x7001,0xF800, 0xF400, 0xF200, 0xF100, 0xF080, 0xF040, 0, 0};

static void clean(char *str)    //清理註解
{
    while(*str != '/' && *str++);
    str[-1] = '\0';
}
static int islab(char *str)     //判斷是否是label 有1無0
{
    while(*str != ',' && *str++);

    return !!(*str);
}

static int isnop(char *str)
{
    return !strcmp(str, "NOP");
}

static int isorg(char *str)
{
    return !strcmp(str, "ORG");
}

static int isend(char *str)
{
    return !strcmp(str, "END");
}

static int ishex(char *str)
{
    return !strcmp(str, "HEX");
}

static int tohex(char *str)
{
    int num;

    sscanf(str, "%x", &num);

    return num;
}

static int tonum(char *str)
{
    return atoi(str);
}

static char *tostr(int n, char *str)
{
    sprintf(str, "%d", n);
    return str;
}

static uint16_t coding(struct line tabel)   //產生機器碼
{
    int code, param = tonum(tabel.param);

    for(int i = 0; instr[i][0]; i++)        //搜尋指令
        if(!strcmp(tabel.instr, instr[i]))
        {
            code = instr_code[i];
            break;
        }
    if(tabel.i)                             //如果I存在，則加上0x8000(最高位元為一)
        return code + param + 0x8000;
    return code + param;
}

static int decode(char *in, struct line *tabel)    //建立表格，傳回的是下一行的LC，如果為END，傳回-1(表格的結束)
{
    char str[4][20] = {};
    int lc = tabel->lc + 1;

    clean(in);
    sscanf(in, "%s%s%s%s", str[0], str[1], str[2], str[3]);

    for(int i = 0; i < 4; i++)  //建立每一行的表格
    {
        if(islab(str[i]))
        {
            int j;

            for(j = 0; str[i][j+1] != '\0'; j++);
            str[i][j] = '\0';
            strcpy(tabel->label, str[i]);
        }
        else
        {
            strcpy(tabel->instr, str[i++]);
            strcpy(tabel->param, str[i++]);
            tabel->i = (str[i][0] == 'I');
            break;
        }
    }

    if(ishex(tabel->instr))     //判斷HEX指令，並轉換為int
    {
        int num;

        sscanf(tabel->param, "%x", &num);
        tostr(num, tabel->param);
    }
    if(isorg(tabel->instr))     //設定下一行的LC
        return tonum(tabel->param);
    if(isend(tabel->instr))
        return -1;
    return lc;
}

int first_pass(FILE *in, struct line *tabel, struct symbol *symbol_tabel)   //處理輸入文件
{
    char str[100];
    int i, j;

    tabel[0].lc = 0;
    for(i = 0; fgets(str, 100, in) != NULL; i++)    //分析組合語言
        tabel[i + 1].lc = decode(str, &tabel[i]);
    for(i = 0, j = 0; tabel[i].lc != -1; i++)   //建立symbol tabel
        if(tabel[i].label[0])
        {
            strcpy(symbol_tabel[j].label, tabel[i].label);
            symbol_tabel[j++].lc = tabel[i].lc;
            symbol_tabel[j].lc = -1;
        }

    return 0;
}

uint16_t *second_pass(struct line *tabel, struct symbol *symbol_tabel)      //傳回機械碼
{
    uint16_t *mcode = malloc(sizeof(uint16_t) * MEM);
    int n = 0;                                                      //LC偏移量，每有一次nop增加一次，意思是跳過n個LC
    for(int i = 0; tabel[i].lc != -1; i++)
    {
        for(int j = 0; symbol_tabel[j].lc != -1; j++)           //symbol->實際地址 並 產生機械碼
            if(!strcmp(tabel[i].param, symbol_tabel[j].label))
            {
                tostr(symbol_tabel[j].lc, tabel[i].param);
                break;
            }
        if(isnop(tabel[i].instr))   //如果是NOP
            n++;    
        else if(!isorg(tabel[i].instr) && !isend(tabel[i].instr))    //忽略ORG & END
            mcode[tabel[i].lc - n] = coding(tabel[i]);
    }

    return mcode;
}
