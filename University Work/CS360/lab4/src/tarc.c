//Alexander Yu - CS360
//September 30th, 2022
//Lab 4A: Tar Create

/*This lab focuses on duplicating the functions of the tar program, more specifically creating, which takes directories and files and turns them into a single batch file.
 * It utilizes many parts of Unix like stat and dirent as well as buffered I/O. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>
#include "jrb.h"
#include "dllist.h"

#define MAXLEN 1001

/**
 * @name print_data
 * @brief processes and prints tarfile data in binary format
 * @param[in] name: the name of the file, including its parent directories
 * @param[in] filename: the full path name; includes directories above parent
 * @param[in] buffer: holds the data of the file collected from stat
 * @param[in] mode: determines what data to print
 */

void print_data(char *name, char *filename, struct stat buffer, int mode)
{
	FILE *file;
	char s[MAXLEN];
	int name_size = (int) strlen(name);

	/* All calls of print_data will print this data. */

	if(mode > 0)
	{
		fwrite(&name_size, 1, sizeof(int), stdout);
		fwrite(name, 1, strlen(name), stdout);
		fwrite(&buffer.st_ino, 1, sizeof(long), stdout);
	}

	/*print_data only prints this data if it's the first time it has been encountered. */

	if(mode > 1)
	{
		fwrite(&buffer.st_mode, 1, sizeof(int), stdout);
		fwrite(&buffer.st_mtime, 1, sizeof(long), stdout);
	}

	/*print_data only prints this data if the data is from a file. */

	if(mode > 2)
	{
		long size = buffer.st_size;
		fwrite(&size, 1, sizeof(long), stdout);
		file = fopen(filename, "rb");
		if(file == NULL)
		{
			perror(filename);
			exit(1);
		}
		fread(s, 1, MAXLEN - 1, file);
		fwrite(s, 1, size, stdout);
		fclose(file);
	}
}

/**
 * @name traverse_directory
 * @brief recursively travels through directory and all subdirectories
 * @param[in] name the full path name
 * @param[in] parent the path name without anything above the parent directory
 * @param[in] a red-black tree containing all inodes encountered
 */

void traverse_directory(char *name, char *parent, JRB inodes)
{
	char s[MAXLEN], parent_name[MAXLEN], pathname[MAXLEN * 2];
	int exists;
	DIR *d;
	struct dirent *de;
	struct stat buffer;
	JRB directories, iter;
	
	sprintf(s, "%s/", name);
	exists = stat(s, &buffer);
	if(exists < 0)
	{
		fprintf(stderr, "Couldn't stat %s\n", s);
		exit(1);
	}

	/*Updates the parent name if a new directory has been reached. */

	if(strcmp(basename(name), parent) != 0)
	{
		sprintf(parent_name, "%s/%s", parent, basename(name));
	}
	else
	{
		strcpy(parent_name, parent);
	}

	if(jrb_find_int(inodes, buffer.st_ino) != NULL)
	{
		print_data(parent_name, NULL, buffer, 1);
	}
	else
	{
		print_data(parent_name, NULL, buffer, 2);
		jrb_insert_int(inodes, buffer.st_ino, JNULL);
	}

	d = opendir(name);
	if(d == NULL)
	{
		perror(name);
		exit(1);
	}
	directories = make_jrb();
	
	/*Traverses the directory, ignoring the '.' and '..' files, and prints data for files. 
	 * If a subdirectory is encountered, adds it to the directories tree. */

	for(de = readdir(d); de != NULL; de = readdir(d))
	{
		if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
		{
			sprintf(s, "%s/%s", name, de->d_name);
			exists = stat(s, &buffer);
			if(exists < 0)
			{
				fprintf(stderr, "Couldn't stat %s\n", s);
				return;
			}
			if(S_ISDIR(buffer.st_mode))
			{
				jrb_insert_str(directories, strdup(parent_name), new_jval_s(strdup(s)));
			}
			else
			{
				sprintf(pathname, "%s/%s", parent_name, de->d_name);
				if(jrb_find_int(inodes, buffer.st_ino) != NULL)
				{
					print_data(pathname, NULL, buffer, 1);
				}
				else
				{
					print_data(pathname, s, buffer, 3);
					jrb_insert_int(inodes, buffer.st_ino, JNULL);
				}	
			}
		}
	}
	closedir(d);

	/*Recursively calls traverse_directory on every subdirectory encountered. 
	 * Also frees memory after every call.*/

	jrb_traverse(iter, directories)
	{
		traverse_directory(jval_s(iter->val), jval_s(iter->key), inodes);
		free(iter->val.s);
		free(iter->key.s);
	}
	jrb_free_tree(directories);
}

int main(int argc, char** argv)
{
	DIR *d;
	JRB inodes;
		
	if(argc != 2)
	{
		fprintf(stderr, "usage: bin/tarc [filename]\n");
		return 1;
	}

	/*Checks if the directory exists.*/

	d = opendir(argv[1]);
	if(d == NULL)
	{
		perror(argv[1]);
		return 1;
	}
	closedir(d);

	inodes = make_jrb();
	traverse_directory(argv[1], basename(argv[1]), inodes);
	jrb_free_tree(inodes);

	return 0;
}
