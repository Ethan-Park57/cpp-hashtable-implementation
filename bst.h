#ifndef BST_H
#define BST_H

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template<typename Key, typename Value>
class Node {
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value& value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
 * Explicit constructor for a node.
 */
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent)
        : item_(key, value), parent_(parent), left_(NULL), right_(NULL) {}

/**
 * Destructor, which does not need to do anything since the pointers inside of a node
 * are only used as references to existing nodes. The nodes pointed to by parent/left/right
 * are freed within the deleteAll() helper method in the BinarySearchTree.
 */
template<typename Key, typename Value>
Node<Key, Value>::~Node() {}

/**
 * A const getter for the item.
 */
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const {
    return item_;
}

/**
 * A non-const getter for the item.
 */
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem() {
    return item_;
}

/**
 * A const getter for the key.
 */
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const {
    return item_.first;
}

/**
 * A const getter for the value.
 */
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const {
    return item_.second;
}

/**
 * A non-const getter for the value.
 */
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue() {
    return item_.second;
}

/**
 * An implementation of the virtual function for retreiving the parent.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const {
    return parent_;
}

/**
 * An implementation of the virtual function for retreiving the left child.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const {
    return left_;
}

/**
 * An implementation of the virtual function for retreiving the right child.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const {
    return right_;
}

/**
 * A setter for setting the parent of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent) {
    parent_ = parent;
}

/**
 * A setter for setting the left child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left) {
    left_ = left;
}

/**
 * A setter for setting the right child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right) {
    right_ = right;
}

/**
 * A setter for the value of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value) {
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
 * A templated unbalanced binary search tree.
 */
template<typename Key, typename Value>
class BinarySearchTree {
public:
    BinarySearchTree();                                                    // TODO
    virtual ~BinarySearchTree();                                           // TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair);  // TODO
    virtual void remove(const Key& key);                                   // TODO
    void clear();                                                          // TODO
    bool isBalanced() const;                                               // TODO
    void print() const;
    bool empty() const;

public:
    /**
     * An internal iterator class for traversing the contents of the BST.
     */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key, Value>& operator*() const;
        std::pair<const Key, Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key, Value>* ptr);
        Node<Key, Value>* current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const;               // TODO
    Node<Key, Value>* getSmallestNode() const;                        // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current);  // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot(Node<Key, Value>* r) const;
    virtual void nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2);

    // Add helper functions here
    Node<Key, Value>* successor(Node<Key, Value>* current);
    int balHelper(Node<Key, Value>* n) const;
    void clearHelper(Node<Key, Value>* n);

protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
 * Explicit constructor that initializes an iterator with a given node pointer.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key, Value>* ptr) {
    current_ = ptr;
}

/**
 * A default constructor that initializes the iterator to NULL.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() : current_(NULL) {}

/**
 * Provides access to the item.
 */
template<class Key, class Value>
std::pair<const Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() const {
    return current_->getItem();
}

/**
 * Provides access to the address of the item.
 */
template<class Key, class Value>
std::pair<const Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->() const {
    return &(current_->getItem());
}

/**
 * Checks if 'this' iterator's internals have the same value
 * as 'rhs'
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const {

    return current_ == rhs.current_;
}

/**
 * Checks if 'this' iterator's internals have a different value
 * as 'rhs'
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const {
    return current_ != rhs.current_;
}

/**
 * Advances the iterator's location using an in-order sequencing
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++() {
    /*
    current_ = successor(current_);
    return *this;
    */

    if (current_ != NULL) {
        if (current_->getRight() != NULL) {
            current_ = current_->getRight();
            // search left until hit leaf
            while (current_->getLeft() != NULL) {
                current_ = current_->getLeft();
            }
        }
        // else go up until current is a left child
        else {
            Node<Key, Value>* p = current_->getParent();
            while (p != NULL && p->getRight() == current_) {
                current_ = p;
                p = p->getParent();
            }
            current_ = p;
        }
    }
    return *this;
}

/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
 * Default constructor for a BinarySearchTree, which sets the root to NULL.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(NULL) {}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() {
    clear();
}

