//Alexander Yu - CS360
//Lab 1: Fields/Dllists/RB-Trees
//September 1st, 2022

/*This lab serves as practice for C and the libraries created for C by Dr. Plank, as well as help the transition from C++ to C. 
 * More specifically, it introduces the libraries that serve as input, lists and trees, and how they are implemented.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "dllist.h"
#include "jrb.h"

/*The Person struct is used as a container for data, which is taken from standard input:
 * - visited and printed are used during depth first and breadth first search respectively
 * - name and sex are strings to assist in printing of data
 * - father and mother are pointers to other Person objects to assist in rb-tree traversal
 * - children is a doubly-linked list to allow for easy addition of other Person objects and traversal
 */

typedef struct Person Person;

struct Person
{
	int visited;
	int printed;
	char *name;
	char *sex;
	Person *father;
	Person *mother;
	Dllist children;
};

/**
 * @name alloc_string
 * @brief Allocates bits for string storage
 * @param[in] str_len no. of bits without null terminator to be allocated
 * @return A pointer to the string with bits allocated
 */

char *alloc_string(int str_len)
{
	char *ptr = malloc(sizeof(char) * (str_len+1));
	return ptr;
}

/**
 * @name create_person
 * @brief Initializes a Person object
 * @param[in] name The name the Person object will have
 * @return A pointer to the new Person object
 */

Person *create_person(name)
char *name;
{
	Person *p;
	size_t str_len = 50;
    p = malloc(sizeof(Person));
	p->father = NULL;
	p->mother = NULL;
	p->children = new_dllist();
	p->name = alloc_string(str_len);
	strcpy(p->name, name);
	p->sex = alloc_string(str_len);
	strcpy(p->sex, "Unknown");
	p->visited = 0;
	p->printed = 0;
	return p;
}

/**
 * @name is_descendant
 * @brief Checks if Person is descended from itself via depth-first search
 * @param[in] p Person object to be check
 * @return Boolean that shows if Person is descended from itself
 */

int is_descendant(p)
Person *p;
{
	Dllist l;
	if(p->visited == 1)
	{
		return 0;
	}
	if(p->visited == 2)
	{
		return 1;
	}
	p->visited = 2;
	dll_traverse(l, p->children)
	{
		if(is_descendant((Person *) l->val.v))
		{
			return 1;
		}
	}
	p->visited = 1;
	return 0;
}

