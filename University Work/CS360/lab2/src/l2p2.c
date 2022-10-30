//Alexander Yu - CS360
//Lab 2, Part 2: Using System Calls Without Buffering
//September 7th, 2022

/*This portion of the lab focuses on practicing with the system calls open, close and read.
 * Like part 1, it processes data in the form of bytes, converting and sorting it into a database.
 * However, it is slower due to the use of system calls.*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "fields.h"
#include "dllist.h"
#include "jrb.h"

/* Same as part 1; a struct used to hold data about IP address. */

typedef struct
{
	unsigned char address[4];
	Dllist names;
	int num;
}ip;

/* Same as part 1; a function to dynamically allocate strings. */

char *alloc_string(int str_len)
{
	char *ptr = malloc(sizeof(char) *(str_len+1));
	return ptr;
}

int main()
{
	char *name;
	int file, num, localAcquired;
	char c[2];
	char str[100];
	long size;
	size_t i;
	ip *IP;
	JRB ipAddresses, it;
	Dllist iter;

	size_t str_len = 50;
	c[1] = '\0';

	ipAddresses = make_jrb();
	file = open("converted", O_RDONLY); 
	if(file	== -1)
	{
		perror("converted");
		exit(1);
	}
	while((read(file, c, 1) != 0))
	{

		/* System calls replace standard I/O, but logic is the same as part 1;
		 * c is a null-terminated character that stores a single byte of data from file.
		 * First 4 bytes are address, next 4 bytes are no. of names, 
		 * remaining bytes are names, null-terminated.*/

		num = 0;	
		IP = malloc(sizeof(ip));
		IP->names = new_dllist();
		for(i = 0; i < 4; i++)
		{
			IP->address[i] = c[0];
			read(file, c, 1);
		}
		for(i = 1000; i >= 1; i /= 10)
		{
			num += c[0]*i;
			if(i != 1)
			{
				read(file, c, 1);
			}
		}
		IP->num = num;
		name = alloc_string(str_len);
		for(i = 0; (int) i < num; i++)
		{
			localAcquired = 0;
			strcpy(name, "");
			read(file, c, 1);
			while(c[0] != '\0')
			{
				if(c[0] == '.' && !localAcquired)
				{
					dll_append(IP->names, new_jval_s(strdup(name)));
					jrb_insert_str(ipAddresses, strdup(name), new_jval_v(IP));
					localAcquired = 1;
					IP->num++;
				}
				strcat(name, c);
				read(file, c, 1);
			}
			dll_append(IP->names, new_jval_s(strdup(name)));
			jrb_insert_str(ipAddresses, strdup(name), new_jval_v(IP));
		}
		free(name);
	}

	close(file);

	/* User input step is exactly the same as part 1;
	 * prompt user, wait until input is received, search database.*/

	printf("Hosts all read in\n\nEnter host name:  ");
	while(scanf("%99s", str) >= 0)
    {
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
            it = jrb_find_str(ipAddresses, str);
            while(strcmp(jrb_next(it)->key.s, it->key.s) == 0)
            {
                it = jrb_next(it);
            }
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

	/* Memory is freed exactly like part 1. 
	 * Traverse the tree and free memory,
	 * making sure not to prematurely free memory.*/
	
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

