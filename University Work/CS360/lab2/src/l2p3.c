//Alexander Yu - CS360
//Lab 2, Part 3: Buffering with System Calls
//September 7th, 2022

/*This portion of the lab focuses on practicing using buffering with system calls.
 * Like the previous parts, it processes data in the form of bytes, converting and sorting it into a database.
 * This program should be as fast as part 1, and definitely faster than part 2.*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "fields.h"
#include "dllist.h"
#include "jrb.h"

/* Same as previous parts. */

typedef struct
{
	unsigned char address[4];
	Dllist names;
	int num;
}ip;

/* Same as previous parts. */

char *alloc_string(int str_len)
{
	char *ptr = malloc(sizeof(char) *(str_len+1));
	return ptr;
}

int main()
{
	/* A 350000 buffer is created to store bytes pre-processing. */ 

	char *name;
	int file, num, localAcquired;
	char c[2];
	char buffer[350000];
	char str[100];
	long size;
	size_t i, j;
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
	i = read(file, buffer, 350000);
	close(file);

	/*-2 is used as the end-of-file to prevent false EOF signals.
	 * i is used to keep track of position in buffer. */

	buffer[i] = -2;
	i = 0;
	memcpy(&c[0], buffer+i, sizeof(char));
	i++;
	while(c[0] != -2)
	{

		/*Logic is the same from previous parts, but accessing data is slightly more complex, due to possible alignment errors.
		 * memcpy is used to access data to prevent bus errors. i is incremented after every memcpy call to move position.
		 * c is a null-terminated character that stores a single byte of data from file.
         * First 4 bytes are address, next 4 bytes are no. of names,
         * remaining bytes are names, null-terminated.*/

		num = 0;	
		IP = malloc(sizeof(ip));
		IP->names = new_dllist();
		for(j = 0; j < 4; j++)
		{
			IP->address[j] = c[0];
			memcpy(&c[0], buffer+i, sizeof(char));
			i++;
		}
		for(j = 1000; j >= 1; j /= 10)
		{
			num += c[0]*j;
			if(j != 1)
			{
				memcpy(&c[0], buffer+i, sizeof(char));
				i++;
			}
		}
		IP->num = num;
		name = alloc_string(str_len);
		for(j = 0; (int) j < num; j++)
		{
			localAcquired = 0;
			strcpy(name, "");
			memcpy(&c[0], buffer+i, sizeof(char));
			i++;
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
				memcpy(&c[0], buffer+i, sizeof(char));
				i++;
			}
			dll_append(IP->names, new_jval_s(strdup(name)));
			jrb_insert_str(ipAddresses, strdup(name), new_jval_v(IP));
		}
		memcpy(&c[0], buffer+i, sizeof(char));
		i++;
		free(name);
	}

	/* User input step is exactly the same as previous parts;
	 * prompt user, wait for input, search database.*/

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

	/* Memory is freed in the exact same way as previous parts. 
	 * Traverse tree and free memory, preventing double frees.*/
	
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

