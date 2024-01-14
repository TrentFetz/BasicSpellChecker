#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>

#include "HashTable.h"

using namespace std;
using namespace cop4530;

void menu()//getmenu option, provided by prof
{
    cout << "\n\n";
    cout << "l - Load Dictionary From File" << endl;
    cout << "a - Add Word" << endl;
    cout << "r - Remove Word" << endl;
    cout << "c - Clear HashTable" << endl;
    cout << "f - Find Word" << endl;
    cout << "d - Dump HashTable" << endl;
    cout << "s - HashTable Size" << endl;
    cout << "w - Write to File" << endl;
    cout << "x - Exit program" << endl;
    cout << "\nEnter choice : ";
}

vector<string> findInHashTable(string word, HashTable<string>& ht)//function to find replacements in the hashtable
{
    vector<string> swaps;
    swaps.reserve(10);
 
    for (size_t i = 0; i < word.length(); ++i)
    {
        for (char j = 'a'; j <= 'z'; ++j)
        {
            string candidate = word;
            candidate[i] = j;
            if (ht.contains(candidate) && find(swaps.begin(), swaps.end(), candidate) == swaps.end())
            {
                swaps.push_back(candidate);
                if (swaps.size() >= 10) {
                    return swaps;
                }
            }
        }
    }
    return swaps;

}

string onlyAlpha(const string& input)//returns only the letters, no commas/periods
{
    string result;
    for (char ch : input)
    {
        if (isalnum(ch))
        {
            result += ch;
        }
    }
    return result;
}

string checkWord(const string& input, string lowercase)
{
    
    if (input.back() == ',' || input.back() == '.')
    {
        lowercase += input.back();
    }
    if (isupper(input[0])) {
        //char firstLetter = toupper(lowercase[0]);
        //lowercase = firstLetter + lowercase.substr(1);
        lowercase[0] = toupper(lowercase[0]);
    }
    return lowercase;
}


void programWithParameters(int argc, char* argv[])
{
    HashTable<string> ht;//create the hashtable

    const char* dictFile = argv[1];//set the first parameter to the dictionary file
    const char* inputFile = argv[2];//set the second parameter to the input word file
    const char* outputFile = argv[3];//set the third parameter to the output, corrected word file

    ifstream dictionary(dictFile);
    ifstream wordFile(inputFile);
    ofstream out(outputFile);

    ht.load(dictFile);//load the dictionary file to hashtable
    string line;//represents a line of the input text doc.
    cout << ht.getSize() << endl;//cout size of the ht
    while (getline(wordFile, line))
    {
        string correctedLine;//line that is corrected 
        istringstream iss(line);//for seperating the line into words
        string word;//represents a single word from the text doc

        while (iss >> word)
        {
            string lowerCase = onlyAlpha(word);//get only the word,no other characters, lower case version
            string upperCase = word;//the word in uppercase version
            transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);//transform word to lower

            if (!ht.contains(lowerCase))
            {
                transform(upperCase.begin(), upperCase.end(), upperCase.begin(), ::toupper);//transform word to upper
                size_t pos = line.find(word);//set variable to find the pos of the word in the line
                while (pos != string::npos) //while the position isnt at max possible, at appropiate spot
                {
                    line.replace(pos, word.length(), upperCase);//replace the word, at the pos, at certain length with uppercase
                    pos = line.find(upperCase, pos + upperCase.length());//reset pos
                }
                vector<string> swaps = findInHashTable(lowerCase, ht);//find possible swaps
                cout << line << endl;//output the line, with the upper case typo

                if (!swaps.empty()) {//if there are swaps available, offer them to the user
                    cout << "====================================" << endl;
                    for (size_t i = 0; i < swaps.size(); i++)
                    {
                        cout << i << "): " << swaps[i] << endl;
                    }
                    cout << "n (no change): " << endl;//provide user with other options
                    cout << "====================================" << endl;
                    cout << "Your choice: ";
                    char choice;//get their input, and perform that action to the corrected line
                    cin >> choice;

                    if (choice == 'n')
                    {
                        correctedLine += word + " ";//no change, add the unchanged word to the line
                        continue;
                    }
                    int input = choice - '0';//swap to int input
                    if (input >= 0 && input < swaps.size())
                    {

                        lowerCase = swaps[input];//add correct swap option
                        lowerCase = checkWord(word, lowerCase);
                        correctedLine += lowerCase + " ";//add to the corrected line
                        continue;
                    }
                    else {//error checking
                        cout << "invalid choice\n";
                    }
                }
            }
            else {
                correctedLine += word + " ";//add normal word to the line
            }
        }
        out << correctedLine << endl;//write corrected line to the output text file
    }
    dictionary.close();//close all files that were opened
    wordFile.close();
    out.close();
}

