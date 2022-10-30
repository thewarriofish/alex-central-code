//Alexander Yu - CS360
//October 1st, 2022
//Lab 4B - Tar Extract

/* This part of the lab focuses on the simluation of the tar program, more specifically the extraction process. It takes a tarc file, reads the data, and creates directories and files based on the data.
 * This lab makes use of chmod, utimes, mkdir and buffered I/O.*/


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <dirent.h>
#include "jrb.h"

#define MAXLEN 1000001

/**
 * This struct holds data that is to be set for directories after their files and subdirectories are created. 
 */

typedef struct 
{
	struct timeval times[2];
	mode_t mode;
}directory;

/**
 * @name exit_free
 * @brief frees memory if the program exits sooner than expected
 * @param[in] tree the JRB tree to be freed
 */

void exit_free(JRB tree)
{
	JRB iter;
	
	/*Makes sure the tree isn't empty, and only frees if a value is present to be freed*/

	if(!(tree->flink == tree))
	{
		jrb_traverse(iter, tree)
		{
			if(iter->key.s != NULL)
			{
				free(iter->key.s);
			}
			if(iter->val.s != NULL)
			{
				free(iter->val.s);
			}
			if(iter->val.v != NULL)
			{
				free(iter->key.v);
			}
		}
	}
	jrb_free_tree(tree);
}

/**
 * @name read_tar
 * @brief reads and processes data from a tarc file
 */

void read_tar()
{
	FILE *file;
	int read;
	struct timeval times[2];
	int name_size, mode;
	char name[MAXLEN], bytes[MAXLEN], pathname[MAXLEN * 2];
	long inode, file_size;
	JRB inodes, directories, iter;
	directory *d;

	inodes = make_jrb();
	directories = make_jrb();

	/*Every call of fread is check to ensure the correct amount of bytes are being read.
	 * Exits if there is a mismatch at any point. */

	read = fread(&name_size, 1, sizeof(int), stdin);
	while(read != 0)
	{

		/*All data entries have a name size, name and inode.*/

		if(read != sizeof(int))
		{
			fprintf(stderr, "Bad tarc file at byte %ld. Tried to read filename size, but only got %d bytes.\n", ftell(stdin), read);
			exit(1);
		}
		read = fread(name, 1, name_size, stdin); 
		if(read != name_size)
		{
			fprintf(stderr, "Bad tarc file at byte %ld. File name size is %d, but bytes read = %d.\n", ftell(stdin), name_size, read);
			exit(1);
		}
		name[name_size] = '\0';
		read = fread(&inode, 1, sizeof(long), stdin);
		if(read != sizeof(long))
		{
			fprintf(stderr, "Bad tarc file for %s. Couldn't read inode\n", name);
			exit(1);
		}

		/*Only entries whose inodes hasn't been encountered will have a mode and modification time.*/

		if(jrb_find_int(inodes, inode) == NULL)
		{
			read = fread(&mode, 1, sizeof(int), stdin);
			if(read != sizeof(int))
			{
				fprintf(stderr, "Bad tarc file for %s. Couldn't read mode\n", name);
				exit(1);
			}
			times[0].tv_sec = time(NULL);
			times[0].tv_usec = 0;
			read = fread(&times[1].tv_sec, 1, sizeof(long), stdin);
			if(read != sizeof(long))
			{
				fprintf(stderr, "Bad tarc file for %s. Couldn't read time\n", name);
				exit(1);
			}
			times[1].tv_usec = 0;
			sprintf(pathname, "./%s", name);
			jrb_insert_int(inodes, inode, new_jval_s(strdup(pathname)));

			/*Only files have a file size and bytes that represent the contents.*/

			if(!(S_ISDIR(mode)))
			{
				read = fread(&file_size, 1, sizeof(long), stdin);
				if(read != sizeof(long))
				{
					fprintf(stderr, "Bad tarc file for %s. Couldn't read size\n", name);
					exit(1);
				}
				read = fread(bytes, 1, file_size, stdin);
				if(read != file_size)
				{
					fprintf(stderr, "Bad tarc file at byte %ld. File size is %ld, but bytes read = %d\n", ftell(stdin), file_size, read);
					exit(1);
				}
				file = fopen(pathname, "w");
				if(file == NULL)
				{
					perror(name);
					exit_free(inodes);
					exit_free(directories);
					exit(1);
				}
				fwrite(bytes, sizeof(char), file_size, file);
				fclose(file);
				utimes(pathname, times);
				chmod(pathname, mode);
			}

			/*Directories are created, but their mode and modification times are set later to allow for more files to be created within.
			 * Mode and mod. time is stored in a struct.*/

			else
			{
				d = malloc(sizeof(directory));
				sprintf(pathname, "./%s", name);
				mkdir(pathname, 040777);
				d->times[0] = times[0];
				d->times[1] = times[1];
				d->mode = mode;
				jrb_insert_str(directories, strdup(pathname), new_jval_v((void *) d));
			}
		}

		/*Entries whose inode has already been encountered is linked to the file associated with said inode.*/

		else
		{
			sprintf(pathname, "./%s", name);
			link(jrb_find_int(inodes, inode)->val.s, pathname);
		}
		read = fread(&name_size, 1, sizeof(int), stdin);
	}

	/*For all directories encountered, set their mode and modification times.*/

	jrb_traverse(iter, directories)
	{
		d = (directory *) iter->val.v;
		chmod(iter->key.s, d->mode);
		utimes(iter->key.s, d->times);
		free(iter->key.s);
		free(d);
	}
	jrb_traverse(iter, inodes)
	{
		free(iter->val.s);
	}
	jrb_free_tree(inodes);
	jrb_free_tree(directories);
}

int main()
{
	read_tar();
	return 0;
}
