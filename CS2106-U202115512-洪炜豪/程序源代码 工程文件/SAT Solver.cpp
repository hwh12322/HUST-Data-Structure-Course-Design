#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#define ROW 9
#define COL 9
using namespace std;

const string filePath = "";

typedef struct Data {
    int data = 0;
    Data *nextData{};
}Data;

typedef struct Head{
    int num = 0;
    Data *nextData{};
    Head *nextHead{};
}Head;
//十字链表定义

void games(int holes ,int *result);
void Examination(void);
Head* initCnf(int &, string &);
bool DPLL(Head*,int*,int);
string initSudoku(int holes);



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


void DestroyLinkedList(Head* LinkedList)
{
    Head* p = LinkedList ;
    Head* p1=NULL;
        while(p){
            Data* e=p->nextData;
            Data* e1=NULL;
            while(e){
                e1=e;
                e=e1->nextData;
                free(e1);
            }
            p1=p;
            p=p1->nextHead;
            free(p1);    
        }
}


bool isEmptyClause(Head* LinkedList) {
    Head* PHead = LinkedList;
    while (PHead != NULL) {
        if (PHead -> num == 0) {
            return true;
        }
        PHead = PHead -> nextHead;
    }
    return false;
}

Head* findSingleClause(Head* LinkedList) {
    while (LinkedList != NULL) {
        if (LinkedList -> num == 1) {
            return LinkedList;
        }
        LinkedList = LinkedList -> nextHead;
    }
    return NULL;
}

Head* addSingleClause(Head* LinkedList,int var) {
    Head* addHead = new Head;
    Data* addData = new Data;
    addData -> data = var;
    addData -> nextData = NULL;
    addHead -> nextData = addData;
    addHead -> num = 1;
    addHead -> nextHead = LinkedList;
    LinkedList = addHead;
    return LinkedList;
}

void deleteHead(Head* Clause,Head *&LinkedList) {
    if (!Clause)
        return;
    if (Clause == LinkedList) {
        LinkedList = Clause -> nextHead;
    } else {
        for (Head* front = LinkedList; front != NULL; front = front -> nextHead) {
            if (front -> nextHead == Clause) {
                front -> nextHead = Clause -> nextHead;
            }
        }
    }
}

void deleteData(int object,Head *&LinkedList) {
    for (Head* pHead = LinkedList; pHead != NULL; pHead = pHead -> nextHead) {
        for (Data* rear = pHead -> nextData; rear != NULL; rear = rear -> nextData) {
            if (rear -> data == object) {  //删除整行
                deleteHead(pHead,LinkedList);
            } else if (abs(rear -> data) == abs(object)) {
                //绝对值相同，只删除这个节点
                if (rear == pHead -> nextData) {
                    //如果是头部节点
                    pHead -> nextData = rear -> nextData;
                    pHead -> num--;
                    free(rear);
                } else {
                    for (Data* front = pHead -> nextData; front != NULL; front = front -> nextData) {
                        if (front -> nextData == rear) {
                            front -> nextData = rear -> nextData;
                            pHead -> num--;
                        }
                    }
                }
            }
        }
    }
}

void printResult(int *result,int varNum) {
    cout << "V ";
    for (int i = 0; i < varNum; i++) {
        if (result[i] == 1) {
            cout << i + 1 << " ";
        } else if (result[i] == 0) {
            cout << -(i + 1) << " ";
        } else {  //第三种情况是变元，可true可false
            cout << i + 1 << " ";
        }
    }
    cout << endl;
}

