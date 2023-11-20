#include "header.h"
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
    ifstream f(file);
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
		printf("1. ����   2. ����   3. ��������(��3��)   4.�������   0.��������ӡ��\n");
        printf("    ��ѡ����Ĳ���[0~4]:\n");
        printf("------------------------------------------------------------------\n");
        printf("�����������������:\n");
        for(int i=0;i<=1;i++){
            for(int j=1;j<=9;j++){
                for(int k=1;k<=9;k++){
                    if(done[i][j][k]==1){
                        cout << i << "����1��0��" << j << "��" << k << "��,���ݣ�" ;
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
                cout << "�������������ϻ�����������,��1��0" << endl ;
                cin >> d;
                cout << "�������������м�����,�ո����,��1 1 1" <<endl ;
                cin >> x >> y >> i;
                while((d==1&&a[x-1][y-1]!=0) || (d==0&&b[x-1][y-1]!=0)){
                    cout << "�˿�������̶����֣����������룡" << endl;
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
                cout << "������Ҫ�������ϻ�����������,��1��0" << endl ;
                cin >> d;
                cout << "�����볷�����м�����,�ո����,��1 1 1" << endl ;
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
                    cout << "���������Ѵ�����" <<endl ;
                    break;
                }
                cout << "������Ҫ�������ϻ�����������,��1��0" << endl ;
                cin >> d;
                cout << "����������,�ո����,��1 1" <<endl ;
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
                                    cout << "�������룺" << i << "����1��0��" << j << "��" << k << "��" << endl;
                                }
                            }   
                        }
                    }
                }
                if(count<holes) cout << "������ȷ�����" << count << "��,����" << holes << "�գ�����Ŭ���ɣ�" << endl;
                else cout << "��ϲ�������������ѹ����˹�ڣ�" << endl;
                getchar();getchar();
		 		break;
        }
    }
}
