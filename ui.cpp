/*
 * Agnieszka Janowicz
 * 
 * Tablice mieszające (hash-tables) z różnymi funkcjami rozpraszania
 * 
 * 
 */

#include "ui.h"




bool getInputFromFile(std::string fileName)
{
	std::fstream file;
	std::string str;
	file.open(fileName.c_str(), std::ios::in);
	if (!file.good())
		return false;
	while (file >> str)
	  vect.push_back(str);
	file.close();
	return true;
}

void addString(std::string str)
{
  vect.push_back(str);
}

static void print_help() 
{
   printf("Usage: ./hashtables [OPTIONS]... \n"
	  " All options may be used in "
	  " their shorthand form (e.g. --file = -file)\n using the first"
	  " letter in the option name.\n"
	  "\t--file FILE\t\tDefines user file containing strings to put into hashtable\n"
	  "\t--input STRING\t\tUser provided string\n"
	  "\t--generate SIZE\t\tAutomatically enerate data.\n"
	  "\t\t\t\tSIZE defines the number of strings to generate.\n"
	  "\t\t\t\tIf SIZE is not given, program will generate 500 strings.\n"
	  "\t--algorithm NUM\t\tThe number of hash algorithm to use.\n"
	  "\t\t\t\tAlgorithms are described in documentation.\n"
	  "\t\t\t\tIf NUM is not given all algorithms will be used.\n"  
	  );
}


void generate(int size)
{
  for (int i=0;i<size;i++)
  {
    int length = rand() % 6 + 2, letter;
    char c;
    std::string string;
    
    for (int j=0;j<length;j++)
    {
      letter =  rand() % 50 + 65;
      if (letter>90)
	letter += 7;
      string.push_back(letter);
    }
    
    addString(string);
  } 
}

static void parse_cmd_line(int argc, char *argv[])
{
   int tw, th, ts;
   char c;
   struct option longopts[]  = 
     {
	  {"filename", required_argument, 0, 'f'},
	  {"input", required_argument, 0, 'i'},
	  {"generate", required_argument, 0, 'g'},
	  {"algorithm", required_argument, 0, 'a'},
	  {NULL, 0, NULL, 0}
     };

   if (argc==1)
     print_help();
   while ((c = getopt_long_only(argc, argv, "f:i:g:a:", longopts, NULL)) != -1)
     {
	switch (c) 
          {
           case 0:
             break;
           case 'f':
	     std::cout << "Loading file " << optarg << "...\n";
	     if (!getInputFromFile(optarg))
	     {
	      std::cout << "Failed to load file. Exiting...";
	      exit(0);
	     }
	     else
	     std::cout << "Loading successful.\n";
             break;
           case 'i':
             addString(optarg);
             break;
           case 'g':
             table_size = atoi(optarg);
	     generate(table_size);
             break;
           case 'a':
             hash_alg = atoi(optarg);
             break;
           default:
             print_help();
             break;
          }
     }
}

void write_results(int alg)
{
    std::string table_start = "<table>";
    std::string table_end = "</table>";
    std::string tr_start = "<tr>";
    std::string tr_end = "</tr>";
    std::string td_start = "<td>";
    std::string td_end = "</td>";
    
    
    plik << table_start << std::endl;
    plik << "<table border=\"1\">" << std::endl;
    
    if (alg==1)
      plik << "<caption>Algorytm 1</caption>" << std::endl;
    else if (alg==2)
      plik << "<caption>Algorytm 2</caption>" << std::endl;
    else
      plik << "<caption>Algorytm 3</caption>" << std::endl;
    
    plik << tr_start << std::endl;
    plik << td_start << " n" << td_end;
    plik << "\t" << td_start << "  t[n]" << td_end;
    plik << "\t" << td_start << "  q[n]" << td_end << std::endl;
    plik << tr_end << std::endl;
  
    for (int i=0;i<measured_value.size();i++)
    {
      if (i==measured_value.size()/2)
	plik << "<tr bgcolor = \"green\">" << std::endl;
      else
	plik << tr_start << std::endl;
      
      plik << td_start << measured_value[i] << td_end;
      plik << "\t" << td_start << measured_time[i] << td_end;
      plik << "\t" << td_start << measured_q[i] << td_end << std::endl;
      plik << tr_end << std::endl;
    }
    
    plik << table_end << std::endl;

}

void do_test(int algorithm)
  {
    double *t = new double [vect.size()];
    double *q = new double [vect.size()];
    int median = 3*vect.size()/4;
    int size;
    test_results *results;
    hashTable hashT(algorithm);
    
    std::cout << "Algorytm " << algorithm << ":\n";
    for (int i=vect.size()/2; i< vect.size();i++)
	{
	  std::vector<std::string> temp;
	  std::vector<double> q;
	  for (int j=0;j<=i;j++)
	    temp.push_back(vect[j]);
	  
	  results = hashT.test(&temp);
	  t[i] = results->elapsed_time;
	  temp.clear();  
	}
	
	if (vect.size()>20)
	  size = 20;
	else 
	  size = vect.size();
	median = (vect.size() - size/2);
	for (int i=vect.size()-size;i<vect.size();i++)
	{
	  q[i] = ((double)t[i]/t[median])*((double)median/i);
	  std::cout << "n " << i << ", t(n): " << t[i] << ", q(n): " << q[i];
	  if (i==median)
	    std::cout << "  <<<<<< [median]";
	  std::cout << "\n";
	  
	  measured_q.push_back(q[i]);
	  measured_time.push_back(t[i]);
	  measured_value.push_back(i);
	}

	
	write_results(algorithm);
	measured_q.clear();
	measured_time.clear();
	measured_value.clear();
}

void three_tests()
{
    std::string table_start = "<Table style=\"float:left; margin-left: 80px\">";
    std::string table_end = "</Table>";
    std::string tr_start = "<tr>";
    std::string tr_end = "</tr>";
    std::string td_start = "<td>";
    std::string td_end = "</td>";
    
    plik << table_start << std::endl;
    plik << "<table border=\"2\">" << std::endl;
    plik << tr_start << std::endl;
    plik << td_start; 
    do_test(HASH_ALGORITHM_1);
    plik << td_end;
    plik << "\t" << td_start; 
    do_test(HASH_ALGORITHM_2);
    plik << td_end;
    plik << "\t" << td_start; 
    do_test(HASH_ALGORITHM_3);
    plik << td_end;
    plik << tr_end;
}


int main(int argc, char *argv[])
{	
	table_size = 5000;
	hash_alg = 0;
	
	plik.open( "results.html", std::ios::out );
	
	parse_cmd_line(argc, argv);
	
	if (hash_alg == HASH_ALGORITHM_ALL)
	{
	  three_tests();
	}
	else
	  do_test(hash_alg);
	vect.clear();
	
	plik.close();
	
	return 0;
}
