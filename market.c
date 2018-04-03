#include <stdlib.h>
#include "market.h"

// Number of assets (IE google, microsoft, etc)
int n_assets = 2;
int cur_time = 0;
int end_time = 48;

t_list* FEL;
t_list* processed_orders;
t_asset* assets;

// Calculate LULD for a particular asset
void calc_limits(t_asset* asset)
{
    // TODO: Accuracy
    float price = asset->price_per_share;
    float dp = price * 0.01;
    asset->luld.min = price - dp;
    asset->luld.max = price + dp;
}

// Setup assets at the start of the simulation
void setup_assets()
{
    assets = malloc(sizeof(t_asset) * n_assets);
    t_list* lists;
    // Google
    lists = calloc(2, sizeof(t_list));
    assets[0].name = "GOOG";
    assets[0].price_per_share = 10;
    assets[0].history_price = &lists[0];
    assets[0].history_time = &lists[1];
    calc_limits(&assets[0]);
    // Microsoft
    lists = calloc(2, sizeof(t_list));
    assets[1].name = "MICR";
    assets[1].price_per_share = 20;
    assets[1].history_price = &lists[0];
    assets[1].history_time = &lists[1];
    calc_limits(&assets[1]);
}

// Create the orders that will be processed during the simulation
void setup_orders()
{
    FEL = malloc(sizeof(t_list));
    processed_orders = malloc(sizeof(t_list));
    int n_orders = 4; // Temporary
    t_order* orders = malloc(sizeof(t_order) * n_orders);
    orders[0].time = 0;
    orders[0].asset = NULL;
    
    orders[1].time = 5;
    orders[1].asset = &assets[0];
    orders[1].quantity = 3;
    
    orders[2].time = 10;
    orders[2].asset = &assets[1];
    orders[2].quantity = 7;
    
    orders[3].time = 24;
    orders[3].asset = NULL;
    for (int i = 0; i < n_orders; i++)
    {
        push_front(FEL, &orders[i]);
    }
}

// Adjust the price of an asset based on an order that went through
float get_change_in_asset_price(t_order* order)
{
    // TODO: Accuracy
    return order->asset->price_per_share * order->quantity * 0.001;
}

// Checks if the price_per_share of an asset will go out-of-bounds
// if this order is processed
int is_valid(t_order* order)
{
    float dp = get_change_in_asset_price(order);
    float p = order->asset->price_per_share + dp;
    return !(p > order->asset->luld.max || p < order->asset->luld.min);
}

// Process an order
void process_order(t_order* order)
{
    push_front(processed_orders, (void*)order);
    // Record asset price at the current time
    int* nums = malloc(sizeof(int) * 2);
    nums[0] = order->asset->price_per_share;
    nums[1] = cur_time;
    push_front(order->asset->history_price, &nums[0]);
    push_front(order->asset->history_time, &nums[1]);
    // Adjust the price
    order->asset->price_per_share += get_change_in_asset_price(order);
}

// Breaks an order into two so that at least one of them is valid
t_order* breakup_order(t_order* order)
{
    // TODO: Accuracy
    t_order* new = malloc(sizeof(t_order));
    new->asset = order->asset;
    new->time = order->time;
    new->quantity = order->quantity / 2;
    order->quantity /= 2;
}

int main(void)
{
    setup_assets();
    setup_orders();
    t_order* cur_order;
    t_order* other_order;
    while (cur_time < end_time)
    {
        cur_order = pop_back(FEL);
        // Indicates the FEL is empty
        if (!cur_order) break;
        // Indicates end/start of a day
        if (!cur_order->asset)
        {
            for (int i = 0; i < n_assets; i++)
            {
                calc_limits(&assets[i]);
            }
        }
        // Valid order
        else if (is_valid(cur_order))
        {
            process_order(cur_order);
        }
        // Invalid order
        else
        {
            // TODO: Adjust the time of the order(s)
            push_front(FEL, (void*)cur_order);
            push_front(FEL, (void*)breakup_order(cur_order));
        }
    }
}





















