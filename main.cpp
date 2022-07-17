#include <vector>
#include <string>
#include <ostream>
#include <istream>
#include <algorithm>
#include "MyVector.h"
/*
 * TEST FILE.
 *
 * Test units appear as the appropriate sector is defined in a header.
 */


// Comparator that will compare people by their age.
struct PersonAgeComparator {
    bool operator()(Person firstPerson, Person secondPerson) {
        if (firstPerson.getAge() == secondPerson.getAge()) {
            return firstPerson.getAge() < secondPerson.getAge();
        }
        return firstPerson.getAge() < secondPerson.getAge();
    }
};

int main() {


    /// Declaration.
    MyVector<float> myFloatVector;
    MyVector<double> myDoubleVector;
    MyVector<bool> myBoolVector;

    /// Initialization.
    MyVector<int> myIntVector = {1, 2, 3, 4, 5, 6};
    MyVector<std::string> myStringVector = {"1", "2", "3", "4", "5", "6"};
    MyVector<char> myCharVector = {'1', '2', '3', '4', '5', '6'};
    MyVector<Person> myPeopleVector = {
            Person("Andriy", 19),
            Person("Viktor", 18)
    };



    /* ============================================================================================================  *
     *                                     MODIFIERS                                                                 |
     * ============================================================================================================  */
#if 0
    myStringVector.pushBack("7");
    myCharVector.pushBack('7');
    myIntVector.pushBack(7);
    myIntVector.print();

    myPeopleVector.emplaceBack("Youssef", 19);
    myPeopleVector.print();

    myStringVector.popBack();
    myCharVector.popBack();
    myIntVector.popBack();
    myIntVector.print();

    myStringVector.clear();
    myCharVector.clear();
    myIntVector.clear();
    myIntVector.print();
#endif

    /* ============================================================================================================  *
     *                                     ELEMENT ACCESS                                                            |
     * ============================================================================================================  */
#if 0
    // Element access examples
    std::cout << myIntVector[3];
    std::cout << myIntVector.at(3);
    std::cout << myStringVector[3];
    std::cout << myStringVector.at(3);
    std::cout << myCharVector[3];
    std::cout << myCharVector.at(3);
#endif

    /* ============================================================================================================  *
     *                                     CAPACITY                                                                  |
     * ============================================================================================================  */
#if 0
    std::cout << myIntVector.getSize();
    std::cout << myIntVector.getCapacity();
    std::cout << myStringVector.getSize();
    std::cout << myStringVector.getCapacity();
    std::cout << myCharVector.getSize();
    std::cout << myCharVector.getCapacity();
#endif

    /* ============================================================================================================  *
     *                                     SERIALIZATION                                                             |
     * ============================================================================================================  */
#if 0
    // The content of myIntVector is written to binary file
    myIntVector.serialize("Serialized.bin");

    // Now the content of that file is deserialized to a new vector
    MyVector<int> myNewIntVector;
    myNewIntVector.deserialize("Serialized.bin");

    // The content of the two vectors is exactly the same
    myIntVector.print();
    myNewIntVector.print();

    // Serializing vector of custom objects
    myPeopleVector.serialize("peopleFile");
    MyVector<Person> myNewPeopleVector;
    myNewPeopleVector.deserialize("peopleFile");
    myNewPeopleVector.print();

#endif
    /* ============================================================================================================  *
     *                                     FIND/SORT                                                                 |
     * ============================================================================================================  */
#if 0
    std::cout << myStringVector.find("2");
    std::cout << myStringVector.find(2, 6, "4");

    // You can also pass custom comparators as arguments:
    myPeopleVector.sort(PersonAgeComparator());
    myPeopleVector.print();
#endif
    /* ============================================================================================================  *
     *                                     ITERATORS                                                                 |
     * ============================================================================================================  */
#if 0
    for(const auto &element : myIntVector) {
        std::cout << element ;
    }

    for (auto it = myIntVector.begin(); it != myIntVector.end(); it++) {
        std::cout << *it;
    }
#endif


    return 0;
}



