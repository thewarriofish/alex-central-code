//Alexander Yu - CS360
//Lab 3: Fakemake
//September 15th, 2022

/*This lab focuses on the use of stat and system calls to emulate a makefile. 
 * It selectively compiles files depending on their age relative to the executable, should it exist. */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include "fields.h"
#include "dllist.h"

/**
 * @name add_files
 * @brief add filenames to a given list
 * @param[in] list: a dllist names will be added to
 * @param[in[ is: file input originates from
 */

void add_files(Dllist list, IS is)
{
	size_t i;
	for(i = 1; i < is->NF; i++)
	{
		dll_append(list, new_jval_s(strdup(is->fields[i])));
	}
}

/**
 * @name delete_mem
 * @brief deletes all allocated memory within a list
 * @param[in] list: dllist whose memory is to be deallocated
 */

void delete_mem(Dllist list)
{
	Dllist iter;
	dll_traverse(iter, list)
	{
		free(jval_s(dll_val(iter)));
	}
	free_dllist(list);
}

/**
 * @name create_cmd
 * @brief forms the command to be executed in shell
 * @param[in] flags: list of flags needed for command
 * @param[in] file: the .c file that is part of the command
 * @return full command as a string
 */

char *create_cmd(Dllist flags, char *file)
{
	Dllist iter;
	char *cmd = malloc(sizeof(char) *(101));
	snprintf(cmd, 100, "gcc -c");
	if(!dll_empty(flags))
	{
		dll_traverse(iter, flags)
		{
			snprintf(cmd + strlen(cmd), 100 - strlen(cmd), " %s", jval_s(dll_val(iter)));
		}
	}
	snprintf(cmd + strlen(cmd), 100 - strlen(cmd), " %s", file);
	return cmd;
}