Head* duplication(Head* LinkedList) {
    //复制，先创建第一行
    Head* lastHead = LinkedList;
    Head* newHead = new Head;
    newHead -> num = lastHead -> num;
    Head* pHead = newHead;
    //第一个Data
    Data* newData = new Data;
    Data* firstLastData = lastHead -> nextData;
    newData -> data = firstLastData -> data;
    pHead -> nextData = newData;
    //第一行
    for (firstLastData = firstLastData -> nextData; firstLastData != NULL; firstLastData = firstLastData -> nextData) {
        Data* temp = new Data;
        temp -> data = firstLastData -> data;
        newData -> nextData = temp;
        newData = newData -> nextData;
    }
    //后面若干行
    for (lastHead = lastHead ->nextHead; lastHead != NULL; lastHead = lastHead -> nextHead) {
        Head* tempHead = new Head;
        Data* tempData = new Data;
        tempHead -> num = lastHead -> num;
        pHead -> nextHead = tempHead;
        pHead = pHead -> nextHead;
        Data* lastData = lastHead -> nextData;
        tempData -> data = lastData -> data;
        pHead -> nextData = tempData;
        for (lastData = lastData -> nextData; lastData != NULL; lastData = lastData -> nextData) {
            Data* temp = new Data;
            temp -> data = lastData -> data;
            tempData -> nextData = temp;
            tempData = tempData -> nextData;
        }
        tempData -> nextData = NULL;
    }
    pHead -> nextHead = NULL;

    return newHead;
}


Data* findMinimal(Head* LinkedList) {
    Data* result = NULL;
    int miniNum = 0xffff;
    for (Head* head = LinkedList; head != NULL; head = head -> nextHead) {
        if (head -> num <= miniNum) {
            miniNum = head -> num;
            result = head -> nextData;
        }
    }
    return result;
}

bool DPLL(Head* LinkedList,int* result,int varNum) {
    Head* pFind = LinkedList;
    Head* singleClause = findSingleClause(pFind);
    while (singleClause != NULL) {
        int data = singleClause -> nextData -> data;
        if (data > 0) {
            result[abs(data) - 1] = 1;
        } else {
            result[abs(data) - 1] = 0;
        }
        deleteHead(singleClause,LinkedList);  //删除单子句
        deleteData(data,LinkedList);   //化简包含相反值的子句
        if (!LinkedList) {
            return true;
        } else if (isEmptyClause(LinkedList)) {
            DestroyLinkedList(LinkedList); 
            return false;
        }
        pFind = LinkedList;
        singleClause = findSingleClause(pFind);
    }
    Data* minimalList = findMinimal(LinkedList);
    int data = minimalList -> data;
    //int data = LinkedList -> nextData -> data;
    Head* last = duplication(LinkedList);
    Head* newLinkedList = addSingleClause(LinkedList,data);
    //分裂策略
    if (DPLL(newLinkedList,result,varNum)) {
        return true;
    } else {
        Head* newLinkedList2 = addSingleClause(last,-data);
        return DPLL(newLinkedList2,result,varNum);
    }
}


//D:\MyCode\Dpll\samples\verify.exe D:\MyCode\Dpll\samples\sudoku.cnf D:\MyCode\Dpll\samples\sudoku.res
void Examination(void)
{
    string file = " ";
    int varNum, clauseNum, i, j, solution, ans = 1;
    static int bv[35000][40];
    bool clause_value[35000] = {0};
    printf("input filename:");
    cin >> file;
    string outputFile = file + ".res";
    string filename = file + ".cnf";
    string file1 = filePath + filename;
    string file2 = filePath + outputFile;
    ifstream fi(file1.c_str());
    if (!fi)
    {
        cout << "Error! File can't open!" << endl;
        exit(-1);
    }
    ifstream fii(file2.c_str());
    if (!fii)
    {
        cout << "Error! File can't open!" << endl;
        exit(-1);
    }
    char ch, readbuf[1145];
    fi >> ch;
    while (ch != 'p')
    {
        fi.getline(readbuf, 1145);
        fi >> ch;
    } //此时已读完字符p
    string cnf;
    fi >> cnf >> varNum >> clauseNum;
    fi.get();
    fii >> ch;
    while (ch != 'v')
    {
        fi.getline(readbuf, 1145);
        fii >> ch;
    } //此时已读完字符v
    int *result = new int[varNum];
    memset(result, 0, sizeof(int) * varNum);
    for (int i = 0; i < varNum; i++)
    {
        fii >> solution;
        result[abs(solution)] = solution;
    }
    result[0] = 0;
    for (i = 0; i < clauseNum; i++)
    { //读取所有子句
        clause_value[i] = false;
        j = 0;
        fi >> bv[i][j];
        do
        {
            if (result[abs(bv[i][j])] == bv[i][j])
                clause_value[i] = true;
            j++;
            fi >> bv[i][j];
        } while (bv[i][j] != 0);
    }
    for (i = 0; i < clauseNum; i++)
    { //输出代入后的
        cout << "第" << i + 1 << "个子句:" << clause_value[i] << endl;
        if (0 == clause_value[i])
            ans = 0;
    }
    for (i = 0; i < clauseNum; i++)
    {
        if (0 == clause_value[i])
        cout << "第" << i + 1 << "个子句:" << clause_value[i] << endl;
    }
    if (ans == 1)
        cout << "All correct!" << endl;
    else
        cout << "Error!" << endl;
}


void games(int holes ,int *result)
{
    int varNum=1458;
    int a1[9][9] = {0},b1[9][9] = {0};
    for (int i = 0; i < varNum / 2; i++) {
        if (result[i] > 0) {
            int t = i;
            int x = t / 81;
            int y = (t - x * 81) / 9;
            a1[x][y] = t - x * 81 - y * 9 + 1;
        }
    }
    for (int i = varNum / 2; i < varNum; i++) {
        if (result[i] > 0) {
            int t = i - 729;
            int x = t / 81;
            int y = (t - x * 81) / 9;
            b1[x][y] = t - x * 81 - y * 9 + 1;
        }
    }
    int a[ROW][COL] = {0};
    int b[ROW][COL] = {0};
    int done[2][10][10] = {0};
    string file=filePath + "sudoku.txt" ;
    ifstream f(file.c_str());
    if (!f) {
        cout << "Error! File1 can't open!" << endl;
        exit(-1);
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            f >> a[i][j];
        }
    }
    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++){
            f >> b[i][j];
        }
    }
    int op=100,i,j,count=0,x,y,d=100,counts=0;
    while(op){
        system("cls"); printf("\n\n");
        printf("------------------Sudoku Mode--------------------- \n");
        cout << endl;
        for (int i = 0; i < 6; i++) {
            if(i % 3 == 0) cout << "-------------------------------" << endl; ;
            for (int j = 0; j < 9; j++) {
            if(j % 3 == 0) cout << "|" ;
            if(a[i][j] != 0) cout << " " << a[i][j] << " " ;
            else cout << "   ";
            }
            cout << "|" << endl;
        }
        for (int i = 6; i < 9; i++) {
            if(i % 3 == 0) cout << "---------------------------------------------------" << endl; 
            for (int j = 0; j < 9; j++) {
                if(j % 3 == 0) cout << "|" ;
                if(a[i][j] != 0) cout << " " << a[i][j] << " " ;
                else cout << "   ";
            }
            cout << "|";
            for (int j = 3; j < 9; j++) {
                if(b[i - 6][j] != 0) cout << " " << b[i - 6][j] << " ";
                else cout << "   ";
                if((j-2) % 3 == 0) cout << "|" ;
            }
            cout  << endl;
        }
        for (int i = 3; i < 9; i++) {
            if (i == 3) cout << "---------------------------------------------------" << endl;
            if (i == 6) cout << "                    -------------------------------" << endl; 
            for (int j = 1; j <= 20; j++) {
                cout << " ";
            }
            for (int j = 0; j < 9; j++) {
                if(j % 3 == 0) cout << "|" ;
                if(b[i][j] != 0) cout << " " << b[i][j] << " ";
                else cout << "   ";
            }
            cout << "|" << endl;
        }
        cout << "                    -------------------------------" << endl; 
		printf("1. 填入   2. 撤回   3. 帮助填入(限3次)   4.检查正误   0.放弃并打印答案\n");
        printf("    请选择你的操作[0~4]:\n");
        printf("------------------------------------------------------------------\n");
        printf("您已填入的数据如下:\n");
        for(int i=0;i<=1;i++){
            for(int j=1;j<=9;j++){
                for(int k=1;k<=9;k++){
                    if(done[i][j][k]==1){
                        cout << i << "（左1右0）" << j << "行" << k << "列,数据：" ;
                        if(i==1) cout << a[j-1][k-1] ;
                        else cout << a[j-1][k-1];
                        cout << endl ;
                    }
                }
            }
        }
        printf("------------------------------------------------------------------\n");
        cin >> op;
        switch(op){
            case 0:break;
            case 1:
                cout << "请输入填入左上或者右下数独,左1右0" << endl ;
                cin >> d;
                cout << "请输入填入行列及数字,空格隔开,如1 1 1" <<endl ;
                cin >> x >> y >> i;
                while((d==1&&a[x-1][y-1]!=0) || (d==0&&b[x-1][y-1]!=0)){
                    cout << "此空已填入固定数字，请重新输入！" << endl;
                    cin >> x >> y >> i;
                }
                if(d==1){
                    a[x-1][y-1]=i;
                    if (x >= 6 && y >= 6) {
                        b[x - 6][y - 6] = i;
                    }
                }
                else if(d==0){
                    b[x-1][y-1]=i;
                    if (x <= 2 && y <= 2) {
                        a[x + 6][y + 6] = i;
                    }
                }
                done[d][x][y]=1;
                getchar();getchar();
		 		break;
            case 2:
                cout << "请输入要撤回左上或者右下数独,左1右0" << endl ;
                cin >> d;
                cout << "请输入撤回行列及数字,空格隔开,如1 1 1" << endl ;
                cin >> x >> y >> i;
                if(d==1){
                    a[x-1][y-1]=0;
                    if (x >= 6 && y >= 6) {
                        b[x - 6][y - 6] = 0;
                    }
                }
                else if(d==0){
                    b[x-1][y-1]=0;
                    if (x <= 2 && y <= 2) {
                        a[x + 6][y + 6] = 0;
                    }
                }
                done[d][x][y]=0;
                getchar();getchar();
		 		break;
            case 3:
                if(counts>=3){
                    cout << "帮助次数已达上限" <<endl ;
                    break;
                }
                cout << "请输入要帮助左上或者右下数独,左1右0" << endl ;
                cin >> d;
                cout << "请输入行列,空格隔开,如1 1" <<endl ;
                cin >> x >> y ;
                if(d==1){
                    a[x-1][y-1]=a1[x-1][y-1];
                    if (x >= 6 && y >= 6) {
                        b[x - 6][y - 6] = b1[x - 6][y - 6];
                    }
                }
                else if(d==0){
                    b[x-1][y-1]=b1[x-1][y-1];
                    if (x <= 2 && y <= 2) {
                        a[x + 6][y + 6] = a1[x + 6][y + 6];
                    }
                }
                done[d][x][y]=1;counts++;
                getchar();getchar();
		 		break;
            case 4:
                count=0;
                for(int i=0;i<=1;i++){
                    for(int j=1;j<=9;j++){
                        for(int k=1;k<=9;k++){
                            if(done[i][j][k]==1){
                                if((i==1&&a[j-1][k-1]==a1[j-1][k-1])||(i==0&&b[j-1][k-1]==b1[j-1][k-1])) count++;
                                else{
                                    cout << "错误填入：" << i << "（左1右0）" << j << "行" << k << "列" << endl;
                                }
                            }   
                        }
                    }
                }
                if(count<holes) cout << "您已正确完成了" << count << "空,共有" << holes << "空，继续努力吧！" << endl;
                else cout << "恭喜你完成了数独！压力马斯内！" << endl;
                getchar();getchar();
		 		break;
        }
    }
}


string getCnf(int a[][COL], int b[][COL]);
void randomFirstRow(int a[], int n)
{
    srand((unsigned)time(NULL));
    int vis[10];
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i < n; i++)
    {
        if (a[i] != 0)
        { //如果已经填过了
            vis[a[i]] = 1;
        }
        else
        {
            a[i] = rand() % 9 + 1;
            while (vis[a[i]])
            {
                a[i] = rand() % 9 + 1;
            }
            vis[a[i]] = 1;
        }
    }
}

bool fillLast(int a[][COL], int row, int col)
{
    //当前要填第row行col列的格子
    if (row == ROW)
    {
        return true;
    }
    if (col == COL)
    {
        return fillLast(a, row + 1, 0);
    }
    if (a[row][col] != 0)
    {
        //如果已经填过了
        return fillLast(a, row, col + 1);
    }
    int vis[10];
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i < 9; i++)
    {
        vis[a[row][i]] = 1;
        vis[a[i][col]] = 1;
    }
    for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++)
    {
        for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
        {
            vis[a[i][j]] = 1;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (vis[i])
        {
            continue;
        }
        a[row][col] = i;
        if (fillLast(a, row, col + 1))
        {
            return true;
        }
    }
    a[row][col] = 0;
    return false;
}

void createSudoku(int a[][COL])
{
    randomFirstRow(a[0], COL);
    fillLast(a, 1, 0);
}

void print(const int a[][COL])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void putHole(int a[][COL], int b[][COL], int holeNum)
{
    srand((unsigned)time(NULL));
    int ii = 0, a1 = 0, b1 = 0;
    while (ii < holeNum)
    {
        int whichSudoku = rand() % 2, x = rand() % 9, y = rand() % 9;
        if (whichSudoku == 0)
        {
            //挖第一个数独上的洞
            while (a[x][y] == 0)
            { //已经被挖过了
                x = rand() % 9;
                y = rand() % 9;
            }
            a1 = a[x][y];
            a[x][y] = 0;
            if (x >= 6 && y >= 6)
            {
                b[x - 6][y - 6] = 0;
            }
        }
        else
        {
            while (b[x][y] == 0)
            {
                x = rand() % 9;
                y = rand() % 9;
            }
            b1 = b[x][y];
            b[x][y] = 0;
            if (x <= 2 && y <= 2)
            {
                a[x + 6][y + 6] = 0;
            }
        }
        if (whichSudoku == 0){
            for (int p = 1; p <= 9; p++){
                int k1=0;
                if (p == a1) continue;
                int vis[10];
                memset(vis, 0, sizeof(vis));
                for (int i1 = 0; i1 < 9; i1++)
                {
                   vis[a[x][i1]] = 1;
                   vis[a[i1][y]] = 1;
                }
                for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++)
                {
                    for (int j = y / 3 * 3; j < y / 3 * 3 + 3; j++)
                    {
                        vis[a[i][j]] = 1;
                    }
                }
                for (int i = 1; i <= 9; i++)
                {
                    if (vis[p])
                    {
                        k1=1;break;
                    }
                }
                if (k1 == 1) continue;
                a[x][y] = p;
                string q = getCnf(a, b);
                int varnum = 0;
                Head *LinkedList = initCnf(varnum, q);
                int *result = new int[1458];
                memset(result, 0, sizeof(int) * varnum);
                bool flag = DPLL(LinkedList, result, 1458);
                if (flag == true){ 
                    if (whichSudoku == 0){
                        a[x][y] = a1;
                        if (x >= 6 && y >= 6){
                            b[x - 6][y - 6] = a1;
                        }
                    }
                    if (whichSudoku != 0){
                        b[x][y] = b1;
                        if (x <= 2 && y <= 2){
                            a[x + 6][y + 6] = b1;
                        }
                    }
                    ii--;
                    break;
                }
            }
        }
        else{
            for (int p = 1; p <= 9; p++){
                int k1=0;
                if (p == b1) continue;
                int vis[10];
                memset(vis, 0, sizeof(vis));
                for (int i1 = 0; i1 < 9; i1++)
                {
                   vis[b[x][i1]] = 1;
                   vis[b[i1][y]] = 1;
                }
                for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++)
                {
                    for (int j = y / 3 * 3; j < y / 3 * 3 + 3; j++)
                    {
                        vis[b[i][j]] = 1;
                    }
                }
                for (int i = 1; i <= 9; i++)
                {
                    if (vis[p])
                    {
                        k1=1;break;
                    }
                }
                if (k1 == 1) continue;
                b[x][y] = p;
                string q = getCnf(a, b);
                int varnum = 0;
                Head *LinkedList = initCnf(varnum, q);
                int *result = new int[1458];
                memset(result, 0, sizeof(int) * varnum);
                bool flag = DPLL(LinkedList, result, 1458);
                if (flag == true){ 
                    if (whichSudoku == 0){
                        a[x][y] = a1;
                        if (x >= 6 && y >= 6){
                            b[x - 6][y - 6] = a1;
                        }
                    }
                    if (whichSudoku != 0){
                        b[x][y] = b1;
                        if (x <= 2 && y <= 2){
                            a[x + 6][y + 6] = b1;
                        }
                    }
                    ii--;
                    break;
                }
            }
        }
        ii++;
    }
}

string getCnf(int a[][COL], int b[][COL])
{
    ofstream fos((filePath + "sudoku.cnf").c_str());
    if (!fos.is_open())
    {
        cout << "Error! Can't open file1" << endl;
        exit(-1);
    }
    int tot = 0, cnt = 0;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (a[i][j] != 0)
            {
                tot++;
            }
            if (b[i][j] != 0)
            {
                tot++;
            }
        }
    }
    fos << "p cnf 1458 " << 29970 + tot << " " << endl;
    //对于已经填了的，直接生成单语句
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (a[i][j] != 0)
            {
                fos << i * 81 + j * 9 + a[i][j] << " " << 0 << endl;
            }
            if (b[i][j] != 0)
            {
                fos << 729 + i * 81 + j * 9 + b[i][j] << " " << 0 << endl;
            }
        }
    }
    //单个格子
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                fos << (i - 1) * 81 + (j - 1) * 9 + k << " ";
            }
            fos << 0 << endl;
            cnt++;
            for (int k = 1; k <= 9; k++)
            {
                fos << 729 + (i - 1) * 81 + (j - 1) * 9 + k << " ";
            }
            fos << 0 << endl;
            cnt++;
            for (int k = 1; k <= 9; k++)
            {
                for (int p = k + 1; p <= 9; p++)
                {
                    fos << -((i - 1) * 81 + (j - 1) * 9 + k) << " " << -((i - 1) * 81 + (j - 1) * 9 + p) << " " << 0 << endl;
                    fos << -(729 + (i - 1) * 81 + (j - 1) * 9 + k) << " " << -(729 + (i - 1) * 81 + (j - 1) * 9 + p) << " " << 0 << endl;
                    cnt += 2;
                }
            }
            // cnt += 2;
        }
    }
    //行
    for (int i = 1; i <= 9; i++)
    {
        for (int k = 1; k <= 9; k++)
        {
            for (int j = 1; j <= 9; j++)
            {
                fos << ((i - 1) * 81 + (j - 1) * 9 + k) << " ";
            }
            fos << 0 << endl;
            for (int j = 1; j <= 9; j++)
            {
                fos << (729 + (i - 1) * 81 + (j - 1) * 9 + k) << " ";
            }
            fos << 0 << endl;
            cnt += 2;
            for (int p = 1; p <= 9; p++)
            {
                for (int q = p + 1; q <= 9; q++)
                {
                    fos << -((i - 1) * 81 + (p - 1) * 9 + k) << " " << -((i - 1) * 81 + (q - 1) * 9 + k) << " " << 0 << endl;
                    fos << -(729 + (i - 1) * 81 + (p - 1) * 9 + k) << " " << -(729 + (i - 1) * 81 + (q - 1) * 9 + k) << " " << 0 << endl;
                    cnt += 2;
                }
            }
        }
    }
    //列
    for (int j = 1; j <= 9; j++)
    {
        for (int k = 1; k <= 9; k++)
        {
            for (int i = 1; i <= 9; i++)
            {
                fos << ((i - 1) * 81 + (j - 1) * 9 + k) << " ";
            }
            fos << 0 << endl;
            for (int i = 1; i <= 9; i++)
            {
                fos << (729 + (i - 1) * 81 + (j - 1) * 9 + k) << " ";
            }
            fos << 0 << endl;
            cnt += 2;
            for (int p = 1; p <= 9; p++)
            {
                for (int q = p + 1; q <= 9; q++)
                {
                    fos << -((p - 1) * 81 + (j - 1) * 9 + k) << " " << -((q - 1) * 81 + (j - 1) * 9 + k) << " " << 0 << endl;
                    fos << -(729 + (p - 1) * 81 + (j - 1) * 9 + k) << " " << -(729 + (q - 1) * 81 + (j - 1) * 9 + k) << " " << 0 << endl;
                    cnt += 2;
                }
            }
        }
    }
    // 3*3
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            int x1 = x * 3 + 1, x2 = x * 3 + 3;
            int y1 = y * 3 + 1, y2 = y * 3 + 3;
            for (int k = 1; k <= 9; k++)
            {
                for (int i = x1; i <= x2; i++)
                {
                    for (int j = y1; j <= y2; j++)
                    {
                        fos << (i - 1) * 81 + (j - 1) * 9 + k << " ";
                    }
                }
                fos << 0 << endl;
                for (int i = x1; i <= x2; i++)
                {
                    for (int j = y1; j <= y2; j++)
                    {
                        fos << 729 + (i - 1) * 81 + (j - 1) * 9 + k << " ";
                    }
                }
                fos << 0 << endl;
                cnt += 2;

                for (int i = x1; i <= x2; i++)
                {
                    for (int j = y1; j <= y2; j++)
                    {
                        for (int p = x1; p <= x2; p++)
                        {
                            for (int q = y1; q <= y2; q++)
                            {
                                if (i == p && j == q)
                                {
                                    continue;
                                }
                                fos << -((i - 1) * 81 + (j - 1) * 9 + k) << " " << -((p - 1) * 81 + (q - 1) * 9 + k) << " " << 0 << endl;
                                fos << -(729 + (i - 1) * 81 + (j - 1) * 9 + k) << " " << -(729 + (p - 1) * 81 + (q - 1) * 9 + k) << " " << 0 << endl;
                                cnt += 2;
                            }
                        }
                    }
                }
            }
        }
    }
    //公共部分
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                fos << 0 - ((i + 6) * 81 + (j + 6) * 9 + k) << " " << (729 + i * 81 + j * 9 + k) << " " << 0 << endl;
                fos << ((i + 6) * 81 + (j + 6) * 9 + k) << " " << 0 - (729 + i * 81 + j * 9 + k) << " " << 0 << endl;
                cnt += 2;
            }
        }
    }

    fos.close();
    return "sudoku.cnf";
}

void printSudoku(int a[][COL], int b[][COL])
{
    ofstream fos((filePath + "sudoku.txt").c_str());
    if (!fos.is_open())
    {
        cout << "Error! Can't open file" << endl;
        exit(-1);
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fos << (a[i][j]) << " ";
        }
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fos << (b[i][j]) << " ";
        }
    }
}

string initSudoku(int holes)
{
    int sudoku1[ROW][COL] = {0};
    int sudoku2[ROW][COL] = {0};
    int holeNum = holes;
    //先生成第一个数独，再来生成第二个
    createSudoku(sudoku1);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sudoku2[i][j] = sudoku1[i + 6][j + 6];
        }
    }
    createSudoku(sudoku2);
    putHole(sudoku1, sudoku2, holeNum);
    printSudoku(sudoku1, sudoku2);
    return getCnf(sudoku1, sudoku2);
}



void display()
{
    printf("                          Menu for SAT Solver                           \n");
    printf("------------------------------------------------------------------------\n");
    printf("         1.Sudoku Games    2.SAT    3.Examination    0.Exit             \n");
    printf("------------------------------------------------------------------------\n");
    printf("Please choose your operation[0-3]:\n");
}

void printAnswer(clock_t startTime, clock_t endTime, bool flag, int *result, int varNum, string filename)
{
    string outputFile = filePath + filename;
    ofstream fos(outputFile.c_str());
    if (!fos.is_open())
    {
        cout << "error! Can't open a new file\n"
             << endl;
        exit(-1);
    }
    if (flag)
    {
        fos << "s " << 1 << endl;
        fos << "v ";
        for (int i = 0; i < varNum; i++)
        {
            if (result[i] == 1)
            {
                fos << i + 1 << " ";
            }
            else if (result[i] == 0)
            {
                fos << -(i + 1) << " ";
            }
            else
            {
                fos << i + 1 << " ";
            }
        }
        fos << endl;
    }
    else
    {
        fos << "s " << -1 << endl;
        fos << "v " << endl;
    }
    fos << "t " << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000.0 << " ms" << endl;
    fos.close();
}

