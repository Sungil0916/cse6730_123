#include "list.h"
struct s_luld
{
    int min, max;
};
typedef struct s_luld t_luld;

struct s_order
{
    int time;
    int quantity;
};
typedef struct s_order t_order;

struct s_ndist
{
    float mean;
    float variance;
};
typedef struct s_ndist t_ndist;
