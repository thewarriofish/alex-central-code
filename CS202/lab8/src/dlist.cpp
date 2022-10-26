//Alexander Yu - CS202
//October 27, 2021
//Lab 8 - Linked Lists

/*This lab focuses on the implementation of a doubly-linked list, via a circular list with a sentinel. Implementation includes methods used by STL such as insert, erase, pop and push. It also uses a non-default constructor, copy constructor, destructor, as well as an assignment overload.*/

#include <iostream>
#include "dlist.hpp"
using namespace std;

Dnode *Dnode::Next() //returns the node after the current node
{
	return flink;
}
Dnode *Dnode::Prev() //returns the node before the current node
{
	return blink;
}

Dlist::Dlist() //constructor; creates a sentinel node that links to itself in the forward and backward direction
{
	sentinel = new Dnode;
	sentinel->s = "";
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	size = 0;
}

Dlist::Dlist(const Dlist &d) //copy constructor
{
	this->sentinel = new Dnode; //creates a new sentinel node
	this->sentinel->flink = this->sentinel;
	this->sentinel->blink = this->sentinel;
	this->size = 0; //intializes size
	*this = d; //calls copy constructor for remaining values
}

Dlist& Dlist::operator= (const Dlist &d) //assignment overload
{
	Dlist tmp;
	Dnode *dn;

	Clear(); //clears list to ensure emptiness
	for(dn = d.Begin(); dn != d.End(); dn = dn->Next()) //adds values from original list into a temporary list
	{
		tmp.Push_Back(dn->s);
	}
	while(!tmp.Empty()) //adds values from temporary list into copied list
	{
		Push_Back(tmp.Pop_Front());
	}
	return *this;
}

Dlist::~Dlist() //destructor; calls clear, then deletes the sentinel node
{
	Clear();
	delete sentinel;
}

void Dlist::Clear() //erases all nodes except the sentinel
{
	while(size > 0)
	{
		Erase(sentinel->Next());
	}
}
bool Dlist::Empty() const //returns if the list is empty
{
	return (size == 0);
}

size_t Dlist::Size() const //returns size of list
{
	return size;
}

void Dlist::Push_Front(const string &s) //adds a node to the front of the list
{
	Insert_After(s, sentinel); //calls Insert_After on the sentinel
}

void Dlist::Push_Back(const string &s) //adds a node to the back of the list
{
	Insert_Before(s, sentinel); //calls Insert_Before on the sentinel
}

string Dlist::Pop_Front() //erases the first node in the list and returns its value
{
	Dnode *d;
	string x;
	d = sentinel->Next(); //sets node for deletion to separate pointer
	x = d->s; //grabs soon-to-be deleted node's string 
	Erase(sentinel->Next()); //erases node
	return x;
}
string Dlist::Pop_Back() //erases the last node in the list and returns its value
{
	Dnode *d;
	string x;
	d = sentinel->Prev(); //sets node for deletion to separate pointer
	x = d->s; //grabs string 
	Erase(sentinel->Prev()); //erases node
	return x;
}

Dnode *Dlist::Begin() const //returns the first node after sentinel in the list
{
	return sentinel->Next();
}
Dnode *Dlist::End() const //returns the node after the last node, aka sentinel
{
	return sentinel;
}
Dnode *Dlist::Rbegin() const //returns the last node in the list
{
	return sentinel->Prev();
}
Dnode *Dlist::Rend() const //returns the node before the first node in the list, aka sentinel
{
	return sentinel;
}

void Dlist::Insert_Before(const string &s, Dnode *n) //adds a node before an already existing node
{
	Dnode *d, *prev, *newnode;
	newnode = new Dnode; //allocates memory for new node
	newnode->s = s; //sets new node's string to parameter
	d = n; //sets existing node and the previous node to a separate pointer
	prev = n->Prev(); //points new node's links to its new neighbors, and vice versa
	d->blink = newnode;
	prev->flink = newnode;
	newnode->flink = d;
	newnode->blink = prev;
	size++;
}

void Dlist::Insert_After(const string &s, Dnode *n) //adds a node after an already existing node
{
	Insert_Before(s, n->Next()); //calls Insert_Before on the n's forward link
}

void Dlist::Erase(Dnode *n) //erases a node from the list
{
	Dnode *d, *prev, *next;
	d = n; //sets node for deletion to separate pointer
	prev = n->Prev(); //sets soon-to-be deleted node's neighbors to prev and next
	next = n->Next();
	next->blink = prev; //prev and next's links are pointed to each other
	prev->flink = next;
	delete d; //deletes node
	size--; //decrements size to signify removed node
}


