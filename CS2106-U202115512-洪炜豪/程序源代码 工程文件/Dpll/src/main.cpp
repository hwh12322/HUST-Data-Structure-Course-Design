#include "header.h"

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
    ofstream fos(outputFile);
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
    ofstream fos(filePath + "sudokuAnswer.out");
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
            fos << a[i][j] << " ";
        }
    }
    for (int i = 6; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fos << a[i][j] << " ";
        }
        for (int j = 3; j < 9; j++)
        {
            fos << b[i - 6][j] << " ";
        }
    }
    for (int i = 3; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fos << b[i][j] << " ";
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
        else if (choice == 3)
        {
            Examination();
        }
        else if (choice == 0)
        {
            break;
        }
        else
        {
            cout << "Please input the right num! " << endl;
            exit(-1);
        }
        display();
        cin >> choice;
    }
    return 0;
}
