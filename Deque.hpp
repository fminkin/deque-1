#ifndef _DEQUE_
#define _DEQUE_

#include <cassert>
#include <algorithm>
#include <iterator>
#include <cstddef>

namespace NDeque
{
    template <class T>
    class Deque
    {   
        T* buffer;
        size_t sizeOfBuffer;
        size_t _begin;
        size_t _end;
        
        void reallocMore()
        {
            if (size() == sizeOfBuffer - 1)
            {
                T *newBuffer = new T[sizeOfBuffer * 2];
                for (size_t i = _begin, j = 0; i != _end; i = (i + 1) & (sizeOfBuffer - 1), ++j)
                {
                    newBuffer[j] = buffer[i];
                }
                delete[] buffer;
                buffer = newBuffer;
                _begin = 0;
                _end = sizeOfBuffer - 1;
                sizeOfBuffer *= 2;
            }
        }
        
        void reallocLess()
        {
            if (size() == sizeOfBuffer / 4 && sizeOfBuffer > 4)
            {
                size_t oldSize = size();
                T *newBuffer = new T[sizeOfBuffer / 2];
                for (size_t i = _begin, j = 0; i != _end; i = (i + 1) & (sizeOfBuffer - 1), ++j)
                {
                    newBuffer[j] = buffer[i];
                }
                delete[] buffer;
                buffer = newBuffer;
                
                _begin = 0;
                _end = oldSize;
                sizeOfBuffer /= 2;
                
            }
        }
        

        
        template<class _T> //_T = const T if constIterator
        class baseIterator : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, _T*, _T&>
        {
            typedef typename std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, _T*, _T&>::difference_type differenceType;
            
            size_t index;
            
            const Deque<T> *deque;
            
            bool belong(size_t index) const
            {
                if (deque->_begin <= deque->_end)
                    return (deque->_begin <= index && index <= deque->_end);
                else
                    return ((deque->_begin <= index && index < deque->sizeOfBuffer) || index <= deque->_end);
            }
            
        public:
            baseIterator(const baseIterator &it) : index(it.index), deque(it.deque)
            {
            }
            
            baseIterator& operator=(const baseIterator &it)
            {
                index = it.index;
                deque = it.deque;
                return *this;
            }
            
            baseIterator(size_t index, const Deque<T> *deque) : index(index), deque(deque)
            {
                assert(belong(index));
            }
            
            baseIterator& operator++()
            {
                index = (index + 1) & (deque->sizeOfBuffer - 1);
                assert(belong(index));
                return *this;
            }
            
            baseIterator operator++(int)
            {
                baseIterator tmp(*this);
                operator++(index);
                return tmp;
            }
            
            baseIterator& operator--()
            {
                index = (index ? index - 1 : deque->sizeOfBuffer - 1);
                assert(belong(index));
                return *this;
            }
            
            baseIterator operator--(int)
            {
                baseIterator tmp(*this);
                operator--();
                return tmp;
            }
            
            bool operator==(const baseIterator & it) const
            {
                return (index == it.index);
            }
            
            bool operator!=(const baseIterator &it) const
            {
                return (index != it.index);
            }
            
            bool operator<(const baseIterator &it) const
            {
                assert(belong(index) && belong(it.index));
                
                if (deque->_begin <= deque->_end)
                {
                    return (index < it.index);
                }
                else
                {
                    if ((deque->_begin <= index && deque->_begin <= it.index) || 
                        (index <= deque->_end && it.index <= deque->_end))
                    {
                            return (index < it.index);   
                    }
                    else
                    {
                        return (deque->_begin <= index);
                    }
                }
            }
            
            bool operator>(const baseIterator &it) const 
            {
                return (it > *this);
            }
            
            bool operator<=(const baseIterator &it) const 
            {
                return !operator>(it);
            }
            
            bool operator>=(const baseIterator &it) const 
            {
                return !operator<(it);
            }
            
            _T& operator*()
            {
                assert(belong(index) && index != deque->_end);
                return deque->buffer[index];
            }
            
            _T* operator->()
            {
                assert(belong(index) && index != deque->_end);
                return (deque->buffer + index);
            }
            
