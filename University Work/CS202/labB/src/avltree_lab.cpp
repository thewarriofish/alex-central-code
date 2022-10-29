//Alexander Yu - CS202
//November 24, 2021
//Lab B: AVL Trees

/*This lab focuses on the implementation of an AVL Tree class, using a binary search tree class as a basis. The code implemented for the lab mainly includes code for insertion and deletion, both of which includes balancing the tree to make it a proper AVL tree.*/



#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "avltree.hpp"
using namespace std;
using CS202::AVLTree;
using CS202::AVLNode;

bool imbalance(const AVLNode *n) //procedure to check if a node is balanced or not
{
	int diff;
	diff = n->left->height - n->right->height; //finds balance factor by finding the difference of a node's children
	if(diff > 1 || diff < -1) //a node is imbalanced if its balance factor exceeds |1|
	{
		return true;
	}
	return false; 
}

void rotate(AVLNode *n) //rotates a node
{
	AVLNode *p, *gp, *middle;
	p = n->parent; //parent of rotated node
	if(p->height == 0) //exits procedure if the node happens to be the root
	{
		return;
	}
	gp = p->parent; //grandparent of rotated node
	if(p->left == n) //left rotation
	{
		middle = n->right; //node's other child
		p->left = middle; //middle becomes parent's left
		middle->parent = p; //sets middle's parent
		n->right = p; //node's right becomes parent
	}
	else if(p->right == n) //right rotation
	{
		middle = n->left; //same as left rotation, with all 'lefts' becoming right and vice-versa
		p->right = middle;
		middle->parent = p;
		n->left = p;
	}
	if(gp->right == p || gp->height == 0) //sets grandparent's child to node
	{
		gp->right = n;
	}
	else if(gp->left == p)
	{
		gp->left = n;
	}
	p->parent = n; //sets new parents for both node and grandparent
	n->parent = gp;
}

void fix_height(AVLNode *n) //fixes the height of a node by comparing with its children
{
	if(n->height == 0) //exits if node is sentinel
	{
		return;
	}
	if(n->left->height == 0 && n->right->height == 0) //if node happens to be a leaf, its height is one
	{
		n->height = 1;
	}
	if(n->left->height > n->right->height) //node's height becomes the larger of its children's height + 1
	{
		n->height = n->left->height + 1;
	}
	else
	{
		n->height = n->right->height + 1;
	}
}

void fix_imbalance(AVLNode *n) //fixes an imbalance via rotation
{
	AVLNode *grandparent; //holds the grandparent of parameter node

	/*the following two if statements are for deletion cases in which the imbalanced node is the root. 
	 * it finds the imbalance via the difference of the node's height and its children's height, then
	 * rotates accordingly. once rotation has completed, the heights of the rotated node and its children
	 * are fixed. n in these cases are considered the imbalanced node.*/

	if(n->height - n->left->height == 1 && imbalance(n)) 
	{
		rotate(n->left);
		fix_height(n->left->left);
		fix_height(n->left->right);
		fix_height(n->left);
	}
	if(n->height - n->right->height == 1 && imbalance(n))
	{
		rotate(n->right);
		fix_height(n->right->left);
		fix_height(n->right->right);
		fix_height(n->right);
	}

	/*the next two if statements are made for both insertion and deletion. in these cases n is considered the grandchild of the imbalanced node*/
	grandparent = n->parent->parent;
	/*this part manages a zig-zig imbalance, which only requires one rotation on n's parent*/
	if(grandparent->height == 0 || (n->parent->right == n && grandparent->right == n->parent) || (n->parent->left == n && grandparent->left == n->parent))
	{
		rotate(n->parent);
		fix_height(n->parent->left);
		fix_height(n->parent->right);
		fix_height(n->parent);
	}
	/*this part manages a zig-zag imbalance, which requires two rotations on n*/
	else if((n->parent->right == n && grandparent->left == n->parent) || (n->parent->left == n && grandparent->right == n->parent))
	{
		rotate(n);
		rotate(n);
		fix_height(n->left);
		fix_height(n->right);
		fix_height(n);
	}	
}

