#include "header.h"
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