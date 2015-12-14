#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <cstring>
#include <cstddef>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <map>

#include <LaTex.h>
#include <xml.h>
#include <pattern_search.h>
#include <str.h>
#include <html.h>

using std::vector;
using namespace std;


#define ID_ARGUMENT 10

int main(void){
       string input = "\\lef \\left prova prova  d prova \\frac{ \\int_{d}^{s}\\sqrt[2]{143xc + 75\\ k}}{\\cos{(x \\sqrt[2]{143x + 75k})}i}= \\frac{\\sin{x}}{k\\sqrt[2]{2}}";
      //  string input = " \\left prova \\left(  \\right\\} \\right\\} d prova \\sin{prova}pr \\log{d}";

     TreeNode *root;
     root = NULL;

     cout << endl << input << endl << endl ;

     //this function creates a list of Token (linked eachother)
     create_token_list(input, root);


     //those functions found argumet between {**} or [**] or (**) and make childs
     token_from_list_to_tree(root, "{" , "}");
     token_from_list_to_tree(root, "[" , "]");
     token_from_list_to_tree(root, "(" , ")");

     //PRINT it generete a html page with a chart
     print_tree_html(root, "tree_after_parser.html", input);

     transform_arguments_into_child_following_structures(root, 0);

     print_tree_html(root, "tree.html", input);


     // pattern recognition

     TreeNode *pattern = NULL;
     pattern = new TreeNode("temp");
     cout << endl << "it did not crased!"<< endl;

     //parser XML tree
     string xml_tree;
     xml_tree = pattern_by_id(10, "xml.txt");

     TreeNode *restore = pattern;

     //pattern = new TreeNode("ciao");
     vector<TreeNode*>null;

     xml_data_builder(pattern, null, xml_tree);




     //parser attributes
     string temp = "ciao = \"ciao  \" prova = \"prova\" dd=\"dd\"";
     map<string,string> no;
     no["prova"] = "prova";
     xml_attributes(no, temp);

     cout << ">>"<< no["ciao"] << endl;

     map<string, string>::iterator curr,end;
     for(curr = no.begin(); curr != no.end() ; ++curr){
            std::cout << curr->first << " == " << curr->second << std::endl;
      }

     if(restore != NULL)
     print_tree_html(restore, "xml.html", "prova");
     cout << endl <<  "end xml" << endl;

     return 0;
}