AVLTree& AVLTree::operator= (const AVLTree &t) //assignment overload        
{
  Clear(); //clears *this to prevent any extraneous data
  this->sentinel->right = recursive_postorder_copy(t.sentinel->right); //uses a recursive procedure to copy over tree
  this->sentinel->right->parent = sentinel; //sets the root node's parent
  this->size = t.size; //sets the size
  return *this;
}

/* I simply took Insert and Delete from their binary search tree
   implementations.  They aren't correct for AVL trees, but they are
   good starting points.  */

bool AVLTree::Insert(const string &key, void *val)
{
  AVLNode *parent;
  AVLNode *n;

  parent = sentinel;
  n = sentinel->right;

  /* Find where the key should go.  If you find the key, return false. */

  while (n != sentinel) {
    if (n->key == key) return false;
    parent = n;
    n = (key < n->key) ? n->left : n->right;
  }

  /* At this point, parent is the node that will be the parent of the new node.
     Create the new node, and hook it in. */

  n = new AVLNode;
  n->key = key;
  n->val = val;
  n->parent = parent;
  n->height = 1;
  n->left = sentinel;
  n->right = sentinel;

  /* Use the correct pointer in the parent to point to the new node. */

  if (parent == sentinel) {
    sentinel->right = n;
  } else if (key < parent->key) {
    parent->left = n;
  } else {
    parent->right = n;
  }
  
  /*new code past this point*/
  while(n->parent != sentinel && n != sentinel) //while loop that ends at the root node
  {
	  if(n->height == n->parent->height) //adds one to a node's parent's height if both heights are the same
	  {
		  n->parent->height++;
	  }
	  if(n->parent->parent != sentinel) //if a node's grandparent isn't the root, checks for imbalance
	  {
		if(imbalance(n->parent->parent))
		{
			fix_imbalance(n);
		}
      }
	  n = n->parent; //traverses backwards towards root
  }
  
  /* Increment the size */
  size++;
  fix_height(sentinel->right); //fixes the height of the root node separately since it is not checked in the while loop
  return true;
}
    
