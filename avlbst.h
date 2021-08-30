#ifndef RBBST_H
#define RBBST_H

#include "bst.h"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the height as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template<typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor and setting
 * the color to red since every new node will be red when it is first inserted.
 */
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
        : Node<Key, Value>(key, value, parent), height_(1) {}

/**
 * A destructor which does nothing.
 */
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

/**
 * A getter for the height of a AVLNode.
 */
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const {
    return height_;
}

/**
 * A setter for the height of a AVLNode.
 */
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height) {
    height_ = height;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template<class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    virtual void insert(const std::pair<const Key, Value>& new_item);  // TODO
    virtual void remove(const Key& key);                               // TODO
        
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // my helper functions
    void rotateLeft(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p);
    void rotateRight(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p);
    bool isLeftChild(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p);
    bool isRightChild(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p);
    void insertFix(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p);
    void removeFix(AVLNode<Key, Value>* n);
    bool rightChildExists(AVLNode<Key, Value>* n);
    bool leftChildExists(AVLNode<Key, Value>* n);
    void updateHeight(AVLNode<Key, Value>* n);

    using iter_type = typename BinarySearchTree<Key, Value>::iterator;
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    // TODO
    Key k = new_item.first;
    Value v = new_item.second;

    // if empty, insert at root and update height
    if (this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(k, v, nullptr);
        return;
    }

    AVLNode<Key, Value>* x = static_cast<AVLNode<Key, Value>*>(this->root_);

    while (x != NULL) {

        if (k < x->getKey()) {
            if (leftChildExists(x)) {
                x = x->getLeft();  // move down left subtree
            } else
                break;  // else we will place at x->left
        }

        else if (k > x->getKey()) {
            if (rightChildExists(x)) {
                x = x->getRight();  // move down right subtree
            } else
                break;  // else we will place at x->right
        }

        else {
            x->setValue(v);  // replace x's val with k's val
            return;
        }
    }

    // node to be inserted
    AVLNode<Key, Value>* n = new AVLNode<Key, Value>(k, v, x);

    // checking where to insert
    if (k < x->getKey()) {  // left child
        x->setLeft(n);
    } else
        x->setRight(n);

    // checking height above n
    if (x->getHeight() == 1) {
        x->setHeight(x->getHeight() + 1);  // update parent's height
        insertFix(n, x);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p) {
    // if at root
    if (p == NULL || p->getParent() == NULL) {
        return;
    }

    AVLNode<Key, Value>* g = p->getParent();  // grandparent
    int gHeightBuf = g->getHeight();          // used for checking if g's height changed later

    // set g's height
    if (!rightChildExists(g) || !leftChildExists(g)) {  // g has only one child, p
        g->setHeight(p->getHeight() + 1);
    } else {  // set it to 1 + max children's height
        g->setHeight(std::max(g->getLeft()->getHeight(), g->getRight()->getHeight()) + 1);
    }

    // if g's height did not change:
    if (g->getHeight() == gHeightBuf) {
        return;
    }
    // if balanced still, aka:
    // both children exist, and diffHeight <= 1
    else if (
            rightChildExists(g) && leftChildExists(g)
            && std::abs(g->getLeft()->getHeight() - g->getRight()->getHeight()) <= 1) {
        insertFix(p, g);
    }

    // unbalanced case:
    else {
        // int h = 0;
        // zig-zig, which needs right rotate to fix
        if (isLeftChild(n, p) && isLeftChild(p, g)) {

            rotateRight(g, p);

            updateHeight(g);
            updateHeight(p);
            /*
            updateHeight(n);
            updateHeight(g);
            updateHeight(p); */
        }

        // zig-zig, which needs left rotate to fix
        else if (isRightChild(n, p) && isRightChild(p, g)) {

            rotateLeft(g, p);
            updateHeight(g);
            updateHeight(p);
            /*
            updateHeight(n);
            updateHeight(g);
            updateHeight(p);*/
        }

        // zig-zag, which needs left, then right rotate to fix
        else if (isRightChild(n, p) && isLeftChild(p, g)) {
            rotateLeft(p, n);
            rotateRight(g, n);
            updateHeight(g);
            updateHeight(p);
            updateHeight(n);
        }

        // zig-zag which needs right, then left rotate to fix
        else if (isLeftChild(n, p) && isRightChild(p, g)) {

            rotateRight(p, n);
            rotateLeft(g, n);
            updateHeight(g);
            updateHeight(p);
            updateHeight(n);
        }
    }

    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // TODO

    AVLNode<Key, Value>* r = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    AVLNode<Key, Value>* pre = nullptr;  // predecessor
    AVLNode<Key, Value>* p = nullptr;

    // if node with key is not found
    if (r == NULL) {
        return;
    }

    // if only root exists
    if (r == this->root_ && r->getLeft() == NULL && r->getRight() == NULL) {
        delete this->root_;
        this->root_ = NULL;
        return;
    }

    // else: BST REMOVE
    if (r != NULL) {
        AVLNode<Key, Value>* temp = NULL;

        // LEAF NODE
        if (r->getLeft() == NULL && r->getRight() == NULL) {
            if (r->getParent()->getLeft() == r) {
                r->getParent()->setLeft(NULL);
            } else {
                r->getParent()->setRight(NULL);
            }
        }

        // 1 CHILD CASE
        else if (r->getLeft() == NULL || r->getRight() == NULL) {
            if (r->getRight() != NULL) {
                r->getRight()->setParent(r->getParent());
                if (this->root_->getKey() != r->getKey()) {
                    if (r == r->getParent()->getRight()) {
                        r->getParent()->setRight(r->getRight());
                    } else {
                        r->getParent()->setLeft(r->getRight());
                    }
                } else {
                    this->root_ = r->getRight();
                }
            } else {
                r->getLeft()->setParent(r->getParent());
                if (this->root_->getKey() != r->getKey()) {
                    if (r == r->getParent()->getRight()) {
                        r->getParent()->setRight(r->getLeft());
                    } else {
                        r->getParent()->setLeft(r->getLeft());
                    }
                } else {
                    this->root_ = r->getLeft();
                }
            }
        }

        // 2 CHILD CASE
        else {
            temp = NULL;
            pre = static_cast<AVLNode<Key, Value>*>(this->predecessor(r));
            nodeSwap(pre, r);
            // PREDECESSOR WAS A LEAF NODE
            if (r->getLeft() == NULL && r->getRight() == NULL) {
                if (r->getParent()->getLeft() == r) {
                    r->getParent()->setLeft(NULL);
                } else {
                    r->getParent()->setRight(NULL);
                }
            }
            // PREDECESSOR HAD 1 CHILD
            else if (r->getLeft() == NULL || r->getRight() == NULL) {
                if (r->getLeft() != NULL) {
                    temp = r->getLeft();
                    nodeSwap(r, temp);
                    temp->setLeft(NULL);
                } else {
                    temp = r->getRight();
                    nodeSwap(r, temp);
                    temp->setRight(NULL);
                }
            }
        }
    }
    p = r->getParent();
    delete r;
    removeFix(p);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n) {

    if (n == NULL) {
        return;
    }

    int nHeightBuf = n->getHeight();
    int newHeightBuf = n->getHeight();
    bool isUnbalanced = false;

    if (rightChildExists(n) && leftChildExists(n)) {  // n has two children
        newHeightBuf = std::max(n->getLeft()->getHeight(), n->getRight()->getHeight()) + 1;
    }

    // n has only one child
    else if (rightChildExists(n)) {  // n has only one child
        newHeightBuf = n->getRight()->getHeight() + 1;
    } else if (leftChildExists(n)) {
        newHeightBuf = n->getLeft()->getHeight() + 1;
    }

    else {  // no children case
        newHeightBuf = 1;
    }
    // DONE calculating N'S HEIGHT

    // n has two children
    if (rightChildExists(n) && leftChildExists(n)
        && std::abs(n->getLeft()->getHeight() - n->getRight()->getHeight()) >= 2) {
        isUnbalanced = true;
    } else if (rightChildExists(n) && !leftChildExists(n) && (n->getRight()->getHeight() >= 2)) {
        isUnbalanced = true;
    } else if (!rightChildExists(n) && leftChildExists(n) && (n->getLeft()->getHeight() >= 2)) {
        isUnbalanced = true;
    }
    // if height is unchaged, return
    else if (nHeightBuf == newHeightBuf) {
        return;
    } else {
        n->setHeight(newHeightBuf);
        removeFix(n->getParent());
    }

    // BALANCE IT!
    if (isUnbalanced) {
        AVLNode<Key, Value>* c = nullptr;  // taller of n's children
        AVLNode<Key, Value>* g = nullptr;  // taller of c's children

        // n has 2 children
        if (rightChildExists(n) && leftChildExists(n)) {
            if (n->getLeft()->getHeight() > n->getRight()->getHeight()) {
                c = n->getLeft();
            } else {
                c = n->getRight();
            }
        }
        // n has 1 child
        else {
            if (rightChildExists(n)) {
                c = n->getRight();
            } else {
                c = n->getLeft();
            }
        }
        // initialize g
        // c has 2 children
        if (rightChildExists(c) && leftChildExists(c)) {
            if (c->getLeft()->getHeight() == c->getRight()->getHeight()) {
                if (isLeftChild(c, n)) {
                    g = c->getLeft();
                } else if (isRightChild(c, n)) {
                    g = c->getRight();
                }
            } else if (c->getLeft()->getHeight() > c->getRight()->getHeight()) {
                g = c->getLeft();
            } else if (c->getLeft()->getHeight() < c->getRight()->getHeight()) {
                g = c->getRight();
            }
        }
        // g has 1 child
        else {
            if (rightChildExists(c)) {
                g = c->getRight();
            } else {
                g = c->getLeft();
            }
        }

        // ROTATIONS
        // case 1: g is left child ("LC") of c and c is LC of n
        if (isLeftChild(g, c) && isLeftChild(c, n)) {
            rotateRight(n, c);

            updateHeight(n);
            updateHeight(c);

            removeFix(c->getParent());
        }

        // case 2: g is right child ("RC") of c and c is RC of n
        else if (isRightChild(g, c) && isRightChild(c, n)) {
            rotateLeft(n, c);

            updateHeight(n);
            updateHeight(c);

            removeFix(c->getParent());
        }

        // case 3: g is RC of c and c is LC of n
        else if (isRightChild(g, c) && isLeftChild(c, n)) {
            rotateLeft(c, g);
            rotateRight(n, g);

            updateHeight(c);
            updateHeight(n);
            updateHeight(g);

            removeFix(g->getParent());
        }

        // case 4: g is LC of c and c is RC of n
        else if (isLeftChild(g, c) && isRightChild(c, n)) {
            rotateRight(c, g);
            rotateLeft(n, g);

            updateHeight(c);
            updateHeight(n);
            updateHeight(g);

            removeFix(g->getParent());
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::updateHeight(AVLNode<Key, Value>* n) {

    if (n == nullptr) {
        return;
    }

    if (n->getRight() != NULL && n->getLeft() != NULL) {
        n->setHeight(std::max(n->getRight()->getHeight(), n->getLeft()->getHeight()) + 1);
    } else if (n->getRight() != NULL) {
        n->setHeight(n->getRight()->getHeight() + 1);
    } else if (n->getLeft() != NULL) {
        n->setHeight(n->getLeft()->getHeight() + 1);
    } else {
        n->setHeight(1);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p) {
    if (g != this->root_) {
        p->setParent(g->getParent());

        // if g is a right child
        if (g->getParent()->getRight() == g) {
            g->getParent()->setRight(p);
        }

        else {
            g->getParent()->setLeft(p);
        }
        g->setRight(p->getLeft());  // flip this?
        if (rightChildExists(g)) {
            g->getRight()->setParent(g);
        }
        g->setParent(p);
        p->setLeft(g);
    }
    // if g is the root
    else {
        p->setParent(nullptr);
        g->setRight(p->getLeft());
        if (rightChildExists(g)) {
            g->getRight()->setParent(g);
        }
        g->setParent(p);
        p->setLeft(g);
        this->root_ = p;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p) {
    if (g != this->root_) {
        p->setParent(g->getParent());
        // if g is a right child
        if (g->getParent()->getRight() == g) {
            g->getParent()->setRight(p);
        } else {
            g->getParent()->setLeft(p);
        }

        g->setLeft(p->getRight());
        if (leftChildExists(g)) {
            g->getLeft()->setParent(g);
        }
        g->setParent(p);
        p->setRight(g);
    }
    // if g is the root
    else {
        p->setParent(nullptr);
        g->setLeft(p->getRight());
        if (leftChildExists(g)) {
            g->getLeft()->setParent(g);
        }
        g->setParent(p);
        p->setRight(g);
        this->root_ = p;
    }
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p) {
    return p->getLeft() == n;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::leftChildExists(AVLNode<Key, Value>* n) {
    return n->getLeft() != nullptr;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::rightChildExists(AVLNode<Key, Value>* n) {
    return n->getRight() != nullptr;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p) {
    return p->getRight() == n;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif
