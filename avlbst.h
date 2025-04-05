#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
  //this->root_ = NULL;

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    //AVLTree();
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight(AVLNode<Key,Value>* a);
    void rotateLeft(AVLNode<Key,Value>* a);
    void insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void remove_fix(AVLNode<Key,Value>* p, int diff);
    void insertH(const std::pair<const Key, Value>& keyValuePair,AVLNode<Key,Value>* curr,AVLNode<Key,Value>* p);
    AVLNode<Key,Value>* findH(const Key& key, AVLNode<Key,Value>* curr) const;
    AVLNode<Key, Value>* intFind(const Key& key) const;
    static AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);
    //AVLNode<Key,Value>* this->root_;
};
/*
template<class Key,class Value>
AVLTree<Key, Value>::AVLTree() : this->root_(nullptr) { }
*/

// LINE TO COPY STATIC CAST: static_cast<AVLNode<Key, Value>*>(this->left_);
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
  if(this->root_ == NULL){
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    AVLNode<Key, Value>* rootNode = static_cast<AVLNode<Key, Value>*>(this->root_);
    rootNode->setLeft(NULL);
    rootNode->setRight(NULL);
    rootNode->setBalance(0);
    rootNode->setParent(NULL);
    return;
  }

  AVLNode<Key, Value>* foundNode = intFind(new_item.first);
  if(foundNode != NULL){
    foundNode->setValue(new_item.second);
    return;
  }

  Key insertKey = new_item.first;
  Key rootKey = this->root_->getKey();

  if(insertKey < rootKey){
    insertH(new_item, static_cast<AVLNode<Key, Value>*>(this->root_)->getLeft(),
                     static_cast<AVLNode<Key, Value>*>(this->root_));
  }
  else{
    insertH(new_item, static_cast<AVLNode<Key, Value>*>(this->root_)->getRight(),
                     static_cast<AVLNode<Key, Value>*>(this->root_));
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertH(const std::pair<const Key, Value> &keyValuePair,
                                  AVLNode<Key, Value>* currNode,
                                  AVLNode<Key, Value>* parentNode)
{
  if(currNode == NULL){
    currNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, parentNode);
    currNode->setLeft(NULL);
    currNode->setRight(NULL);
    currNode->setBalance(0);

    if(currNode->getKey() < parentNode->getKey()){
      parentNode->setLeft(currNode);
    }
    else{
      parentNode->setRight(currNode);
    }

    int8_t parentBalance = parentNode->getBalance();
    if(parentBalance == -1 || parentBalance == 1){
      parentNode->setBalance(0);
      return;
    }
    else if(parentBalance == 0){
      if(parentNode->getLeft() == currNode){
        parentNode->setBalance(-1);
      }
      else{
        parentNode->setBalance(1);
      }
      insert_fix(parentNode, currNode);
      return;
    }
  }

  Key insertKey = keyValuePair.first;
  Key currKey = currNode->getKey();

  if(insertKey < currKey){
    insertH(keyValuePair, currNode->getLeft(), currNode);
  }
  else{
    insertH(keyValuePair, currNode->getRight(), currNode);
  }
}

