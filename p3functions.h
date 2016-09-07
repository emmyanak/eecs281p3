#include <string>
#include <string.h>
#include <strings.h>
#include <cstddef>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

struct Entry{

//int tstamp; 
long long int tstamp;
string string_time; 
string cat;
string msg;
int entryID; 

};

long long int char_to_int(char c)
{
	long long temp = (long long)c;
	return temp - 48; 
}

long long int timestamp_converter(string t)
{
    long long int month = 0;
    long long int day = 0;
    long long int hour = 0;
    long long int min = 0; 
    long long int sec = 0;

	if(t[0] == '0')
	{
		month = char_to_int(t[1]);
	}
	else
	{
	    month = char_to_int(t[0])*10 + char_to_int(t[1]);
	}
	if(t[3] == '0')
	{
		day = char_to_int(t[4]);
	}
	else
	{
	    day = char_to_int(t[3])*10 + char_to_int(t[4]);
	}
	if(t[6] == '0')
	{
		hour = char_to_int(t[7]);
	}
	else
	{
	    hour = char_to_int(t[6])*10 + char_to_int(t[7]);
	}
	if(t[9] == '0')
	{
		min = char_to_int(t[10]);
	}
	else
	{
	    min = char_to_int(t[9])*10 + char_to_int(t[10]);
	}
	if(t[12] == '0')
	{
		sec = char_to_int(t[13]);
	}
	else
	{
	    sec = char_to_int(t[12])*10 + char_to_int(t[13]);
	}

return (100000000*month + 1000000*day + 10000*hour + 100*min + sec); 

}

bool timestamp_compare(const Entry &entry1, const Entry &entry2) //return true if entry 2 higher than entry 1

{
	/*
	
   string tstr_1 = entry1.tstamp;
   string tstr_2 = entry2.tstamp;
   
   tstr_1.erase(std::remove(tstr_1.begin(), tstr_1.end(), ':'), tstr_1.end());
   tstr_2.erase(std::remove(tstr_2.begin(), tstr_2.end(), ':'), tstr_2.end());
   */
   /*
   cout << tstr_1 << endl;  
   cout << tstr_2 << endl; 
   */
   /*
   unsigned long int timestamp1 = std::stol(tstr_1);
   unsigned long int timestamp2 = std::stol(tstr_2);
    */

   if (entry1.tstamp == entry2.tstamp)
   {
      if (entry1.cat == entry2.cat)
      {
      	return entry1.entryID < entry2.entryID; 
      }
      return entry1.cat < entry2.cat; 
   }
   return entry1.tstamp < entry2.tstamp; 
}


//functor for sorting the original vector

long int timestamp_search(string t1, string t2, vector<Entry> &v, vector<Entry>::iterator &low_it, vector<Entry>::iterator &high_it) //would be much faster on a sorted vector
{

//cout << "TIMESTAMP SEARCH CALLED " << endl; 
   long long int time_low = timestamp_converter(t1);
   long long int time_high = timestamp_converter(t2);
   
   vector<Entry>::iterator vector_iterator;

   /*
   t1.erase(std::remove(t1.begin(), t1.end(), ':'), t1.end());
   t2.erase(std::remove(t2.begin(), t2.end(), ':'), t2.end());
   

   unsigned long int timestamp1 = stol(t1);
   unsigned long int timestamp2 = stol(t2);
   unsigned long int entry_timestamp = 0; 
   string string_tstamp = "whatever";
   */
   
   Entry dummy;
   dummy.tstamp =  time_low; 
   low_it = lower_bound(v.begin(), v.end(), dummy, timestamp_compare);
   dummy.tstamp = time_high;
   high_it = lower_bound(v.begin(), v.end(), dummy, timestamp_compare); //maybe change this to lower bound also 
  
   //cout << "TIMESTAMP SEARCH ENDED" << endl;
   return high_it-low_it; 

}

//CURRENTLY CASE SENSITIVE

