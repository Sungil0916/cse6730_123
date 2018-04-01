#include "list.h"
struct s_luld
{
    int min, max;
};
typedef struct s_luld t_luld;

struct s_asset
{
    char* name;
    float price_per_share;
    struct s_luld luld;
    t_list* history_price;
    t_list* history_time;
    
};
typedef struct s_asset t_asset;

struct s_order
{
    int time;
    struct s_asset *asset;
    int quantity;
};
typedef struct s_order t_order;
