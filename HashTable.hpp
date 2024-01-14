//
// Created by Trent on 11/26/2023.
//
#include "HashTable.h"

#include <iostream>
#include <algorithm>
#include <functional>


using namespace cop4530;
using namespace std;


//constructor. Create a hash table, where the size of the vector is set to prime_below(size) 
//(where size is default  to 101), where prime_below() is a private member function of the HashTable and it is provided to you (also below for more details).

template <typename T>
HashTable<T>::HashTable(size_t size) : size(prime_below(size))  {
    hashtable.resize(prime_below(size));
}

//destructor. Delete all elements in hash table.
template <typename T>
HashTable<T>::~HashTable() {makeEmpty();}

//check if x is in the hash table
template <typename T>
bool HashTable<T>::contains(const T&x) 
{
    auto & whichList = hashtable[myhash(x)];
    return find(begin(whichList), end(whichList), x)!=end(whichList);
}

// add x into the hash table. Don't add if x is already in the hash table. 
// Return true if x is inserted; return false otherwise.
template <typename T>
bool HashTable<T>::insert(const T &x) {

    auto &whichList = hashtable[myhash(x)];
    auto it = find(begin(whichList),end(whichList), x);
    if(it != end(whichList))
        return false;

    whichList.push_back(x);

    if(++size>hashtable.size())
        rehash();

    return true;
}

//move version of insert
template <typename T>
bool HashTable<T>::insert(T &&x) {

    auto &whichList = hashtable[myhash(x)];
    auto it = find(begin(whichList),end(whichList), x);
    if(it != end(whichList))
        return false;

    whichList.push_back(move(x));

    if(++size>hashtable.size())
        rehash();

    return true;
}

//delete x if it is in the hash table.Return true if x is deleted, return false otherwise.
template <typename T>
bool HashTable<T>::remove(const T &x) {
    auto & whichList = hashtable[myhash(x)];
    auto itr = find(begin(whichList),end(whichList), x);
    if(itr == end(whichList))
        return false;
    whichList.erase(itr);
    --size;
    return true;
}

//delete all elements in the hash table
template <typename T>
void HashTable<T>::clear() {
    makeEmpty();
    size = 11;
}

//load the content of the file with name filename into the hash table.In the file, 
//each line contains a single value to be inserted to the hash table
template <typename T>
bool HashTable<T>::load(const char *filename) {

    ifstream is(filename);
    if(!is.is_open())
    {
        cout << "Cannot open file " << filename << endl;
        return false;
    }
    string line;
    while(getline(is,line)){
        transform(line.begin(),line.end(),line.begin(), ::tolower);
        insert(line);
    }
    is.close();
    return true;
}

// display all entries in the hash table. 
//Elements in one entry of the hash table (linked list) are separated by the tab character ("\t")
template <typename T>
void HashTable<T>::dump(){
    for(const auto & line : hashtable){
        for(const auto& ele : line)
        cout << ele << "\t";
    }
}

//write all elements in the hash table into a file with name filename. Each element occupies one line.
template <typename T>
bool HashTable<T>::write_to_file(const char *filename) {
    ofstream outputFile(filename);
    if(outputFile.is_open()){
        for(const auto & line : hashtable)
        {
            for(const auto& ele : line)
                outputFile << ele << "\t";
        }
        //outputFile << dump();
        outputFile.close();
        return true;
    }
    return false;
}

//get size of hash table
template <typename T>
int HashTable<T>::getSize() {
    return size;
}

//function to return the next prime number
template <typename T>
unsigned long HashTable<T>::nextPrime(unsigned long n)
{
    if(n<=1)
        return 2;

    while(true){
        bool isPrime = true;
        for(unsigned long i = 2; i*i <= n;++i)
        {
            if(n%i==0)
                isPrime=false;
                break;
        }
        if(isPrime)
            return n;
        ++n;
    }
}

//prublic func to return prime below, for preferred ht size
template <typename T>
unsigned long HashTable<T>::getPrime_below(unsigned long n) {
    return prime_below(n);
}


//===================Private HashTable Interfaces==============
//delete all elements in the hash table. The public interface clear() will call this function.
template <typename T>
void HashTable<T>::makeEmpty() {
    for(auto & thisList : hashtable)
    {
        thisList.clear();
    }
}

//Rehash function. Called when the number of elements in the hash table is greater than the size of the vector.
template <typename T>
void HashTable<T>::rehash() {
    vector<list<T>> oldLists = hashtable;

    hashtable.resize(nextPrime(2 * hashtable.size()));
    for(auto & thisList: hashtable)
        thisList.clear();
    size = 0;
    for(auto &thisList : oldLists)
    {
        for(auto & x :thisList){
            insert(std::move(x));
        }
    }
}

//return the index of the vector entry where x should be stored.
//For the implementation of this function, please do not use the simple example hash functions we discussed in class.
//You should use the one provided in STL(std::hash).The example implementation of hash table in the textbook and lecture used this STL hash function, 
//and you should follow the similar steps.Note that std::hash is defined in header file <functional>.
template <typename T>
size_t HashTable<T>::myhash(const T &x) {

    static hash<T> hf;
    return hf(x) % hashtable.size();
}

// two helpful functions to determine the proper prime numbers used in setting up the vector size. 
//Whenever you need to set hash table to a new size "sz", call prime_below(sz) to determine the new proper underlying vector size. 
//These two functions have been provided in hashtable.h and hashtable.hpp.
template <typename T>
unsigned long HashTable<T>::prime_below (long n)
{
    if (n > max_prime)
    {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime)
    {
        return max_prime;
    }
    if (n <= 1)
    {
        std::cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    std::vector <long> v (n+1);
    setPrimes(v);
    while (n > 2)
    {
        if (v[n] == 1)
            return n;
        --n;
    }

    return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n)
template <typename T>
void HashTable<T>::setPrimes(std::vector<long>& vprimes)
{
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for( i = 2; i*i < n; ++i)
    {
        if (vprimes[i] == 1)
            for(j = i + i ; j < n; j += i)
                vprimes[j] = 0;
    }
}
