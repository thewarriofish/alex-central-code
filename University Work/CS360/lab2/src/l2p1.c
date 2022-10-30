//Alexander Yu - CS360
//Lab 2, Part 1: Buffering with Standard I/O
//September 7th, 2022

/*This portion of the lab focuses on practicing with the standard in/out functions given by C.
 * It processes data in the form of bytes, converting and sorting it into a database.*/


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "dllist.h"
#include "jrb.h"

/* The ip struct holds three distinct variables:
 * - the IP address, which is split into 4 parts
 * - a list of alias used by the IP address
 * - the number of nodes pointing to this struct,
 *   used for memory deallocation*/

typedef struct
{
	unsigned char address[4];
	Dllist names;
	int num;
}ip;

/* taken from famtree.c */
/**
 * @name alloc_string
 * @brief Allocates bits for string storage
 * @param[in] str_len no. of bits without null terminator to be allocated
 * @return A pointer to the string with bits allocated
 */

char *alloc_string(int str_len)
{
	char *ptr = malloc(sizeof(char) *(str_len+1));
	return ptr;
}

int main()
{
	FILE *file;
	char *name;
	int num, localAcquired;
	char c[2];
	char str[100];
	long size;
	size_t i;
	ip *IP;
	JRB ipAddresses, it;
	Dllist iter;

	size_t str_len = 50;

	/*c is null-terminated for strcat() to work.*/

	c[1] = '\0';

	ipAddresses = make_jrb();
	file = fopen("converted", "rb");
	if(file == NULL)
	{
		perror("converted");
		exit(1);
	}

	/*This while loop uses fgetc to acquire byte data.
	 * It run until both c contains EOF and file has marked EOF. */

	while((c[0] = fgetc(file)) != EOF || !feof(file))
	{
		num = 0;	
		IP = malloc(sizeof(ip));
		IP->names = new_dllist();

		/*The first 4 bytes are the IP address.*/

		for(i = 0; i < 4; i++)
		{
			IP->address[i] = c[0];
			c[0] = fgetc(file);
		}

		/*The next 4 bytes are the number of alias.
		 * Because the bytes are listed in big-endian, 
		 * descending powers of 10 are used as iterators.
		 * The byte is simply multiplied by the iterator and summed. */

		for(i = 1000; i >= 1; i /= 10)
		{
			num += c[0]*i;
			if(i != 1)
			{
				c[0] = fgetc(file);
			}
		}
		IP->num = num;

		/*After the first 8 bytes, all the names of the address are listed. Each name is null-terminated. 
		 * Local names are not listed if it is part of the absolute machine name.
		 * Each name is processed by inserting the address into an rb-tree using the name 
		 * as a key and the address as a val. In addition, the name is put into the address'
		 * list of names.*/

		name = alloc_string(str_len);
		for(i = 0; (int) i < num; i++)
		{
			localAcquired = 0;
			strcpy(name, "");
			while((c[0] = fgetc(file)) != '\0')
			{

			/*If the program is processing an absolute machine name,
			 * detected by the presence of a period,
			 * and it hasn't found a local machine name yet,
			 * the current name is added to the tree. 
			 * The no. of entries is incremented to denote this.*/
				
				if(c[0] == '.' && !localAcquired)
				{
					dll_append(IP->names, new_jval_s(strdup(name)));
					jrb_insert_str(ipAddresses, strdup(name), new_jval_v(IP));
					localAcquired = 1;
					IP->num++;
				}
				strcat(name, c);
			}
			dll_append(IP->names, new_jval_s(strdup(name)));
			jrb_insert_str(ipAddresses, strdup(name), new_jval_v(IP));
		}
		free(name);
	}
	fclose(file);

	/*Once all data is processed, user can access the created database via standard input. */
	
	printf("Hosts all read in\n\nEnter host name:  ");
	while(scanf("%99s", str) >= 0)
	{
	
		/*This if statement prevents blank lines from prompting the user again.*/

		if(strcmp(str, "") == 0)
		{
			printf("\n");
			continue;
		}
		if(jrb_find_str(ipAddresses, str) == NULL)
		{
			printf("no key %s\n\n", str);
		}
		else
		{
			/*Because jrb_find_str traverses backwards through the tree to find a key,
			 * the last node with the key will usually be found first.
			 * In the event that is not the case, the iterator moves
			 * down until it reaches the last node in the tree with the key.*/
		
			it = jrb_find_str(ipAddresses, str);
			while(strcmp(jrb_next(it)->key.s, it->key.s) == 0)
			{
				it = jrb_next(it);
			}

			/* Should there be multiple addresses with the same local name, all are printed. */

			while(strcmp(it->key.s, str) == 0)
			{
				IP = (ip *) it->val.v;
				for(i = 0; i < 4; i++)
				{
					printf("%d", IP->address[i]);
					if(i != 3)
					{
						printf(".");
					}
				}
				printf(":");
				dll_traverse(iter, IP->names)
				{
					printf(" %s", jval_s(iter->val));		
				}
				printf("\n\n");
				it = jrb_prev(it);
			}
		}
		printf("Enter host name:  ");
	}

	/*Remaining code serves to free memory. Inputstruct is freed first,
	 * then tree is traversed. Because there are multiple entries with the same pointer,
	 * an entry and its allocated variables are freed only if the entry is the last one
	 * in the tree. Key of tree is always freed no matter what.*/
	
	jrb_traverse(it, ipAddresses)
	{
		IP = (ip *) it->val.v;
		IP->num--;
		if(IP->num == 0)
		{
			dll_traverse(iter, IP->names)
			{
				free(jval_s(dll_val(iter)));
			}
			free_dllist(IP->names);
			free(IP);
		}
		free(it->key.s);
	}
	jrb_free_tree(ipAddresses);
	exit(0);
}

