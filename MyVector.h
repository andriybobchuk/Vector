//
// Custom implementation of a vector without the use of STL containers.
// Created by Andriy Bobchuk on 12/28/2021.
//

#ifndef VECTOR_MYVECTOR_H
#define VECTOR_MYVECTOR_H

#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// Class to test if vector is working with custom objects
class Person {
private:
    std::string mName;
    int mAge;

public:
    Person() {};

    Person(std::string newName, int newAge) {
        mName = newName;
        mAge = newAge;
    }

    std::string getName() { return this->mName; }

    int getAge() { return this->mAge; }

    // I will use the member function print() belonging to MyVector, thus I need this:
    friend std::ostream &operator<<(std::ostream &os, const Person &person) {
        os << "Name: " << person.mName << "\tAge: "  << person.mAge;
        return os;
    }

    // Needed for the deserialization:
    friend std::istream &operator>>(std::istream &is, Person &person) {
        std::string tmp;
        is >> tmp >> person.mName >> tmp >> person.mAge;
        return is;
    }

//    // I will sort people by parameters
//    bool operator<(const Person &personToCompareWith) const {
//        return mAge < personToCompareWith.mAge;
//    }
//
//    bool operator>(const Person &personToCompareWith) const {
//        return mAge > personToCompareWith.mAge;
//    }
};

template<typename T>
class MyVector {

private:
    std::shared_ptr<T[]> mData = nullptr; /// main data storage
    size_t mSize = 0; /// Current getSize
    size_t mCapacity = 0; /// Current getCapacity

public:
    /**
     * Default constructor, calls memory allocation function
     */
    MyVector() { memAlloc(0); }

    /**
     * Enables to construct vector filled with elements using the initializer list
     * @param initializerList - List of elements we want to push to our vector
     */
    MyVector(std::initializer_list<T> initializerList) {
        for (auto &element: initializerList)
            pushBack(element);
    }

    // &
    MyVector(MyVector<T> &myVector) {

        /// Allocate a new block of memory with the same getCapacity:
        std::shared_ptr<T[]> newMemBlock(new T[myVector.mCapacity]);

        /// Move elements from old block to a new one:
        for (size_t i = 0; i < myVector.mSize; i++) {
            newMemBlock.get()[i] = myVector.mData.get()[i];
        }

        /// Reset mData field to store our new block of memory:
        mData = newMemBlock;

        /// Set a new getCapacity value
        mCapacity = myVector.mCapacity;
        mSize = myVector.mSize;

//        this->mData = myVector.mData;
//        this->mSize = myVector.mSize;
//        this->mCapacity = myVector.mCapacity;
    }

    // &&
    MyVector(MyVector<T> &&myVector) noexcept {
        std::shared_ptr<T[]> newMemBlock(new T[myVector.mCapacity]);
        for (size_t i = 0; i < myVector.mSize; i++) {
            newMemBlock.get()[i] = std::move(myVector.mData.get()[i]);
        }
        mData = newMemBlock;
        mCapacity = myVector.mCapacity;
        mSize = myVector.mSize;
    }

    // &
    MyVector<T> &operator=(const MyVector<T> &anotherVector) {
        std::shared_ptr<T[]> newMemBlock(new T[anotherVector.mCapacity]);
        for (size_t i = 0; i < anotherVector.mSize; i++) {
            newMemBlock.get()[i] = anotherVector.mData.get()[i];
        }
        mData = newMemBlock;
        mCapacity = anotherVector.mCapacity;
        mSize = anotherVector.mSize;
    }

    // &&
    MyVector<T> &operator=(MyVector<T> &&anotherVector) noexcept {
        std::shared_ptr<T[]> newMemBlock(new T[anotherVector.mCapacity]);
        for (size_t i = 0; i < anotherVector.mSize; i++) {
            newMemBlock.get()[i] = std::move(anotherVector.mData.get()[i]);
        }
        mData = newMemBlock;
        mCapacity = anotherVector.mCapacity;
        mSize = anotherVector.mSize;
    }

    ~MyVector() {}


    /* ============================================================================================================  *
     *                                     MODIFIERS                                                                 |
     * ============================================================================================================  */

    /**
     * Increases the getSize by one, adds one element to the end.
     * If getCapacity is too small it doubles it.
     * @param element - thingy we need to add
     */
    void pushBack(const T &element) {

        // When not enough getCapacity we double it, but we cannot double zero
        if (mCapacity == 0) {
            memAlloc(1);
        } else if (mCapacity <= mSize) {
            memAlloc(mCapacity * 2);
        }
        mData.get()[mSize] = element;
        mSize++;
    };

    /**
     * EmplaceBack function takes not the constructor of the object as a parameter [When it comes to some complex
     * types of objects like structs], it takes the list of arguments needed to build this object in our
     * memory block IN PLACE.
     *
     * With that being said we are saving one move operation, which is not really demanding but still.
     *
     * @param args - list of arguments of a variable getSize, thus variadic template is used.
     * @return object we need.
     */
    template<typename... Args>
    T &emplaceBack(Args &&... args) {

        /*
         * Exactly the same code as push fun, only last line differs
         */

        // When not enough getCapacity we double it, but we cannot double zero
        if (mCapacity == 0) {
            memAlloc(1);
        } else if (mCapacity <= mSize) {
            memAlloc(mCapacity * 2);
        }

        // Instead of making mData[mSize] equal to object we forward all our
        // arguments to the constructor:
        mData.get()[mSize] = T(std::forward<Args>(args)...);;
        mSize++;
        return mData.get()[mSize];
    };

    /**
     * Removes the last element of a vector.
     * Like in a standard vector class, this function doesn't change the getCapacity, only the getSize.
     */
    void popBack() {
        if (mSize > 0) {
            mSize--;
            mData.get()[mSize].~T();
        }
    };

    /**
     * Removes all elements from the vector.
     * Capacity stays the same, getSize = 0.
     */
    void clear() {
        for (size_t i = 0; i < mSize; i++) {
            mData.get()[i].~T();
        }
        mSize = 0;
    }

    /**
      * A request to reduce getCapacity() to getSize().
      */
    void shrinkToFit() {
        for (size_t i = mSize; i < mCapacity; i++) {
            mData.get()[i].~T();
        }
        mCapacity = mSize;
    };

    /**
     * If the given value of newSize is less than the getSize at present then extra elements are demolished.
     * If newSize is more than current getSize of container then upcoming elements are appended at the end of the vector.
     * @param newSize
     */
    void resize(size_t newSize) {

        if (newSize < mSize) {
            for (size_t i = newSize; i < mSize; i++) {
                mData.get()[i].~T();
            }
            mSize = newSize;
        }
    };

    /* ============================================================================================================  *
     *                                     ELEMENT ACCESS                                                            |
     * ============================================================================================================  */

    /**
     * Allows us to access a certain element of a vector by its position in a container.
     *
     * @param position - index of the element we want to access.
     * @return the actual object we asked for.
     */
    T &operator[](size_t position) { return mData.get()[position]; }

    /**
     * The compiler will pick the const overload only when being called on a const object, otherwise it will
     * always prefer to use the non-const method.
     *
     * @param position - index of the vector's element we want to access.
     * @return the actual object we asked for.
     */
    const T &operator[](size_t position) const { return mData.get()[position]; }

    /**
     * Same as operator[] allows to access a certain element of a vector by its position.
     * @param position
     * @return object at the given position.
     */
    T &at(size_t position) { return mData.get()[position]; }


    /* ============================================================================================================  *
     *                                     CAPACITY                                                                  |
     * ============================================================================================================  */

    size_t getSize() { return mSize; } /// Returns private mSize

    size_t getCapacity() { return mCapacity; } /// Returns private mCapacity

    /* ============================================================================================================  *
     *                                     SERIALIZATION                                                             |
     * ============================================================================================================  */

    /**
     * Serializes the whole container with all its elements into a binary file.
     *
     * @param fileName
     */

    void serialize(const std::string &fileName) {

        // Opening the binary file by the name:
        std::ofstream outFileStream(fileName, std::ios::binary);

        if (outFileStream.good()) {

            // Firstly, write the number of elements in our vector:
            outFileStream.write((char *) &mSize, sizeof(mSize));

            for (size_t i = 0; i < mSize; i++) {

                std::stringstream stringStream; // stringStream representing each element
                stringStream << mData.get()[i];
                size_t stringSize = stringStream.str().size();

                // Writing the length of generated string:
                outFileStream.write((char *) &(stringSize), sizeof(stringSize));

                // Writing this string:
                outFileStream.write((char *) (stringStream.str().data()), sizeof(char) * stringSize);
            }
            outFileStream.close();
        }
    }


    /**
     * Deserializes the container from a binary file.
     *
     * @param fileName
     */
    void deserialize(const std::string &fileName) {
        std::ifstream inputFileStream(fileName, std::ios::binary);
        if (inputFileStream) {

            size_t numberOfElements;
            inputFileStream.read((char *) &numberOfElements, sizeof(numberOfElements));

            for (size_t i = 0; i < numberOfElements; i++) {

                std::stringstream stringStream;
                T element; // Object we are reading from file

                std::string data; // String representing current object

                size_t strSize;
                inputFileStream.read((char *) &strSize, sizeof(strSize)); // Read length of this string

                for (size_t j = 0; j < strSize; j++) {
                    data += inputFileStream.get(); // from inputFileStream to string
                }

                stringStream << data; // String to stringStream
                stringStream >> element; // Get element from stringStream
                pushBack(element); // Add it to vector

            }

            inputFileStream.close();
        }
    }



