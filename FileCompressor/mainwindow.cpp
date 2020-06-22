#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<bits/stdc++.h>

#include <QFileDialog>
#include<bitset>
using namespace std;

string filecontent="";
string treecontent="";

string huffmanfile ="";
string huffmantree="";


class HuffTree{
public:
    int val;
    char letter;
    HuffTree *left;
    HuffTree *right;

    HuffTree(int val, char c){
        this->val = val;
        this->letter=c;
        this->left = NULL;
        this->right = NULL;
    }
};

void InOrder(HuffTree *tree){
    if(tree){
        InOrder(tree->left);
        cout<<tree->val<<endl;
        InOrder(tree->right);
    }
}

class Compare
{
public:
    bool operator() (HuffTree* a, HuffTree* b)
    {
        if(a->val>b->val)
            return true;
        else
        {
            return false;
        }

    }
};


std::vector<std::string> tokenizer( const std::string& p_pcstStr, char delim )  {
        std::vector<std::string> tokens;
        std::stringstream   mySstream( p_pcstStr );
        std::string         temp;

        while( getline( mySstream, temp, delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
    }


std::vector<std::string> split(std::string const &input) {
    std::istringstream buffer(input);
    std::vector<std::string> ret;

    std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(ret));
    return ret;
}

vector <pair<int, char>> FrequencyDect (string s){
    map<char,int> mp;
    for(int i=0;i<s.size();i++){
        mp[s[i]]++;
    }
    vector <pair<int, char>> v;
    for(int i=0;i<s.size();i++){
        if(mp[s[i]]!=0){
            v.push_back(make_pair(mp[s[i]],s[i]));
            mp[s[i]]=0;
        }
    }
    return v;

}

void print (priority_queue<HuffTree*, vector<HuffTree*>, Compare> q){
    while(q.size()){
        cout<<q.top()->letter<<" "<<q.top()->val<<endl;
        q.pop();
    }
}

HuffTree *makeTree(vector<pair<int,char>> &frq){
    priority_queue<HuffTree*, vector<HuffTree*>, Compare> q;
    for(int i=0;i<frq.size();i++){
        q.push(new HuffTree(frq[i].first,frq[i].second));
    }
    //print(q);
    while(q.size()>1){
        HuffTree *left = q.top();
        q.pop();
        HuffTree *right = q.top();
        q.pop();

        HuffTree *nd = new HuffTree(left->val+right->val,'~');
        nd->left = left;
        nd->right=right;
        q.push(nd);
    }


    return q.top();
}

void getValues(HuffTree *root, string s, map<char,string>& vtr){
    if(!root){
        return;
    }
    if(root->letter!='~'){
        vtr[root->letter]=s;
        return;
    }
    else{
        getValues(root->left,s+"0",vtr);
        getValues(root->right,s+"1",vtr);
    }

}

void Serialize( HuffTree *root, string& s){
    if(!root){
       // cout<<"# # ";
        s+="# # ";
    }
    else{
       // cout<<root->letter<<" "<<root->val<<" ";
        s+=root->letter;
        s+=" ";
        s+=to_string(root->val);
        s+=" ";
       // s= s+string(root->letter)+" "+to_string(root->val)+" ";
        Serialize(root->left,s);
        Serialize(root->right,s);
    }
}

void Space2UnderScore(string &s){
    for(int i=0;i<s.size();i++){
        if(s[i]==' ') //|| s[i]=='\t' || s[i]=='\n' || s[i]=='\b' || s[i]=='\v' || s[i]=='\r' || s[i]=='\f' || s[i]=='\a')
            s[i]='_';
    }
}

void UnderScore2Space(string &s){
    for(int i=0;i<s.size();i++){
        if(s[i]=='_')
            s[i]=' ';
    }
}

std::string bin2ascii(std::string data){
    int cnt=0;
    while(data.size()%8!=0){
        data+='0';
        cnt++;
    }
    std::stringstream sstream(data);
    std::string output;
    while(sstream.good())
    {
        std::bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output += c;
    }
    output=output+std::to_string(cnt);
    return output;
}

string char2bin( char c){
    string str="";
       for (int i = 7; i >= 0; --i) {
       ((c & (1 << i))? str+='1' : str+='0');
    }
    return str;
}
string strToBinary(string s)
{
    int n = s.length();
    int dj = (s[s.size()-1])-'0';
    s.pop_back();

    string ret ="";


    for (int i = 0; i < n-2; i++)
    {
        string dj = char2bin(s[i]);
        //cout<<dj<<endl;
        ret=ret+dj;
    }

    while(dj--){
        ret.pop_back();
    }


    return ret;
}

string decode(HuffTree *tree, string s, int &i){
    string str="";

    while(i<=s.size()){
    if(tree->letter!='~'){
        str+=tree->letter;
        return str;
    }
    else if(s[i]=='0'){
        if(tree->left){
            tree=tree->left;
            i++;
        }
        else{
            return str;
        }
    }
    else if(s[i]=='1'){
        if(tree->right){
            tree=tree->right;
            i++;
        }
        else{
            return str;
        }
    }
    }
    return "";
}

void deSerialize(HuffTree* &root, fstream &f){
    string string_of_letter;
    string string_of_val;
    f>>string_of_letter;
    f>>string_of_val;

    if(string_of_letter=="#" || string_of_val=="#"){
        return;
    }
    root = new HuffTree(stoi(string_of_val),string_of_letter[0]);
    deSerialize(root->left,f);
    deSerialize(root->right,f);
}

void deSerialize(HuffTree* &root, fstream &f, int& i, vector<string> &v){
    string string_of_letter=v[i];
    string string_of_val=v[i+1];
    i+=2;


    if(string_of_letter=="#" || string_of_val=="#"){
        return;
    }
    root = new HuffTree(stoi(string_of_val),string_of_letter[0]);
    deSerialize(root->left,f,i,v);
    deSerialize(root->right,f,i,v);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->encode->isChecked()){
        try{

            QString filename = QFileDialog::getOpenFileName(
                        this,
                        tr("Open Text File"),
                        "/home",
                        "Text File (*.txt)"
                        );

            std::ifstream t(filename.toLocal8Bit().constData());
            std::string s((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());

            Space2UnderScore(s);
            vector <pair<int, char>> frq = FrequencyDect(s);
            HuffTree *tree = makeTree(frq);
            map<char,string> vtr;
            getValues(tree,"",vtr);
            string serializer = "";
            Serialize(tree,serializer);
            string res="";
            for(auto i:s)
                res+=vtr[i];
            ui->textEdit_2->setText(QString(res.c_str()));
            res = bin2ascii(res);
            ui->textEdit->setText(QString(res.c_str()));
            filecontent=res;
            treecontent=serializer;
            ui->textEdit_3->setText(QString(serializer.c_str()));

        }
        catch(exception e){

        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->encode->isChecked()){
        try {
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save Huffman Encoded File"),
                                       "/home",
                                       tr("Huffman (*.huff *.hoff)"));
                fstream f;
                f.open(fileName.toLocal8Bit().constData(), ios::out);
                f<<filecontent;
                f.close();

            fileName = QFileDialog::getSaveFileName(this, tr("Save Huffman Tree File"),
                                       "/home",
                                       tr("Tree Huffman (*.tree)"));

            f.open(fileName.toLocal8Bit().constData(), ios::out);
            f<<treecontent;
            f.close();
            ui->textEdit->setText("");
            ui->textEdit_2->setText("");
            ui->textEdit_3->setText("");

        } catch (exception e) {

        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->decode->isChecked()){
        try {
            QString filename = QFileDialog::getOpenFileName(
                        this,
                        tr("Open HuffMan Tree File"),
                        "/home",
                        "Huffman (*.tree)"
                        );
            huffmantree = filename.toLocal8Bit().constData();

                std::ifstream t(huffmantree);
                std::string res((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());

            ui->textEdit->setText("");
          //  ui->textEdit_2->setText("");
            ui->textEdit_3->setText(QString(res.c_str()));

        } catch (exception e) {
            ui->textEdit->setText("");
            ui->textEdit_2->setText("");
            ui->textEdit_3->setText("");
        }

    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(ui->decode->isChecked()){
        try {
            QString filename = QFileDialog::getOpenFileName(
                        this,
                        tr("Open HuffMan File"),
                        "/home",
                        "Huffman (*.huff *.hoff)"
                        );
            huffmanfile = filename.toLocal8Bit().constData();

            std::ifstream t(huffmanfile);
            std::string res((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

        ui->textEdit->setText("");
      //  ui->textEdit_3->setText("");
        ui->textEdit_2->setText(QString(strToBinary(res).c_str()));

        } catch (exception e) {
            ui->textEdit->setText("");
            ui->textEdit_2->setText("");
            ui->textEdit_3->setText("");
        }

    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(ui->decode->isChecked()){
        try {
                std::ifstream t(huffmanfile);
                std::string res((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());
                res = strToBinary(res);

                cout<<res<<endl;

                std::ifstream t2(huffmantree);
                std::string mytree((std::istreambuf_iterator<char>(t2)),
                             std::istreambuf_iterator<char>());
                vector<string> v = tokenizer(mytree,' ');
                cout<<mytree<<endl;

                for(auto i:v)
                    cout<<i<<endl;


                fstream f;
                f.open(huffmantree, ios::in);
                HuffTree *root;
                int i=0;
                deSerialize(root,f,i,v);
                //deSerialize(root,f);
                InOrder(root);


                string back="";
                for(int i=0;i<res.size();){
                    back+=decode(root,res,i);
                }
                UnderScore2Space(back);
                //cout<<back<<endl;

                ui->textEdit->setText(QString(back.c_str()));


        } catch (exception e) {

        }
    }
}