            baseIterator operator+(const differenceType &i) const
            {
                size_t newIndex = (index + i) & (deque->sizeOfBuffer - 1);
                assert(belong(newIndex));
                return baseIterator(newIndex, deque);
            }
            
            friend baseIterator operator+(const differenceType &i, const baseIterator &it)
            {
                return it + i;
            }
            
            baseIterator& operator+=(const differenceType &i)
            {
                index = (index + i) & (deque->sizeOfBuffer - 1);
                assert(belong(index));
                return *this;
            }
            
            baseIterator operator-(const differenceType &i) const
            {
                size_t newIndex = (index - i + deque->sizeOfBuffer) & (deque->sizeOfBuffer - 1);
                assert(belong(newIndex));
                return baseIterator(newIndex, deque);
            }
            
            differenceType operator-(const baseIterator &it) const
            {
                assert(belong(index) && belong(it.index));
                return (index - it.index + deque->sizeOfBuffer) & (deque->sizeOfBuffer - 1);
            }
        };

    public:
        typedef baseIterator<T> iterator;
        typedef baseIterator<const T> constIterator;
        Deque()
        {
            sizeOfBuffer = 2;
            _begin = 0;
            _end = 0;
            buffer = new T[sizeOfBuffer];
        }
        
        size_t size() const
        {
            return (_end - _begin + sizeOfBuffer) & (sizeOfBuffer - 1);
        }
        
        bool empty() const
        {
            return (_begin == _end);
        }
        
        T & back()
        {
            assert(_begin != _end);
            return buffer[(_end ? _end - 1 : sizeOfBuffer - 1)];
        }
        
        T & front()
        {
            assert(_begin != _end);
            return buffer[_begin];
        }
        
        const T & back() const
        {
            assert(_begin != _end);
            return buffer[(_end ? _end - 1 : sizeOfBuffer - 1)];
        }
        
        const T & front() const
        {
            assert(_begin != _end);
            return buffer[_begin];
        }
        
        void pushBack(T x)
        {
            reallocMore();
            buffer[_end] = x;
            _end = (_end + 1) & (sizeOfBuffer - 1);
        }
        
        void pushFront(T x)
        {
            reallocMore();
            _begin = (_begin - 1 + sizeOfBuffer) & (sizeOfBuffer - 1);
            buffer[_begin] = x;
        }
        
        void popBack()
        {
            assert(_begin != _end);
            reallocLess();
            _end = (_end - 1 + sizeOfBuffer) & (sizeOfBuffer - 1);
        }
        
        void popFront()
        {
            assert(_begin != _end);
            reallocLess();
            _begin = (_begin + 1) & (sizeOfBuffer - 1);
        }
        
        T &operator[](size_t i)
        {
            return buffer[(_begin + i) & (sizeOfBuffer - 1)];
        }
        
        const T &operator[](size_t i) const
        {
            return buffer[(_begin + i) & (sizeOfBuffer - 1)];
        }
        
        iterator begin() 
        {
            return iterator(_begin, this);
        }
        
        iterator end()
        {
            return iterator(_end, this);
        }
        
        constIterator cbegin() 
        {
            return constIterator(_begin, this);
        }
        
        constIterator cend()
        {
            return constIterator(_end, this);
        }
        
        constIterator begin() const
        {
            return cbegin();
        }
        
        constIterator end() const
        {
            return cend();
        }
        
        std::reverse_iterator<iterator> rbegin()
        {
            return std::reverse_iterator<iterator>(end());
        }
        
        std::reverse_iterator<iterator> rend()
        {
            return std::reverse_iterator<iterator>(begin());
        }
        
        std::reverse_iterator<constIterator> crbegin() 
        {
            return std::reverse_iterator<constIterator>(cend());
        }
        
        std::reverse_iterator<constIterator> crend() 
        {
            return std::reverse_iterator<constIterator>(cbegin());
        }
        
        std::reverse_iterator<constIterator> rbegin() const
        {
            return std::reverse_iterator<constIterator>(end());
        }
        
        std::reverse_iterator<constIterator> rend() const
        {
            return std::reverse_iterator<constIterator>(begin());
        }
        ~Deque()
        {
            delete[] buffer;
        }
    };
};
#endif