    /* ============================================================================================================  *
     *                                     FIND/SORT                                                                 |
     * ============================================================================================================  */

    /**
     * @param element
     * @return position of the requested element.
     */
    int find(const T &element) {

        for (int i = 0; i < mSize; i++) {
            if (mData.get()[i] == element) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @param begin start of a search zone.
     * @param end end of a search zone.
     * @param element
     * @return position of the requested element.
     */
    int find(int begin, int end, const T &element) {

        if (begin >= 0 && begin < end && end <= mSize) {
            for (int i = begin; i < end; i++) {
                if (mData.get()[i] == element) {
                    return i;
                }
            }
        }
        return -1;
    }

    /**
     * Sorts vector of complex structures by the given comparator
     * @tparam Compare Class name of our comparator
     * @param compare
     */
    template<typename Compare>
    void sort(Compare compare) {

        for (int i = 0; i < mSize - 1; i++) {
            for (int j = 0; j < mSize - i - 1; j++) {
                if (!compare(mData.get()[j], mData.get()[j + 1])) {
                    swap(mData.get()[j], mData.get()[j + 1]);
                }
            }
        }
    }

    /**
     * Simple sort function, uses bubble  sort.
     */
    void sort() {

        for (int i = 0; i < mSize - 1; i++) {
            for (int j = 0; j < mSize - i - 1; j++) {
                if (mData.get()[j] < mData.get()[j + 1]) {
                    swap(mData.get()[j], mData.get()[j + 1]);
                }
            }
        }
    }

    /* ============================================================================================================  *
     *                                     ITERATORS                                                                 |
     * ============================================================================================================  */

private:
    class MyIterator {
        T *mIteratorPointer; // Pointer to current position of our iterator
    public:

        explicit MyIterator(T *ptr) { mIteratorPointer = ptr; }

        // Pre increment:
        MyIterator &operator++() noexcept {
            mIteratorPointer++;
            return *this;
        }

        // Post increment:
        MyIterator operator++(int) {
            MyIterator iterator = *this;
            ++(*this);
            return iterator;
        }

        MyIterator &operator--() {
            mIteratorPointer--;
            return *this;
        }

        MyIterator operator--(int) {
            MyIterator iterator = *this;
            --(*this);
            return iterator;
        }

        T *operator->() { return mIteratorPointer; }

        T &operator*() { return *mIteratorPointer; }

        bool operator==(const MyIterator &iteratorToCompareWith) const {
            return mIteratorPointer == iteratorToCompareWith.mIteratorPointer;
        }

        bool operator!=(const MyIterator &iteratorToCompareWith) const {
            return mIteratorPointer != iteratorToCompareWith.mIteratorPointer;
        }
    };


public:
    MyIterator begin() {
        return MyIterator(mData.get()); // Points at the 0th obj of the collection
    }

    MyIterator end() {
        return MyIterator(mData.get() + mSize); // Points right past the last byte of memory we own
    }

    /* ============================================================================================================  *
     *                                     UTIL                                                                      |
     * ============================================================================================================  */

private:
    /**
    * Memory allocation function, algorithm:
     *
    *      1. Allocate a new memory block with a required getCapacity.
    *      2. Copy elements from old block to a new one.
    *      3. Reload mData block.
    *      4. Set a new getCapacity value.
    * @param requiredCapacity - new getCapacity.
    */
    void memAlloc(size_t requiredCapacity) {
        /// Allocate a new block of memory with a new getCapacity:
        std::shared_ptr<T[]> newMemBlock(new T[requiredCapacity]);

        /// Move elements from old block to a new one:
        for (size_t i = 0; i < mSize; i++) {
            newMemBlock.get()[i] = std::move(mData.get()[i]);
        }

        /// Reset mData field to store our new block of memory:
        mData = newMemBlock;

        /// Set a new getCapacity value
        mCapacity = requiredCapacity;
        //std::cout << requiredCapacity << " memory cells allocated\n";
    }

    void swap(T &x, T &y) {
        T temp = x;
        x = y;
        y = temp;
    }

public:


    void print() {
        try {
            for (size_t i = 0; i < mSize; i++) {
                std::cout << mData.get()[i] << "; ";
            }
            std::cout << std::endl;
        } catch (std::exception e) {
            std::cout << "Ooops " << e.what() << std::endl;
        }
    }


};

#endif //VECTOR_MYVECTOR_H

