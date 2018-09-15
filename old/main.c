#include <stdio.h>
#include <string.h>
#include "ash.h"
#define MEM 4096

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("ERROR!%d", argc);
        return -1;
    }

    int count;
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "wb");
    asmcode arr[MAX];
    uint16_t *out;
    
    count = first_pass(arr, input);
    out = second_pass(arr);
    fwrite(out, sizeof(uint16_t), MEM, output);

    fclose(input);
    fclose(output);
    print_code(arr, count);

    return 0;

}
