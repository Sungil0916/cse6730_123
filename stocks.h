// Defines a min/max threshold
typedef struct s_threshold {
    float min;
    float max;
} t_threshold;

// Defines an asset
typedef struct s_asset {
    char name[4];           // Name of the asset
    float price_per_share;  // Price per share, in $$$
    t_threshold luld;
} t_asset;

// Defines an order for buying or selling assets
typedef struct s_order {
    t_asset* asset;    // Asset associated with this order
    int time;        // Hours since start of simulation
    int quantity;    // Number of shares (+buy, -sell)
} t_order;

// Struct for a linked list
typedef struct s_list_node {
    struct s_list_node* next;
    struct s_list_node* prev;
    void* value;
} t_list_node;

// Linked list
typedef struct s_linked_list {
    t_list_node* front;
    t_list_node* back;
    int size;
} t_linked_list;

// Push a value to the front of the list
void push_front(t_linked_list* list, void* value);

// Push a value to the end of the list
void push_back(t_linked_list* list, void* value);

// Pop the value at the front of the list
void* pop_front(t_linked_list* list);

// Pop the value at the end of the list
void* pop_back(t_linked_list* list);

// Print (for debugging)
void print_list(t_linked_list* list);
