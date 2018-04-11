#include "list.h"
struct s_luld
{
    double min, max;
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
    double mean;
    double variance;
};
typedef struct s_ndist t_ndist;
