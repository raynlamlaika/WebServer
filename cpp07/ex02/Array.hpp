// #ifndef ARRAY_HPP
// # define ARRAY_HPP


// // check typenmae the real deffrent more just for coding structur
// template <class T>
// class Array
// {
// private :
//     T *arr;
//     int size
// public :
//     Array()
//     {
//         arr = new T[0];
//     };
//     Array(unsigned int n)
//     {
//         arr = new T[n](0);
//     }

//     Array(const Array &other) : arr(new T[other.size]), size(other.size)
//     {
//         for (int i = 0 ; i < size ; i ++)
//         {
//             other.arr[i] = this.arr[i];
//         }
//     }
//     Array &operator=(const Array &other) : 
//     {
//         // check for allocation 
//         if (this != other)
//         {
//             arr(new T[other.size]), size(other.size)
//             for (int i = 0 ; i < size ; i ++)
//             {
//                 other.arr[i] = this.arr[i];
//             }
//         }
//     }
//     Array &operator[](unsigned int index)
//     {
//         if (index > size)
//             return (NULL);
//         return(arr[index]);
//     }
//     Array operator[](unsigned int index) const
//     {
//         if (index > size)
//             return (NULL);
//         return(arr[index]);
//     }

// };





// #endif








#ifndef ARRAY_HPP
# define ARRAY_HPP

#include <stdexcept>  // for std::out_of_range

template <class T>
class Array
{
private:
    T* arr;
    size_t size;  // Use size_t for array size

public:
    // Default constructor
    Array() : arr(new T[0]), size(0) {}

    // Constructor with size parameter
    Array(unsigned int n) : arr(new T[n]()), size(n) {}

    // Copy constructor
    Array(const Array& other) : arr(new T[other.size]), size(other.size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            arr[i] = other.arr[i];  // Copy the values
        }
    }

    // Assignment operator
    Array& operator=(const Array& other)
    {
        if (this != &other)  // Check for self-assignment
        {
            // Deallocate current memory
            delete[] arr;

            // Allocate new memory and copy over the size and data
            size = other.size;
            arr = new T[size];
            for (size_t i = 0; i < size; ++i)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Array()
    {
        delete[] arr;
    }

    // Non-const index operator (for modification)
    T& operator[](unsigned int index)
    {
        if (index >= size)  // Check for out of bounds
            throw std::out_of_range("Index out of bounds");
        return arr[index];
    }

    // Const index operator (for read-only access)
    const T& operator[](unsigned int index) const
    {
        if (index >= size)  // Check for out of bounds
            throw std::out_of_range("Index out of bounds");
        return arr[index];
    }

    // Getter for size
    size_t getSize() const
    {
        return size;
    }
};

#endif



