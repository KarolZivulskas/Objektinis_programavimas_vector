#include <iostream>
#include <vector>
#include <iomanip>

#include "Timer.hpp"
#include "Vector.hpp"

using namespace std;

void doPushBackTest();

void testAssign();

void testInsert();

void testPopBack();

void testReserve();

void testRelationalOperators();

int main() {
    doPushBackTest();
    testAssign();
    testInsert();
    testPopBack();
    testReserve();
    testRelationalOperators();

    return 0;
}

void doPushBackTest() {
    Timer timer;
    vector<int> sizes = { 10000, 100000, 1000000, 10000000, 100000000 };

    for (auto size : sizes) {
        cout << "--- Vector::push_back test of size " << size << ":" << endl;

        Vector<int> customVector;
        int customCapacityCounter = 0;
        timer.reset();
        for (int i = 0; i < size; i++) {
            customVector.push_back(i);
            if (customVector.size() == customVector.capacity()) {
                customCapacityCounter++;
            }
        }

        cout << "Custom vector time: "
            << std::fixed << std::setprecision(5) << timer.elapsed() << "s. "
            << "Capacity changed " << customCapacityCounter << " times" << endl;

        vector<int> stdVector;
        int originalCapacityCounter = 0;
        timer.reset();
        for (int i = 0; i < size; i++) {
            stdVector.push_back(i);
            if (stdVector.size() == stdVector.capacity()) {
                originalCapacityCounter++;
            }
        }
        cout << "std::vector time: "
            << std::fixed << std::setprecision(5) << timer.elapsed() << "s. "
            << "Capacity changed " << originalCapacityCounter << " times" << endl;

        cout << endl;
    }
}

void testAssign() {
    cout << "--- Vector::assign ---" << endl;
    Vector<int> first;
    first.assign(7, 100);

    Vector<int>::iterator it;
    it = first.begin() + 1;

    Vector<int> second;
    second.assign(it, first.end() - 1);

    Vector<int> third;
    int values[] = { 1776, 7, 4 };
    third.assign(values, values + 3);

    cout << "Size of first Vector: " << int(first.size()) << " (expected 7)" << endl;
    cout << "Size of second Vector: " << int(second.size()) << " (expected 5)" << endl;
    cout << "Size of third Vector: " << int(third.size()) << " (expected 3)" << endl;
    cout << endl;
}

void testInsert() {
    cout << "--- Vector::insert ---" << endl;

    Vector<int> array(3, 1);
    Vector<int>::iterator it = array.begin();

    it = array.insert(it, 2);
    array.insert(it, 2, 3);

    cout << "Vector contains: ";
    for (int value : array) {
        cout << value << ", ";
    }

    cout << "(expected 3, 3, 2, 1, 1, 1)" << endl;
    cout << endl;
}

void testPopBack() {
    cout << "--- Vector::pop_back ---" << endl;

    Vector<int> array;
    array.push_back(100);
    array.push_back(200);
    array.push_back(300);

    int valueAccumulator(0);
    while (!array.empty()) {
        valueAccumulator += array.back();
        array.pop_back();
    }

    cout << "Vector elements add up to " << valueAccumulator << " (expected: 600)" << endl;
    cout << "Vector::size(): " << array.size() << " (expected: 0)" << endl;
    cout << "Vector::capacity() " << array.capacity() << " (expected: 4)" << endl;
    cout << endl;
}

void testReserve() {
    cout << "--- Vector::reserve ---" << endl;

    Vector<int>::size_type capacity;

    Vector<int> firstVector;
    Vector<int> firstVectorChanges;

    capacity = firstVector.capacity();
    for (int i = 0; i < 100; ++i) {
        firstVector.push_back(i);
        if (capacity != firstVector.capacity()) {
            capacity = firstVector.capacity();
            firstVectorChanges.push_back(capacity);
        }
    }

    Vector<int> secondVector;
    Vector<int> secondVectorChanges;

    capacity = secondVector.capacity();
    secondVector.reserve(100);
    for (int i = 0; i < 100; ++i) {
        secondVector.push_back(i);
        if (capacity != secondVector.capacity()) {
            capacity = secondVector.capacity();
            secondVectorChanges.push_back(capacity);
        }
    }

    cout << "First Vector capacity changes: ";
    for (int value : firstVectorChanges) {
        cout << value << ", ";
    }
    cout << "(expected 1, 2, 4, 8, 16, 32, 64, 128)" << endl;

    cout << "Second Vector capacity changes: ";
    for (int value : secondVectorChanges) {
        cout << value << ", ";
    }
    cout << "(expected 100)" << endl;
    cout << endl;
}

void testRelationalOperators() {
    cout << "--- std::relational operators (Vector) ---" << endl;

    Vector<int> first = { 1, 1, 1 };
    Vector<int> second = { 2, 2 };

    cout << "Compare two vectors: ";
    cout << "First: ";
    for (int value : first) {
        cout << value << ", ";
    }

    cout << "and Second: ";
    for (int value : second) {
        cout << value << ", ";
    }
    cout << endl;

    cout << "first == second: " << std::boolalpha << (first == second) << " (expected false)" << endl;
    cout << "first != second: " << std::boolalpha << (first != second) << " (expected true)" << endl;
    cout << "first <  second: " << std::boolalpha << (first < second) << " (expected true)" << endl;
    cout << "first >  second: " << std::boolalpha << (first > second) << " (expected false)" << endl;
    cout << "first <= second: " << std::boolalpha << (first <= second) << " (expected true)" << endl;
    cout << "first >= second: " << std::boolalpha << (first >= second) << " (expected false)" << endl;
    cout << endl;
}