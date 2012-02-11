#pragma once

#include <QString>
#include <QStack>
#include <QQueue>

#include "Enum.h"

#define null NULL
#define pure = 0
#define throws throw

#include <iostream>
#define cout std::cout

using namespace std;

class TraverseType : public Enum<QString> {
private:

    TraverseType(const QString& value) : Enum<QString>(value) {
    }

public:
    static const TraverseType DEPTH_TRAVERSE;
    static const TraverseType WIDTH_TRAVERSE;
};

template<typename T>class Tree {
public:
    typedef T DataType;

    class IDataProcessor {
    public:
        virtual void dataProcessingStarts() pure;
        virtual void dataProcessingEnds() pure;
        virtual void processData(DataType& nodeData) pure;
        virtual void processData(const DataType& nodeData) const pure;
        virtual const TraverseType& getTraverseType() const pure;
    };

    class OutOfBoundsExeption {
    private:
        QString message;
    public:

        OutOfBoundsExeption() {
            this->message = "";
        }

        OutOfBoundsExeption(const QString& message) {
            this->message = message;
        }

        OutOfBoundsExeption(const OutOfBoundsExeption& that) {
            this->message = that.message;
        }

        virtual ~OutOfBoundsExeption() {
        }
    };

protected:

    class Node {
    public:
        T data;
        bool free;
        size_t id;

        Node* parent;
        Node* nextBrother;
        Node* prevBrother;
        Node* child;

        Node(size_t id, Node* parent, bool free = false) {
            this->id = id;
            this->free = free;

            this->parent = parent;
            this->nextBrother = null;
            this->prevBrother = null;
            this->child = null;
        }

        Node(const T& data, size_t id, Node* parent, bool free = false) {
            this->id = id;
            this->data = data;
            this->free = free;

            this->parent = parent;
            this->nextBrother = null;
            this->prevBrother = null;
            this->child = null;
        }

        Node(const Node& orig) {
            this->data = orig.data;
            this->id = orig.id;
            this->free = orig.free;

            this->parent = orig.parent;
            this->nextBrother = orig.nextBrother;
            this->prevBrother = orig.prevBrother;
            this->child = orig.child;
        }

        ~Node() {
            if (parent && parent->child == this) parent->child = nextBrother ? nextBrother : prevBrother;

            if (prevBrother) prevBrother->nextBrother = this->nextBrother;
            if (nextBrother) nextBrother->prevBrother = this->prevBrother;
        }

        inline Node* getFirstChild() {
            while (child && child->prevBrother) {
                child = child->prevBrother;
            }

            return child;
        }

        inline void setFirstChild(Node* node) {
            while (child && child->prevBrother) {
                child = child->prevBrother;
            }

            child = node;
        }

        inline void setIteratorToDefault() {
            child = new Node(0, parent, true);
        }
    };

    Node* root;

    Node* getNode(size_t index) const {
        if (!root->child) root->setIteratorToDefault();

        if (root->child->id < index) {
            while (true) {
                if (!root->child->nextBrother || (root->child->id < index && root->child->nextBrother->id > index) || root->child->id == index) break;

                root->child = root->child->nextBrother;
            }
        } else if (root->child->id > index) {
            while (true) {
                if (!root->child->prevBrother || (root->child->id > index && root->child->prevBrother->id < index) || root->child->id == index) break;

                root->child = root->child->prevBrother;
            }
        }

        if (root->child->id != index) {
            Node* node = new Node(index, root, true);

            if (root->child->id < index) {
                node->prevBrother = root->child;
                node->nextBrother = root->child->nextBrother;
                root->child->nextBrother = node;

                root->child = root->child->nextBrother;
            } else {
                node->prevBrother = root->child->prevBrother;
                node->nextBrother = root->child;
                root->child->prevBrother = node;

                root->child = root->child->prevBrother;
            }
        }

        return root->child;
    }

    Tree(Node* root) {
        this->root = root;
    }

public:

    Tree() {
        root = new Node(0, null, true);
    }

    Tree(const Tree& orig) {
        this->root = orig.root;
    }

    virtual ~Tree() {

    }

    inline Tree getSubTree(size_t index) const {
        return Tree(getNode(index));
    }

    inline const T& set(const T& data) {
        this->root->data = data;
        this->root->free = false;

        return data;
    }

    inline T& get() const throws(OutOfBoundsExeption) {
        if (root->free) {
            throw OutOfBoundsExeption();
        }

        return root->data;
    }

    void walk(IDataProcessor& processor) {
        processor.dataProcessingStarts();

        if (processor.getTraverseType() == TraverseType::DEPTH_TRAVERSE) {
            QStack<Node*> nodesStack = QStack<Node*>();

            nodesStack.push(root);

            while (!nodesStack.empty()) {
                Node* current = nodesStack.pop();
                Node* firstChild = current->getFirstChild();

                if (!current->free) processor.processData(current->data);

                if (current->nextBrother) {
                    nodesStack.push(current->nextBrother);
                }

                if (firstChild) {
                    nodesStack.push(firstChild);
                }
            }
        } else if (processor.getTraverseType() == TraverseType::WIDTH_TRAVERSE) {
            QQueue<Node*> nodesQueue = QQueue<Node*>();
            nodesQueue.push_back(root);

            while (!nodesQueue.empty()) {
                Node* current = nodesQueue.pop_front();
                Node* firstChild = current->getFirstChild();

                if (!current->free) processor.processData(current->data);

                if (firstChild) {
                    current = firstChild;
                    nodesQueue.push_back(current);

                    while (current->nextBrother) {
                        nodesQueue.push_back(current->nextBrother);
                        current = current->nextBrother;
                    }
                }
            }
        }

        processor.dataProcessingEnds();
    }

    void dispose() {
        QStack<Node*> nodesStack = QStack<Node*>();

        nodesStack.push(root);

        while (!nodesStack.empty()) {
            Node* current = nodesStack.pop();
            Node* firstChild = current->getFirstChild();

            if (current->nextBrother) {
                nodesStack.push(current->nextBrother);
            }

            if (firstChild) {
                nodesStack.push(firstChild);
            }

            // @todo нормальное удаление
            /*
             * Не работает, скорее всего потому, что ноды не удаляются.
             */
            // delete current 

            /*
             * Работает, т.к. помечает ноды как пустые
             */
            current->free = true; // работает
        }
    }

    Tree clone() const {
        Tree newTree = Tree();
        newTree.root = new Node(root->data, 0, null);

        QQueue<Node*> nodesQueue = QQueue<Node*>();
        QQueue<Node*> newNodesQueue = QQueue<Node*>();

        nodesQueue.push_back(root);
        newNodesQueue.push_back(newTree.root);

        while (!nodesQueue.empty()) {
            Node* current = nodesQueue.front();
            Node* newCurrent = newNodesQueue.front();

            nodesQueue.pop_front();
            newNodesQueue.pop_front();

            Node* firstChild = current->getFirstChild();
            if (!firstChild) continue;
            current = firstChild;
            nodesQueue.push_back(current);

            newCurrent->setFirstChild(new Node(current->data, current->id, newCurrent, current->free));
            newCurrent = newCurrent->getFirstChild();
            newNodesQueue.push_back(newCurrent);
            while (current->nextBrother) {
                current = current->nextBrother;
                nodesQueue.push_back(current);

                newCurrent->nextBrother = new Node(current->data, current->id, newCurrent, current->free);
                newCurrent = newCurrent->nextBrother;
                newNodesQueue.push_back(newCurrent);
            }
        }

        return newTree;
    }

    inline bool equals(const Tree<T>& that) const {
        return this->root && that->root && this->root->data == that.root->data;
    }

    inline Tree operator[](unsigned int index) const {
        return getSubTree(index);
    }

    inline bool operator==(const Tree<T> other) const {
        return equals(other);
    }

    inline const Tree<T> & operator=(const Tree<T>& data) {
        this->root = data.root;
        return *this;
    }

    inline const T& operator=(const T& data) {
        return set(data);
    }

    inline operator T&() const throws(OutOfBoundsExeption) {
        return get();
    }

};