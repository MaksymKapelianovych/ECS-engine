#pragma once

#include <vector>

template <typename T>
using Array = std::vector<T>;

template <class E, class CharT> 
std::basic_ostream<CharT> & operator<<(std::basic_ostream<CharT> &os, const std::vector<E> &v)
{
    os << '('; 
    auto iter = v.begin(); 
    auto end  = v.end(); 
    if (iter != end) { 
        os << *iter; 
        while (++iter != end) { 
            os << ',' << *iter; 
        } 
    } 
    return os << ')';
}

namespace utils
{
    template<typename ContainerType>
    void swap_remove(ContainerType &container, size_t index)
        {
            //Don't swap the back element with itself, and also don't swap out of range.
            /*
                The (index+1) is to prevent the back element from swapping with itself.
        
                This could be more clearly written as:	if(index >= (container.size()-1))
                ...but since 'container.size()' is unsigned,
                the -1 would accidentally loop it around if size() returns 0, which is a likely occurance.
            */
            if((index+1) >= container.size())
                return;

            //Swap the element with the back element.
            std::swap(container[index], container.back());

            //Pop the back of the container, deleting our old element.
            container.pop_back();
        }
}

