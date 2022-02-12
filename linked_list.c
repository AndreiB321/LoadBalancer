/* Copyright 2021 <Andrei Bulica> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "utils.h"

linked_list_t* ll_create(unsigned int data_size)
{
    linked_list_t *list = malloc(sizeof(*list));
    DIE(list == NULL, "Malloc error");
    // initializare valori
    list->head = NULL;
    list->data_size = data_size;
	list->size = 0;
    return list;
}


static ll_node_t *get_nth_node(unsigned int n, linked_list_t *list)
{
    ll_node_t *node = list->head;
    unsigned int i;
    for (i = 0; i < n; ++i)
        node = node->next;
    return node;
}


void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev_node, *new_node;
	if (!list)
		return;
    // caz in care indicele este mai mare decat dimenisunea listei
    if (n > list->size) {
        n = list->size;
    }

    new_node = malloc(sizeof(*new_node));
	DIE(NULL == new_node, "Malloc failed!");

	new_node->data = malloc(list->data_size);
	DIE(NULL == new_node->data, "Malloc failed!");

	memcpy(new_node->data, new_data, list->data_size);
    // adaugare pe prima pozitie
	if (n == 0 || list->head == NULL) {
		new_node->next = list->head;
		list->head = new_node;
	} else if (n > 0) {
        // adaugare pe a n-a pozitie
		prev_node = get_nth_node(n - 1, list);
		new_node->next = prev_node->next;
		prev_node->next = new_node;
	}
	list->size++;
}

ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *removed;
    if (!list || !list->size)
        return NULL;
    // stergere prima pozitie
    if (!n) {
        removed = list->head;
        list->head = removed->next;
    } else {
        // stergere a n-a pozitie
        prev = get_nth_node(n - 1, list);
        removed = prev->next;
        prev->next = prev->next->next;
    }
    --list->size;
    return removed;
}

void ll_free(linked_list_t** pp_list)
{
    ll_node_t *tmp, *node;
    if (!pp_list || !*pp_list)
        return;

    node = (*pp_list)->head;
    // stergere element cu element din lista
    while (node != NULL) {
        tmp = node;
        node = node->next;
        // eliberare memorie nod
        free(tmp->data);
        free(tmp);
    }
    free(*pp_list);
    pp_list = NULL;
}
