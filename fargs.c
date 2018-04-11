#include "fargs.h"
#include <string.h>
int get_arg_index(char* flag, int argc, char** argv)
{
    int start = (flag[0] == '-') ? 1 : 0;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i] + 1, &flag[start]) == 0)
        {
            return i;
        }
    }
    return -1;
}
