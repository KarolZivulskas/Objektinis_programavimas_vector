#pragma once

#include <memory>
#include <limits>
#include <algorithm>

using namespace std;

template<class T>
class Vector {
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef T value_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // CONSTRUCTOR

    // 1. empty container constructor (default constructor)
    // Constructs an empty container, with no elements.
    Vector() {
        create();
    }

    // 2. fill constructor
    // Constructs a container with `size` elements. Each element is a copy of `value` (if provided).
    explicit Vector(size_type size, const T& value) {
        create(size, value);
    }

    // 3. range constructor
    // Constructs a container with as many elements as the range [first, last],
    // with each element emplace-constructed from its corresponding element in that range, in the same order.
    template<class InputIterator>
    Vector(InputIterator first, InputIterator last) {
        create(first, last);
    }

    // 4. copy constructor
    // Constructs a container with a copy of each of the elements in vector, in the same order.
    Vector(const Vector& vector) {
        create(vector.begin(), vector.end());
    }

    // 5. move constructor
    // Constructs a container that acquires the elements of vector.
    Vector(Vector&& vector) noexcept {
        create(vector.begin(), vector.end());
        vector.swap(*this);
    }

    // 6. initializer list constructor
    // Constructs a container with a copy of each of the elements in il, in the same order.
    Vector(const std::initializer_list<T>& il) {
        create(il.begin(), il.end());
    }

    // 7. size constructor
    // Constructs an empty container with a given size
    explicit Vector(size_type n) {
        create();
        resize(n);
    }



    // DESTRUCTOR

    // Deallocates all the storage capacity allocated by the Vector using its allocator.
    ~Vector() {
        destroy();
    }



    // OPERATOR =

    // 1. Copy assignment
    // Copies all the elements from x into the container (with x preserving its contents).
    Vector& operator=(const Vector& x) {
        if (this != &x) {
            destroy();
            create(x.begin(), x.end());
        }

        return *this;
    }

    // 2. Move assignment
    // Moves the elements of x into the container (x is left in an unspecified but valid state).
    Vector& operator=(Vector&& x) noexcept {
        if (this != &x) {
            destroy();
            create(x.begin(), x.end());
        }

        return *this;
    }

    // 3. The initializer list assignment (same as initializer list constructor)



    // ITERATORS

    // Return iterator to beginning
    // Returns an iterator pointing to the first element in the vector.
    iterator begin() noexcept {
        return data;
    }

    const_iterator begin() const noexcept {
        return data;
    }

    // Return const_iterator to beginning
    // Returns a const_iterator pointing to the first element in the container.
    const_iterator cbegin() const noexcept {
        return begin();
    }

    // Return const_iterator to end
    // Returns a const_iterator pointing to the past-the-end element in the container.
    const_iterator cend() const noexcept {
        return end();
    }

    // Return const_reverse_iterator to reverse beginning
    // Returns a const_reverse_iterator pointing to the last element in the container (i.e., its reverse beginning).
    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    // Return const_reverse_iterator to reverse end
    // Returns a const_reverse_iterator pointing to the theoretical element preceding the first element
    // in the container (which is considered its reverse end).
    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    // Return iterator to end
    // Returns an iterator referring to the past-the-end element in the vector container.
    iterator end() noexcept {
        return available;
    }

    const_iterator end() const noexcept {
        return available;
    }

