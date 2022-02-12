/* Copyright 2021 <Andrei Bulica> */
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

typedef struct ll_node_t ll_node_t;
struct ll_node_t {
    void* data;
    ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t {
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
};

// alocare lista
linked_list_t*
ll_create(unsigned int data_size);

// adaugare nod pe pozitia n
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* data);

// stergere nod de pe pozitia n
ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n);

// eliberare lista
void
ll_free(linked_list_t** pp_list);


#endif  // LINKED_LIST_H_
