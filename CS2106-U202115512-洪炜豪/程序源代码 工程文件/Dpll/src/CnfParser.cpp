#include"header.h"

ifstream openFile(string file) {  //����ifstream��Ĺ��캯������һ���ļ�����������
    ifstream fis(file.c_str());
    return fis;
}
Head* buildLinkedList(ifstream& fis,int clauseNum) {   //��cnf�ļ�����ʮ������
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
        fis.get();  //����
        Head* tp = new Head;
        headRear -> nextHead = tp;
        END = headRear;
        headRear = headRear -> nextHead;
    }
    END -> nextHead = NULL;
    return HEAD;
}

void printLinkedList(Head* HEAD) {    //��������cnf�־�
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
    //��cnf�ļ�
    string file = filePath + filename;
    ifstream fis = openFile(file);

    char ch,readbuf[1145];
    fis >> ch;
    while (ch != 'p') {
        fis.getline(readbuf,1145);
        fis >> ch;
    }//��ʱ�Ѷ����ַ�p
    string cnf;
    int clauseNum;
    fis >> cnf >> varNum >> clauseNum;
    fis.get();

    //��������
    Head* HEAD = buildLinkedList(fis,clauseNum);
    //�����������
    //printLinkedList(HEAD);

    return HEAD;
}