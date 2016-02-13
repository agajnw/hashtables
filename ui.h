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
#include <getopt.h>
#include <cmath>
#include <iostream>
#include "hashtable.h"

static int table_size;
static int hash_alg;
std::vector<std::string> vect;
std::vector<double> measured_q;
std::vector<double> measured_time;
std::vector<int> measured_value;
std::fstream plik;


bool getInputFromFile(std::string fileName, std::vector<std::string> *vect);
static void print_help();
void addString(std::string str);
static void parse_cmd_line(int argc, char *argv[]);
void do_test(int algorithm);