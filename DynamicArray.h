#pragma once
#include <iostream>
#include <algorithm>
#include <cassert>

template<typename Var>
class DynamicArray;

template<typename Var>
void swap(DynamicArray<Var>& lhs, DynamicArray<Var>& rhs);

template <typename Var>
class DynamicArray
{
    Var* data_;
    int capacity_;
    int size_;

public:

    using value_type = Var;

    DynamicArray();  //defaul constructor
    DynamicArray(std::initializer_list<Var> lst); //    DynamicArray<T> name{1,2,3,4,5,...};
    explicit DynamicArray(const Var* arr, int size);        //converts primitive Var array to DynamicArray
    explicit DynamicArray(const Var* begin, const Var* end); //converts a section of a primitive Var array to a DynamicArray. end is exclusive
    explicit DynamicArray(int new_size, const Var& val = Var{}); // DynamicArray<T> name(5, 70); => 70, 70, 70, 70, 70
    DynamicArray(const DynamicArray& rhs); //copy ctor
    DynamicArray(DynamicArray&& rhs) noexcept; //will learn in upper-years...
    ~DynamicArray(); //dtor

    DynamicArray& operator=(DynamicArray rhs); //copy/move assignment operator

    Var& operator[](int i); //lookup index
    const Var& operator[](int i) const; //for "const Var"

    //basic container operators
    bool IsEmpty() const; 
    int Capacity() const; //2* multipier (faster than 1.5*, but less memory-efficient)
    int Size() const;


    //specialized container operators
    Var& Front(); //gets ref to front index
    const Var& Front() const;
    Var& Back(); //gets ref to back index
    const Var& Back() const;
    Var* begin(); //required for "(for const auto& i : vector) {...}"
    const Var* begin() const;
    Var* end(); //required for "(for const auto& i : vector) {...}"
    const Var* end() const;



    void PushBack(const Var& val);
    void PushBack(Var&& val);
    void PopBack();
    void Clear();


    //custom memory management
    void Resize(int new_size, const Var& val = Var{});
    void Reserve(int new_capacity);
    void ShrinkToFit(); //shrinks capacity to size
    Var* Data(); //returns the address of the internal data
    const Var* Data() const;

    friend void swap<Var>(DynamicArray<Var>& lhs, DynamicArray<Var>& rhs);
};

template<typename Var>
void swap(DynamicArray<Var>& lhs, DynamicArray<Var>& rhs)
{
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.capacity_, rhs.capacity_);
    swap(lhs.size_, rhs.size_);
}


template <typename Var>
DynamicArray<Var>::DynamicArray() : 
    data_(static_cast<Var*>(operator new[](sizeof(Var)))), capacity_(1), size_(0)
{
    //empty
}

template <typename Var>
DynamicArray<Var>::DynamicArray(const Var* arr, int size) :
    data_(static_cast<Var*>(operator new[](size * sizeof(Var)))), capacity_(static_cast<int>(size)), size_(static_cast<int>(size))
{
    assert(size >= 0);
    std::uninitialized_copy(arr, arr + size, data_);
}

template <typename Var>
DynamicArray<Var>::DynamicArray(const Var* begin, const Var* end) : DynamicArray(begin, static_cast<int>(end - begin)) 
{
    assert(begin <= end);
}

template <typename Var>
DynamicArray<Var>::DynamicArray(std::initializer_list<Var> lst) : DynamicArray(lst.begin(), static_cast<int>(lst.size())) 
{
    assert(lst.size() >= 0);
}

template <typename Var>
DynamicArray<Var>::DynamicArray(int new_size, const Var& val) : 
    data_(static_cast<Var*>(operator new[](new_size * sizeof(Var)))), capacity_(new_size), size_(new_size)
{
    assert(new_size >= 0);

    //std::fill calls operator=, which is undefined behavior when used on uninitialized object)
    //alternative: loop + placement new
    std::uninitialized_fill(data_, data_ + size_, val);
}

template <typename Var>
DynamicArray<Var>::DynamicArray(const DynamicArray& rhs) : 
    data_(static_cast<Var*>(operator new[](rhs.capacity_ * sizeof(Var)))), capacity_(rhs.capacity_), size_(rhs.size_)
{
    assert(rhs.capacity_ >= 0);

    //std::copy calls operator=, which is undefined behavior when used on uninitialized object)

    //alternative: loop + placement new
    std::uninitialized_copy(rhs.data_, rhs.data_ + rhs.size_, data_);
}

template <typename Var>
DynamicArray<Var>::DynamicArray(DynamicArray&& rhs) noexcept :
    data_(std::exchange(rhs.data_, nullptr)), capacity_(std::exchange(rhs.capacity_, 0)), size_(std::exchange(rhs.size_, 0)) {}

