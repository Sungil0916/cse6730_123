	static int n_assets = 1;
	static t_asset assets[1];
	static t_linked_list input_queue;
	

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
	    sample_order->asset = &(assets[0]);
	    sample_order->time = 1;
	    sample_order->quantity = 3;
	    // Push the order into the input queue
	    push_front(&input_queue, (void*)sample_order);
	}
	

	// Checks if an order is able to go through
	int is_valid(t_order* order, t_asset* asset) 
	{	    
	// ... check if the order will make
			double new_price;
			new_pricce = update_asset_price(sample_order.quantity);
	    	if ((new_price > asset.luld.max) || (new_price < asset.luld.min))
			{
				add_to_order_book(sample_order.quantity);
				
			}
			;//up date orderbook
	    else
		{
				push_back(&input_queue, (void*)order);
		}
	        ;//push back it
	    // ... order->asset->price go out of range
	    return 0;
	}
	

	// Add an order to the order book
	void add_to_order_book(t_order* order) {
		
	    // ... find the corresponding order in the order book,
	    //What i understood from this structure you put Sam is that this linked_list basically forms our oderbook. IF that is true, them we need to have several linked_list each of which for a seperate stock, such as Google.
		
	    // ... add (match->quantity += order.quantity)
		struct linked_list* sample_order.quantity = (struct linked_list*)malloc(sizeof( sample_order.quantity)); //allocate node for new order	
		linked_list->size = sample_order.quantity;																 //put in new order quantity in the new node	
		new_node->next =(*head_ref);																			 //make next of the new order as head
		(*head_ref) = new_node; 																				 // move head point to the new node
		
	}
	

	// Updates the price of an asset
	void update_asset_price(t_asset* asset) {
		
		struct EventData *d;
		
	    if (order.quantity <0){
		assets.price_per_share = assets.price_per_share - order.quantity * 0.001*assets.price_per_share; //deduct 0.001 per sell order quantity 
		}
		else
		{
			assets.price_per_share = assets.price_per_share + order.quantity * 0.01*assets.price_per_share;//add 0.001 per buy order quantity 
		};
		return asset.price_per_share;
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