int main()
{
	IS is;
	JRB people;
	JRB iter;
	Dllist l, toprint;
	Person *p, *child;
	int i;
	char *name;
	size_t str_len = 50;
	is = new_inputstruct(NULL);
	people = make_jrb();
	while(get_line(is) >= 0)
	{
		if(is->NF > 1)
		{
			/* Each possible entry is confined to its own if statement to improve readability */
			
			/*For PERSON, a new Person object is initialized if it is not in tree.
			 * p is set to Person object with name to allow for object's variables to be modified.*/
			
			if(strcmp(is->fields[0], "PERSON") == 0)
			{
				name = alloc_string(str_len);
				for(i = 1; i < is->NF; i++) 
				{
					strcat(name, is->fields[i]);
					if(i != is->NF - 1)
					{
						strcat(name, " ");
					}
				}
				if(jrb_find_str(people, name) == NULL) 
				{
					p = create_person(name);
					(void) jrb_insert_str(people, strdup(name), new_jval_v(p));	
				}
				else 
				{
					p = (Person *) jrb_find_str(people, name)->val.v;
				}
				free(name);
			}

			/*Sets p's sex if it hasn't been defined, checks if p's sex matches data entry otherwise.*/
			
			else if(strcmp(is->fields[0], "SEX") == 0)
			{
				if(strcmp(p->sex, "Unknown") == 0) 
				{
					if(strcmp(is->fields[1], "M") == 0)
					{
						strcpy(p->sex, "Male");
					}
					else if(strcmp(is->fields[1], "F") == 0)
					{
						strcpy(p->sex, "Female");
					}
					else 
					{
						fprintf(stderr, "Bad input - invalid entry for sex\n");
						exit(1);
					}
				}
				else if(p->sex[0] != is->fields[1][0])
				{
					fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
					exit(1);
				}
			}

			/*FATHER/MOTHER initializes new Person object and adds p to new Person's children list if not found in tree,
			 * checks if sex and father/mother fields are correct if found in tree.*/

			else if(strcmp(is->fields[0], "FATHER") == 0)
			{	
				name = alloc_string(str_len);
				strcpy(name, "");
				for(i = 1; i < is->NF; i++)
				{
					strcat(name, is->fields[i]);
					if(i != is->NF - 1)
					{
						strcat(name, " ");
					}
				}
				if(p->father == NULL)
				{
					if(jrb_find_str(people, name) == NULL)
					{
						p->father = create_person(name);
						dll_append(p->father->children, new_jval_v(p));
						(void) jrb_insert_str(people, strdup(name), new_jval_v(p->father));	
					}
					else
					{
						p->father = (Person *) jrb_find_str(people, name)->val.v;
						dll_append(p->father->children, new_jval_v(p));
					}
					free(name);
					if(strcmp(p->father->sex, "Unknown") == 0)
					{
						strcpy(p->father->sex, "Male");
					}
					else if(strcmp(p->father->sex, "Male") != 0)
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						exit(1);
					}
				}
				else
				{
					if(strcmp(p->father->name, name) != 0)
					{
						fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
						exit(1);
					}
				}
			}
			else if(strcmp(is->fields[0], "MOTHER") == 0)
			{	
				name = alloc_string(str_len);
				strcpy(name, "");
				for(i = 1; i < is->NF; i++)
				{
					strcat(name, is->fields[i]);
					if(i != is->NF - 1)
					{
						strcat(name, " ");
					}
				}
				if(p->mother == NULL)
				{
					if(jrb_find_str(people, name) == NULL)
					{
						p->mother = create_person(name);
						dll_append(p->mother->children, new_jval_v(p));
						(void) jrb_insert_str(people, strdup(name), new_jval_v(p->mother));	
					}
					else
					{
						p->mother = (Person *) jrb_find_str(people, name)->val.v;
						dll_append(p->mother->children, new_jval_v(p));
					}
					free(name);
					if(strcmp(p->mother->sex, "Unknown") == 0)
					{
						strcpy(p->mother->sex, "Female");
					}
					else if(strcmp(p->mother->sex, "Female") != 0)
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						exit(1);
					}
				}
				else
				{
					if(strcmp(p->mother->name, name) != 0)
					{
						fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
						exit(1);
					}
				}
			}

			/*FATHER_OF/MOTHER_OF creates new Person object and adds it to p's children list if not found in tree,
			 * checks if father/mother field matches p if found in tree.
			 * p's sex is checked for correctness, and set if it is NULL. */

			else if(strcmp(is->fields[0], "FATHER_OF") == 0)
			{	
				name = alloc_string(str_len);
				strcpy(name, "");
				for(i = 1; i < is->NF; i++)
				{
					strcat(name, is->fields[i]);
					if(i != is->NF - 1)
					{
						strcat(name, " ");
					}
				}
				if(jrb_find_str(people, name) == NULL)
				{
					child = create_person(name);
					child->father = p;
					dll_append(p->children, new_jval_v(child));
					(void) jrb_insert_str(people, strdup(name), new_jval_v(child));	
				}
				else
				{
					child = (Person *) jrb_find_str(people, name)->val.v;
					if(child->father == NULL)
					{
						child->father = p;
						dll_append(p->children, new_jval_v(child));
					}
					else if(strcmp(child->father->name, name) != 0)
					{
						fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
						exit(1);
					}
				}
				free(name);
				if(strcmp(p->sex, "Unknown") == 0)
				{
					strcpy(p->sex, "Male");
				}
				else if(strcmp(p->sex, "Male") != 0)
				{
					fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
					exit(1);
				}
			}
			else if(strcmp(is->fields[0], "MOTHER_OF") == 0)
			{	
				name = alloc_string(str_len);
				strcpy(name, "");
				for(i = 1; i < is->NF; i++)
				{
					strcat(name, is->fields[i]);
					if(i != is->NF - 1)
					{
						strcat(name, " ");
					}
				}
				if(jrb_find_str(people, name) == NULL)
				{
					child = create_person(name);
					child->mother = p;
					dll_append(p->children, new_jval_v(child));
					(void) jrb_insert_str(people, strdup(name), new_jval_v(child));	
				}
				else
				{
					child = (Person *) jrb_find_str(people, name)->val.v;
					if(child->mother == NULL)
					{
						child->mother = p;
						dll_append(p->children, new_jval_v(child));
					}
					else if(strcmp(child->mother->name, name) != 0)
					{
						fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
						exit(1);
					}
				}
				free(name);
				if(strcmp(p->sex, "Unknown") == 0)
				{
					strcpy(p->sex, "Female");
				}
				else if(strcmp(p->sex, "Female") != 0)
				{
					fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
					exit(1);
				}
			}
		}
	}

	/*Sets up queue for breadth-first search and checks for cycles in tree.
	 * Only Person objects with children are checked for cycles.
	 * Only Person objects without parents are inserted into queue.
	 * Prints out parents before children.*/
	toprint = new_dllist(); 
	jrb_traverse(iter, people)
	{
		p = ((Person *) iter->val.v);
		if(!dll_empty(p->children) && is_descendant(p)) 
		{
			fprintf(stderr, "Bad input -- cycle in specification\n");
			exit(1);
		}
		if(p->mother == NULL && p->father == NULL) 
		{
			dll_append(toprint, new_jval_v(p));
		}
	}
	while(!dll_empty(toprint)) 
	{
		p = (Person *) dll_first(toprint)->val.v;
		dll_delete_node(dll_first(toprint));
		if(p->printed == 0)
		{
			if((p->mother == NULL || p->mother->printed == 1) && (p->father == NULL || p->father->printed == 1))
			{
				printf("%s\n", p->name);
				printf("  Sex: %s\n", p->sex);
				if(p->father != NULL)
				{
					printf("  Father: %s\n", p->father->name);
				}
				else
				{
					printf("  Father: Unknown\n");
				}
				if(p->mother != NULL)
				{
					printf("  Mother: %s\n", p->mother->name);
				}
				else
				{
					printf("  Mother: Unknown\n");
				}
				printf("  Children:");
				if(dll_empty(p->children))
				{
					printf(" None\n");
				}
				else
				{
					printf("\n");
					dll_traverse(l, p->children)
					{
						child = (Person *) (l->val.v);
						printf("    %s\n", (child->name));
						dll_append(toprint, new_jval_v(child));
					}
				}
				printf("\n");
				p->printed = 1;
				free_dllist(p->children);
			}
		}
	}
	free_dllist(toprint);
	jrb_traverse(iter, people)
	{
		p = ((Person *) iter->val.v);
		free(p->name);
		free(p->sex);
		free(p);
		free(iter->key.s);
	}
	jrb_free_tree(people);
	exit(0);
}
