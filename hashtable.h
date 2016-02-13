/*
 * Agnieszka Janowicz
 * 
 * Tablice mieszające (hash-tables) z różnymi funkcjami rozpraszania
 * 
 * 
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sys/time.h>

#define SIZE_T 13
#define HASH_ALGORITHM_ALL 0
#define HASH_ALGORITHM_1 1
#define HASH_ALGORITHM_2 2
#define HASH_ALGORITHM_3 3


struct test_results //struktura pomocnicza do przechowywania danych o tescie
{
  int count_elems[SIZE_T];
  int elements_number;
  int hash_algorithm;
  double elapsed_time;
};


class hashTable //klasa zawierajaca i obslugujaca tablice mieszajaca
{
  struct ListElement
  {
    std::string string;
    ListElement* prev;
    ListElement* next;
    ListElement(std::string s): string(s), prev(NULL), next(NULL) {};
    ListElement(): string(NULL), prev(NULL), next(NULL) {};
  };

  ListElement** table;
  test_results results;
  
  int hash1(std::string& s);
  int hash2(std::string& s);
  int hash3(std::string s);
  int hashIt(std::string s);
  
public:
  
  hashTable(int algorithm)
  {
    results.hash_algorithm = algorithm;
    table = new ListElement * [SIZE_T];
    results.elapsed_time = 0;
    for (int i=0;i<SIZE_T;i++)
      table[i] = NULL;
  }
  
  ~hashTable()
  {
    clear();
    delete table;
  }
  
  bool erase(std::string str);
  bool clear();
  void add(std::string s);
  void erase(int k);
  void count_elements();
  bool load_strings(std::vector<std::string>* vect);
  test_results* test(std::vector<std::string>* vect);
};
