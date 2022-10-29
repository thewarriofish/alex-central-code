//Alexander Yu - CS202
//November 16, 2021
//Lab A: Binary Search Trees


/*This lab focuses on the implementations of several methods relating to binary search trees, namely calls to find a tree's depth and height, a call for an ordered list of a tree's keys, and an assignment overload and copy constructor which not only copies, but also balances the tree.*/



#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS202::BSTree;
using CS202::BSTNode;

/* ---------------------------------------------- */
/* You'll write all of these as part of your lab. */

int BSTree::Depth(const string &key) const //finds the depth of the node with key
{
  BSTNode *current; //holds current node
  int i; //counts how many nodes have been traversed
  current = sentinel->right; //starts at first node
  if(Find(key) == NULL) //returns -1 if key is not found in tree
  {
	  i = -1;
	  current = NULL;
  }
  while(current != NULL) //traverses tree for node with key
  {
	  if(key == current->key) //ends traversal if key @ current node equals target key
	  {
		  current = NULL;
	  }
	  else if(key > current->key) //heads right if key @ current node is less than target key
	  {
		  i++; 
		  current = current->right;
	  }
	  else if(key < current->key) //heads left if key @ current node is more than target key
	  {
		  i++;
		  current = current->left;
	  }
  }
  return i;
}
           
int BSTree::Height() const //finds height of tree
{
  int height;
  if(sentinel->right == sentinel) //if tree is empty, height is zero
  {
	  return 0;
  }
  if(sentinel->right->right == sentinel && sentinel->right->left == sentinel) //if tree only contains one node, height is 1
  {
	  return 1;
  }
  height = 0;
  height = recursive_find_height(sentinel->right); //uses recursive method to find height
  return height;
}

vector <string> BSTree::Ordered_Keys() const //creates a string vector that is a list of keys in ascending order
{
  vector <string> rv;
  make_key_vector(sentinel->right, rv); //uses recursive method to create vector
  return rv;
}
    
BSTree::BSTree(const BSTree &t) //copy constructor       
{
  this->sentinel = new BSTNode; //initializes new tree's sentinel
  this->sentinel->left = this->sentinel;
  this->sentinel->right = this->sentinel;
  this->size = 0; //initalizes new tree's size
  *this = t; //uses assignment overload to copy over values
}

BSTree& BSTree::operator= (const BSTree &t) //assignment overload and tree rebalancer
{
  vector <string> keys; //holds an ordered key vector
  vector <void *> vals; //holds an ordered value vector
  keys = t.Ordered_Keys(); 
  vals = t.Ordered_Vals();
  Clear();
  this->sentinel->right = make_balanced_tree(keys, vals, 0, keys.size()); //sets this tree's first node to the first node of the rebalanced tree
  this->sentinel->right->parent = sentinel; //sets the first node's parent to this tree's sentinel
  this->size = t.size; //sets this tree's size to t's size
  return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const //recursive method to find tree height
{
  int hl, hr, h;
  if(n->left == sentinel && n->right == sentinel) //if node is leaf, ends recursion and adds 1 for each node traversed
  {
	  return 1;
  }
  hl = 0; //height of left branch
  hr = 0; //height of right branch
  if(n->left != sentinel) 
  {
	  hl = recursive_find_height(n->left) + 1; //recursively finds height of left branch
  }
  if(n->right != sentinel)
  {
	  hr = recursive_find_height(n->right) + 1; //recursively finds height of right branch
  }
  if(hl > hr) //traverses along left branch if its height is larger
  {
	  h = hl;
  }
  else //traverses along right branch if its height is larger
  {
	  h = hr;
  }	
  return h;
}

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const //recursive method to create an ordered key vector
{
	bool flag; //flag to check if value @ root of tree has been used
	flag = false;
	if(n->left == sentinel && n->right == sentinel) //if node is leaf, adds value into vector
	{
		v.push_back(n->key);
		return;
	}
	if(n->left != sentinel) //recursively adds values from left branch, then adds root; flag is true due to root being used
	{
		make_key_vector(n->left, v);
		v.push_back(n->key);
		flag = true;
	}
	if(n->right != sentinel) //adds root only if there was no left branch, then recursively adds right branch to vector
	{
		if(!flag)
		{
			v.push_back(n->key);
		}
		make_key_vector(n->right, v);
	}
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, //creates a balanced tree using given vectors
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
  size_t x; //contains number of indices in tree/subtree
  int root; //holds root of tree
  BSTNode *node; //holds current node
  node = new BSTNode;
  if(num_indices == 1) //ends recursion and creates a leaf node using values stored in vecotrs
  {
	  node->key = sorted_keys[first_index];
	  node->val = vals[first_index];
	  node->left = sentinel;
	  node->right = sentinel;
	  return node;
  } 
  if(num_indices == 0) //ends recursion and no new node is created; node is deleted to prevent memory leaks
  {
	  delete node;
	  return sentinel;
  }
  x = first_index + num_indices;
  root = num_indices/2;
  node->key = sorted_keys[root + first_index]; //current node's key and val are taken from vectors; root + first_index is the median of tree/subtree
  node->val = vals[root + first_index];

/*recursive method follows this pattern: 
 * num_indices is divided by 2
 * left side uses first_index given in function call and current num_indices
 * right side uses a first_index that is equal to the total number of indices minus the current num_indices, and current num_indices
 * if total number of indices is even, right side's num_indices is one less than left side's*/

  if(num_indices % 2 == 0) //if the number of indices is even, one side will have one less index
  {
      num_indices /= 2; //splits tree
	  node->left = make_balanced_tree(sorted_keys, vals, first_index, num_indices); //recursively creates left side of subtree
	  node->left->parent = node;
	  num_indices--; //removes one index for the right side
	  node->right = make_balanced_tree(sorted_keys, vals, (x - num_indices), num_indices); //recursively creates right side of subtree	
	  node->right->parent = node;
  }
  else //if the number of indices is odd, boths sides will have same number of indices
  {
	  num_indices /= 2; //splits tree
	  node->left = make_balanced_tree(sorted_keys, vals, first_index, num_indices); //recursively creates left side of subtree
	  node->left->parent = node;
      node->right = make_balanced_tree(sorted_keys, vals, (x - num_indices), num_indices); //recursively creates right side of subtree	
	  node->right->parent = node;
  }
  return node;
}