int category_search(string look_for, unordered_map<string, vector<int> > &category_map, vector<int>::iterator &start, vector<int>::iterator &end)
{
    //cout << "CAT SEARCH BEGIN " << endl;
   std::transform(look_for.begin(), look_for.end(), look_for.begin(), ::tolower);

     
	if (category_map[look_for].size() == 0)//nothing was found
	{
     start = category_map[look_for].begin();
     end = category_map[look_for].end();
		return 0; 
	}

    start = category_map[look_for].begin();
    end = category_map[look_for].end();

    //cout << "CAT SEARCH END " << endl; 
    return (int)(end-start);

}


vector<string> string_chop_one(string victim1)
{
    vector<string> b; 
	int begin = 0;
	int end = 0;
	int found = false; 
    int i = 0; 
    string lowercase_substring = "fuckme"; 

	while (i != (int)victim1.size())
	{
	    //cout << "victim at index " << i << ": " << victim1[i]  << endl;
		if (isalnum(victim1[i]) != 0 && found == false)
		{
			//cout << " begin: " << i << endl; 
			begin = i; 
			found = true;
		}
		else
		{
			if (isalnum(victim1[i]) == 0)
			{
				//cout << " according to islanum " << victim1[i] << " is not alphanumeric " << endl; 
				//cout << " end: " << i << endl; 
				end = i; 
			}
			if (end > begin && found == true)
			{
				lowercase_substring = victim1.substr(begin, end-begin);
				std::transform(lowercase_substring.begin(), lowercase_substring.end(), lowercase_substring.begin(), ::tolower);
				b.push_back(lowercase_substring);
				found = false; 
			}
		}
		i++;
	}

	if (found == true)
	{
		end = (int)victim1.size(); 
		lowercase_substring = victim1.substr(begin, end-begin);
		std::transform(lowercase_substring.begin(), lowercase_substring.end(), lowercase_substring.begin(), ::tolower);
		b.push_back(lowercase_substring);
	} 

	return b; 
}



vector<string> string_chop_two(string victim1, string victim2)
{

	vector<string> a = string_chop_one(victim1);
	vector<string> b = string_chop_one(victim2);

	vector<string> combined = a;


	for (int i = 0; i < (int)b.size(); i++)
       {
       	combined.push_back(b[i]);
       }
	return combined; 
}


int keyword_search(string keyword_in, std::unordered_map<string, vector<int> > &keyword_map, vector<int> &k_searched, vector<int>::iterator &k_begin, vector<int>::iterator &k_end)
{
	//cout << " KEYWORD SEARCH BEGIN " << endl; 
	k_searched.clear(); 

	vector<string> parsed_keywords = string_chop_one(keyword_in);

	std::vector<int>::iterator vec_it;

	int i = 1; 

    k_searched = keyword_map[parsed_keywords[0]]; 

	while (i < (int)parsed_keywords.size())
	{
	  vec_it = set_intersection(k_searched.begin(), k_searched.end(), keyword_map[parsed_keywords[i]].begin(), keyword_map[parsed_keywords[i]].end(), k_searched.begin());
	  k_searched.resize(vec_it-k_searched.begin());
    
	  if((int)k_searched.size() == 0)
	  {
	  	return 0; 
	  }
    
	i++;
	}//end of while loop 

    k_begin = k_searched.begin();
    k_end = k_searched.end(); 
    // cout << " out of while " << endl; 
   // cout << "KEYWORD SEARCH END " << endl; 
	return (int)k_searched.size(); 

}

///SEARCH FUNCTIONS ^^^^^^


///EXCERPT LIST EDITING \/\/\/\/\/\/\/

void append_log_entry(vector<Entry> &entries, vector<int> &excerpt, int id_num)
{
   //cout << " append log entry begin " << endl; 
   bool stop = false; 
   int i = 0; 
   int stored_value = 0; 
   
   while(i < (int)entries.size() && stop == false)
   {
      if(entries[i].entryID == id_num)
      {
      	stop = true;
      	stored_value = i; 
      }
      i++; 
   }
   excerpt.push_back(stored_value);
   //cout << " append log entry end " << endl; 
   return; 

}


