/* Copyright 2021 <Andrei Bulica> */
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "help.h"

#define MAX_BUCKET_NUMBER 10

unsigned int hash_function_key(void *a)
{
    unsigned char *puchar_a = (unsigned char *) a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}


static ll_node_t *find_key(linked_list_t *bucket, void *key, unsigned int *pos)
{
    ll_node_t *current_node = bucket->head;
    unsigned int i;
	for (i = 0; i < bucket->size; ++i) {
		if (strcmp(key, ((info_t*)current_node->data)->key) == 0) {
			*pos = i;
			return current_node;
		}
		current_node = current_node->next;
	}
	return NULL;
}

server_memory* init_server_memory()
{
	unsigned int i;
	server_memory *server = malloc(sizeof(*server));
	DIE(server == NULL, "Malloc failed!");

	server->hashtable =  calloc(1, sizeof(*server->hashtable));
    DIE(server->hashtable == NULL, "Malloc failed");

	server->hashtable->buckets = calloc(MAX_BUCKET_NUMBER,
										sizeof(server->hashtable->buckets));
    DIE(server->hashtable->buckets == NULL, "Malloc failed");
	// initializare bucket-uri
	for (i = 0; i < MAX_BUCKET_NUMBER; ++i)
		server->hashtable->buckets[i] = ll_create(sizeof(info_t));
    server->hashtable->hmax = MAX_BUCKET_NUMBER;
    server->hashtable->size = 0;
    server->hashtable->hash_function = hash_function_key;
	return server;
}

void server_store(server_memory* server, char* key, char* value)
{
	unsigned int position, index = server->hashtable->hash_function(key) %
						 server->hashtable->hmax;
	ll_node_t *node = find_key(server->hashtable->buckets[index],
							   key, &position);
	info_t data;

	if (node != NULL) {
		free(((info_t*)node->data)->value);
		((info_t*)node->data)->value = strdup(value);
		return;
    }
	data.key = strdup(key);
	data.value = strdup(value);
	ll_add_nth_node(server->hashtable->buckets[index], 0, &data);
}

void server_remove(server_memory* server, char* key)
{
	unsigned int position, index = server->hashtable->hash_function(key) %
						 server->hashtable->hmax;
	ll_node_t *node = find_key(server->hashtable->buckets[index],
							   key, &position);
	ll_remove_nth_node(server->hashtable->buckets[index], position);
	free_data(node);
	free(node);
}

char* server_retrieve(server_memory* server, char* key)
{
	unsigned int position, index = server->hashtable->hash_function(key) %
							  server->hashtable->hmax;
    ll_node_t *node = find_key(server->hashtable->buckets[index],
							   key, &position);
    if (node != NULL) {
        return ((info_t*)node->data)->value;
	}
	return NULL;
}

void free_data(ll_node_t* prev)
{
	free(((info_t*)prev->data)->key);
	free(((info_t*)prev->data)->value);
	free(prev->data);
}

void free_server_memory(server_memory* server)
{
	unsigned int i;
	ll_node_t *node;
	for (i = 0; i < server->hashtable->hmax; ++i) {
		while (server->hashtable->buckets[i]->size > 0) {
			node = ll_remove_nth_node(server->hashtable->buckets[i], 0);
			free_data(node);
			free(node);
		}
		free(server->hashtable->buckets[i]);
	}
	free(server->hashtable->buckets);
	free(server->hashtable);
	free(server);
	server = NULL;
}