/**
 * Returns true if tree is empty
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const {
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const {
    printRoot(root_);
    std::cout << "\n";
}

/**
 * Returns an iterator to the "smallest" item in the tree
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const {
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
 * Returns an iterator whose value means INVALID
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const {
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
 * Returns an iterator to the item with the given key, k
 * or the end iterator if k does not exist in the tree
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& k) const {
    Node<Key, Value>* curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * An insert method to insert into a Binary Search Tree.
 * The tree will not remain balanced when inserting.
 */
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value>& keyValuePair) {
    // use k and v for ease of use
    Key k = keyValuePair.first;
    Value v = keyValuePair.second;

    // if empty, make keyValuePair argument the root
    if (root_ == NULL) {
        Node<Key, Value>* r = new Node<Key, Value>(k, v, NULL);
        root_ = r;
        return;
    }

    Node<Key, Value>* x = root_;

    while (x != NULL) {

        if (k < x->getKey()) {
            if (x->getLeft() != NULL) {
                x = x->getLeft();  // move down left subtree
            } else {
                break;
            }
        }

        // if k > x's key
        else if (k > x->getKey()) {
            if (x->getRight() != NULL) {
                x = x->getRight();  // move down right subtree
            } else {
                break;
            }
        }

        else if (k == x->getKey()) {
            x->setValue(keyValuePair.second);  // replace x's val with k's val
            return;
        }
    }
    Node<Key, Value>* n = new Node<Key, Value>(k, v, x);  // create new node to be inserted

    // checking where to insert
    if (k < x->getKey()) {  // left child
        x->setLeft(n);
    } else {  // right child
        x->setRight(n);
    }
}

/**
 * A remove method to remove a specific key from a Binary Search Tree.
 * The tree may not remain balanced after removal.
 */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key) {
    Node<Key, Value>* r = internalFind(key);  // node to remove
    Node<Key, Value>* p;                      // predecessor

    // if node with key is not found
    if (r == NULL) {
        return;
    }

    // if only root exists
    if (r == root_ && r->getLeft() == NULL && r->getRight() == NULL) {
        delete root_;
        root_ = NULL;
        return;
    }

    // if we found r
    if (r != NULL) {
        if (r->getRight() != NULL && r->getLeft() != NULL) {  // if 2 children
            p = predecessor(r);
            nodeSwap(p, r);

            // if old predecessor had a right child
            if (r->getRight() != NULL) {
                nodeSwap(r, r->getRight());
            } else if (r->getLeft() != NULL) {  // if left child exists
                nodeSwap(r, r->getLeft());
            }

            if (r->getParent()->getLeft() == r) {
                r->getParent()->setLeft(NULL);
            } else if (r->getParent()->getRight() == r) {
                r->getParent()->setRight(NULL);
            }

        } else if (r->getRight() != NULL) {  // if only a right child exists

            // and that right child is not a leaf node
            if (r->getParent() != NULL && r->getRight()->getRight() != NULL && r->getRight()->getLeft() != NULL) {

                // if r is a left child
                if (r->getParent()->getLeft() == r) {
                    r->getParent()->setLeft(r->getRight());
                    r->getRight()->setParent(r->getParent());
                }
                // if r is a right child
                else if (r->getParent()->getRight() == r) {
                    r->getParent()->setRight(r->getRight());
                    r->getRight()->setParent(r->getParent());
                }

            }

            // same case as above, but r is a root
            else if (r->getRight()->getRight() != NULL && r->getRight()->getLeft() != NULL && r == root_) {
                r->getRight()->setParent(NULL);
                root_ = r->getRight();
            }

            else {
                nodeSwap(r, r->getRight());
                r->getParent()->setRight(NULL);
            }

        } else if (r->getLeft() != NULL) {  // if only a left child exists

            // and that left child is not a leaf node
            if (r->getParent() != NULL && r->getLeft()->getLeft() != NULL && r->getLeft()->getRight() != NULL) {

                // if r is a left child
                if (r->getParent()->getLeft() == r) {
                    r->getParent()->setLeft(r->getLeft());
                    r->getLeft()->setParent(r->getParent());
                }
                // if r is a right child
                else if (r->getParent()->getRight() == r) {
                    r->getParent()->setRight(r->getLeft());
                    r->getLeft()->setParent(r->getParent());
                }

            }

            // same case as above, but r is a root
            else if (r->getLeft()->getLeft() != NULL && r->getLeft()->getRight() != NULL && r == root_) {
                r->getLeft()->setParent(NULL);
                root_ = r->getLeft();
            }

            else {
                nodeSwap(r, r->getLeft());
                r->getParent()->setLeft(NULL);
            }
        }

        // if removing a leaf node
        else {
            if (r->getParent()->getLeft() == r) {
                r->getParent()->setLeft(NULL);
            } else if (r->getParent()->getRight() == r) {
                r->getParent()->setRight(NULL);
            }
        }
    }
    delete r;
}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current) {
    if (current != NULL) {
        if (current->getLeft() != NULL) {
            current = current->getLeft();
            // search the right subtree until hit leaf
            while (current->getRight() != NULL) {
                current = current->getRight();
            }
        }
        // else go up until current is a right child
        else {
            Node<Key, Value>* p = current->getParent();
            while (p->getLeft() == current && p != NULL) {
                current = p;
                p = p->getParent();
            }
            current = p;
        }
    }
    return current;
}

