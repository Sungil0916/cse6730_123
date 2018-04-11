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
int orders_per_hour = 1000;
int include_all = 0;
double open_price, cur_price;

int n_order_dist = 1;
t_ndist* order_dists;
double* order_dist_weights;

t_list FEL;
t_list history_price;
t_list history_time;
t_luld threshold;
char* filename = "./history.csv";

int processed_orders;
time_t real_start_time;

// Random normal distribution
double rand_normal(t_ndist dist)
{
    double total = 0;
    for (int i = 0; i < 12; i++)
        total += rand() / (double)RAND_MAX;
    total -= 6;
    return (total * dist.variance) + dist.mean;
}

// Random multinormal distribution
double multi_rand_normal(int count, t_ndist* dists, double* weights)
{
    int i;
    double total, curTotal;
    for (i = 0; i < count; i++)
        total += (weights == NULL) ? 1 : weights[i];
    double pick = total * rand() / (double)RAND_MAX;
    double dt;
    for (i = 0; i < count; i++)
    {
        dt = (weights == NULL) ? 1 : weights[i];
        if (pick >= curTotal && pick < curTotal + dt)
            return rand_normal(dists[i]);
        curTotal += dt;
    }
}

// Calculate LULD for the asset
void calc_limits()
{
    // TODO: Accuracy
    double dp = cur_price * 0.5;
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
    int n_orders = n_hours * orders_per_hour;
    t_order* order;
    for (int i = 0; i < n_orders; i++)
    {
        order = malloc(sizeof(t_order));
        order->time = (i / orders_per_hour) + cur_time;
        order->quantity = (int)multi_rand_normal(n_order_dist, order_dists, order_dist_weights);
        push_front(&FEL, order);
    }
}

// Adjust the price of an asset based on an order that went through
double get_change_in_asset_price(t_order* order)
{
    // TODO: Accuracy
    return cur_price * order->quantity * 0.000005;
}

// Checks if the price_per_share of an asset will go out-of-bounds
// if this order is processed
int is_valid(t_order* order)
{
    double dp = get_change_in_asset_price(order);
    double p = cur_price + dp;
    return p <= threshold.max && p >= threshold.min;
}

// Process an order
void process_order(t_order* order)
{
    // Record asset price at the current time
    double* nums = malloc(sizeof(double) * 2);
    nums[0] = cur_price;
    nums[1] = (double)cur_time;
    push_back(&history_price, &nums[0]);
    push_back(&history_time, &nums[1]);
    // Adjust the price
    cur_price += get_change_in_asset_price(order);
    free(order);
    processed_orders++;
}

// Breaks an order into two so that at least one of them is valid
t_order* breakup_order(t_order* order)
{
    // TODO: Accuracy
    t_order* new = malloc(sizeof(t_order));
    if (FEL.size > 0)
    {
        order->time = ((t_order*)FEL.first->data)->time + 1;
        new->time = ((t_order*)FEL.first->data)->time + 1;
    }
    else
    {
        new->time = order->time + 1;
    }
    new->quantity = order->quantity / 2;
    order->quantity /= 2;
}

void breakdown()
{
    clear(&FEL);
    clear(&history_price);
    clear(&history_time);
    free(order_dists);
    free(order_dist_weights);
}

void err(const char* str)
{
    fprintf(stderr, "%s\n", str);
    exit(-1);
}

// Setup simulation parameters
void get_args(int argc, char* argv[])
{
    int ind;
    
    // Prices
    ind = get_arg_index("open", argc, argv);
    if (ind < 0)
        err("No \"-open\" argument provided.");
    open_price = atof(argv[ind + 1]);
    if (!open_price || open_price < 0)
        err("\"-open\" must be a double-precision float value greater than 0.");
    
    ind = get_arg_index("luld", argc, argv);
    if (ind >= 0)
    {
        use_luld = 1;
        if (ind + 1 < argc && (strcmp(argv[ind + 1], "0") == 0 || strcmp(argv[ind + 1], "false") == 0))
            use_luld = 0;
    }
    
    ind = get_arg_index("include_all", argc, argv);
    if (ind >= 0)
    {
        include_all = 1;
        if (ind + 1 < argc && (strcmp(argv[ind + 1], "0") == 0 || strcmp(argv[ind + 1], "false") == 0))
            include_all = 0;
    }
    
    // Distributions
    ind = get_arg_index("n_means", argc, argv);
    if (ind >= 0)
        n_order_dist = atoi(argv[ind + 1]);
    
    int mean_ind = get_arg_index("order_mean", argc, argv);
    int var_ind = get_arg_index("order_var", argc, argv);
    int weight_ind = get_arg_index("order_weight", argc, argv);
    if (mean_ind < 0)
        err("No \"-order_mean\" argument provided.");
    if (var_ind < 0)
        err("No \"-order_var\" argument provided.");
    if (weight_ind >= 0)
        order_dist_weights = malloc(sizeof(double) * n_order_dist);
    
    order_dists = malloc(sizeof(t_ndist) * n_order_dist);
    double tmp;
    for (int i = 0; i < n_order_dist; i++)
    {
        tmp = atof(argv[mean_ind + 1 + i]);
        if (!tmp)
            err("\"-n_means\" incorrectly set for number of given parameters");
        order_dists[i].mean = tmp;
        
        tmp = atof(argv[var_ind + 1 + i]);
        if (!tmp)
            err("\"-n_means\" incorrectly set for number of given parameters");
        order_dists[i].variance = tmp;
        if (order_dist_weights)
        {
            tmp = atof(argv[weight_ind + 1 + i]);
            if (!tmp)
                err("\"-n_means\" incorrectly set for number of given parameters");
            order_dist_weights[i] = tmp;
        }
    }
    
    // Etc
        
    ind = get_arg_index("orders_per_hour", argc, argv);
    if (ind >= 0)
        orders_per_hour = atof(argv[ind + 1]);
    
    ind = get_arg_index("file", argc, argv);
    if (ind >= 0)
        filename = argv[ind + 1];
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
    int prev_time = (int)(*(double*)history_time.first->data);
    int cur_time;
    for (int i = 1; i < history_price.size; i++)
    {
        cur_time = (int)(*(double*)element_at(&history_time, i));
        if (include_all || cur_time > prev_time)
        {
            fprintf(file, "%d,%0.2f\n",
                prev_time,
                *(double*)element_at(&history_price, i - 1));
            prev_time = cur_time;
        }
        free(element_at(&history_price, i - 1));
    }
    fprintf(file, "%d,%0.2f\n",
                cur_time,
                *(double*)element_at(&history_price, history_price.size - 1));
    free(element_at(&history_price, history_price.size - 1));
    fclose(file);
    fprintf(stdout, "Price history written to \"%s\".\n", filename);
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





















