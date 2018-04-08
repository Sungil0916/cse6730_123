#include "fargs.h"
#include <string.h>
char* get_arg(char* flag, int argc, char** argv)
{
    int start = (flag[0] == '-') ? 1 : 0;
    for (int i = 0; i < argc - 1; i++)
    {
        if (strcmp(argv[i] + 1, &flag[start]) == 0)
        {
            return argv[i + 1];
        }
    }
    return 0;
}
