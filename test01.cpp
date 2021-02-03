/*test01.cpp*/

//
// Unit tests for threaded binary search tree
//

#include <iostream>
#include <vector>

#include "bstt.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(1) empty tree") 
{

  bstt<int,int> tree3;
  tree3.insert(30, 9);
  tree3.insert(15, 8);
  tree3.insert(8, 7);
  tree3.insert(25, 6);
  tree3.insert(20, 5); 
  tree3.insert(28, 4); 
  tree3.insert(50, 3); 
  tree3.insert(70, 2); 
  tree3.insert(60, 1);
  tree3.insert(10, 1);
  tree3.insert(75, 1);
    

   tree3.dump(std::cout);
    

  cout<<endl<<endl;
  
  cout<<"first begin/next"<<endl;
  tree3.begin();
  int key2;
  while (tree3.next(key2)){
    cout <<"Key: "<< key2 << endl;
  }
  cout<<endl;
  cout<<"second begin/next"<<endl;
  tree3.begin();
  int key3;
  while (tree3.next(key3)){
    cout <<"Key: "<< key3 << endl;
  }

  
  
  
    
    
  
}
