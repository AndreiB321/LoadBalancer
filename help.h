/* Copyright 2021 <Andrei Bulica> */
#ifndef HELP_H_
#define HELP_H_

#include "load_balancer.h"


char *strdup(char *src);

// cautare urmatorul server
void find_next_server(load_balancer* main, unsigned int hash,
                      unsigned int *position);

// stergerea unui element din vector
void delete_element(load_balancer* main, unsigned int position);

// adaugare element in vector
void add_element(load_balancer* main, unsigned int position,
                 unsigned int hash, int server_id, unsigned int server_replica);

// cautare cel mai mare element mai mic decat o valore data
int lower_bound(load_balancer* main, unsigned int value);

// cautare indice server dupa eticheta "tag"
unsigned int get_server(load_balancer* main, unsigned int hash);


#endif  // HELP_H_