void printCompleteSudoku(int *result, int varNum)
{
    ofstream fos((filePath + "sudokuAnswer.out").c_str());
    int a[9][9] = {0},b[9][9] = {0};
    for (int i = 0; i < varNum / 2; i++) {
        if (result[i] > 0) {
            int t = i;
            int x = t / 81;
            int y = (t - x * 81) / 9;
            a[x][y] = t - x * 81 - y * 9 + 1;
        }
    }
    for (int i = varNum / 2; i < varNum; i++) {
        if (result[i] > 0) {
            int t = i - 729;
            int x = t / 81;
            int y = (t - x * 81) / 9;
            b[x][y] = t - x * 81 - y * 9 + 1;
        }
    }
    for (int i = 0; i < 6; i++)
    {
        if (i % 3 == 0)
            cout << "-------------------------------" << endl;
        ;
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0)
                cout << "|";
            cout << " " << a[i][j] << " ";
        }
        cout << "|" << endl;
    }
    for (int i = 6; i < 9; i++)
    {
        if (i % 3 == 0)
            cout << "---------------------------------------------------" << endl;
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0)
                cout << "|";
            cout << " " << a[i][j] << " ";
        }
        cout << "|";
        for (int j = 3; j < 9; j++)
        {
            cout << " " << b[i - 6][j] << " ";
            if ((j - 2) % 3 == 0)
                cout << "|";
        }
        cout << endl;
    }
    for (int i = 3; i < 9; i++)
    {
        if (i == 3)
            cout << "---------------------------------------------------" << endl;
        if (i == 6)
            cout << "                    -------------------------------" << endl;
        for (int j = 1; j <= 20; j++)
        {
            cout << " ";
        }
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0)
                cout << "|";
            cout << " " << b[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << "                    -------------------------------" << endl;
    if (!fos.is_open())
    {
        cout << "Error! Can't open file" << endl;
        exit(-1);
    }
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fos << (a[i][j]) << " ";
        }
    }
    for (int i = 6; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fos << (a[i][j]) << " ";
        }
        for (int j = 3; j < 9; j++)
        {
            fos << (b[i - 6][j]) << " ";
        }
    }
    for (int i = 3; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fos << (b[i][j]) << " ";
        }
    }
}

int main()
{
    display();
    int choice = 0, holes = 0;
    cin >> choice;
    while (choice)
    {
        if (choice == 1)
        {
            cout << "Please enter the number of holes to be dug!" << endl;
            cin >> holes;
            string filename = initSudoku(holes);
            int varnum = 0;
            Head *LinkedList = initCnf(varnum, filename);
            int *result = new int[varnum];
            memset(result, 0, sizeof(int) * varnum);
            clock_t startTime, endTime;
            startTime = clock();
            bool flag = DPLL(LinkedList, result, varnum);
            endTime = clock();
            printAnswer(startTime, endTime, flag, result, varnum, "sudoku.res");
            games(holes, result);
            printCompleteSudoku(result, varnum);
        }
        else if (choice == 2)
        {
            int varnum;
            string file = " ";
            cin >> file;
            string suffix = ".cnf";
            string filename = file + suffix;
            Head *LinkedList = initCnf(varnum, filename);
            int *result = new int[varnum];
            memset(result, 0, sizeof(int) * varnum);
            clock_t startTime, endTime;
            string outputFile = file + ".res";
            startTime = clock();
            bool flag = DPLL(LinkedList, result, varnum);
            endTime = clock();
            printAnswer(startTime, endTime, flag, result, varnum, outputFile);
        }
        else if (choice == 3) {
        	Examination();
		}
 		else if (choice == 0) {
            break;
        } 
        else {
            cout << "Please input the right num! " << endl;
            exit(-1);
        }
        display();
        cin >> choice;
    }
    return 0;
}