/**
 * A method to remove all contents of the tree and
 * reset the values in the tree for use again.
 */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear() {
    if (empty()) {
        return;
    }
    clearHelper(root_);
    root_ = NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value>* n) {

    if (n->getLeft() != NULL) {
        clearHelper(n->getLeft());
    }
    if (n->getRight() != NULL) {
        clearHelper(n->getRight());
    }
    delete n;
}

/**
 * A helper function to find the smallest node in the tree.
 */
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const {
    // go left until leaf node, return
    Node<Key, Value>* x = root_;

    while (x != NULL && x->getLeft() != NULL) {
        x = x->getLeft();
    }

    return x;
}

/**
 * Helper function to find a node with given key, k and
 * return a pointer to it or NULL if no item with that key
 * exists
 */
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const {
    Node<Key, Value>* x = root_;

    while (x != NULL && x->getKey() != key) {
        if (key < x->getKey()) {
            x = x->getLeft();
        } else if (key > x->getKey()) {
            x = x->getRight();
        }
        if (x == NULL) {
            break;
        }
    }

    return x;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const {
    bool b = balHelper(root_);
    return b;
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::balHelper(Node<Key, Value>* n) const {
    // if empty, return true
    if (n == NULL) {
        return true;
    }
    int lHeight = 0;
    int rHeight = 0;

    // if at least 1 child exists
    if (n->getLeft() != NULL || n->getRight() != NULL) {
        if (n->getLeft() != NULL) {
            lHeight = balHelper(n->getLeft());

            if (lHeight == 0) {
                return 0;
            }
        }
        if (n->getRight() != NULL) {
            rHeight = balHelper(n->getRight());

            if (rHeight == 0) {
                return 0;
            }
        }
    } else {
        return 1;
    }

    if (lHeight - rHeight > 1 || rHeight - lHeight > 1) {
        return false;
    } else {
        if (lHeight >= rHeight) {
            return ++lHeight;
        } else {
            return ++rHeight;
        }
    }
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2) {
    if ((n1 == n2) || (n1 == NULL) || (n2 == NULL)) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if (n1p != NULL && (n1 == n1p->getLeft()))
        n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if (n2p != NULL && (n2 == n2p->getLeft()))
        n2isLeft = true;

    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if ((n1r != NULL && n1r == n2)) {
        n2->setRight(n1);
        n1->setParent(n2);
    } else if (n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    } else if (n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    } else if (n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);
    }

    if (n1p != NULL && n1p != n2) {
        if (n1isLeft)
            n1p->setLeft(n2);
        else
            n1p->setRight(n2);
    }
    if (n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if (n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if (n2p != NULL && n2p != n1) {
        if (n2isLeft)
            n2p->setLeft(n1);
        else
            n2p->setRight(n1);
    }
    if (n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if (n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }

    if (this->root_ == n1) {
        this->root_ = n2;
    } else if (this->root_ == n2) {
        this->root_ = n1;
    }
}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
