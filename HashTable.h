//
// Created by Trent on 11/26/2023.
//

#ifndef COP4530_HASHTABLE_H
#define COP4530_HASHTABLE_H

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <utility>

namespace cop4530 {

    template <typename T>
    class HashTable {

    public:

        static const unsigned int default_capacity = 11;
        static const unsigned int max_prime = 1301081;

        //constructor. Create a hash table, where the size of the vector is set to prime_below(size)
        explicit HashTable(size_t size = 101); 

        //destructor. Delete all elements in hash table.
        ~HashTable();

        //check if x is in the hash table.
        bool contains(const T &x);

        //add x into the hash table. Don't add if x is already in the hash table.
        //Return true if x is inserted; return false otherwise.
        bool insert(const T &x);

        //move version
        bool insert(T &&x);

        //delete x if it is in the hash table.Return true if x is deleted, return false otherwise.
        bool remove(const T&x);

        //delete all elements in the hash table
        void clear();

        //load the content of the file with name filename into the hash table.In the file, each line contains a single value to be inserted to the hash table.
        bool load(const char *filename);

        //display all entries in the hash table. Elements in one entry of the hash table (linked list) are separated by the tab character 
        void dump();

        //write all elements in the hash table into a file with name filename. Each element occupies one line.
        bool write_to_file(const char *filename);

        //gets the size of hte hash table
        int getSize();

        //gets the prime after the int n;
        unsigned long nextPrime(unsigned long n);

        //gets prime below int n;
        unsigned long getPrime_below(unsigned long n);

    private:
        //hash table to store dictionary elements
        std::vector<std::list<T>> hashtable;
        
        //stores dictionary size
        size_t size;

        //delete all elements in the hash table. The public interface clear() will call this function.
        void makeEmpty();

        //Rehash function. Called when the number of elements in the hash table is greater than the size of the vector.
        void rehash();

        //return the index of the vector entry where x should be stored.
        size_t myhash(const T&x);

        //two helpful functions to determine the proper prime numbers used in setting up the vector size.
        void setPrimes(std::vector<long>&);
        unsigned long prime_below(long n);

    };


#include "HashTable.hpp"

} // cop4530

#endif //COP4530_HASHTABLE_H