int main(int argc, char* argv[]) 
{
    if (argc == 1) 
    {
        unsigned long htSize;//variable for hash table sizee, unsigned as non negative and long cause large number

        cout << "Enter preferred hash table capacity:";

        cin >> htSize;//get hash table size from user

        if (htSize <= 1)
        {
            cout << "** input too small\n"
                 << "set to default capacity\n";
                  htSize = 11;
        }

        HashTable<string> ht(htSize);
        cout << ht.getSize() << endl;//cout the prime of entered size, for hash

        char input;
        while (input != 'x') 
        {
            //x to quit
            menu();// menu option
            cin >> input;
            switch (input) 
            {//switch case for different menu input

                case 'l': 
                {//option to load dictionary
                    string filename;
                    cout << "Enter dictionary filename to load from: ";//load file from user input
                    cin >> filename;//therefore can use different files for dictionary
                    const char* fn = filename.c_str();//pass to string
                    if (ht.load(fn)) {
                        cout << "Dictionary loaded successfully. \n";
                    }
                    else cout << "Cannot open file " << filename << endl;
                    break;
                }
                case 'a': 
                {//option to add word
                    string insertWord;
                    cout << "Enter word: ";
                    cin >> insertWord;
                    if (ht.insert(insertWord))//if not in hash table already
                    {
                        cout << "word " << insertWord << " added.\n";
                    }
                    else
                    {
                        cout << "word already in dictionary\n";
                    }
                    break;
                }

                case 'r': 
                {//option to remove word
                    string removeWord;
                    cout << "Enter word: ";
                    cin >> removeWord;
                    if (ht.remove(removeWord)) 
                    {//if in hash table, delete
                        cout << "word deleted\n";
                    }
                    else//else not in hash table
                        cout << "word not deleted\n";
                    break;
                }

                case 'c': 
                {//clear the hash table of all its elements
                    ht.clear();
                    cout << "cleared!\n";
                    break;
                }

                case 'f': 
                {//option to look up word
                    string lookUp;
                    cout << "Enter word: ";
                    cin >> lookUp;
                    if (ht.contains(lookUp))//if in hash tablr
                    {
                        cout << "word found!\n";
                    }
                    else//else not in hash table;
                        cout << "Word not in hash table\n";
                    break;
                }

                case 'd': 
                {//dump the table
                    ht.dump();
                    break;
                }

                case 's': 
                {//return the tables size
                    cout << ht.getSize();
                    break;
                }

                case 'w': 
                {//write contents of hash table to a file
                    string filename;
                    cout << "Enter dictionary file name to write to: ";
                    cin >> filename;
                    const char* fn = filename.c_str();
                    if (ht.write_to_file(fn))
                    {
                        cout << "Written successfully\n";
                    }
                    else 
                    {
                        cout << "Write failed\n";
                    }
                    
                    break;
                }
                case 'x':
                {
                    cout << "Have a great day\n";
                }
                default://default for error input
                    cout << " *****Error: Invalid entry.  Try again.\n";
            }
        }
    }
    else if (argc == 4) //if the program has enough parameters, run this version
    {
        programWithParameters(argc, argv);
    }
    else cout << argv[1] << " error with file input, either run with no files, or three.\n(dictionary check_file output_file)";

}
