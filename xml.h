#ifndef XML_H
#define XML_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdbool.h>
#include <vector>

#include <LaTex.h>

using namespace std;
using std::vector;

/* pattern file is written as:
 * ID:{
 * some XML data structure
 * }
 * it return *?* that is the XML is contanuted in ID:{*?*}
 */

class xml
{
public:

    string arg_node;
    //associative array for XML attributes
    map<string,string> attributes;
    bool find(string attribute);

};


    int substring_first_position(const string& referenceString, const string& subString, int start_index);
    string pattern_by_id(int id, char *filename);
    string parser_XML_tree(string &xml, string &argument);
    xml XML_parser(string &xml);
    void xml_attributes(map<string, string> &attributes, string xml_string);

    void xml_data_builder(TreeNode *&node, vector<TreeNode*> &brother, string xml_tree);



#endif // PLANET_H