bool AVLTree::Delete(const string &key)
{
  AVLNode *n, *parent, *mlc;
  string tmpkey;
  void *tmpval;

  /* Try to find the key -- if you can't return false. */

  n = sentinel->right;
  while (n != sentinel && key != n->key) {
    n = (key < n->key) ? n->left : n->right;
  }
  if (n == sentinel) return false;

  /* We go through the three cases for deletion, although it's a little
     different from the canonical explanation. */
  parent = n->parent;
  /* Case 1 - I have no left child.  Replace me with my right child.
     Note that this handles the case of having no children, too. */

  if (n->left == sentinel) {
    if (n == parent->left) {
      parent->left = n->right;
    } else {
      parent->right = n->right;
    }
    if (n->right != sentinel) n->right->parent = parent;
    delete n;
    size--;

  /* Case 2 - I have no right child.  Replace me with my left child. */

  } else if (n->right == sentinel) {
    if (n == parent->left) {
      parent->left = n->left;
    } else {
      parent->right = n->left;
    }
    n->left->parent = parent;
    delete n;
    size--;

  /* If I have two children, then find the node "before" me in the tree.
     That node will have no right child, so I can recursively delete it.
     When I'm done, I'll replace the key and val of n with the key and
     val of the deleted node.  You'll note that the recursive call 
     updates the size, so you don't have to do it here. */

  } else {
    for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
    tmpkey = mlc->key;
    tmpval = mlc->val;
    Delete(tmpkey);
    n->key = tmpkey;
    n->val = tmpval;
	/*new code past this point*/
	/*because the case of a deleted node having two children is separated from the other two cases, checking for imbalance
	 * must occur separately. compared to the code to check for imbalance for the other two cases, this code uses the node
	 * itself, while the other code uses the node's parent*/
	if(imbalance(n)) 
	{
		if(n->height - n->left->height == 1) //checks for left rotation
		{
			if(n->left->height - n->left->left->height == 1) //checks for zig-zig
			{
				fix_imbalance(n->left->left);
			}
			else if(n->left->height - n->left->right->height == 1) //checks for zig-zag
			{
				fix_imbalance(n->left->right);
			}
		}
		else if(n->height - n->right->height == 1) //checks for right rotation
		{
			if(n->right->height - n->right->right->height == 1) //checks for zig-zig
			{
				fix_imbalance(n->right->right);
			}
			else if(n->right->height - n->right->left->height == 1) //checks for zig-zag
			{
				fix_imbalance(n->right->left);
			}
		}
	}
	else
	{
		fix_height(n); //fixes the height of a node if it happens to be balanced after deletion
	}
	fix_height(sentinel->right); //fixes the height of the root node
    return true;
  }
  while(parent->parent != sentinel) //while loop that ends at the root node
  {
	if(imbalance(parent))
	{
		if(parent->height - parent->left->height == 1) //checks for left rotation
		{
			if(parent->left->height - parent->left->left->height == 1) //checks for zig-zig
			{
				fix_imbalance(parent->left->left);
			}
			else if(parent->left->height - parent->left->right->height == 1) //checks for zig-zag
			{
				fix_imbalance(parent->left->right);
			}
		}
		else if(parent->height - parent->right->height == 1) //checks for right rotation
		{
			if(parent->right->height - parent->right->right->height == 1) //checks for zig-zig
			{
				fix_imbalance(parent->right->right);
			}
			else if(parent->right->height - parent->right->left->height == 1) //checks for zig-zag
			{
				fix_imbalance(parent->right->left);
			}
		}
	}
	else //fixes height if node's parent is balanced after deletion
	{
		fix_height(parent);
	}	
    parent = parent->parent; //traverses backwards towards root
  }
  fix_height(sentinel->right); //fixes the height of the root node, then checks if it is imbalanced
  if(imbalance(sentinel->right)) //a separate check occurs due to while loop ending at root
  {
	  if(sentinel->right->left->left->height < sentinel->right->left->right->height) //checks for left-right zig-zag
	  {
		  fix_imbalance(sentinel->right->left->right);
	  }
	  else if(sentinel->right->right->left->height > sentinel->right->right->right->height) //checks for right-left zig-zag
	  {
		  fix_imbalance(sentinel->right->right->left);
	  } 
	  else //zig-zig otherwise
	  {
		  fix_imbalance(sentinel->right);
	  }
	  fix_height(sentinel->right); //fixes height of root
  }
  return true;
}
               
/* You need to write these two.  You can lift them verbatim from your
   binary search tree lab. */

vector <string> AVLTree::Ordered_Keys() const //makes a vector of ordered keys; taken from bstree_lab
{
  vector <string> rv;
  make_key_vector(sentinel->right, rv);
  return rv;
}
    
void AVLTree::make_key_vector(const AVLNode *n, vector<string> &v) const //recursive method to create an ordered vector of keys; taken from bstree_lab from lab A
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
     
size_t AVLTree::Height() const //finds height of tree
{
  if(sentinel->right == sentinel)
  {
	return 0;
  }
  return sentinel->right->height; //height of tree is height of root
}

/* You need to write this to help you with the assignment overload.
   It makes a copy of the subtree rooted by n.  That subtree is part
   of a different tree -- the copy will be part of the tree that
   is calling the method. */

AVLNode *AVLTree::recursive_postorder_copy(const AVLNode *n) const //recursive procedure to copy a tree
{
  AVLNode *node; //holds node to copy
  node = new AVLNode;
  if(n->height == 1) //if node is leaf, sets all values then returns; base case
  {
	  node->key = n->key;
	  node->val = n->val;
	  node->height = n->height;
	  node->left = sentinel;
	  node->right = sentinel;
	  return node;
  }
  //sets values, as well as sentinels for children as a base
  node->key = n->key;
  node->val = n->val;
  node->height = n->height;
  node->left = sentinel;
  node->right = sentinel;
  if(n->left->height != 0) //if n has any children, recursively calls on said children
  {
	node->left = recursive_postorder_copy(n->left);
    node->left->parent = node;
  }
  if(n->right->height != 0)
  {
	node->right = recursive_postorder_copy(n->right);
    node->right->parent = node;
  }
  return node;
}
