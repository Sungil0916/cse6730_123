#include <stdlib.h>
#include "list.h"

// TODO: Add "add_before" and "add_after" to support
//       adding data anywhere in the list

void* element_at(t_list *list, int index)
{
    int i = 0;
    t_list_node* cur_node = list->first;
    while (cur_node && i++ < index)
        cur_node = cur_node->next;
    if (cur_node)
        return cur_node->data;
    return NULL;
}

void push_front(t_list *list, void* data)
{
    t_list_node* new_node = calloc(1, sizeof(t_list_node));
    new_node->data = data;
    new_node->next = list->first;
    if (list->size == 0)
    {
        list->last = new_node;
    }
    else
    {
        list->first->prev = new_node;
    }
    list->first = new_node;
    list->size ++;
}

void* pop_back(t_list *list)
{
    if (list->size == 0)
    {
        return NULL;
    }
    t_list_node* node = list->last;
    list->last = node->prev;
    void* data = node->data;
    if (node->prev)
    {
        node->prev->next = NULL;
    }
    free(node);
    list->size --;
    return data;
}
