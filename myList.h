#ifndef MYLIST_H
#define MYLIST_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include "allocator.h"

template <class, class Alloc, class ...Args>
struct has_construct : std::false_type { };

template <class Alloc, class ...Args>
struct has_construct<decltype(
    (void)std::declval<Alloc>().construct(std::declval<Args>()...)
), Alloc, Args...> : std::true_type { };



template <class Alloc>
struct notstd_allocator_traits
{
    using pointer = typename Alloc::value_type*;
    using const_pointer = const typename Alloc::value_type*;
    //...other typedefs

    static pointer allocate(Alloc& a, std::size_t n) {
        return a.allocate(n);
    }

    static void deallocate(Alloc& a, pointer p, std::size_t n) noexcept{
        a.deallocate(p, n);
    }

    template <class Tp, class... Args, class = std::enable_if<has_construct<Alloc, Tp*, Args...>::value> >
    static void construct(Alloc& a, Tp* p, Args&&... args) {
        a.construct(p, std::forward<Args>(args)...);
    }

    template <class Tp, class... Args, class = void, class = std::enable_if<!has_construct<Alloc, Tp*, Args...>::value> >
    static void construct(Alloc&, Tp* p, Args&&... args) {
        ::new ((void*)p) Tp(std::forward<Args>(args)...);
    }
};

template <typename T>
struct Node
{
    Node* next=nullptr;
    Node* previous=nullptr;
    T val;
};

template <typename T>
struct MyIterator
{
    Node<T>* node=nullptr;

    MyIterator(){}
    MyIterator(Node<T>* ptr): node(ptr){}
    MyIterator(const MyIterator& other) {node=other.node; }
    MyIterator(MyIterator&& other) {node=other.node; other.node=nullptr;}
    MyIterator& operator++()
    {
        if (node) {
            node=node->next; return (*this);  }
        else throw std::out_of_range("bad iteration");
    }
    MyIterator& operator--()
    {
        if (node) {
            node=node->previous; return (*this);  }
        else throw std::out_of_range("bad iteration");
    }
    MyIterator& operator++(int){return operator++();}
    MyIterator& operator--(int){return operator--();}
    void operator=(const MyIterator& other){node=other.node;}
    void operator=(MyIterator&& other){node=other.node;other.node=nullptr;}
    bool operator==(const MyIterator& other){return (node==other.node)? true : false;}
    bool operator!=(const MyIterator& other){return (node==other.node)? false : true;}
    MyIterator operator+(size_t i){
        MyIterator temp=*this;
        for (size_t j=0; j<i; j++) {temp.operator++(); return temp;}
    }
    MyIterator operator-(size_t i){
        MyIterator temp=*this;
        for (size_t j=0; j<i; j++) {temp.operator--(); return temp;}
    }
    bool isValid(){return (node)? true : false; }
    T& operator*(){ if (node) return node->val; else throw std::out_of_range("bad iteration");}

};



//template <typename T, typename Alloc=My_allocator<int>>
template <typename T, typename Alloc=std::allocator<T>>
class MyList
{

    using NodeAlloc=typename Alloc::template rebind<Node<T>>::other;
    MyIterator<T> m_interface_it; //Для выдачи потребителям



    template <typename U>
    void plus(MyList<T,U>& other)
    {
        for (auto it=other.begin(); it!=other.end(); it++)
        {
            push_back(*it);
        }
    }

    void reset()
    {
        m_alloc= NodeAlloc();
        m_tail=notstd_allocator_traits<NodeAlloc>::allocate(m_alloc,1);
        m_tail->previous=nullptr;
        m_head=nullptr;
        m_size=0;
    }

public:

    MyList()
    {
        //m_tail=new Node<T>();
        m_tail=notstd_allocator_traits<NodeAlloc>::allocate(m_alloc,1);
        m_tail->previous=nullptr;
    }

//    ~MyList()
//    {
//        delete m_tail;
//    }

    MyList(std::initializer_list<T> init_list)
    {
        //m_tail=new Node<T>();
        m_tail=notstd_allocator_traits<NodeAlloc>::allocate(m_alloc,1);
        m_tail->previous=nullptr;
        for (T x: init_list) push_back(x);

    }

    template <typename U>
    MyList(const MyList<T,U>& other)
    {
        //m_tail=new Node<T>();
        m_tail=notstd_allocator_traits<NodeAlloc>::allocate(m_alloc,1);
        m_tail->previous=nullptr;
        plus(other);
    }

    MyList(const MyList<T,Alloc>& other)
    {
        m_head=other.head;
        m_tail=other.tail;
        m_alloc=other.m_alloc;
        m_size=other.m_size;
    }

    MyList(MyList<T,Alloc>&& other)
    {

        m_alloc=other.m_alloc;
        m_head=other.m_head;
        m_tail=other.m_tail;
        m_size=other.m_size;
        other.reset();
    }



    MyIterator<T>& begin()
    {
        m_interface_it=MyIterator<T>(m_head);
        return m_interface_it;
    }

    MyIterator<T>& end()
    {
        m_interface_it=MyIterator<T>(m_tail);
        return m_interface_it;
    }


    void push_back(const T& val)
    {
        Node<T>* newNode = notstd_allocator_traits<NodeAlloc>::allocate(m_alloc,1);
        if (!newNode) throw  std::bad_alloc();
        newNode->val=val;
        newNode->next=m_tail;
        if (m_tail->previous)
        {
            m_tail->previous->next=newNode;
            newNode->previous=m_tail->previous;
        }
        else m_head=newNode;
        m_tail->previous=newNode;
        m_size++;

        //std::cout<<"push_back"<<m_size<<" znach"<<m_tail->previous->val<<" addr"<<newNode<<" "<<m_size<<std::endl;
    }