// insert-fix helper function 
template <typename Key, typename Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
  if(parent == NULL || parent->getParent() == NULL){
    return;
  }

  AVLNode<Key, Value>* grandparent = parent->getParent();

  if(grandparent->getLeft() == parent){
    grandparent->updateBalance(-1);

    if(grandparent->getBalance() == 0){
      return;
    }
    else if(grandparent->getBalance() == -1){
      insert_fix(grandparent, parent);
    }
    else if(grandparent->getBalance() == -2){
      if(parent->getLeft() == child){
        rotateRight(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
        return;
      }
      else{
        rotateLeft(parent);
        rotateRight(grandparent);

        int8_t childBalance = child->getBalance();
        if(childBalance == -1){
          parent->setBalance(0);
          grandparent->setBalance(1);
        }
        else if(childBalance == 0){
          parent->setBalance(0);
          grandparent->setBalance(0);
        }
        else if(childBalance == 1){
          parent->setBalance(-1);
          grandparent->setBalance(0);
        }
        child->setBalance(0);
        return;
      }
    }
  }
  else{
    grandparent->updateBalance(1);

    if(grandparent->getBalance() == 0){
      return;
    }
    else if(grandparent->getBalance() == 1){
      insert_fix(grandparent, parent);
    }
    else if(grandparent->getBalance() == 2){
      if(parent->getRight() == child){
        rotateLeft(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
        return;
      }
      else{
        rotateRight(parent);
        rotateLeft(grandparent);

        int8_t childBalance = child->getBalance();
        if(childBalance == 1){
          parent->setBalance(0);
          grandparent->setBalance(-1);
        }
        else if(childBalance == 0){
          parent->setBalance(0);
          grandparent->setBalance(0);
        }
        else if(childBalance == -1){
          parent->setBalance(1);
          grandparent->setBalance(0);
        }
        child->setBalance(0);
        return;
      }
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* targetNode = intFind(key);
  if(targetNode == NULL){
    return;
  }

  if((targetNode->getLeft() != NULL) && (targetNode->getRight() != NULL)){
    AVLNode<Key, Value>* predNode = predecessor(targetNode);
    nodeSwap(targetNode, predNode);
  }

  // One child (left)
  if((targetNode->getLeft() != NULL) && (targetNode->getRight() == NULL)){
    AVLNode<Key, Value>* parentNode = targetNode->getParent();
    if(parentNode == NULL){
      this->root_ = targetNode->getLeft();
      this->root_->setParent(NULL);
      delete targetNode;
      return;
    }

    int balanceDiff;
    if(parentNode->getLeft() == targetNode){
      parentNode->setLeft(targetNode->getLeft());
      balanceDiff = 1;
    }
    else{
      parentNode->setRight(targetNode->getLeft());
      balanceDiff = -1;
    }

    targetNode->getLeft()->setParent(parentNode);
    delete targetNode;
    remove_fix(parentNode, balanceDiff);
    return;
  }

  // One child (right)
  else if((targetNode->getLeft() == NULL) && (targetNode->getRight() != NULL)){
    AVLNode<Key, Value>* parentNode = targetNode->getParent();
    if(parentNode == NULL){
      this->root_ = targetNode->getRight();
      this->root_->setParent(NULL);
      delete targetNode;
      return;
    }

    int balanceDiff;
    if(key < parentNode->getKey()){
      parentNode->setLeft(targetNode->getRight());
      balanceDiff = 1;
    }
    else{
      parentNode->setRight(targetNode->getRight());
      balanceDiff = -1;
    }

    targetNode->getRight()->setParent(parentNode);
    delete targetNode;
    remove_fix(parentNode, balanceDiff);
    return;
  }

  // No children
  if((targetNode->getLeft() == NULL) && (targetNode->getRight() == NULL)){
    AVLNode<Key, Value>* parentNode = targetNode->getParent();
    if(parentNode == NULL){
      this->root_ = NULL;
      delete targetNode;
      return;
    }

    int balanceDiff;
    if(parentNode->getLeft() == targetNode){
      parentNode->setLeft(NULL);
      balanceDiff = 1;
    }
    else{
      parentNode->setRight(NULL);
      balanceDiff = -1;
    }

    delete targetNode;
    remove_fix(parentNode, balanceDiff);
    return;
  }
}

// remove-fix helper function
template <typename Key, typename Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value>* node, int diff)
{
  if(node == NULL) return;

  AVLNode<Key, Value>* parentNode = node->getParent();
  int nextDiff = 0;

  if(parentNode != NULL){
    nextDiff = (parentNode->getLeft() == node) ? 1 : -1;
  }

  if(diff == -1){
    if((node->getBalance() + diff) == -2){
      AVLNode<Key, Value>* child;
      if(node->getBalance() <= 0){
        child = node->getLeft();
      }
      else{
        child = node->getRight();
      }

      if(child->getBalance() == -1){
        rotateRight(node);
        node->setBalance(0);
        child->setBalance(0);
        remove_fix(parentNode, nextDiff);
      }
      else if(child->getBalance() == 0){
        rotateRight(node);
        node->setBalance(-1);
        child->setBalance(1);
        return;
      }
      else if(child->getBalance() == 1){
        AVLNode<Key, Value>* grandchild = child->getRight();
        rotateLeft(child);
        rotateRight(node);

        if(grandchild->getBalance() == 1){
          node->setBalance(0);
          child->setBalance(-1);
        }
        else if(grandchild->getBalance() == 0){
          node->setBalance(0);
          child->setBalance(0);
        }
        else{
          node->setBalance(1);
          child->setBalance(0);
        }

        grandchild->setBalance(0);
        remove_fix(parentNode, nextDiff);
      }
    }
    else if((node->getBalance() + diff) == -1){
      node->setBalance(-1);
      return;
    }
    else if((node->getBalance() + diff) == 0){
      node->setBalance(0);
      remove_fix(parentNode, nextDiff);
    }
  }

  else if(diff == 1){
    if((node->getBalance() + diff) == 2){
      AVLNode<Key, Value>* child;
      if(node->getBalance() <= 0){
        child = node->getLeft();
      }
      else{
        child = node->getRight();
      }

      if(child->getBalance() == 1){
        rotateLeft(node);
        node->setBalance(0);
        child->setBalance(0);
        remove_fix(parentNode, nextDiff);
      }
      else if(child->getBalance() == 0){
        rotateLeft(node);
        node->setBalance(1);
        child->setBalance(-1);
        return;
      }
      else if(child->getBalance() == -1){
        AVLNode<Key, Value>* grandchild = child->getLeft();
        rotateRight(child);
        rotateLeft(node);

        if(grandchild->getBalance() == -1){
          node->setBalance(0);
          child->setBalance(1);
        }
        else if(grandchild->getBalance() == 0){
          node->setBalance(0);
          child->setBalance(0);
        }
        else{
          node->setBalance(-1);
          child->setBalance(0);
        }

        grandchild->setBalance(0);
        remove_fix(parentNode, nextDiff);
      }
    }
    else if((node->getBalance() + diff) == 1){
      node->setBalance(1);
      return;
    }
    else if((node->getBalance() + diff) == 0){
      node->setBalance(0);
      remove_fix(parentNode, nextDiff);
    }
  }
}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current)
{
    // TODO
    if(current == NULL) return NULL;
    // If left child exists, predecessor is rightmost node of left subtree
    AVLNode<Key,Value>* pred = NULL;
    if(current->getLeft() != NULL){
      pred = current->getLeft();
      while(pred->getRight() != NULL){
        pred = pred->getRight();
      }
      return pred;
    }
    // else walk up ancestor chain UNTIL 
    // you find first right child pointer, its parent is PRED.
    else{
      pred = current;
      // walk up ancestor chain 
      while(pred->getParent() != NULL){
        AVLNode<Key,Value>* temp = pred;
        pred = pred->getParent();
        // check for right child pointer 
        if(pred->getRight() == temp){
          return pred;
        }
      }
      // if root is reached, there is no PRED, value is minimum
      return NULL;
    }
}
/*
// AVL Tree rotations - helper functions 
template <typename Key, typename Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* a){


}
template <typename Key, typename Value>
void AVLTree<Key,Value>::rotateLeft(AVLNode<Key,Value>* a){

}
*/
/*
template<class Key, class Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* y){
  if((y == NULL) || (y->getLeft())) return;

  AVLNode<Key,Value>* z = y->getParent();
  AVLNode<Key,Value>* x = y->getLeft();
  AVLNode<Key,Value>* b = x->getRight();
  if(z==NULL){
    // y is root 
    if(x!= NULL){
      x->setParent(NULL);
    }
    else{

    }
    
    y->setParent(x);
    

  }
}
*/
template<class Key, class Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* z){
  if(z==NULL|| z->getLeft()==NULL){
    return;
  }

  AVLNode<Key,Value>* y = z->getLeft();
  AVLNode<Key,Value>* b = y->getRight();

  z->setLeft(b);
  if(b != NULL){
    b->setParent(z);
  }

  y->setRight(z);

  AVLNode<Key,Value>* zparent = z->getParent();
  y->setParent(zparent);
  z->setParent(y);

  if(zparent==NULL){
    (this->root_)=y;
  }
  else{
    if(zparent->getLeft()==z){
      zparent->setLeft(y);
    }
    else{
      zparent->setRight(y);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key,Value>::rotateLeft(AVLNode<Key,Value>* x){
  if(x==NULL || x->getRight()==NULL){
    return;
  }

  AVLNode<Key,Value>* y = x->getRight();
  AVLNode<Key,Value>* b = y->getLeft();

  x->setRight(b);
  if(b != NULL){
    b->setParent(x);
  }

  y->setLeft(x);

  AVLNode<Key,Value>* xparent = x->getParent();
  y->setParent(xparent);
  x->setParent(y);
  
  if(xparent==NULL){
    (this->root_)=y;
  }
  else{
    if(xparent->getLeft()==x){
      xparent->setLeft(y);
    }
    else{
      xparent->setRight(y);
    }
  }
}
                           

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::intFind(const Key& key) const
{
    // TODO
  if(this->root_ == NULL){
    return NULL;
  }
  Key rootKey = this->root_->getKey();
  if(key == rootKey){
    return static_cast<AVLNode<Key, Value>*>(this->root_);
  }
  // traverse left 
  else if(key < rootKey){
    return findH(key, static_cast<AVLNode<Key, Value>*>(this->root_)->getLeft());
  }
  // traverse right
  else{
    return findH(key,static_cast<AVLNode<Key, Value>*>(this->root_)->getRight());
  }
}

template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::findH(const Key& key, AVLNode<Key,Value>* curr) const{
  if(curr == NULL){
    return NULL;
  }
  Key currKey = curr->getKey();
  if(currKey == key){
    return curr;
  }
  else if(key < currKey){
    return findH(key,curr->getLeft());
  }
  else{
    return findH(key,curr->getRight());
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif