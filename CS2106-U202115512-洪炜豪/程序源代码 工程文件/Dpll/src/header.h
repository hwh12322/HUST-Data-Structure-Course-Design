#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#define ROW 9
#define COL 9
using namespace std;

const string filePath = R"(C:\Users\cy\Desktop\�ļ�\���ݽṹ����\Dpll\samples\)";

typedef struct Data {
    int data = 0;
    Data *nextData{};
}Data;

typedef struct Head{
    int num = 0;
    Data *nextData{};
    Head *nextHead{};
}Head;
//ʮ��������

void games(int holes ,int *result);
void Examination(void);
Head* initCnf(int &, string &);
bool DPLL(Head*,int*,int);
string initSudoku(int holes);

