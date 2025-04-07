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
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* a);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* a);

    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);
  
    Node<Key, Value>* internalFind(const Key& key) const;
    static AVLNode<Key, Value>* predecessor(Node<Key, Value>* current);


};

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    if(n==nullptr) return;
    AVLNode<Key, Value>* p = n->getParent();
    int8_t ndiff=0;

    if(p!=nullptr){
        if(n==p->getLeft()){
            ndiff=1;
        }else{
            ndiff = -1;
        }
    }
    if(diff==-1)
    {
        if(n->getBalance() + diff == -2){
            AVLNode<Key, Value>* c = n->getRight();
            if (c == nullptr) return;

            if(c->getBalance()<=0){
                rotateLeft(n);
                if(c->getBalance()==0)
                {
                    n->setBalance(-1);
                    c->setBalance(1);
                    return;
                }else{
                    n->setBalance(0);
                    c->setBalance(0);
                    removeFix(p,ndiff);
                }
            }else 
            {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance()==1){
                    n->setBalance(-1);
                    c->setBalance(0);
                }else if(g->getBalance()==0){
                    n->setBalance(0);
                    c->setBalance(0);
                }else {
                    n->setBalance(0);
                    c->setBalance(1);
                }
                g->setBalance(0);
                removeFix(p,ndiff);
            }
        }else if(n->getBalance() + diff == -1){
            n->setBalance(-1);
            return;
        }else if(n->getBalance() + diff == 0){
            n->setBalance(0);
            removeFix(p,ndiff);
        }
    }else if(diff==1)
    {
        if(n->getBalance() + diff == 2){
            AVLNode<Key, Value>* c = n->getLeft();
            if (c == nullptr) return;

            if(c->getBalance()>=0){
                rotateRight(n);
                if(c->getBalance()==0)
                {
                    n->setBalance(1);
                    c->setBalance(-1);
                    return;
                }else{
                    n->setBalance(0);
                    c->setBalance(0);
                    removeFix(p,ndiff);
                }
            }else 
            {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance()==-1){
                    n->setBalance(1);
                    c->setBalance(0);
                }else if(g->getBalance()==0){
                    n->setBalance(0);
                    c->setBalance(0);
                }else {
                    n->setBalance(0);
                    c->setBalance(-1);
                }
                g->setBalance(0);
                removeFix(p,ndiff);
            }
        }else if(n->getBalance() + diff == 1){
            n->setBalance(1);
            return;
        }else if(n->getBalance() + diff == 0){
            n->setBalance(0);
            removeFix(p,ndiff);
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    if(p==nullptr) return;

   AVLNode<Key, Value>* g = p->getParent();
   if (g == nullptr) return;


   if(p == g->getLeft()){
        g->updateBalance(-1);

        if(g->getBalance()==-1){
            insertFix(g, p);
        }else if(g->getBalance()==-2){
            if(n==p->getLeft()){
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }else{
                rotateLeft(p);
                rotateRight(g);
                if(n->getBalance()==-1){
                    p->setBalance(0);
                    g->setBalance(1);
                }else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                }else if(n->getBalance()==1){
                    p->setBalance(-1);
                    g->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }else{
        g->updateBalance(1);
        
        if(g->getBalance()==1){
            insertFix(g, p);
        }else if(g->getBalance()==2){
            if(n==p->getRight()){
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }else{
                rotateRight(p);
                rotateLeft(g);
                if(n->getBalance()==-1){
                    p->setBalance(0);
                    g->setBalance(-1);
                }else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                }else if(n->getBalance()==-1){
                    p->setBalance(1);
                    g->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* a)
{
    AVLNode<Key, Value>* b = a->getRight();
    
    if(b == nullptr) return a;
    
    a ->setRight(b->getLeft());
    if(b->getLeft()!=nullptr){ // if b has a left node, set its new parent to a 
        b->getLeft()->setParent(a); 
    }
    b->setParent(a->getParent()); 
    if(a->getParent()==nullptr){ //if its the root node reset root
        this->root_=b;
    }else if(a==a->getParent()->getLeft()){ // if a is a left child 
        a->getParent()->setLeft(b);
    }else{ //a is a right child 
        a->getParent()->setRight(b);
    }

    b->setLeft(a);
    a->setParent(b);    

    //update height for both a and b 
    int aHeight = a->getBalance();
    int bHeight = b->getBalance();

    a->setBalance(aHeight+1);
    b->setBalance(bHeight-1);
    
    
    return b;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* a)
{
    AVLNode<Key, Value>* b = a->getLeft();
    
    if(b == nullptr) return a;
    
    a ->setLeft(b->getRight());
    if(b->getRight()!=nullptr){ // if b has a left node, set its new parent to a 
        b->getRight()->setParent(a); 
    }
    b->setParent(a->getParent()); 
    if(a->getParent()==nullptr){ //if its the root node reset root
        this->root_=b;
    }else if(a==a->getParent()->getRight()){ // if a is a left child 
        a->getParent()->setRight(b);
    }else{ //a is a right child 
        a->getParent()->setLeft(b);
    }

    b->setRight(a);
    a->setParent(b);    
    int aHeight = a->getBalance();
    int bHeight = b->getBalance();

    a->setBalance(aHeight-1);
    b->setBalance(bHeight+1);
    
    return b;

}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_==nullptr)//empty
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    
    Node<Key, Value>* curr = this->root_;
    Node<Key, Value>* p = nullptr;

    while(curr!=nullptr){ //y walk the tree to a leaf
        p = curr;
        if(new_item.first < curr->getKey()){
            curr=curr->getLeft();
        }else if(new_item.first > curr->getKey()){
            curr = curr->getRight();
        }else{
            curr->setValue(new_item.second);
            return;

        }
    }
    // insert the new node as its child
    AVLNode<Key, Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, static_cast<AVLNode<Key, Value>*>(p));

    if(new_item.first<p->getKey()){
        p->setLeft(newNode);
    }else{
        p->setRight(newNode);
    }

    insertFix(static_cast<AVLNode<Key, Value>*>(p),newNode); 
  
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{

    // TODO
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(n==nullptr) return;

    if(n->getLeft()&&n->getRight()){
        AVLNode<Key, Value>* pred = this->predecessor(n);
        this->nodeSwap(n, pred);  
    }
    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* child = nullptr;
    if(n->getLeft()){
        child = n->getLeft();
    }else if(n->getRight()){
        child=n->getRight();
    }

    if(child != nullptr){
        child->setParent(p);
    }
    if(p == nullptr){
        this->root_ = child;
    } else if(p->getLeft() == n){
        p->setLeft(child);
    } else{
        p->setRight(child);
    }
    
    
    
    int8_t diff=0;

    if(p!=nullptr){
        if(p->getLeft()==n){
            diff= 1;
        }else if(p->getRight()==n){
            diff= -1;
        }        
    }
    delete n;
    this->removeFix(p, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
Node<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& key) const
{
    // DONE
    //find node with given 'key'
    // return pointer to key or NULL if not item found 

    Node<Key, Value>* current = this->root_;
   
    while(current !=nullptr)
    {
        if(key == current->getKey())
        {
            return current;
        }
        else if (key>current->getKey()){
            current = current -> getRight();
        }
        else if(key<current->getKey()){
            current = current -> getLeft();
        }
    }
    return nullptr;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    // DONE
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.
    if(current==nullptr) return nullptr;

   
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(current);
    //if current node has left child -  traverse to rightmost node in left subtree
    if(temp->getLeft())
    {
        AVLNode<Key, Value>* nodePred = temp->getLeft();

        while(nodePred->getRight()){
            nodePred=nodePred->getRight();
        }
        return nodePred;
    }
    //else (no L child)  Walk up the ancestor chain until finding a right child
    else{
        // find the first node who is a right child of his parent
        //if get to the root w/o finding a node who is a right child, there is no predecessor
        AVLNode<Key, Value>* parent = temp->getParent();
        while(parent && temp == parent ->getParent()){
            current=parent;
            parent = temp->getParent();
        }
        return parent;
    }

}

#endif