    // Return reverse iterator to reverse beginning
    // Returns a reverse iterator pointing to the last element in the vector (i.e., its reverse beginning).
    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    // Return reverse iterator to reverse end
    // Returns a reverse iterator pointing to the theoretical element preceding the first element
    // in the vector (which is considered its reverse end).
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }



    // CAPACITY

    // Return size
    // Returns the number of elements in the vector.
    size_type size() const noexcept {
        return available - data;
    }

    // Return maximum size
    // Returns the maximum number of elements that the vector can hold.
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    // Change size
    // Resizes the container so that it contains n elements.
    void resize(size_type n) {
        if (n < size()) {
            available = data + n;
            iterator it = limit;
            while (it != available) {
                alloc.destroy(--it);
            }
        }
        else if (n > capacity()) {
            grow(n);
            resize(n);
        }
        else if (n > size()) {
            available += n - size();
        }
    }

    void resize(size_type n, const value_type& value) {
        if (n > capacity()) {
            grow(n);
            resize(n, value);
        }
        else if (n > size()) {
            size_type i = size();
            available += n - size();

            for (; i < n; i++) {
                data[i] = value;
            }
        }
        else {
            resize(n);
        }
    }

    // Return size of allocated storage capacity
    // Returns the size of the storage space currently allocated for the vector, expressed in terms of elements.
    size_type capacity() const {
        return limit - data;
    }

    // Test whether vector is empty
    // Returns whether the vector is empty (i.e. whether its size is 0).
    bool empty() const noexcept {
        return size() == 0;
    }

    // Request a change in capacity
    // Requests that the vector capacity be at least enough to contain n elements.
    void reserve(size_type n) {
        if (n > capacity()) {
            grow(n);
        }
    }

    // Shrink to fit
    // Requests the container to reduce its capacity to fit its size.
    void shrink_to_fit() {
        if (limit > available) {
            limit = available;
        }
    }



    // ELEMENT ACCESS

    // Access element with operator[]
    // Returns a reference to the element at position n in the vector container.
    T& operator[](size_type n) {
        return data[n];
    }

    const T& operator[](size_type n) const {
        return data[n];
    }

    // Access element with at()
    // Returns a reference to the element at position n in the vector.

    reference at(size_type n) {
        if (n < 0 || n >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[n];
    }

    const_reference at(size_type n) const {
        if (n < 0 || n >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[n];
    }


    // Access first element
    // Returns a reference to the first element in the vector.
    reference front() {
        return data[0];
    }

    const_reference front() const {
        return data[0];
    }

    // Access last element
    // Returns a reference to the last element in the vector.
    reference back() {
        return data[size() - 1];
    }

    const_reference back() const {
        return data[size() - 1];
    }

    // Access data
    // Returns a direct pointer to the memory array used internally by the vector to store its owned elements.
    value_type* _data() noexcept {
        return data;
    }

    const value_type* _data() const noexcept {
        return data;
    }



    // MODIFIERS

    // Assign vector content
    // Assigns new contents to the vector, replacing its current contents, and modifying its size accordingly.

    // 1. Range assign
    // The new contents are elements constructed from each of the elements in the range between first and last,
    // in the same order.
    template<class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        destroy();
        create(first, last);
    }

    // 2. Fill assign
    // The new contents are n elements, each initialized to a copy of val.
    void assign(size_type n, const value_type& val) {
        destroy();
        create(n, val);
    }

    // 3. Initializer list assign
    // The new contents are copies of the values passed as initializer list, in the same order.
    void assign(initializer_list<value_type> il) {
        destroy();
        create(il);
    }

    // Add element at the end
    // Adds a new element at the end of the vector, after its current last element.
    // The content of val is copied (or moved) to the new element.
    void push_back(const value_type& val) {
        if (available == limit) {
            grow();
        }
        unchecked_append(val);
    }

    void push_back(value_type&& val) {
        if (available == limit) {
            grow();
        }
        unchecked_append(val);
    }

    // Delete last element
    // Removes the last element in the vector, effectively reducing the container size by one.
    void pop_back() {
        iterator new_available = available;
        alloc.destroy(--new_available);
        available = new_available;
    }

    // Insert elements
    // The vector is extended by inserting new elements before the element at the specified position,
    // effectively increasing the container size by the number of elements inserted.

    // 1. Single element insert
    iterator insert(iterator position, const value_type& val) {
        return insert(position, 1, val);
    }

    // 2. Fill insert
    iterator insert(iterator position, size_type n, const value_type& val) {
        if (position < begin() || position >= end()) {
            throw std::out_of_range("Index out of range");
        }

        if (available + n >= limit) {
            size_type index = position - begin();
            grow(n);
            position = begin() + index;
        }

        int i = 0;
        for (iterator it(available + n - 1); it != (position + n - 1); it--, i++) {
            *it = data[size() - i - 1];
        }

        std::uninitialized_fill(position, position + n, val);
        available += n;

        return position;
    }

    // Erase elements
    // Removes from the vector either a single element (position) or a range of elements ([first,last)).

    iterator erase(iterator position) {
        if (position < data || position > available) {
            throw std::out_of_range("Index out of range");
        }

        iterator new_available = std::uninitialized_copy(position + 1, available, position);
        alloc.destroy(available + 1);

        available = new_available;

        return position;
    }

    iterator erase(iterator first, iterator last) {
        iterator new_available = std::uninitialized_copy(last, available, first);

        iterator it = available;
        while (it != new_available) {
            alloc.destroy(--it);
        }

        available = new_available;
        return last;
    }

    // Swap content
    // Exchanges the content of the container by the content of x,
    // which is another vector object of the same type. Sizes may differ.
    void swap(Vector& x) {
        std::swap(data, x.data);
        std::swap(available, x.available);
        std::swap(limit, x.limit);
    }

    // Clear content
    // Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.
    void clear() noexcept {
        destroy();
    }



    // NON-MEMBER FUNCTION OVERLOADS

    // Relational operators for vector
    // Performs the appropriate comparison operation between the vector containers and rhs.

    bool operator==(const Vector<T>& rhs) const {
        return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
    }

    bool operator!=(const Vector<T>& rhs) {
        return !(*this == rhs);
    }

    bool operator<(const Vector<T>& rhs) {
        return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
    }

    bool operator>(const Vector<T>& rhs) {
        return std::lexicographical_compare(rhs.begin(), rhs.end(), begin(), end());
    }

    bool operator>=(const Vector<T>& rhs) {
        return !(*this < rhs);
    }

    bool operator<=(const Vector<T>& rhs) {
        return !(*this > rhs);
    }

    // Exchange contents of vectors
    // The contents of container x are exchanged with those of y.
    // Both container objects must be of the same type (same template parameters), although sizes may differ.
    void swap(Vector<T>& x, Vector<T>& y) {
        std::swap(x, y);
    }

private:
    iterator data; // pirmasis elementas
    iterator available; // pirmasis elementas po paskutinio sukonstruoto elemento
    iterator limit; // pirmasis elementas po paskutinės rezervuotos vietos
    std::allocator<T> alloc;

    void create() {
        data = available = limit = nullptr;
    }

    void create(size_type size, const T& value) {
        data = alloc.allocate(size);
        limit = available = data + size;
        std::uninitialized_fill(data, limit, value);
    }

    void create(const_iterator i, const_iterator j) {
        data = alloc.allocate(j - i);
        limit = available = std::uninitialized_copy(i, j, data);
    }

    void destroy() {
        if (data) {
            iterator it = available;
            while (it != data) {
                alloc.destroy(--it);
            }
            alloc.deallocate(data, limit - data);
        }
        data = limit = available = nullptr;
    }

    void grow(size_type new_capacity = 1) {
        size_type new_size = std::max(2 * capacity(), new_capacity);
        iterator new_data = alloc.allocate(new_size);
        iterator new_available = std::uninitialized_copy(data, available, new_data);

        destroy();

        data = new_data;
        available = new_available;
        limit = new_data + new_size;
    }

    void unchecked_append(const T& value) {
        alloc.construct(available++, value);
    }
};