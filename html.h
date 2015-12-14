#ifndef HTML_H
#define HTML_H

#include <str.h>
#include <iostream>
using namespace std;

void print_html(ofstream &output, TreeNode * p, string parent);
void print_tree_html(TreeNode *root, char *filename, string description);



#endif

