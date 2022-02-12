/* Copyright 2021 <Andrei Bulica> */
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "help.h"

#define MAX_SERVERS 100000


unsigned int hash_function_servers(void *a)
{
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}


load_balancer* init_load_balancer()
{
    load_balancer *load = calloc(1, sizeof(*load));
    DIE(load == NULL, "Malloc failed!");
    load->size_servers = 0;
    load->servers = calloc(MAX_SERVERS, sizeof(load->servers));
    DIE(load->servers == NULL, "Malloc failed!");
    load->hashring = calloc(3 * MAX_SERVERS, sizeof(load->hashring));
    DIE(load->hashring == NULL, "Malloc failed!");
    return load;
}

void loader_store(load_balancer* main, char* key, char* value, int* server_id)
{
	unsigned int hash;
    hash = hash_function_key(key);
    *server_id = get_server(main, hash);
    server_store(main->servers[*server_id], key, value);
}


char* loader_retrieve(load_balancer* main, char* key, int* server_id)
{
	unsigned int hash;
    hash = hash_function_key(key);
    *server_id = get_server(main, hash);
    return server_retrieve(main->servers[*server_id], key);
}


void loader_add_server(load_balancer* main, int server_id)
{
	unsigned int server_to_hash, hash, i, position;
    int x;
    unsigned int j, next_server;
    char *key, *value;
    ll_node_t *node;
    // initializare server
    main->servers[server_id] = init_server_memory();

    for (i = 0; i < 3; i++) {
        // hash-uire replica
        server_to_hash = i * MAX_SERVERS + server_id;
        hash = hash_function_servers(&server_to_hash);
        // adaugare server
        position = lower_bound(main, hash);
        add_element(main, position, hash, server_id, i + 1);
        find_next_server(main, hash, &next_server);
        // rearanjare elemente
        if ((unsigned)server_id != main->hashring[next_server].tag) {
            for (j = 0; j < main->servers[main->hashring[next_server].tag]->
                            hashtable->hmax; ++j) {
                node = main->servers[main->hashring[next_server].tag]->
                       hashtable->buckets[j]->head;
                while (node != NULL) {
                    // se aloca key si value pentru a fi stocate in noul server
                    key = strdup(((info_t*)node->data)->key);
                    value = strdup(((info_t*)node->data)->value);
                    node = node->next;
                    server_remove(main->servers[main->
                                  hashring[next_server].tag], key);
                    loader_store(main, key, value, &x);
                    // eliberare memorie
                    free(key);
                    free(value);
                }
            }
        }
    }
}

void loader_remove_server(load_balancer* main, int server_id)
{
    ll_node_t *node;
    char *key, *value;
    unsigned int i = 0;
    int x;
    // se cauta server-ul in hashring
    while (i < main->size_servers) {
        if ((unsigned)server_id == main->hashring[i].tag)
            delete_element(main, i);
        else
            i++;
    }

    for (i = 0; i < main->servers[server_id]->hashtable->hmax; ++i) {
        node = main->servers[server_id]->hashtable->buckets[i]->head;
        while (node != NULL) {
            // alocare cheie si valoare
            key = ((info_t*)node->data)->key;
            value = ((info_t*)node->data)->value;
            // se muta pe urmatorul server
            loader_store(main, key, value, &x);
            // eliberare cheie si valoare
            // urmatorul nod
            node = node->next;
        }
    }
    // se elibereaza memoria server-ului
    free_server_memory(main->servers[server_id]);
}

void free_load_balancer(load_balancer* main)
{
    unsigned int i;

    if (main == NULL)
		return;

    for (i = 0; i < main->size_servers; ++i) {
        if (main->hashring[i].replica == 1)
            free_server_memory(main->servers[main->hashring[i].tag]);
    }
    free(main->servers);
    free(main->hashring);
    free(main);
    main = NULL;
}