int
main(int argc, char** argv)
{
	struct stat buffer;
	IS is;
	Dllist iter, c_lines, h_lines, l_lines, f_lines, o_files, c_update;
	char name[50];
	char exec[50];
	char obj[50];
	char cmd[100];
	time_t newest, max_htime, c_time;
	
	if(argc == 1)
	{
		strcpy(name, "fmakefile");
	}
	else
	{
		strcpy(name, argv[1]);
	}

	is = new_inputstruct(name);
	if(is == NULL)
	{
		perror(name);
		jettison_inputstruct(is);
		return 1;
	}

	c_lines = new_dllist();
	h_lines = new_dllist();
	l_lines = new_dllist();
	f_lines = new_dllist();

	/*Setting the 1st byte of exec to null terminator allows for checking of E line presence.*/
	exec[0] = '\0';

	/* This while loop reads the .fm file and adds all lines to their correct lists. If a single invalid line exists the program exits with an error. */

	while(get_line(is) >= 0)
	{
		if(is->NF != 0)
		{	
			if(strcmp(is->fields[0], "E") == 0)
			{
				if(exec[0] != '\0')
				{
					fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", is->line);
					delete_mem(c_lines);
					delete_mem(h_lines);
					delete_mem(l_lines);
					delete_mem(f_lines);
					jettison_inputstruct(is);
					return 1;
				}
				strcpy(exec, is->fields[1]);
			}
			else if(strcmp(is->fields[0], "C") == 0)
			{
				add_files(c_lines, is);
			}
			else if(strcmp(is->fields[0], "H") == 0)
			{
				add_files(h_lines, is);
			}
			else if(strcmp(is->fields[0], "L") == 0)
			{
				add_files(l_lines, is);
			}
			else if(strcmp(is->fields[0], "F") == 0)
			{
				add_files(f_lines, is);
			}
			else
			{
				fprintf(stderr, "fakemake (1): Lines must start with C, H, E, F or L\n");
				delete_mem(c_lines);
				delete_mem(h_lines);
				delete_mem(l_lines);
				delete_mem(f_lines);
				jettison_inputstruct(is);
				return 1;
			}
		}
	}

	/*If exec's first byte is still null terminator, no E line was found.*/

	if(exec[0] == '\0')
	{
		fprintf(stderr, "No executable specified\n");
		delete_mem(c_lines);
		delete_mem(h_lines);
		delete_mem(l_lines);
		delete_mem(f_lines);
		jettison_inputstruct(is);
		return 1;
	}

	/*First traversal finds the most recent header file available.*/

	max_htime = 0;
	dll_traverse(iter, h_lines)
	{
		if(stat(jval_s(dll_val(iter)), &buffer) == 0)
		{
			if(buffer.st_mtime > max_htime)
			{
				max_htime = buffer.st_mtime;
			}
		}
	}
	newest = max_htime;
	
	/*Second traversal finds any source files that have been updated and marks them for recompilation.
	 * Also finds the oldest object file and adds any non-existent files to the update list. */

	o_files = new_dllist();
	c_update = new_dllist();
    dll_traverse(iter, c_lines)
	{
		if(stat(jval_s(dll_val(iter)), &buffer) < 0)
		{
			dll_append(c_update, new_jval_s(jval_s(dll_val(iter))));
		}
		else
		{
			c_time = buffer.st_mtime;
			if(c_time > newest)
			{
				newest = c_time;
			}
			strcpy(obj, jval_s(dll_val(iter)));
			obj[strlen(obj)-1] = 'o';
			dll_append(o_files, new_jval_s(strdup(obj)));
			if(stat(obj, &buffer) < 0 || buffer.st_mtime < c_time || buffer.st_mtime < max_htime)
			{
				dll_append(c_update, new_jval_s(jval_s(dll_val(iter))));
			}
			if(buffer.st_mtime > newest)
			{
				newest = buffer.st_mtime;
			}
		}
	}
	
	/*Checks if executable actually needs to be compiled. If the executable exists, no files need to be updated, 
	 * and either the executable is more recent than the newest file available, then no compilation is required.*/ 
	if(stat(exec, &buffer) == 0 && c_update == c_update->flink && buffer.st_mtime >= newest)
	{
		printf("%s up to date\n", exec);
		delete_mem(c_lines);
		delete_mem(h_lines);
		delete_mem(l_lines);
		delete_mem(f_lines);
		delete_mem(o_files);
		free_dllist(c_update);
		jettison_inputstruct(is);
		return 0;
	}

	/*Third traversal checks each file to be updated. Creates command to be executed in shell.
	 * If file doesn't exist, perror and exit. If compilation fails, exits.*/
	
	dll_traverse(iter, c_update)
	{
		char *ptr = create_cmd(f_lines, jval_s(dll_val(iter)));
		strcpy(cmd, ptr);
		free(ptr);
		if(stat(jval_s(dll_val(iter)), &buffer) < 0)
		{
			fprintf(stderr, "fmakefile: ");
			perror(jval_s(dll_val(iter)));
			delete_mem(c_lines);
			delete_mem(h_lines);
			delete_mem(l_lines);
			delete_mem(f_lines);
			delete_mem(o_files);
			free_dllist(c_update);
			jettison_inputstruct(is);
			return 1;
		}
		printf("%s\n", cmd);
		if(system(cmd) != 0)
		{
			fprintf(stderr, "Command failed.  Exiting\n");
			delete_mem(c_lines);
			delete_mem(h_lines);
			delete_mem(l_lines);
			delete_mem(f_lines);
			delete_mem(o_files);
			free_dllist(c_update);
			jettison_inputstruct(is);
			return 1;
		}
	}

	/*Prints and executes the shell command for compiling executable. Exits if compilation fails. */

	strcpy(cmd, "");
	snprintf(cmd, 100, "gcc -o %s", exec);
	if(!dll_empty(f_lines))
	{
		dll_traverse(iter, f_lines)
		{
			snprintf(cmd + strlen(cmd), 100 - strlen(cmd), " %s", jval_s(dll_val(iter)));
		}
	}
	dll_traverse(iter, o_files)
	{
		snprintf(cmd + strlen(cmd), 100 - strlen(cmd), " %s", jval_s(dll_val(iter)));
	}
	if(!dll_empty(l_lines))
	{
		dll_traverse(iter, l_lines)
		{
			snprintf(cmd + strlen(cmd), 100 - strlen(cmd), " %s", jval_s(dll_val(iter)));
		}
	}

	printf("%s\n", cmd);
	if(system(cmd) != 0)
	{
		fprintf(stderr, "Command failed.  Fakemake exiting\n");
		delete_mem(c_lines);
		delete_mem(h_lines);
		delete_mem(l_lines);
		delete_mem(f_lines);
		delete_mem(o_files);
		free_dllist(c_update);
		jettison_inputstruct(is);
		return 1;
	}

	/*Deletes all memory.*/

	delete_mem(c_lines);
	delete_mem(h_lines);
	delete_mem(l_lines);
	delete_mem(f_lines);
	delete_mem(o_files);
	free_dllist(c_update);
	jettison_inputstruct(is);
	return 0;
}
