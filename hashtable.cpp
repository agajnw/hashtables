/*
 * Agnieszka Janowicz
 *
 * Tablice mieszające (hash-tables) z różnymi funkcjami rozpraszania
 *
 *
 */

#include "hashtable.h"


int hashTable::hash1(std::string& s)
{
    int ret=0;
    int c;
    int i=0;
    while (i<s.size())
    {
        c=s[i];
        ret = c + (ret << 3);
        i++;
    }
    if (ret<0) ret=-ret;
    return ret%SIZE_T;
}

int hashTable::hash2(std::string& s)
{
    int ret=0;
    for (int i=0; i<s.size(); i++)
        ret = ret + int(s.at(i));
    if (ret<0) ret=-ret;
    return ret%SIZE_T;
}

int hashTable::hash3(std::string s)
{
    //czesto stosowane wartosci m i r (dobrze dzialajace)
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    unsigned int h = (rand()%20) ^ s.size();


    // Mieszanie 4 bajtow na raz

    const unsigned char * data = (const unsigned char *)&s;

    int len = s.size();

    while(len >= 4)
    {
        unsigned int k = *(unsigned int *)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    // Obsluga pozostalych bajtow

    switch(len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    //Dodatkowe mieszanie
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h%SIZE_T;
}

int hashTable::hashIt(std::string s)
{
    int val;
    clock_t start, stop;
    switch (results.hash_algorithm)
    {
    case (HASH_ALGORITHM_1):
    {
        start = clock();
        val =  hash1(s);
        stop = clock();
        results.elapsed_time += double(stop - start) / CLOCKS_PER_SEC * 1000;
        break;
    }
    case (HASH_ALGORITHM_2):
    {
        start = clock();
        val =  hash2(s);
        stop = clock();
        results.elapsed_time += double(stop - start) / CLOCKS_PER_SEC * 1000;
        break;
    }
    case (HASH_ALGORITHM_3):
    {
        start = clock();
        val =  hash3(s);
        stop = clock();
        results.elapsed_time += double(stop - start) / CLOCKS_PER_SEC * 1000;
        break;
    }
    }
    return val;
}


//usuwanie wyrazu z tablicy
bool hashTable::erase(std::string str)
{
    int hash = hashIt(str);
    ListElement* temp = table[hash];
    temp = table[hash];
    if ((temp->string).compare(str)==0)
    {
        table[hash] = temp->next;
        delete temp;
    }
    else if (table[hash]->next != NULL)
    {
        while (temp->next != NULL)
        {
            temp = temp->next;
            if ((temp->string).compare(str)==0)
            {
                if (temp->next)
                {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                }
                else
                    temp->prev->next = NULL;
                delete temp;
                return true;
            }
        }
        table[hash] = NULL;
        delete temp;
    }
    //nie udalo sie znalezc
    return false;
}

//czyszczenie zawartosci tablicy
bool hashTable::clear()
{
    for (int i=0; i<SIZE_T; i++)
        if (table[i])
            erase(i);
}

//dodanie nowego elementu do tablicy
void hashTable::add(std::string s)
{
    int hash;
    ListElement* elem,* temp;
    hash = hashIt(s);
    elem = new ListElement(s);
    temp = table[hash];
    if (temp != NULL) //jesli w tablicy znajduje sie element o tej samej wartosci mieszajacej
    {
        while (temp->next != NULL) //idziemy na koniec listy dwukierunkowej
            temp = temp->next;
        temp->next = elem;
        elem->prev = temp;
    }
    else
    {
        table[hash] = elem;
    }
}

//usuwa wszystkie elementy w tablicy w komorce k
void hashTable::erase(int k)
{
    ListElement* temp = table[k];
    if (table[k]->next == NULL) //jesli tylko 1 element o tej wartosci mieszajacej
    {
        table[k] = NULL;
        delete temp;
    }
    else //usuwanie wszystkich wartosci z listy dwukierunkowej
    {
        ListElement* to_erase = temp;
        while (temp->next)
        {
            temp = temp->next;
            delete to_erase;
            to_erase = temp;
        }
        table[k] = NULL;
        delete to_erase;
    }
}

//zlicza liczbe elementow o danej wartosci mieszajacej
void hashTable::count_elements()
{
    int count[SIZE_T];
    int value;
    for (int i=0; i<SIZE_T; i++)
    {
        if (!table[i])
            value = 0;
        else
        {
            value = 1;
            ListElement *temp = table[i];
            while (temp->next)
            {
                value++;
                temp = temp->next;
            }
        }
        results.count_elems[i] = value;
    }
}

//skopiowanie elementow wektora vect do tablicy mieszajacej
bool hashTable::load_strings(std::vector<std::string>* vect)
{
    for (int i=0; i<vect->size(); i++)
        add(vect->at(i));
}


//wywolanie testu algorytmu mieszajacego
test_results* hashTable::test(std::vector<std::string>* vect)
{
    results.elements_number = vect->size();

    load_strings(vect);
    count_elements();
    clear();

    return &results;
}
