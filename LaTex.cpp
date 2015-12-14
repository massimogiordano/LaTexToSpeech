#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <cstring>
#include <cstddef>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>

#include "xml.h"
#include "pattern_search.h"
#include "string.h"

using std::vector;
using namespace std;


#define ID_ARGUMENT 10



//function to parser the LaTex formula

void treeInsert(TreeNode *&root, string newItem, int id) {
   if ( root == NULL ) {
      root = new TreeNode( newItem );
      root-> id = id;
      return;
   }
   treeInsert( root->right, newItem, id);
}

int counter(TreeNode *node, string search_for){
        int count = 0;
        while (node != NULL)
        {
            if (node->item == search_for)
               count++;
               node = node->right;
        }
        return count;
    }

void token_from_list_to_tree(TreeNode *node, string argument_open, string argument_close) {

    TreeNode *begin = node;
    TreeNode *before;
    TreeNode *open;
    TreeNode *temp;
    open = before  = NULL;
    temp = node;

    //this loop let us look inside sub-tree (sub-list) from child argument "inside_argument"
    while (temp){
        if(temp->child.size() == 1){
            token_from_list_to_tree(temp->child[0], argument_open, argument_close);
        }
        temp = temp->right;
    }


    //controllo che numero di { sia uguale al numero di } e diverso da zero.
    if(counter(node, argument_open) == counter(node, argument_close) && counter(node, argument_open) != 0 ){
        while (node->item != argument_close){
               if (node->item == argument_open){
               open = node;
               before = temp;
            }
               temp = node;
               node = node->right;
        }

        open->item += argument_close;
        open->child.push_back(open->right);
        open->child[0] = open->right;
        open->right = node->right;
        temp->right = NULL;

        token_from_list_to_tree(begin, argument_open, argument_close);

    }
    temp = begin;
}

int give_line_of_id(int id){

    ifstream file("structures.txt");
    string str;
    int line = 0;
    int stop = 0;

    while (getline(file, str) && stop==0){
        str = str.substr(0,7); //#number of digit id
        line++;
        if(id ==  atoi( str.c_str() )) stop = 1;
    }

    if(stop == 0)
        return NULL;
    else
        return line;
}

string give_next_argument_by_id(int id, int next){

    string argument, temp;
    ifstream file("structures.txt");


    for(int i=0; i<give_line_of_id(id); i++){
        getline(file, argument);
    }

    argument = argument.substr(argument.find(',')+1);

    for(int i=0; i<=next; i++ ){
        temp = "";
        if(argument.find(',') < 100){ //this could be wrong. but if there is not , .find(',') return very big number
            temp =  argument.substr(0, argument.find(','));
            argument = argument.substr( argument.find(',')+1);

        }else{
            if(argument.find(';') < 100){
                temp =  argument.substr(0, argument.find(';'));
                argument = "";
            }else{
                i = next +1;
                temp = "";
            }
        }

    }

    return temp;

}

int give_id_structure(string structure, int id){

    ifstream file("structures.txt");
    string str;
    int line = 0;
    int lenght_id = 10;
    int id_found = 0;

    int start = 0;
    if(id != NULL)  start = give_line_of_id(id);
    while (getline(file, str) && id_found == 0){
            line++;
            if(line>start){
                if(str.substr(lenght_id, str.find(',')-lenght_id) == structure){
                    str = str.substr(0,7); //#number of digit id
                    id_found =  atoi( str.c_str() );
                }
            }
     }

    if(id_found != 0) return id_found;
    else return 0;

}

void copy_object(TreeNode *one, TreeNode *two){

    one->child = two->child;
    one->id    = two->id;
    one->item  = two->item;
    one->right = two->right;

}

bool give_pointer_of_argument(TreeNode *& argument, TreeNode *& next, string structure){

    if(argument->item == structure){
        if(argument->child.size() > 0){    //if is {}->** it has only 1 child
            next = argument->right;
            copy_object(argument,argument->child[0]) ;
            return true;
        }else{
            return false;
        }

    }else{
        //I do not like it at all!
        if(argument->id == ID_ARGUMENT){
            return false;
        }else{
            string item = argument->item;
            if(item[0] == '\\'){

            }else{

            }
        }
    }

}

