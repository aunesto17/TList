//
//  main.cpp
//  AED_ListaT
//
//  Created by Alexander Arturo Baylon Ibanez on 10/10/15.
//  Copyright © 2015 aunesto17. All rights reserved.
//

#include <iostream>
#include <thread>
#include <vector>
#include <random>

using namespace std;

#define NUM_THREADS std::thread::hardware_concurrency();

template <class T>
struct cNode {
public:
    cNode<T> * m_next;
    cNode<T> * m_prev;
    T m_data;
    cNode(T x)
    {
        m_data = x;
        m_next = m_prev = nullptr;
    }
};


template <class T>
class listaT {
    char letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',};
public:
    cNode<T> * m_head;
    cNode<T> * m_tail;
    listaT();
    bool push_front(T);
    bool push_back(T);
    T pop_front();
    T pop_back();
    bool find(T,cNode<T>**&);
    void printList();
    ~listaT(){};
};

vector<int> crear_vector_random(int);

void push_front_parallel(vector<int> & vec,listaT<int> * l,int ini,int fin)
{
    for (int i=ini;i<fin ; i++)
        l->push_back(vec[i]);
}

int main(int argc, const char * argv[]) {
    listaT<int> prueba;
    vector<int> vec = crear_vector_random(1000);
    //////////////////////////
    int numT    = NUM_THREADS;
    int tamPart = vec.size()/numT;
    thread * th = new thread[numT];

    for (int i=0; i<numT; i++) {
        th[i] = thread(push_front_parallel, vec, &prueba, i*tamPart, (i+1)*tamPart);
    }
    for (int j=0; j<numT; j++) {
        th[j].join();
    }
    return 0;
}

template<class T>
void listaT<T>::printList()
{
    cNode<T> * temp = m_head;
    int i = 0;
    cout << "-> ";
    while (temp!=nullptr) {
        cout << letras[i++] << "(" << temp->m_data << ")" << " -> ";
        temp = temp->m_next;
    }
    cout << "end" << endl;
}


template<class T>
listaT<T>::listaT()
{
    m_head = m_tail = nullptr;
}

template<class T>
bool listaT<T>::push_front(T x)
{
    cNode<T> ** p = &m_head;
    cNode<T> * t  = new cNode<T>(x);
    t->m_next     = *p;
    if (!*p) m_head = m_tail = t;
    else
    {
        (*p)->m_prev    = t;
        *p              = t;
    }
    return 1;
}


template<class T>
bool listaT<T>::push_back(T x)
{
    cNode<T> ** p   = &m_tail;
    cNode<T> * t    = new cNode<T>(x);
    t->m_prev       = *p;
    if (!*p) m_head = m_tail = t;
    else
    {
        (*p)->m_next    = t;
        *p              = t;
    }
    return 1;
}

template<class T>
T listaT<T>::pop_front()
{
    cNode<T> ** p   = &(m_head->m_next);
    cNode<T> *  t   = m_head;
    T temp          = t->m_data;
    if (!(*p)) m_head = m_tail = nullptr;
    else{
        m_head          = *p;
        m_head->m_prev  = nullptr;
    }
    delete t;
    return temp;
}


template<class T>
T listaT<T>::pop_back()
{
    //p apunta al puntero  que apunta al nodo anterior a la cola
    cNode<T> ** p     = &(m_tail->m_prev);
    cNode<T> *  t     = m_tail;
    T temp            = t->m_data;
    //Si solo habia un nodo
    if (!(*p)) m_head = m_tail = nullptr;
    else{
        //la cola es igual al nodo anterior a la cola
        m_tail        = *p;
        //la nueva cola apunta a null
        m_tail->m_next= nullptr;
    }
    delete t;
    return temp;
}


vector<int> crear_vector_random(int n) {
    auto sem  = random_device()();
    auto gen  = mt19937(sem);
    auto dist = uniform_int_distribution<int>();
    auto v    = vector<int>(n);
    generate(begin(v), end(v), bind(dist, gen));
    return v;
}


