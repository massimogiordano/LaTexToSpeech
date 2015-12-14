#include "xml.h"
#include "pattern_search.h"
#include "str.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdbool.h>

using namespace std;

/* pattern file is written as:
 * ID:{
 * some XML data structure
 * }
 * it return *?* that is the XML is contanuted in ID:{*?*}
 */
string pattern_by_id(int id, char*filename){
    string line;
    string pattern = "";
    char *filename_html = new char[100];
    sprintf(filename_html, filename);
    ifstream myfile (filename_html);
    if (myfile.is_open()){

      while ( getline (myfile,line) ){
          // convert 10:{ .. all the pattern .. } int decimal 10
          int pattern_id = atoi(line.substr(0, line.find(":")).c_str());

          if(pattern_id == id){
              while(getline (myfile,line)){

                  if(line[0] == '}') break;
                  pattern.append(line);
              }
          break;
          }

      }
      //I might want delete the space here
      myfile.close();
    }else cout << "Unable to open file";


    return pattern;
}

int substring_first_position(const string& referenceString, const string& subString, int start_index) {

    int leght = subString.size();
    int max_leght = referenceString.size();

    int i = start_index;

    while(i<max_leght){
        string temp = referenceString.substr(i,leght);
        if(temp==subString)
        {
            break;
        }
        i++;
    }

    if(i == max_leght) return 1000000;
    else return i;
}

/* es of input: <arg> ..?.. </arg> <after/><nodes/>
 * the function return the string "arg"
 * ..?.. (that is the contentent of the argument)
 * the string xml is modified into: <after/><nodes/>
 */
xml XML_parser(string &xml){ //add attribute assiciate vecotor

    xml::xml xml_node;

    string node;
    string attributes="";
    string argument;

    if(xml.find("<") < 10^6 && !xml.empty()){ //the function .find() return a big number if does not find anything

        xml = xml.substr(xml.find("<"));
        //can be both <node> or <node attributes="something">
        int i=0, start = 0;

        while(xml[i] != ' ' && xml[i] !='>'&& xml[i] != '/' ) i++;
        while(xml[i + start] !='>') start++;

        node = xml.substr(0,i); //node = "<node"
                                   //end_node = "</node"

        //node as <node  /> has no child
        if(xml[i + start - 1] == '/'){

            //attribute have to be parser yet
            //set of information for xml object
            argument = "";
            attributes = xml.substr(i+1,i+start-1); // <node ..?.. />
            xml = xml.substr(i+start+1);
            node = node.substr(1);

        }else{
            attributes = xml.substr(i+1,i+start); // <node ..?.. />
            string end_node = "</"+node.substr(1);

            //look if there are childs with the same node

            int index = 1;
            int pos = i + start;

            //this loop select all the argument
            while(index !=0 && pos < xml.size()){
                int sub_node_open  = substring_first_position(xml,    node, pos);
                int sub_node_close = substring_first_position(xml,end_node, pos);
                int singol_node    = substring_first_position(xml,"/>", pos);

                int control = 0;
                if(sub_node_open < xml.size())
                    control = substring_first_position(xml,">", sub_node_open );

                if(singol_node +1 != control){
                    if(sub_node_open<sub_node_close){
                        index ++;
                        pos = sub_node_open + 1;
                    }else{
                        index --;
                        pos = sub_node_close + 1 ;
                    }
                }else{
                    pos = control;
                }


        }

        if(pos < xml.size()){
            argument = xml.substr(i+start+1, pos-(i+start+2)); //selection of argument
        }else{
            cout << "error in parser_XML_tree()";
        }

        int end = substring_first_position(xml, ">", pos-1);
        xml = xml.substr(end+1);
        node = node.substr(1);

       }

    }else{// if the string do not contain any "<"
        xml = "";
        node = "";
        argument = "";
    }

    //convert string of only space into empty string
    if(argument.find_first_not_of(' ') != std::string::npos){}else{ argument = ""; }
    if(xml.find_first_not_of(' ') != std::string::npos){}else xml = "";

    //creace xml objact with value
    xml_node.arg_node = argument;
    xml_node.attributes["item"] = node;

    map<string,string> attributes_array;
    xml_attributes(attributes_array, attributes);


    xml_node.attributes = attributes_array;

    return xml_node;

}

bool xml::find(string attribute_name){
    bool find= false;
    map<std::string, std::string>::iterator curr, end;
    for(curr = attributes.begin(), end = attributes.end();  curr != end;  ++curr){
        if(curr->first == attribute_name) find = true;
    }

    return find;


}

void xml_attributes(map<string,string> &attributes, string xml_string){

    //if string contain attrib = "value"
    if (xml_string.find('=') != string::npos){

        string xml_attrib = xml_string.substr(0, xml_string.find("="));
                xml_string = xml_string.substr(xml_string.find("=")+1);

        attributes[delSpaces(xml_attrib)] = inside_quotes(xml_string);
        xml_attributes(attributes,xml_string);
    }
}

void xml_data_builder(TreeNode *&node, vector<TreeNode*> &brother, string xml_tree){

    xml xml_node =  XML_parser(xml_tree);

    if(xml_node.attributes["item"] != "str" ){
        node->item = xml_node.attributes["item"];
        node->attributes = xml_node.attributes;
        TreeNode *temp, *temp2;
        temp = new TreeNode();

        if(!xml_node.arg_node.empty()){
            node->child.push_back(temp);
            temp2 = node->child[node->child.size()-1];
            xml_data_builder(temp2, node->child ,xml_node.arg_node);
        }

    }else{  // if there is <str> are not chil but node.right

        if(!xml_node.arg_node.empty()) {

            string list_of_arg = xml_node.arg_node;

            while(!list_of_arg.empty()){

               xml list = XML_parser(list_of_arg);
               cout << list_of_arg << endl;

               if(node!=NULL){

                    node->item = list.attributes["item"];
                   if(!list_of_arg.empty())
                    node->right = new TreeNode();

               }

               if(!list.arg_node.empty()){
                   TreeNode *temp;
                   node->child.push_back(temp);
                   node->child[node->child.size()-1] = new TreeNode();
                   xml_data_builder(node->child[node->child.size()-1],node->child,list.arg_node);
               }

               node = node->right;

            }
        }
    }

    if(!xml_tree.empty()){
        TreeNode *child_temp= new TreeNode();
        brother.push_back(child_temp);
        xml_data_builder(brother[brother.size()-1], brother ,xml_tree);

    }

}