void transform_arguments_into_child_following_structures(TreeNode *node, int tentatives){

    bool go;

    if(node->item[0] == '\\'){

        int id = give_id_structure(node->item, tentatives);
        //need to set condition su id- if the funzitn do not return enything stop here the funziont
        int ok = 0;

        TreeNode *temp;
        TreeNode *next;
        TreeNode *restore;
        vector<TreeNode*> child_temp;


        next    = node->right;
        restore = node->right;

        int num_arg = 0;
        string type_argument = give_next_argument_by_id(id, num_arg);

        //for(int n_child = 0; n_child < 1000;  n_child++){ //it can be a bad code - 1000 is set just as big number
        while(type_argument != ""){
            ok ++;

            if(node->right)
            temp = node->right;

            //type_argument = give_next_argument_by_id(id, n_child); da mettere in fondo

            if(type_argument == "_" || type_argument == "^"){
            if(temp->item == type_argument){
                if(temp->right){
                   temp = temp->right;
                   type_argument =  "{}"; //after _ or ^ I am expecting {**} something
                }else{
                    //not good - the string end (not space for argument)
                }
            }
            }


            go = give_pointer_of_argument(temp, next, type_argument);


            if(go){

                child_temp.push_back(temp);
                node->right = next;

            }else{
                cout << "false";

            }
            num_arg ++;
            type_argument = give_next_argument_by_id(id, num_arg);

    }//end of while type_argument exist

       if(go){
            node->child = child_temp;
            for(int i=0; i<child_temp.size(); i++)
                transform_arguments_into_child_following_structures(child_temp[i], 0);
            if(next!= NULL)
                transform_arguments_into_child_following_structures(next, 0);
       }else{
            if(next!= NULL)
                transform_arguments_into_child_following_structures(next, 0);
       }
    }else{
        if(node->right)
            transform_arguments_into_child_following_structures(node->right, 0);
        if(node->child.size()>0)
            transform_arguments_into_child_following_structures(node->child[0], 0);
    }


}

//PARSER FOR LATEX FORMULA - long fuction, it must be optimaze
void create_token_list(string input, TreeNode *&root){

    bool print = true;

    string token;
    string end_token_structure = "#&|ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string token_argument      = "_^{[()]}";
    string token_operator      = "=+-/*><,'";
    string tokne_number        = "0123456789";
    string alphabet              = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    input += ".";
    for(int i=0; i<input.size(); i++){

        //token for structure like \example
        if(input[i] == '\\'){
            string temp = input.substr(i+1, input.size());
            size_t end_of_token = temp.find_first_not_of(end_token_structure);
              if (end_of_token!=std::string::npos){
                string excemption = "";
                int add =0 ;
                //this considers when \structure\{ -> \structure{ (it is the same token)
                if( input.substr(i+end_of_token+1,2) == "\\{"  ){
                    excemption = "{";
                    add +=2;
                }
                if( input.substr(i+end_of_token+1,2) == "\\}" ){
                    excemption = "{";
                    add +=2;
                }
                token = input.substr(i, end_of_token+1) + excemption;
                treeInsert(root,token, 1);
                i += end_of_token+1 + add;

                if(print) cout << token << setw(42- token.size())  << "(structure)" << endl;
              }
        }

        //token for "_^{[()]}" (it means the begin or end of en argument)
        size_t end_of_token = token_argument.find(input[i]);
        if (end_of_token!=std::string::npos){
            token = input[i];
            treeInsert(root, token, ID_ARGUMENT);
            if(print) printf( "%-30c %s\n", input[i], "(argument)");

        }

        end_of_token = token_operator.find(input[i]);
        if (end_of_token!=std::string::npos){
            token = input[i];
            treeInsert(root, token, 2);
            if(print) printf( "%-30c %s\n", input[i], "(operator)" );
        }

        if(isdigit(input[i])){
            string temp = input.substr(i+1, input.size());
            size_t end_of_token = temp.find_first_not_of(tokne_number);
              if (end_of_token!=std::string::npos)
              {
                token = input.substr(i, end_of_token+1);
                treeInsert(root, token, 3);
                i += end_of_token;

                if(print) cout << token << setw(38 - token.size())  << "(value)" << endl;
              }

        }

        end_of_token = alphabet.find(input[i]);
        if (end_of_token!=std::string::npos){
            string temp = input.substr(i, input.size());
            size_t end_of_token = temp.find_first_not_of(alphabet);
              if (end_of_token!=std::string::npos){
                  token = input.substr(i, end_of_token);
                  treeInsert(root, token, 4);
                  i+= token.size()-1;

                  if(print) cout << token << setw(41 - token.size())  << "(variable)" << endl;
              }

        }
    }



}