int append_key_search(vector<int>::iterator &start, vector<int>::iterator &end, vector<int> &excerpt)
{

  // cout << " append key search begin " << endl; 
   vector<int>::iterator temp = start; 
   while(start != end)
   {
   	excerpt.push_back(*start); 
   	start++;
   }
   start = temp; //restor start vector

   //cout << " append key search end " << endl; 
  // cout << (int)(end- start);
   return (int)(end-start); 

}

int append_cat_search(vector<int>::iterator &start, vector<int>::iterator &end, vector<int> &excerpt)
{
	//cout << "append category search begin " << endl;

   vector<int>::iterator temp = start; 
   while(start != end)
   {
   	  excerpt.push_back(*start);
   	  start++; 
   }
   start = temp; //restore start vector
  // cout << "append cat search end " << endl; 
  // cout << (int)(end-start);
   return (int)(end-start); 

}

int append_time_search(vector<Entry>::iterator &start, vector<Entry>::iterator &end, vector<Entry>::iterator &beginning_of_log, vector<int> &excerpt)
{

  //cout << "append time search begin " << endl; 
   vector<Entry>::iterator temp = start; 
   while(start != end)
   {
   	  excerpt.push_back((int)(start - beginning_of_log));
   	  start++; 
   }
   start = temp; 
   //cout << (int)(end-start) << endl; 
   return (int)(end-start); 
   //cout << " append time search end " << endl; 

}

void delete_log_entry(int int_in, vector<int> &excerpt)
{
	if (int_in == (int)excerpt.size()-1) //check dis
	{
		excerpt.pop_back(); 
		return; 
	}
    excerpt.erase(excerpt.begin() + int_in);
	return; 
}

void move_to_beginning(int int_in, vector<int> &excerpt)
{
	
	int temp; 
	temp = excerpt[int_in];
	delete_log_entry(int_in, excerpt);
	excerpt.insert(excerpt.begin(), temp);
	
	return;
}

void move_to_end(int int_in, vector<int> &excerpt)
{
	int temp; 
	temp = excerpt[int_in];
	delete_log_entry(int_in, excerpt);
	excerpt.push_back(temp);
	return; 
}

void sort_excerpt_list(vector<int> &excerpt)
{
    std::sort(excerpt.begin(), excerpt.end());	
	return; 
}

void clear_excerpt_list(vector <int> &excerpt)
{
	excerpt.clear();
	return;
}

void print_time_search(ostream& ss, vector<Entry> &entries, vector<Entry>::iterator &time_begin, vector<Entry>::iterator &time_end) 
{
    vector<Entry>::iterator temp = time_begin; 
    int indx = (int)(time_begin - entries.begin()); //starting point 
    //cout << " index starting " << indx << endl;  
    while(time_begin != time_end)
    {
    	//cout << " index " << indx << endl; 
    	ss <<  entries[indx].entryID << "|" << entries[indx].string_time << "|";
    	ss << entries[indx].cat << "|" << entries[indx].msg << '\n';

    	indx++;
    	time_begin++;
    }
	return;
}

void print_cat_search(ostream& ss, vector<Entry> &entries, vector<int>::iterator &start_cat, vector<int>::iterator &end_cat) 
{
    
    while (start_cat != end_cat)	
    { 
    	ss <<  entries[*start_cat].entryID << "|" << entries[*start_cat].string_time;
    	ss << "|" << entries[*start_cat].cat << "|" << entries[*start_cat].msg << '\n';
    	start_cat++; 
    }
	return;
}

void print_key_search(ostream& ss, vector<Entry> &entries, vector<int>::iterator start_key, vector<int>::iterator end_key) 
{
    while (start_key != end_key)	
    {
    	ss <<  entries[*start_key].entryID << "|" << entries[*start_key].string_time;
    	ss << "|" << entries[*start_key].cat << "|" << entries[*start_key].msg << '\n';
    	start_key++; 
    }
	return;
}

void print_excerpt(ostream& ss, vector<int> &excerpt, vector<Entry> &entries)
{
    
    int index = 0; 
    for (int i = 0; i < (int)excerpt.size(); i++)	
    {
    	index = excerpt[i]; 
    	ss << i << "|" << entries[index].entryID << "|" << entries[index].string_time << "|" << entries[index].cat << "|" << entries[index].msg << '\n';
    }
	return;
}








































