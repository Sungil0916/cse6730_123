#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "market.h"
#include "fargs.h"

int cur_time = 9;
int end_time = 17;
int use_luld = 0;
float open_price, cur_price;
float order_mean, order_var;
t_list FEL;
t_list history_price;
t_list history_time;
t_luld threshold;
char* filename = "./history.csv";

int processed_orders;
time_t real_start_time;

// Random normal distribution
float rand_normal(float mean, float var)
{
    float total = 0;
    for (int i = 0; i < 12; i++)
        total += rand() / (float)RAND_MAX;
    total -= 6;
    return (var * total) + mean;
}

// Calculate LULD for the asset
void calc_limits()
{
    // TODO: Accuracy
    float dp = cur_price * 0.01;
    threshold.min = cur_price - dp;
    threshold.max = cur_price + dp;
}

// Create the orders that will be processed during the simulation
void setup_orders()
{
    clear(&FEL);
    clear(&history_price);
    clear(&history_time);
    int n_hours = end_time - cur_time;
    int orders_per_hour = 10;
    int n_orders = n_hours * orders_per_hour;
    t_order* order;
    for (int i = 0; i < n_orders; i++)
    {
        order = malloc(sizeof(t_order));
        order->time = (i / orders_per_hour) + cur_time;
        order->quantity = (int)rand_normal(order_mean, order_var);
        push_front(&FEL, order);
    }
}

// Adjust the price of an asset based on an order that went through
float get_change_in_asset_price(t_order* order)
{
    // TODO: Accuracy
    return cur_price * order->quantity * 0.001;
}

// Checks if the price_per_share of an asset will go out-of-bounds
// if this order is processed
int is_valid(t_order* order)
{
    float dp = get_change_in_asset_price(order);
    float p = cur_price + dp;
    return !(p > threshold.max || p < threshold.min);
}

// Process an order
void process_order(t_order* order)
{
    // Record asset price at the current time
    float* nums = malloc(sizeof(int) * 2);
    nums[0] = cur_price;
    nums[1] = (float)cur_time;
    push_back(&history_price, &nums[0]);
    push_back(&history_time, &nums[1]);
    free(order);
    // Adjust the price
    cur_price += get_change_in_asset_price(order);
    processed_orders++;
}

// Breaks an order into two so that at least one of them is valid
t_order* breakup_order(t_order* order)
{
    // TODO: Accuracy
    t_order* new = malloc(sizeof(t_order));
    new->time = order->time;
    new->quantity = order->quantity / 2;
    order->quantity /= 2;
}

void breakdown()
{
    clear(&FEL);
    clear(&history_price);
    clear(&history_time);
}

void err(const char* str)
{
    fprintf(stderr, "%s\n", str);
    exit(-1);
}

// Setup simulation parameters
void get_args(int argc, char* argv[])
{
    char* arg;
    arg = get_arg("open", argc, argv);
    if (!arg)
        err("No \"-open\" argument provided.");
    open_price = atof(arg);
    if (!open_price || open_price < 0)
        err("\"-open\" must be a floating-point value greater than 0.");
    
    arg = get_arg("luld", argc, argv);
    if (arg && strcmp(arg, "0") != 0 && strcmp(arg, "false") != 0)
        use_luld = 1;
    
    arg = get_arg("order_mean", argc, argv);
    if (!arg)
        err("No \"-order_mean\" argument provided.");
    order_mean = atof(arg);
    
    arg = get_arg("order_var", argc, argv);
    if (!arg)
        err("No \"-order_var\" argument provided.");
    order_var = atof(arg);
    
    arg = get_arg("file", argc, argv);
    if (arg)
        filename = arg;
}

void print_results()
{
    fprintf(stdout, "r-time\tproc\tigno\topen\t\tclose\n");
    fprintf(stdout, "%0.2f\t%d\t%d\t$%0.2f\t\t$%0.2f\n",
        difftime(time(NULL), real_start_time),
        processed_orders,
        FEL.size,
        open_price, cur_price);
    // Write to file
    FILE* file = fopen(filename, "w+");
    fprintf(file, "time,price\n");
    for (int i = 0; i < history_price.size; i++)
        fprintf(file, "%d,%0.2f\n",
            (int)(*(float*)element_at(&history_time, i)),
            *(float*)element_at(&history_price, i));
    fclose(file);
    fprintf(stdout, "Price history written to \"./history.csv\".\n");
    fprintf(stdout, "Simulation terminated successfully.\n");
}

int main(int argc, char* argv[])
{    
    // Setup
    get_args(argc, argv);
    cur_price = open_price;
    setup_orders();
    calc_limits();
    
    // Run
    real_start_time = time(NULL);
    t_order* cur_order;
    while (cur_time < end_time)
    {
        cur_order = pop_back(&FEL);
        // Indicates the FEL is empty
        if (!cur_order) break;
        cur_time = cur_order->time;
        // Valid order
        if (!use_luld || is_valid(cur_order))
        {
            process_order(cur_order);
        }
        // Invalid order
        else
        {
            // TODO: Adjust the time of the order(s)
            push_front(&FEL, (void*)cur_order);
            push_front(&FEL, (void*)breakup_order(cur_order));
        }
    }
    print_results();
    breakdown();
}





















