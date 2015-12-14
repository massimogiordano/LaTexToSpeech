#include "xml.h"
#include "pattern_search.h"
#include "string.h"
#include "LaTex.h"


using namespace std;

void print_html(ofstream &output, TreeNode * p, string parent){

    TreeNode *temp = p;

    output << "[{v:'" << temp <<"', f:'" << add_back_slash( temp->item )<< "'}, '" << parent <<"'],"<<endl;

    if(temp->right)
        temp = temp->right;

    if(p->child.size() > 0){
        for(auto i=0; i < p->child.size(); i++){
            temp=p->child[i];
            if(p->child.size()>1)
                //&#8595 this draw a nice arraw in the chart
                output << "[{v:'" << temp << "-"<< i  <<"', f:'&#8595'}, '" << p <<"'],"<<endl;

            stringstream out;
            if(p->child.size()>1)
                out << temp<<"-"<<i;
            else
                out<<p;
            string parent_new = out.str();
            print_html(output, p->child[i], parent_new);
        }
    }

    if(p->right) print_html(output, p->right, parent);
}

void print_tree_html(TreeNode *root, char *filename, string description){

    //print tree in HTML format
    char *filename_html = new char[100];
    sprintf(filename_html, filename);
    ofstream output (filename_html);
    output << description ;
    output << "<script id=\"text/javascript\"src=\"https://www.google.com/jsapi?autoload={'modules':[{'name':'visualization','version':'1.1','packages':['orgchart']}]}\"></script><div id=\"chart_div\"></div><script>google.setOnLoadCallback(drawChart);function drawChart() {var data = new google.visualization.DataTable();data.addColumn('string', 'Name');data.addColumn('string', 'Manager');data.addRows([";
    print_html(output, root, " ");
    output << "]);var chart = new google.visualization.OrgChart(document.getElementById('chart_div'));chart.draw(data, {allowHtml:true}, {allowCollapse:true}); } </script>";
    output.close();

}
