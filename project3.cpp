#include <string.h>
#include <string>
#include <algorithm> //transform 
#include <iostream>
#include <sstream>
#include <fstream>
#include "p3functions.h"
#include <unordered_map>
#include <stdio.h>
#include <ctype.h>

using namespace std; 

int main(int argc, char **argv) {


string filename = "file"; 
if (argc == 2) 

{
	if (strncmp(argv[1],"-h", 2) == 0 || strncmp(argv[1], "--help", 6) == 0)
	{
	  cout << " help output msg " << endl; 
	  exit(0);
	}
    filename = argv[1]; 
}


string timestamp = "sos";
string category = "sos"; 
string message = "sos"; 
int entryID = 0; 
stringstream os; 
//vector of structs
vector<Entry> entries; 
Entry data_entry; 

//entry list and search vectors 
int prev_search = 4;
vector<int> excerpt_list;

//iterators for searches 
vector<Entry>::iterator t_search_high;
vector<Entry>::iterator t_search_low;
vector<Entry>::iterator entries_begin; 
vector<int>::iterator category_search_begin;
vector<int>::iterator category_search_end;
vector<int>::iterator keyword_search_begin;
vector<int>::iterator keyword_search_end;


vector<int> keyword_search_results; 
//reading in the file

//cout << "GONNA OPEN FILE " << endl; 
ifstream infile;
infile.open(filename);

///int stop = 0; 

while (!infile.eof()) {
    //cout << "in loop " << endl; 
	getline(infile, timestamp, '|'); //timestamp

    if(timestamp != "")
    {
	data_entry.string_time = timestamp;
	data_entry.tstamp = timestamp_converter(timestamp);
	
	getline(infile, category, '|');//category 
	data_entry.cat = category; 

	getline(infile, message); //time stamp 
	data_entry.msg = message;
 

	data_entry.entryID = entryID;

	entries.push_back(data_entry); 
	entryID++;
    } 
}

infile.close(); 
os << (int)entries.size() << " entries read" << '\n'; 

std::sort(entries.begin(), entries.end(), timestamp_compare);


//unordered map for keyword search 
unordered_map<string, vector<int>> keywords;
unordered_map<string, vector<int>> categories; 

vector<string> temp; 
int j = 0; 
string lowercase_cat; 





for (int i = 0; i < (int)entries.size(); i++)
{
  j = 0; 
  
  temp = string_chop_two(entries[i].cat, entries[i].msg);
  
  lowercase_cat = entries[i].cat;
  std::transform(lowercase_cat.begin(), lowercase_cat.end(), lowercase_cat.begin(), ::tolower);
  categories[lowercase_cat].push_back(i);

  while (j != (int)temp.size())
  {
       auto itr = keywords.find(temp[j]);  // unordered_map<string, vector<int> >::iterator 

	   if (itr != keywords.end()) //this word is in it alreADY 
	   {
		    if ((itr->second).back() != i)
		    {
		  	   keywords[temp[j]].push_back(i);
		    }
	   }
	   else 
	   {
	    	keywords[temp[j]].push_back(i);
	   }
	    
	   
    j++;    
  }//while loop for strings 
}


bool search_occured = false; 
char user_input = 'w';
int user_integer; 
long int timestamp_search_out; 
string timestamp1;
string timestamp2; 
string category_input; 
string keyword_input; 
string unwanted; 
int output_int = 0; 
string blank_;

int i = 0; 

entries_begin = entries.begin();



while(user_input != 'q') {
	os << '%' << " ";  
    cin >> user_input; 
    i++; 
	switch (user_input)
	{
		case 't': //timestamp search
		    
		    ws(cin);
		    getline(cin, timestamp1, '|');
		    getline(cin, timestamp2);
		 
			if (timestamp1.size() != 14|| timestamp2.size() != 14) //check that both are 14 characters 
			{
				cerr << "Error: Invalid time search command" << '\n';
				break; 
			}
			search_occured = true; 
			prev_search = 0; 
			timestamp_search_out = timestamp_search(timestamp1, timestamp2, entries, t_search_low, t_search_high); 
			os << timestamp_search_out << " entries found" << '\n'; 
			//cout << " leaving timestamp " << endl; 
			break; 
		case 'c':
		    search_occured = true; 
		    prev_search = 1;
		    ws(cin);
		    getline(cin, category_input);
		    output_int = category_search(category_input, categories, category_search_begin, category_search_end);
		    os << output_int << " entries found" << '\n'; 
		    break; 
		case 'k':
		    search_occured = true; 
		    prev_search = 2;
		    getline(cin, keyword_input); 
		    output_int = keyword_search(keyword_input, keywords, keyword_search_results, keyword_search_begin, keyword_search_end);
		    os << output_int << " entries found" << '\n'; 
		    break; 
		case 'a': //append log entry 
		    cin >> user_integer;
		    if (user_integer < 0 || user_integer >= (int)entries.size())  
		    {
				cerr << "Error: Invalid append log entry command i" << '\n';
			}
			append_log_entry(entries, excerpt_list, user_integer);
            os << "log entry " << user_integer << " appended" << '\n';
			break; 
		case 'r': //append search results
		    if (!search_occured)
		    {
		    	cerr << "Error: Invalid command" << '\n';
		    	break; 
		    }
		    if(prev_search == 0) //TIMESTAMP SEARCH OCCURED PREVIOUSLY 
		    { 
		    	output_int = append_time_search(t_search_low, t_search_high, entries_begin, excerpt_list);
		    }
		    if(prev_search == 1) //CATEGORY SEARCH PREVIOUSLY 
		    {
		    	output_int = append_cat_search(category_search_begin, category_search_end, excerpt_list); 

		    }
		    if(prev_search == 2)//KEYWORD SEARCH PREVIOUSLY 
		    {
		    	keyword_search_begin = keyword_search_results.begin();
		    	keyword_search_end = keyword_search_results.end(); 
                output_int = append_key_search(keyword_search_begin, keyword_search_end, excerpt_list); 
		    }
		    os << output_int << " log entries appended" << '\n';
		    break; 
		case 'd': //delete log entry 
		    cin >> user_integer; 
		     if (user_integer < 0 || user_integer >= (int)excerpt_list.size())
		    {
		    	cerr << "Error: Invalid command " << endl; 
		    	break; 
		    }
		    delete_log_entry(user_integer, excerpt_list);
		    os << "excerpt list entry " << user_integer << " deleted" << '\n'; 
		    break; 
		case 'b': //move entry to begining 
		    cin >> user_integer; 
		     if (user_integer < 0 || user_integer >= (int)excerpt_list.size())
		    {
		    	cerr << "Error: Invalid command " << endl; 
		    	break; 
		    }
		    move_to_beginning(user_integer, excerpt_list);
		    os << "excerpt list entry " << user_integer << " moved" << '\n'; 
		    break; 
		case 'e': //move entry to end 
		    cin >> user_integer; 
		    if (user_integer < 0 || user_integer >= (int)excerpt_list.size())
		    {
		    	cerr << "Error: Invalid command " << endl; 
		    	break; 
		    }
		    move_to_end(user_integer, excerpt_list);
		    os << "excerpt list entry " << user_integer << " moved" << '\n'; 
		    break; 
		case 's':
		    sort_excerpt_list(excerpt_list);
		    os << "excerpt list sorted" << '\n';
		    break; 
		case 'l':
		    clear_excerpt_list(excerpt_list);
		    os << "excerpt list cleared" << '\n';
		    break; 
		case 'g': //
		   // cout <<  " came to g " << endl; 
		    if (!search_occured)
		    {
		    	cerr << "Error: Invalid command" << '\n';
		    	break; 
		    }
		    if(prev_search == 0) //TIMESTAMP SEARCH OCCURED PREVIOUSLY 
		    {
		    	//cout << " going to print time " << endl; 
		    	print_time_search(os, entries, t_search_low, t_search_high);
		    	//cout << " not back " << endl; ///os
		    }
		    if(prev_search == 1) //CATEGORY SEARCH PREVIOUSLY 
		    {
		    	print_cat_search(os, entries, category_search_begin, category_search_end); //os
		    }
		    if(prev_search == 2)//KEYWORD SEARCH PREVIOUSLY 
		    {
                print_key_search(os, entries, keyword_search_begin, keyword_search_end); //
		    }
		    break;
		case 'p':
			print_excerpt(os, excerpt_list, entries);
			break;   
		case '#':
		    getline(cin, blank_);
		    break;
		case 'q':
		    cout << os.str();
		    exit(0); 
		    break;
		default:
		    getline(cin, unwanted);
		    cerr << "Error: Invalid command" << '\n';
		    break;
		}//end of switch 


}//end of while 

cout << os.str(); 

}












