    void pop_back()
    {
        if (!m_tail->previous) return;
        auto temp=m_tail->previous;
        m_tail->previous=temp->previous;

        if (temp->previous) temp->previous->next=m_tail;
        else m_head=nullptr;
        notstd_allocator_traits<NodeAlloc>::deallocate(m_alloc,temp,1);
        m_size--;
        if (m_size==0) m_tail->previous=nullptr;
    }

    void push_front(const T& val)
    {
        Node<T>* newNode = notstd_allocator_traits<NodeAlloc>::allocate(m_alloc,1);
        if (!newNode) throw  std::bad_alloc();
        m_size++;
        newNode->val=val;
        if (m_head)
        {
            m_head->previous=newNode;
            newNode->next=m_head;
        }
        else
        {
            m_tail->previous=newNode;
            newNode->next=m_tail;
        }
        m_head=newNode;

    }

    void pop_front()
    {
        auto temp=m_head;
        if (m_head->next!=m_tail) m_head=m_head->next;
        else m_head=nullptr;

        notstd_allocator_traits<NodeAlloc>::deallocate(m_alloc,temp,1);
        m_size--;
        if (m_size==0) m_tail->previous=nullptr;
    }

    T& front()
    {
        if (m_head) return m_head->val;
        else throw std::out_of_range("try use empty list");
    }

    T& back()
    {
        if (m_tail->previous) return m_tail->previous->val;
        else throw std::out_of_range("try use empty list");
    }

    std::pair<bool, MyIterator<T>&> insert(const T& val, MyIterator<T>& it)
    {
        m_interface_it=MyIterator<T>();
        if (!it.isValid() )  return std::pair<bool, MyIterator<T>&>(false, m_interface_it);
        if (it.node==m_tail) return std::pair<bool, MyIterator<T>&>(false, m_interface_it);
         Node<T>* newNode = notstd_allocator_traits<NodeAlloc>::allocate(m_alloc,1);
         if (!newNode) return std::pair<bool, MyIterator<T>&>(false, m_interface_it);
         newNode->val=val;
         if (it.node->previous)
         {
             newNode->previous=it.node->previous;
             newNode->previous->next=newNode;
         }
         else
         {
             m_head=newNode;
         }
         newNode->next=it.node;
         it.node->previous=newNode;

         m_size++;
         m_interface_it=MyIterator<T>(newNode);
         return std::pair<bool, MyIterator<T>&>(true, m_interface_it);

    }

    void erase(MyIterator<T>& it)
    {
        if (!it.isValid() || (it.node==m_tail)) return;
        auto temp=it.node;

        if (it.node->previous)
        {
            it.node->previous->next=it.node->next;
        }
        else
        {
            if (it.node->next!=m_tail) m_head=it.node->next;
            else m_head=nullptr;
        }
        it.node->next->previous=it.node->previous;
        notstd_allocator_traits<NodeAlloc>::deallocate(m_alloc,temp,1);
        m_size--;
        if (m_size==0) m_tail->previous=nullptr;
    }

    void clear()
    {
        if (m_size==0) return;
        m_size=0;
        auto idx=m_head;
        while(idx!=m_tail)
        {
            auto temp=idx;
            idx=idx->next;
            notstd_allocator_traits<NodeAlloc>::deallocate(m_alloc,temp,1);
        }
        m_head=nullptr;
        m_tail->previous=nullptr;
    }

    MyIterator<T>& find(const T val)
    {
        for (auto it=begin(); it!=end(); it++)
        {
            if (*it==val)
            {
                m_interface_it=it;
                return m_interface_it;
            }
        }
        m_interface_it.node=nullptr;
        return m_interface_it;
    }

    T& at(size_t index)
    {
        if (index<size())
            return *(begin()+index);
        else throw std::out_of_range("index out of range");
    }

    template <typename U>
    void operator=(MyList<T,U>& other)
    {
        clear();
        if (other.size()==0) return;
        plus(other);
    }

    void operator=(MyList<T,Alloc>& other)
    {
        clear();
        if (other.size()==0) return;
        plus(other);
    }


    template <typename U>
    bool operator==(MyList<T,U>& other)
    {
        if (size()!=other.size()) return false;
        if (size()==0) return true;

        for (size_t i=0; i<size(); i++)
        {
            if (*(begin()+i)!=*(other.begin()+i)) return false;
        }
        return true;
    }

    template <typename U>
    bool operator!=(MyList<T,U>& other)
    {
       return !operator==(other);
    }

    template <typename U>
    MyList<T,Alloc> operator+(MyList<T,U>& other)
    {
        MyList<T,Alloc> temp;
        for (auto it=begin(); it!=end(); it++)
        {
            temp.push_back(*it);
        }
        for (auto it=other.begin(); it!=other.end(); it++)
        {
            temp.push_back(*it);
        }
        return temp;
    }

    bool isEmpty()
    {
        return (m_head==nullptr)? true : false;
    }

    size_t size()
    {
        return m_size;
    }

    using iterator=MyIterator<T>;

private:
    Node<T>* m_head=nullptr;
    Node<T>* m_tail=nullptr;
    size_t m_size=0;
    NodeAlloc m_alloc;

};


#endif // MYLIST_H
