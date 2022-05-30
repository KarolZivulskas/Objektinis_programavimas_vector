# Objektinis_programavimas_vector
Eksperimentinių vector klasės tyrimų repozitorija

# Implementacija bei tyrimai

**Darbo tikslas** - sukurti `std::vector` kopiją ir atlikti šios klasės testus.

## Testai:

- [Vector::assign](#vectorassign)
- [Vector::insert](#vectorinsert)
- [Vector::pop_back](#vectorpop_back)
- [Vector::push_back](#vectorpush_back)
- [Vector::reserve](#vectorreserve)
- [Relational operators](#Relational-operators)

---

## Vector::assign

```cpp
// 1. Range assign
template<class InputIterator>
void assign(InputIterator first, InputIterator last);

// 2. Fill assign
void assign(size_type n, const value_type &val);

// 3. Initializer list assign
void assign(initializer_list<value_type> il)
```

Priskiria Vector klasei naują turinį, pakeičiant jo esamą turinį ir atitinkamai modifikuojant konteinerio dydį.

**Range assign (1)** versijoje naujas turinys yra elementai, sukonstruoti iš kiekvieno elemento, esančio diapazone nuo _first_ iki _last_.

**Fill assign (2)** versijoje naujas turinys yra _n_ elementai, kurių kiekvienas inicializuotas su _val_ reikšme.

**Initializer list assign (3)** versijoje naujas turinys yra reikšmių, perduotų kaip sąrašas kopijos, ta pačia tvarka.

### Test

```cpp
Vector<int> first;
first.assign(7, 100); // Fill assign

Vector<int>::iterator it;
it = first.begin() + 1;

Vector<int> second;
second.assign(it, first.end() - 1); // Range assign

Vector<int> third;
int values[] = {1776, 7, 4};
third.assign(values, values + 3); // Range assign

cout << "Size of first Vector: " << first.size() << endl;
cout << "Size of second Vector: " << second.size() << endl;
cout << "Size of third Vector: " << third.size() << endl;
cout << endl;
```

### Rezultatas

```bash
Size of first Vector: 7
Size of second Vector: 5
Size of third Vector: 3
```

---

## Vector::insert

```cpp
// 1. Single element insert
iterator insert(iterator position, const value_type &val);

// 2. Fill insert
iterator insert(iterator position, size_type n, const value_type &val);
```

Vektorius didinimas įterpiant naujus elementus prieš elementą nurodytoje vietoje. Tai padidina konteinerio dydį įterptų elementų skaičiumi.

Šis metodas sukelia automatinį atminties perskirstymą tik tuo atveju, jei naujas vektoriaus dydis viršija dabartinę vektoriaus talpą (capacity).

### Test

```cpp
Vector<int> array(3, 1);
Vector<int>::iterator it = array.begin();

it = array.insert(it, 2); // Single element insert
array.insert(it, 2, 3); // Fill insert

cout << "Vector contains: ";
for (int value : array) {
    cout << value << ", ";
}
cout << endl;
```

### Rezultatas

```bash
Vector contains: 3, 3, 2, 1, 1, 1
```

---

## Vector::pop_back

```cpp
void pop_back();
```

Pašalinamas paskutinis konteinerio elementas, efektyviai sumažinant konteinerio dydį vienu. Pašalintas elementas yra sunaikinamas.

### Test

```cpp
Vector<int> array;
array.push_back(100);
array.push_back(200);
array.push_back(300);

int valueAccumulator(0);
while (!array.empty()) {
    valueAccumulator += array.back();
    array.pop_back();
}

cout << "Vector elements add up to " << valueAccumulator << endl;
cout << "Vector::size(): " << array.size() << endl;
cout << "Vector::capacity() " << array.capacity() << endl;
cout << endl;
```

### Rezultatas

```bash
Vector elements add up to 600
Vector::size(): 0
Vector::capacity(): 4
```

---

## Vector::push_back

```cpp
void push_back (const value_type &val);
```

Pridedamas naujas elementas vektoriaus gale, po jo paskutinio elemento. _val_ reikšmė nukopijuojama (arba perkeliama) į naują elementą.

Efektyvumo analizė lyginant `Vector` ir `std::vector` metodą `push_back`. Tikrinama, kiek laiko užtruko konteinerio užpildymas ir kiek kartų buvo atliktas atminties perskirstymas.

|                 | 10 000  | 100 000 | 1 000 000 | 10 000 000 | 100 000 000 |
| :-------------- | :-----: | :-----: | :-------: | :--------: | :---------: |
| **Vector**      |         |
| time            | 0.00037 | 0.00333 |  0.02769  |  0.32023   |   2.89299   |
| capacity change |   14    |   17    |    20     |     24     |     27      |
| **stl::vector** |         |
| time            | 0.00039 | 0.00313 |  0.02852  |  0.32092   |   3.13779   |
| capacity change |   14    |   17    |    20     |     24     |     27      |

---

## Vector::reserve

```cpp
void reserve(size_type n);
```

Metodas Reikalauja, kad vektoriaus talpos būtų bent jau tiek, kad joje tilptų _n_ elementų.

Jei _n_ yra didesnis nei dabartinė vektoriaus talpa, tuomet yra įvykdomas atminties perskirstymas ir konteinerio talpa yra padidindama iki _n_ (arba daugiau).

### Test

```cpp
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
cout << endl;

cout << "Second Vector capacity changes: ";
for (int value : secondVectorChanges) {
    cout << value << ", ";
}
cout << endl;
```

### Rezultatas

```bash
First Vector capacity changes: 1, 2, 4, 8, 16, 32, 64, 128
Second Vector capacity changes: 100
```

---

## Relational operators

```cpp
bool operator==(const Vector<T> &rhs);
bool operator!=(const Vector<T> &rhs);
bool operator<(const Vector<T> &rhs);
bool operator>(const Vector<T> &rhs);
bool operator>=(const Vector<T> &rhs);
bool operator<=(const Vector<T> &rhs);
```

Operatoriai atlieka atitinkamą vektoriniaus ir _rhs_ palyginimo operaciją.

_Lygybės palyginimas_ (operatorius ==) atliekamas pirmiausia lyginant dydžius, o jei jie sutampa, tai elementai lyginami nuosekliai naudojant operatorių ==, sustojant ties pirmu neatitikimu (tarsi būtų naudojamas algoritmas `std::equl`).

_Mažiau nei_ palyginimas (operatorius <) elgiasi taip, lyg būtų naudojamas algoritmas `std::lexicographic_compare`, kuris lygina elementus nuosekliai, naudodamas operatorių `<` abipusiu būdu (t.y. tikrindamas tiek `a < b`, tiek `b < a`) ir sustodamas pirmą kartą.

Kitose operacijose naudojami operatoriai == ir < norint palyginti elementus, lyg būtų atliktos šios lygiavertės operacijos:

| operation | equivalent operation |
| :-------: | :------------------: |
|  a != b   |      !(a == b)       |
|   a > b   |        b < a         |
|  a <= b   |       !(b < a)       |
|  a >= b   |       !(a < b)       |

### Test

```cpp
Vector<int> first = {1, 1, 1};
Vector<int> second = {2, 2};

cout << "first == second: " << std::boolalpha << (first == second) << endl;
cout << "first != second: " << std::boolalpha << (first != second) << endl;
cout << "first <  second: " << std::boolalpha << (first < second) << endl;
cout << "first >  second: " << std::boolalpha << (first > second) << endl;
cout << "first <= second: " << std::boolalpha << (first <= second) << endl;
cout << "first >= second: " << std::boolalpha << (first >= second) << endl;
cout << endl;
```

### Rezultatas

```bash
first == second: false
first != second: true
first <  second: true
first >  second: false
first <= second: true
first >= second: false
```

## Išvados

Galime teigti, jog eksperimentinė vector klasė prilygsta standartiniam vector tipui. Kai kur pasiekiama netgi geresnių veikimo rezultatų.
