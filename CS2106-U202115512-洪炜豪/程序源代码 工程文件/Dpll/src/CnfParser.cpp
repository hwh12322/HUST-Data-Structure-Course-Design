#include"header.h"

ifstream openFile(string file) {  //利用ifstream类的构造函数创建一个文件输入流对象
    ifstream fis(file.c_str());
    return fis;
}
Head* buildLinkedList(ifstream& fis,int clauseNum) {   //将cnf文件输入十字链表
    Head* HEAD = new Head;
    Head* headRear = HEAD;
    Head* END = new Head;
    for (int i = 0; i < clauseNum; i++) {
        int temp;
        fis >> temp;
        Data* front = new Data;
        front -> data = temp;
        headRear -> nextData = front;
        headRear -> num++;
        fis >> temp;
        while (temp != 0) {
            Data* rear = new Data;
            front -> nextData = rear;
            rear -> data = temp;
            front = front -> nextData;
            headRear -> num++;
            fis >> temp;
        }
        front -> nextData = NULL;
        fis.get();  //换行
        Head* tp = new Head;
        headRear -> nextHead = tp;
        END = headRear;
        headRear = headRear -> nextHead;
    }
    END -> nextHead = NULL;
    return HEAD;
}

void printLinkedList(Head* HEAD) {    //输出读入的cnf字句
    Head* Phead = HEAD;
    Data* front = NULL;
    cout << "CnfParser" << endl;
    while (Phead != NULL) {
        front = Phead -> nextData;
        while (front != NULL) {
            cout << front -> data << " ";
            front = front -> nextData;
        }
        cout << endl;
        Phead = Phead -> nextHead;
    }
}

Head* initCnf(int &varNum, string &filename) {
    //打开cnf文件
    string file = filePath + filename;
    ifstream fis = openFile(file);

    char ch,readbuf[1145];
    fis >> ch;
    while (ch != 'p') {
        fis.getline(readbuf,1145);
        fis >> ch;
    }//此时已读完字符p
    string cnf;
    int clauseNum;
    fis >> cnf >> varNum >> clauseNum;
    fis.get();

    //构建链表
    Head* HEAD = buildLinkedList(fis,clauseNum);
    //输出整个链表
    //printLinkedList(HEAD);

    return HEAD;
}