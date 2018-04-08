// Node of a linked list
struct s_list_node {
    struct s_list_node *prev, *next;
    void* data;
};
typedef struct s_list_node t_list_node;

// A linked list
struct s_list {
    struct s_list_node *first, *last;
    int size;
};
typedef struct s_list t_list;

// Resets the list
void clear(t_list *list);

// Gets an element in the list with 0-based indexing
void* element_at(t_list *list, int index);

// Push to the front of the list
void push_front(t_list *list, void* data);

// Push to the back of the list
void push_back(t_list *list, void* data);

// TODO: Implement
/*
// Add data after a particular location
void add_after(t_list *list, t_list_node* node, void* data);

// Add data before a particular location
void add_before(t_list *list, t_list_node* node, void* data);
*/

// Pop from the back of the list
void* pop_back(t_list *list);