template <typename Var>
DynamicArray<Var>::~DynamicArray()
{
    //destruct objects
    std::destroy(data_, data_ + size_);

    //free the memory
    operator delete[](data_);
}


//operators overloading
template <typename Var>
DynamicArray<Var>& DynamicArray<Var>::operator=(DynamicArray<Var> rhs)
{
    swap(*this, rhs);
    return *this;
}

template <typename Var>
Var& DynamicArray<Var>::operator[](int i)
{
    assert(0 <= i && i < size_);
    return data_[i];
}

template <typename Var>
const Var& DynamicArray<Var>::operator[](int i) const
{
    assert(0 <= i && i < size_);
    return data_[i];
}





//basic container operators
template <typename Var>
bool DynamicArray<Var>::IsEmpty() const
{
    return size_ == 0;
}

template<typename Var>
int DynamicArray<Var>::Capacity() const
{
    return capacity_;
}

template <typename Var>
int DynamicArray<Var>::Size() const
{
    return size_;
}



//specialized container operators
template <typename Var>
Var& DynamicArray<Var>::Front()
{
    assert(!this->IsEmpty());
    return data_[0];
}

template <typename Var>
const Var& DynamicArray<Var>::Front() const
{
    assert(!this->IsEmpty());
    return data_[0];
}

template <typename Var>
Var& DynamicArray<Var>::Back()
{
    assert(!this->IsEmpty());
    return data_[size_ - 1];
}

template <typename Var>
const Var& DynamicArray<Var>::Back() const
{
    assert(!this->IsEmpty());
    return data_[size_ - 1];
}

template <typename Var>
Var* DynamicArray<Var>::begin()
{
    return data_;
}
template <typename Var>
const Var* DynamicArray<Var>::begin() const
{
    return data_;
}
template <typename Var>
Var* DynamicArray<Var>::end()
{
    return data_ + size_; //pointer arithmetic
}
template <typename Var>
const Var* DynamicArray<Var>::end() const
{
    return data_ + size_; //pointer arithmetic
}





template <typename Var>
void DynamicArray<Var>::PushBack(const Var& val)
{
    //check if exceed capacity
    if (size_ >= capacity_) Reserve(2 * capacity_);

    //copy construct
    new(data_ + size_) Var(val);
    ++size_;
}

template <typename Var>
void DynamicArray<Var>::PushBack(Var&& val)
{
    //check if exceed capacity
    if (size_ >= capacity_) Reserve(2 * capacity_);

    //move construct
    new(data_ + size_) Var(std::move(val));
    ++size_;
}

template <typename Var>
void DynamicArray<Var>::PopBack()
{
    data_[--size_].~Var();
}

template <typename Var>
void DynamicArray<Var>::Clear()
{
    //destroy the objects
    std::destroy(data_, data_ + size_);
    size_ = 0;
}

//custom memory management
template <typename Var>
void DynamicArray<Var>::Resize(int new_size, const Var& val)
{
    assert(new_size >= 0);

    //shrinking
    if (new_size < size_)
    {
        //destroy truncated objects
        std::destroy(data_ + new_size, data_ + size_);
    }
    else 
    {
        //expand capacity if appropriate
        Reserve(new_size);

        //fill with the new objects
        std::uninitialized_fill(data_ + size_, data_ + new_size, val);
    }

    size_ = new_size;
}

template <typename Var>
void DynamicArray<Var>::Reserve(int new_capacity)
{
    if (new_capacity > capacity_)
    {
        //allocate heap with increased capacity
        Var* new_data = static_cast<Var*>(operator new[](new_capacity * sizeof(Var)));

        //move in the old data
        std::uninitialized_move(data_, data_ + size_, new_data);

        //destroy the old data, free the old heap
        std::destroy(data_, data_ + size_);
        operator delete[](data_);

        //update the container
        data_ = new_data;
        capacity_ = new_capacity;
    }
}

template <typename Var>
void DynamicArray<Var>::ShrinkToFit()
{
    //allocate heap with size-equivalent capacity
    Var* new_data = static_cast<Var*>(operator new[](size_ * sizeof(Var)));

    //move in the old data
    std::uninitialized_move(data_, data_ + size_, new_data);

    //destroy the old data, free the old heap
    std::destroy(data_, data_ + size_);
    operator delete[](data_);

    //update the container
    data_ = new_data;
    capacity_ = size_;
}

template <typename Var>
Var* DynamicArray<Var>::Data()
{
    return data_;
}

template <typename Var>
const Var* DynamicArray<Var>::Data() const
{
    return data_;
}