#pragma once

#include <QString>
#include <QStack>
#include <QQueue>

#include "../Utils/Enum.h"

#include "../defines.h"

class TraverseType : public Enum<QString> {
protected:

    TraverseType(const QString& value) : Enum<QString>(value) {
    }

public:
    static const TraverseType DEPTH_TRAVERSE;
    static const TraverseType WIDTH_TRAVERSE;

};

template<typename T> class Tree {
protected:

    struct Node {
        size_t id;
        T* data;

        size_t* referencesCounter;
        
        Node* parent;
        Node* child; // first child

        Node* next;
        Node* prev;

        Node(size_t id, Node* parent, size_t* referencesCounter = new size_t(1)) {
            this->id = id;
            this->data = null;
            
            this->referencesCounter = referencesCounter;
            
            this->parent = parent;
            this->child = null;

            this->next = null;
            this->prev = null;
        }

        Node(T* data, size_t id, Node* parent) {
            this->id = id;
            this->data = data;
            
            this->referencesCounter = parent->referencesCounter;

            this->parent = parent;
            this->child = null;

            this->next = null;
            this->prev = null;
        }

        ~Node() {
            if (data) delete data;

            if (parent && parent->child == this) parent->child = next ? next : prev;

            if (next) next->prev = this->prev;
            if (prev) prev->next = this->next;
        }

        void set(Node * that) {
            this->id = that->id;
            this->data = that->data;

            this->parent = that->parent;
            this->child = that->child;

            this->next = that->next;
            this->prev = that->prev;
        }

    };

    Node* root;
    mutable Node* iterator;

    Node* getNode(size_t index) const {
        if (!iterator) {
            if (!root->child) {
                root->child = new Node(0, root, root->referencesCounter);
            }

            iterator = root->child;
        }

        if (iterator->id < index) {
            while (true) {
                if (!iterator->next || (iterator->id < index && iterator->next->id > index) || iterator->id == index) break;

                iterator = iterator->next;
            }
        } else if (iterator->id > index) {
            while (true) {
                if (!iterator->prev || (iterator->id > index && iterator->prev->id < index) || iterator->id == index) break;

                iterator = iterator->prev;
            }
        }

        if (iterator->id != index) {
            Node* node = new Node(index, root, root->referencesCounter);

            if (iterator->id < index) {
                node->prev = iterator;
                node->next = iterator->next;
                iterator->next = node;

                iterator = iterator->next;
            } else {
                node->prev = iterator->prev;
                node->next = iterator;
                iterator->prev = node;

                iterator = iterator->prev;
            }
        }

        return iterator;
    }

    Tree(Node* root) {
        this->root = root;
        this->iterator = root ? root->child : null;
        
        (*this->root->referencesCounter)++;
    }

public:

    enum TreeGetter {
        SUPER
    };

    class OutOfBoundsExeption {
    public:

        OutOfBoundsExeption() {
        }

        virtual ~OutOfBoundsExeption() {
        }
    };

    class TraverseStoppedExeption {
    public:

        TraverseStoppedExeption() {

        }

        virtual ~TraverseStoppedExeption() {

        }

    };

    class DataProcessor {
    protected:
        void stop() const throws(TraverseStoppedExeption) {
            throw TraverseStoppedExeption();
        }

    public:
        virtual void dataProcessingStarts() const { }
        virtual void dataProcessingEnds() const { }
        virtual void processData(Tree<T>& nodeProvider) { }
        virtual void processData(const Tree<T>& nodeProvider) const { }
        virtual const TraverseType& getTraverseType() const pure;

    };

    Tree() {
        root = new Node(0, null);
        iterator = null;
    }

    Tree(const Tree& orig) {
        this->root = orig.root;
        this->iterator = orig.iterator;
        
        (*this->root->referencesCounter)++;
    }

    virtual ~Tree() { 
        (*root->referencesCounter)--;
        if(!(*root->referencesCounter)) {
            delete root->referencesCounter;
            
            dispose();
        }
    }

    inline Tree getSupertree() const throws(OutOfBoundsExeption) {
        if (!root->parent) throw OutOfBoundsExeption();

        return Tree(root->parent);
    }

    inline Tree getSubtree(size_t index) const {
        return Tree(getNode(index));
    }

    inline const T& set(const T& data) {
        if (root->data) {
            *root->data = data;
        } else {
            root->data = new T(data);
        }

        return data;
    }

    inline T& get() const throws(OutOfBoundsExeption) {
        if (!root || !root->data) {
            throw OutOfBoundsExeption();
        }

        return *root->data;
    }

    void traverse(DataProcessor& processor) {
        processor.dataProcessingStarts();

        try {
            if (processor.getTraverseType() == TraverseType::DEPTH_TRAVERSE) {
                QStack<Node*> nodesStack = QStack<Node*>();

                nodesStack.push(root);

                while (!nodesStack.empty()) {
                    Node* current = nodesStack.pop();
                    if (current->data) {
                        Tree<T> provider = Tree<T > (current);
                        processor.processData(provider);
                    }

                    if (current->next) {
                        nodesStack.push(current->next);
                    }

                    if (current->child) {
                        nodesStack.push(current->child);
                    }
                }
            } else if (processor.getTraverseType() == TraverseType::WIDTH_TRAVERSE) {
                QQueue<Node*> nodesQueue = QQueue<Node*>();
                nodesQueue.push_back(root);

                while (!nodesQueue.empty()) {
                    Node* current = nodesQueue.front();
                    nodesQueue.pop_front();

                    Node* firstChild = current->child;

                    if (current->data) {
                        Tree<T> provider = Tree<T > (current);
                        processor.processData(provider);
                    }

                    if (firstChild) {
                        current = firstChild;
                        nodesQueue.push_back(current);

                        while (current->next) {
                            nodesQueue.push_back(current->next);
                            current = current->next;
                        }
                    }
                }
            }
        } catch (TraverseStoppedExeption) {
            // All is ok;
        }

        processor.dataProcessingEnds();
    }

    void traverse(const DataProcessor& processor) const {
        processor.dataProcessingStarts();

        try {
            if (processor.getTraverseType() == TraverseType::DEPTH_TRAVERSE) {
                QStack<Node*> nodesStack = QStack<Node*>();

                nodesStack.push(root);

                while (!nodesStack.empty()) {
                    Node* current = nodesStack.pop();
                    if (current->data) {
                        Tree<T> provider = Tree<T > (current);
                        processor.processData(provider);
                    }

                    if (current->next) {
                        nodesStack.push(current->next);
                    }

                    if (current->child) {
                        nodesStack.push(current->child);
                    }
                }
            } else if (processor.getTraverseType() == TraverseType::WIDTH_TRAVERSE) {
                QQueue<Node*> nodesQueue = QQueue<Node*>();
                nodesQueue.push_back(root);

                while (!nodesQueue.empty()) {
                    Node* current = nodesQueue.front();
                    nodesQueue.pop_front();

                    Node* firstChild = current->child;

                    if (current->data) {
                        Tree<T> provider = Tree<T > (current);
                        processor.processData(provider);
                    }

                    if (firstChild) {
                        current = firstChild;
                        nodesQueue.push_back(current);

                        while (current->next) {
                            nodesQueue.push_back(current->next);
                            current = current->next;
                        }
                    }
                }
            }
        } catch (TraverseStoppedExeption) {
            // All is ok;
        }

        processor.dataProcessingEnds();
    }

    Tree clone() const {
        Tree<T> newTree = Tree<T > ();
        newTree.root = new Node(root->data ? new T(*root->data) : null, 0, null);

        QQueue<Node*> nodesQueue = QQueue<Node*>();
        QQueue<Node*> newNodesQueue = QQueue<Node*>();

        nodesQueue.push_back(root);
        newNodesQueue.push_back(newTree.root);

        while (!nodesQueue.empty()) {
            Node* current = nodesQueue.front();
            Node* newCurrent = newNodesQueue.front();

            nodesQueue.pop_front();
            newNodesQueue.pop_front();

            Node* firstChild = current->child;
            if (!firstChild) continue;
            current = firstChild;
            nodesQueue.push_back(current);

            newCurrent->child = new Node(current->data ? new T(*current->data) : null, current->id, newCurrent);
            newCurrent = newCurrent->child;

            newNodesQueue.push_back(newCurrent);
            while (current->next) {
                current = current->next;
                nodesQueue.push_back(current);

                newCurrent->next = new Node(current->data ? new T(*current->data) : null, current->id, newCurrent);
                newCurrent = newCurrent->next;
                newNodesQueue.push_back(newCurrent);
            }
        }

        return newTree;
    }

    template<typename> friend class Tree;

    template<typename ResultType> Tree<ResultType> to(ResultType(*transform)(const T&)) const {
        Tree<ResultType> newTree = Tree<ResultType > ();
        newTree.root->data = new ResultType(transform(*root->data));

        QQueue<typename Tree<T>::Node*> nodesQueue = QQueue<typename Tree<T>::Node*>();
        QQueue<typename Tree<ResultType>::Node*> newNodesQueue = QQueue<typename Tree<ResultType>::Node*>();

        nodesQueue.push_back(root);
        newNodesQueue.push_back(newTree.root);

        while (!nodesQueue.empty()) {
            typename Tree<T>::Node* current = nodesQueue.front();
            typename Tree<ResultType>::Node* newCurrent = newNodesQueue.front();

            nodesQueue.pop_front();
            newNodesQueue.pop_front();

            Node* firstChild = current->child;
            if (!firstChild) continue;
            current = firstChild;
            nodesQueue.push_back(current);

            newCurrent->child = new typename Tree<ResultType>::Node(current->data ? new ResultType(transform(*current->data)) : null, current->id, newCurrent);
            newCurrent = newCurrent->child;

            newNodesQueue.push_back(newCurrent);
            while (current->next) {
                current = current->next;
                nodesQueue.push_back(current);

                newCurrent->next = new typename Tree<ResultType>::Node(current->data ? new ResultType(transform(*current->data)) : null, current->id, newCurrent);
                newCurrent = newCurrent->next;
                newNodesQueue.push_back(newCurrent);
            }
        }

        return newTree;
    }

    void clear() {
        delete root->data;
        root->data = null;
    }

    void dispose() {
        QStack<Node*> nodesStack = QStack<Node*>();

        nodesStack.push(root);

        while (!nodesStack.empty()) {
            Node* current = nodesStack.pop();

            if (current->next) {
                nodesStack.push(current->next);
            }
            if (current->child) {
                nodesStack.push(current->child);
            }

            delete current;
        }
    }

    inline bool equals(const Tree<T>& that) const {
        return this->root && that->root && this->root->data == that.root->data;
    }

    inline bool isEmpty() const {
        return !this->root || !this->root->data;
    }

    inline bool isRoot() const {
        return !root->parent;
    }

    inline bool isLeaf() const {
        if (!root->child) {
            return true;
        }

        Node* current = root->child;
        while (current) {
            if (current->data) return false;

            current = current->next;
        }

        return true;
    }

    inline Tree operator[](size_t index) const {
        return getSubtree(index);
    }

    inline Tree operator[](TreeGetter) const {
        return getSupertree();
    }

    inline bool operator==(const Tree<T> other) const {
        return equals(other);
    }

    inline const Tree<T>& operator=(const Tree<T>& data) {
        root = data.root;
        iterator = data.iterator;

        return *this;
    }

    inline const T& operator=(const T& data) {
        return set(data);
    }

    inline operator T& () throws(OutOfBoundsExeption) {
        return get();
    }

    inline operator const T& () const throws(OutOfBoundsExeption) {
        return get();
    }

};