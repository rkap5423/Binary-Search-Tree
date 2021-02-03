/*bstt.h
 UIC CS 251
 Raj Kapadia
 */

//
// Threaded binary search tree
//

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    NODE* leftNode;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* leftNode; //Node to denote current position of tree, initially at leftmost node
  bool isLeft=true;//boolean to denote if LeftNode is actually pointing at the leftmost item
  
  /*helper function to delete a tree, takes a node as a parameter, usually the Root*/
  void helperDel(NODE* cur) const{
      if (cur == nullptr)
       return;
     else
     {
        helperDel(cur->Left); //recursively move left
        if (cur->isThreaded==false)
            helperDel(cur->Right);//recursively move right if we're not at a threaded node
        delete(cur);//free node
     }
     
  }
  
  /*helper function to copy a tree, takes a node as a parameter, usually the Root*/
  void helperCopy(NODE* cur)
  {
     if (cur == nullptr)
       return;
     else
     {
        insert(cur->Key,cur->Value); //insert all the old nodes into the newly constructed tree
        helperCopy(cur->Left);
        if (cur->isThreaded==false)
            helperCopy(cur->Right);
      }
  }
  
  /*helper function to output a tree, takes a node as a parameter, usually the Root*/
  void helperPrint(NODE* cur,ostream& output) const{
      if (cur==nullptr)
          return;
      else{
          helperPrint(cur->Left,output);
          
          if (cur->isThreaded==false)
              output<<"("<<cur->Key<<","<<cur->Value<<")"<<endl;//print key and value is node isn't threaded   
          else{
              if (cur->Right==nullptr){
                  output<<"("<<cur->Key<<","<<cur->Value<<")"<<endl; //print key and value if we've reached the rightmost node
              }
              else{
                  output<<"("<<cur->Key<<","<<cur->Value<<","<<cur->Right->Key<<")"<<endl; //print key, value, and thread
              }
              
              }
      }
      if (cur->isThreaded==false)
          helperPrint(cur->Right,output);
  
  }
  
 /*helper function to find the leftmost node in a tree and set the internal state/position of the tree to that node
  Takes a node as parameter, usually the Root*/ 
 void leftMost(NODE* cur) 
{ 
    NODE* temp=cur;
    if (temp == nullptr) 
       return; 
  
    while (temp->Left != nullptr)//search through entire left side of tree 
        temp = temp->Left; 
    isLeft=true;//set boolean flag that the internal state is at the lowest value to true
    leftNode=temp; //update internal state
} 

 /*helper function to find the leftmost node in a tree and return that node
  Take a node as a parameter*/
NODE* leftMost2(NODE* cur) 
{ 
    NODE* temp=cur;
    if (temp == nullptr) 
       return nullptr; 
  
    while (temp->Left != nullptr) 
        temp = temp->Left; 
  
    return temp; 
} 

/*Helper function to find and return the next in order node in the tree.*/
NODE* inOrder(){
    NODE *cur = leftNode; 
        if (cur != NULL)  
        { 
            // If this node is a thread node, then go to the inorder right node 
            if (cur->isThreaded) 
                cur = cur->Right; 
            else // Else go to the leftmost child in right subtree 
                cur = leftMost2(cur->Right); 
        } 
        leftNode=cur; //update internal state
        return cur;
}

/*Function to find and return the node that contains a particular key. The key to search for is a parameter */
NODE* search2(KeyT key) const
  {
    NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key){
          return cur; // found in tree
      }  
      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else
      {
        if (cur->isThreaded){ 
            cur=nullptr;
        }
        else{
            cur = cur->Right; //search right
        }
      }
    } 
    // if we get here, not found
    return nullptr; 
  }

public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }

  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
    Root=nullptr; //start empty tree
    Size=0;
    leftNode=other.leftNode; //copy internal state
    helperCopy(other.Root); //copy tree
  }

	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
    helperDel(Root); //deletes tree
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
    this->clear(); //clears current tree
    helperCopy(other.Root); //copies "other" tree to current tree
    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
    helperDel(Root);//deletes current tree
    Root=nullptr;//Starts a tree back at the root, 
    Size=0; //with size of zero and no internal state
    leftNode=nullptr;
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    NODE* cur = Root;

    while (cur != nullptr)
    {
      if (key == cur->Key){
          value=cur->Value; //return found key
          return true; // found key, so return true
      }  
        

      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else
      {
        if (cur->isThreaded){
            cur=nullptr;
        }
        else{
            cur = cur->Right; //search right
        }
      }
    } 
  
    // if get here, not found
    return false;
    
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;

    // 1. Search to see if tree already contains key:
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return;

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        if (cur->isThreaded){
            prev = cur;
            cur=nullptr;
        }
        else{
            prev = cur;
            cur = cur->Right; //search right
       }
        
        
      }
    }
    
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    
    NODE* n=new NODE();
    n->Key=key;
    n->Left=nullptr; //left will always be null, since it can't be threaded
    n->Value=value;
    n->isThreaded=false;

    if (cur==NULL && prev == NULL){ //checks if we need to insert a Root
      Root=n;
      n->Right=nullptr;
      Size++;
      }
    else{
      if (key<prev->Key){ //inserts left
         n->Right=prev; //sets thread to parent
         prev->Left=n; //links parent to new child
         n->isThreaded=true;
         }
      else{//inserts right
         n->Right=prev->Right; //sets thread to parent's thread
         prev->isThreaded=false;
         n->isThreaded=true;//remove's parent's thread
         prev->Right=n;//links parent to new child     
         }
         
      Size++;
       }
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
    ValueT temp;
    if (search(key,temp)) //searches for key, returns it if found
        return temp;
    else //returns default value if not found
        return ValueT{};
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
    NODE* find=search2(key); //searches for key and creates a node pointing to the node containing the key
    if (find==nullptr||find->Right==NULL) //returns default if key isn't found or if it doesn't have a 'right'
        return KeyT{};
    else{//otherwise, returns the key to the right
        return find->Right->Key;
    }
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    if (Size==0) //if tree is empty, stop function
        return;
    leftMost(Root); //finds the leftmost node, and sets internal state to it
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
     if (Size==0)//if tree is empty, stop function
         return false;
     if (isLeft==true){ //condition to print out first key in tree upon first call to next
         key=leftNode->Key;
         isLeft=false;
         return true;
     }
         
     NODE* temp=inOrder(); //gets next in order node
     if (temp==nullptr)
         return false;
     else{
         key=temp->Key; //returns the next in order key
         return true;
     }
    return false;
  }

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //

    helperPrint(Root,output);
    output << "**************************************************" << endl;
  }
	
};

