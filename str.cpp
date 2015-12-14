#include "xml.h"
#include "pattern_search.h"
#include "string.h"

#include <string>
#include <iostream>

using namespace std;

string delSpaces(string &str){
   str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
   return str;
}

string inside_quotes(string &text){
    string::size_type   start_position = 0;
    string::size_type    end_position = 0;


    start_position = text.find("\"");
    if (start_position != std::string::npos){
        // start after the double quotes.
        ++start_position;
        // look for end position;
        end_position = text.find("\"",start_position);
        if (end_position != std::string::npos){
            string temp = text;
            text = text.substr(end_position+1);
            return temp.substr(start_position, end_position - start_position );
       }
    }
}

string add_back_slash(string input){
    for(int i = 0; i<input.size(); i++){
        if(input[i] == '\\'){
            input  = input.substr(0, i+1) + input.substr(i, input.size());
            i++;
        }
    }
    return input;
}
