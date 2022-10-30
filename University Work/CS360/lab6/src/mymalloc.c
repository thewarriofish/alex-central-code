//Alexander Yu - CS360
//Lab 6 - JMalloc
//October 13th, 2022

/*This lab emulates the functions of malloc, or in other words, allocates memory without malloc. 
 *It uses sbrk to allocate memory and a linked list to manage free nodes.*/

#include "mymalloc.h"
#include <unistd.h>
#include <stdio.h>

/*Strut taken from Joshua's notes; a struct representing a node in the free tree.
 * Contains only a node's size and the next node in the list.*/

typedef struct __node_t {
    unsigned int size; 
    struct __node_t *next; 
} node_t;

/*Global variable that represents the first node of the list.*/

node_t* head = NULL; 

/** Function taken from Joshua's notes
 * @name nalloc
 * @brief allocates memory for node n and sets properties
 * @param[in] size - the size of the node to be allocated
 * @return n - the allocated node
 */

node_t* nalloc(size_t size){
    node_t* n = sbrk(size);
	n->size = size;
    return n;
}

/**
 * @name new_block
 * @brief creates a block of memory to be used;
 *		  blocks smaller than 8192 are split
 * @param[in] size - the size of the block to be made
 * @return the block created
 */

node_t* new_block(size_t size){

	/*Blocks with sizes larger than 8192 are simply set to that size*/

	if(size > 8192)
	{
		return nalloc(size);
	}

	/*Otherwise the block is created by a spliting a block of size 8192 and freeing the leftovers*/

	node_t* n = sbrk(8192);
	n->size = 8192 - size;
	node_t* ret = (void *) n + (n->size);
	ret->size = size;
	my_free((void *) n + 8);
	return ret;
}

/**
 * @name my_malloc
 * @brief emulates malloc
 * @param[in] size - amount of memory wanted by user
 * @return the address of the node with the allocated memory
 */

void* my_malloc(size_t size) {
	node_t* prev = NULL; 
    int padding = size % 8;
	if(padding != 0)
	{
		padding = 8 - padding;
	}
	//total_bytes is equal to size, plus padding to a multiple of 8, plus 8 bookkeeping bytes
	size_t total_bytes = size + padding + 8;
	node_t* node = (node_t *) free_list_begin();
	//if the free list is empty, creates a new block
	if(!node)
	{
		return (void *) new_block(total_bytes) + 8;
	}
	//iterates through free list, finding the node whose size is less than total_bytes
	while(node && node->size < total_bytes)
	{
		//if there is only one node in free list, creates a new block
		if(!(node->next))
		{
			return (void *) new_block(total_bytes) + 8;
		}
		prev = node;
		node = free_list_next(node);
	}
	//if node requires splitting, creates a node at new address, 
	//sets properties and returns the new node's address
	if(node->size - total_bytes > 8)
	{
		node_t* new = (void *) node + (node->size - total_bytes);
		new->size = total_bytes;
		node->size = node->size - total_bytes;
		return (void *) new + 8;
	}
	//otherwise removes the node from the free list and returns the node's address
	else
	{
		if(prev == NULL)
		{
			head = free_list_next(head);
		}
		else
		{
			prev->next = free_list_next(node);
		}
	}
	return (void *) node + 8;
}

/**
 * @name my_free
 * @brief puts the allocated memory into the free list
 * @param[in] ptr - address to be freed
 */ 

void my_free(void* ptr) {
	//if the list is uninitialized, makes ptr the first in the list
    if(!head)
	{
		head = ptr - 8;
		return;
	}
	//otherwise, places the node in the correct position; free nodes are listing in ascending order
	node_t* current;
	current = head;
	while(current->next && (void *) current->next < (ptr - 8))
	{
		current = current->next;
	}
	((node_t *) (ptr - 8))->next = current->next;
	current->next = ptr - 8;
}

/** Function taken from Joshua's notes
 * @name free_list_begin
 * @brief function to return head global variable
 * @return the node stored in head
 */

void* free_list_begin() {
    return head;
}

/** Function taken from Joshua's notes
 * @name free_list_next
 * @brief function to return next node in list
 * @return the value stored in node's next
 */

void* free_list_next(void* node) {
	return ((node_t *) node)->next;
}

/**
 * @name coalesce_free_list
 * @brief combines adjacent nodes in free list
 */

void coalesce_free_list() {
    node_t* node = head; 
	//if free list's size is bigger than 1, iterates through list
	while(node && node->next)
	{
		//if a node's next address is equal to the node's address plus its size, combines
		while((void *) node + node->size == (void *) node->next)
		{
			node->size += node->next->size;
			node->next = node->next->next;
		}
		node = node->next;
	}
}
