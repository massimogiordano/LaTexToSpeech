#ifndef LATEX_H
#define LATEX_H

#include <str.h>
#include <iostream>
#include <sstream>

using namespace std;

struct TreeNode {

   string item;                   // Name of the structure.
   int id;
   TreeNode *right;              // Pointer to the next element of the liked list of token.

   /*this is to organize argument of a certain Latex' strucutre
    *Ex. \frac{value one}{\sqrt{2 + 3}}
    *ROOT = \frac
    *    --> value               (child 1.0)   sub.string
    *           -->one           (right 1.1)
    *    --> \sqrt               (child 2.0)
    *           -->2              (right 2.1.0) sub.string
    *           -->+              (right 2.1.1)
    *           -->3              (right 2.1.2) end of sub.string [right = NULL]
    *
    * The vector "child" contains pointers to the first
    *  element of the string (list of token)
    * inside each argument of a LaTex structure.
    */

   vector<TreeNode*> child;
   //This associative array is for the pattern search
   map<string,string> attributes;

   TreeNode(string str = "") {
      item = str;
      id = 0;
      right = NULL;
   }
};

void treeInsert(TreeNode *&root, string newItem, int id);
void create_token_list(string input, TreeNode *&root);
void token_from_list_to_tree(TreeNode *node, string argument_open, string argument_close);
void copy_object(TreeNode *one, TreeNode *two);
void transform_arguments_into_child_following_structures(TreeNode *node, int tentatives);

string give_next_argument_by_id(int id, int next);

int counter(TreeNode *node, string search_for);
int give_line_of_id(int id);
int give_id_structure(string structure, int id);

bool give_pointer_of_argument(TreeNode *& argument, TreeNode *& next, string structure);



#endif

