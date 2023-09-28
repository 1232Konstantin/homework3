#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <memory>



template <class T>
struct My_allocator {
    using value_type = T;
    static constexpr size_t defaultSize=11;
    std::shared_ptr<T> memory_bank;
    size_t next_free;
    size_t current_size=0;

    My_allocator() noexcept
    {

    }

    My_allocator (My_allocator<T>& other) noexcept
    {
        memory_bank=other.memory_bank;
        next_free=other.next_free;
        current_size=other.current_size;
    }

    // template< class Other>
    // My_allocator (My_allocator<Other>& other) noexcept
    // {
    //     if (sizeof (Other)==sizeof (T))
    //     {
    //             memory_bank=std::reinterpret_pointer_cast<T,Other>(other.memory_bank);
    //             next_free=other.next_free;
    //             current_size=other.current_size;
    //     }

    // }

    My_allocator (My_allocator<T>&& other) noexcept
    {
        memory_bank=other.memory_bank;
        next_free=other.next_free;
        current_size=other.current_size;
        other.memory_bank.reset();
        other.next_free=0;
        other.current_size=defaultSize;
    }

    void operator=(const My_allocator<T>& other)
    {
        memory_bank=other.memory_bank;
        next_free=other.next_free;
        current_size=other.current_size;
    }

    T* allocate (std::size_t n)
    {

        if (current_size==0)
        {
            auto temp=(defaultSize<n)? n: defaultSize;
            current_size=temp;
            memory_bank.reset(static_cast<T*>(::operator new(temp*sizeof(T))));
            next_free=0;
        }

        if (next_free+n<=current_size)
        {
            auto temp=memory_bank.get()+next_free;
            next_free+=n;
            return (temp);
        }

        throw  std::bad_alloc();
        return nullptr;
    }
    void deallocate (T* p, std::size_t n)
    {
        auto temp=(p-memory_bank.get())/sizeof (T);
        if (next_free-temp<=n) next_free-=n;
    }


    My_allocator select_on_container_copy_construction() const
       {
           //std::cout << "My_allocator::select_on_container_copy_construction()" << std::endl;
           return My_allocator(*this);
       }

    template< class U >
       struct rebind
       {
           typedef My_allocator<U> other;
       };
       using propagate_on_container_copy_assignment = std::true_type;
       using propagate_on_container_move_assignment = std::true_type;
       using propagate_on_container_swap = std::true_type;
};

template <class T, class U>
bool operator== (const My_allocator<T>& a1, const My_allocator<U>& a2) noexcept
{
    if (a1.memory_bank.get()==a2.memory_bank.get())
    return true;
    else return false;
}

template <class T, class U>
bool operator!= (const My_allocator<T>& a1, const My_allocator<U>& a2) noexcept
{
    if (a1.memory_bank.get()==a2.memory_bank.get())
    return false;
    else return true;
}




#endif // ALLOCATOR_H
