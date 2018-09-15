#include <stdio.h>
#include <stdint.h>
#include "asm.h"

int main(int argc, char *argv[])
{
    struct line table[MEM];
    struct symbol symbol_table[MEM];
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "wb");
    uint16_t *out;

    if(argc < 3)
    {
        printf("ERROR!%d", argc);
        return -1;
    }

    first_pass(input, table, symbol_table);
    out = second_pass(table, symbol_table);

    fwrite(out, sizeof(uint16_t), MEM, output);
    fclose(input);
    fclose(output);

    return 0;
}
