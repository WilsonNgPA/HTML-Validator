// Ng, Pei Aou Wilson 100332292. Instructor: Bita Shadgar

#ifndef STACK_H
#define STACK_H

template<typename T>
class stack {
    public:
        // Default constructor
        stack();
        // Copy constructor
        stack(const stack<T> &s);
        // Destructor
        ~stack();
        // Assignment operator
        const stack<T>& operator=(const stack<T> &s);
        // Check if stack is empty
        bool isEmpty();
        // Push x into stack
        void push(T x);
        // Return top element from stack
        T peek();
        // Remove top element from stack and return it
        T pop();
    private:
        // node struct
        struct node {
            T data;
            node* next;
        };
        // Top node of stack
        node* top;
        // Copy stack helper function
        node* copyStack(node* p);
        // Delete stack helper function
        node* deleteStack(node* p);
};

template<typename T>
stack<T>::stack() {
    top = nullptr;
}

template<typename T>
stack<T>::stack(const stack<T> &s) {
    top = copyStack(s.top);

}

template<typename T>
stack<T>::~stack() {
    top = deleteStack(top);
}

template<typename T>
const stack<T>& stack<T>::operator=(const stack<T> &s) {
    if(&s == this) {
        return *this;
    }
    top = deleteStack(top);
    top = copyStack(s.top);
    return *this;
}

template<typename T>
bool stack<T>::isEmpty() {
    return !top;
}

template<typename T>
void stack<T>::push(T x) {
    top = new node{x, top};
}

template<typename T>
T stack<T>::peek() {
    if(isEmpty()) {
        std::cerr << "Empty, cannot peek" << std::endl;
        exit(1);
    }
    return top -> data;
}

template<typename T>
T stack<T>::pop() {
    T result = peek();
    node* temp = top -> next;
    delete top;
    top = temp;
    return result;
}

template<typename T>
typename stack<T>::node* stack<T>::copyStack(node* p) {
    if(!p) {
        return p;
    }
    return new node{p -> data, copyStack(p -> next)};
}

template<typename T>
typename stack<T>::node* stack<T>::deleteStack(node* p) {
    if(p) {
        deleteStack(p -> next);
        delete p;
    }
    return nullptr;
}

#endif