/* Copyright 2021 <Andrei Bulica> */
#include <stdlib.h>
#include <string.h>

#include "help.h"

char *strdup(char *src) {
    unsigned int size = strlen(src) + 1;
	char *dst = calloc(size, 1);
    DIE(dst == NULL, "Malloc failed!");
    memcpy(dst, src, size);
    return dst;
}

void find_next_server(load_balancer* main, unsigned int hash,
                      unsigned int *position)
{
    // se cauta indicele cel mai apropiat de valoarea ceruta, cu elemntul
    // corespunzator mai mic
    *position = lower_bound(main, hash);
    if (*position == main->size_servers - 1) {
        *position = 0;
        return;
    }
    *position = *position + 1;
}

void delete_element(load_balancer* main, unsigned int position)
{
    unsigned int i;
    // shiftare elemente cu o pozitie la stanga
    for (i = position; i < main->size_servers; ++i)
        main->hashring[i] = main->hashring[i + 1];
    main->size_servers--;
}

void add_element(load_balancer* main, unsigned int position,
                 unsigned int hash, int server_id, unsigned int server_replica)
{
    unsigned int i;
    // shiftare elemente cu o pozitie la stanga
    for (i = main->size_servers; i > position; --i)
        main->hashring[i] = main->hashring[i - 1];
    // setare nou element
    main->hashring[position].hash = hash;
    main->hashring[position].tag = server_id;
    main->hashring[position].replica = server_replica;
    main->size_servers++;
}

int lower_bound(load_balancer* main, unsigned int value)
{
    int left, right, med;
    left = 0;
    right = main->size_servers;

    while (left < right) {
        med = (left + right) / 2;
        if (value <= main->hashring[med].hash) {
            right = med;
        } else {
            left = med + 1;
        }
    }
    return left;
}

unsigned int get_server(load_balancer* main, unsigned int hash)
{
    unsigned int position = lower_bound(main, hash);
    // daca a ajuns la final, il pozitionam la inceput
    if (position == main->size_servers)
        position = 0;
    return main->hashring[position].tag;
}
