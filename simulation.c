#include <stdio.h>
#include <stdlib.h>
#include "stocks.h"

int n_assets = 1;
t_asset assets[n_assets];
t_linked_list input_queue;

// Sets up the global assets included in the model
void setup_assets() {
    assets[0].name[0] = 'G'; assets[0].name[1] = 'O';
    assets[0].name[2] = 'O'; assets[0].name[3] = 'G';
    assets[0].price_per_share = 10; // Example start price
    assets[0].luld.min = 5;
    assets[0].luld.max = 15;
    // ... repeat for other assets ...
}

// Sets up the initial input queue
void setup_orders() {
    // Create the order
    t_order* sample_order = malloc(sizeof(t_order));
    sample_order->asset = &assets[0];
    sample_order->time = 1;
    sample_order->quantity = 3;
    // Push the order into the input queue
    push_front(&input_queue, (void*)sample_order);
}

// Checks if an order is able to go through
int is_valid(t_order* order) {
    // ... check if the order will make
    // ... order->asset->price go out of range
    return 0;
}

// Add an order to the order book
void add_to_order_book(t_order* order) {
    // ... find the corresponding order in the order book,
    // ... add (match->quantity += order.quantity)
}

// Updates the price of an asset
void update_asset_price(t_asset* asset) {
    // ... insert maths here
    float update_percentage=0;
	if(price_per_share < luld.max and price_per_share >luld.min){
		if(order.quantity > 0)
			update_percentage=order.quantity/100;
		elseif (order.quantity <0)
			update_percentage= -1 * order.quantity/100;
		asset.price_per_share *= (1+update_percentage);
	}
	else
		// request to stop the trade (because the luld range exceeded)
    
}

// The main step
void main_step() {
    t_order* order = pop_back(&input_queue);
    if (is_valid(order)) {
        add_to_order_book(order);
        update_asset_price(order->asset);
    }
    else {
        push_front(&input_queue, (void*)order);
        // ... or something like that
    }
}

int main(void) {
    setup_assets();
    setup_orders();
    while (input_queue.size)
        main_step();
}
