#include <iostream>
#include <ostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
// Because of its triply nested loop structure, LUP-DECOMPOSITION has a running time of ‚.n3/, which is the same as that of LU-DECOMPOSITION. Thus,
// pivoting costs us at most a constant factor in time.



void printVect(vector<vector<double>> &vect)
{
    for (int i = 0; i < vect.size(); i++)
    {

        for (int j = 0; j < vect[i].size(); j++)
        {
            cout<<vect[i][j]<<" ";
         }
         cout<<endl;
    }
}

void InitVect(vector<vector<double>> &vect, int n)
{
  vector <double>temp;
    for (int i = 0; i < n; i++)
    {
        temp.clear();
    for(int j =0;j<n;j++){
    temp.push_back(0);
    }
    vect.push_back(temp);
    }
}
/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
// if back of x and y match, jump diagonal
// check if new value matches with above or left, move to matching (recursive)



/* Driver code */
int main()
{
    
    // Given A matrix 
    // vector<vector<double>> A{
    //      { 382 ,  384 ,  321 ,  122  , 173} ,
    //      { 236 ,  840  , 770 ,  101  , 590},
    //       {288  , 606  , 766  , 554  , 496} ,
    //      {635   ,572 ,  411 ,  790,61},
    //       {500  , 922 ,  589 , 938  , 370}

    // };
    //  vector<vector<double>> A{
    //      {1,2,0},
    //      {3,4,4},
    //      {5,6,3}

    // };
//     vector<vector<double>> A{{170 ,  456  , 115 ,  437  , 855 ,  423   ,783  , 242 ,  552 ,  931},
// {  797,	93 ,  963 ,  912   ,626	,59  , 396 ,  359 ,  303 ,  778},
// { 318  , 236	,14 ,  190 ,  357 ,  904 ,  249 ,  823 ,  747 ,  492},
// {533   ,915   ,777   ,271   ,518  , 946  , 410,	25  , 197   ,441},
// { 174 ,  161 ,  819  , 154 ,  408 ,  496 ,  105,	52 ,  690 ,  452},
// { 606  , 828 ,  870  , 144	,85  , 494 ,  140   ,177  , 191  , 313},
// {270   ,543	,93 ,  871  , 247 ,  344 ,  943,   653 ,  375,   513},
//  {658  , 997  , 406  , 584  , 132  , 901  , 957  , 735  , 629  , 516},
//  { 693	,87 ,  267 ,  554 ,  192,   375 ,  580,   651 ,  783 ,  820} ,
//  {751  , 448   ,802 ,  153  , 247  , 120,	69  , 456,	90  , 797}  };
    // vector<vector<double>> A{
    //   { 4, 3, 3,1,2},
    //   { 3, 6, 2,7,8},
    //   { 2, 1, 4,13,2},
    //   {5,2,4,5,9},
    //   {9,12,4,5,3}};
    // vector<vector<double>>A {
    //     {1,2,3},
    //     {5,6,7},
    //     {6,7,8}
    // };
    // vector<vector<double>>A {
    //     {-1,3,6,2,8},
    //     {5,-3,-1,8,3},
    //     {6,7,9,-2,3},
    //     {12,1,4,8,3},
    //     {-7,5,1,3,9}
    // };
    vector<vector<double>>A{
        {2,7,6,2},
        {9,5,1,3},
        {4,3,8,4},
        {5,6,7,8}
    };
    
    // cout<<"A before:"<<endl;
    //  printVect(A);
    //defining row size, colomn size, and pi[0]=0, pi[1]=1, ...
    int rows=A.size();
    int cols=A[0].size();
    vector<vector<double>> L;
    vector<vector<double>> U;
    vector<vector<double>> P;
    InitVect(L,rows);
    InitVect(U,rows);
    InitVect(P,rows);
 
    for(int i=0;i<rows;i++){
        for(int j=0;j<rows;j++){
            if(i==j){
                L[i][j]=1;
                P[i][j]=1;
            }
        }
    }
    double p;
    int kdash;
    double temp;
    for(int k=0;k<rows;k++){
        p=0;
        for(int i=k;i<rows;i++){
         
            //replace if negative 
            if(A[i][k]<0){A[i][k]=A[i][k]*-1;}
            if(A[i][k]>p){
                p=A[i][k];
                kdash=i;
            }
        }
         if(p==0){
            cout<<"Singular Matrix"<<endl;
           //return 1;
        }
       for(int i=0; i<rows;i++){
            //exchange A[k][i] with A[kdash][i]
            temp=A[kdash][i];
            A[kdash][i]=A[k][i];
            A[k][i]=temp;
        }
        for(int i=0; i<rows;i++){
            //exchange A[k][i] with A[kdash][i]
            temp=P[kdash][i];
            P[kdash][i]=P[k][i];
            P[k][i]=temp;
        }
        //48
        for(int i =k+1; i<rows;i++){
             A[i][k]=A[i][k]/A[k][k];    //holds factor needed to multiply number above
            for(int j=k+1;j<rows;j++){
                A[i][j]=A[i][j]-(A[i][k]*A[k][j]);
            }
        }


    }
    for(int i=0;i<rows;i++){
        for(int j=0;j<rows;j++){
            if(i>j){
                L[i][j] = A[i][j];
            }
            else{
                U[i][j] = A[i][j];
            }
        }
    }

    cout<<"L:"<<endl;
    printVect(L);
    cout<<"U:"<<endl;
    printVect(U);
    cout<<"P:"<<endl;
    printVect(P);

    /*Determinant of matrix LUP= Det(L)*Det(U)*Det(P)

    Det(triangle matrix) = product of diagonal
    "if the elements of the diagonal are not 1, they will be zero, and then there has been a swap. 
    So we simply subtract the sum of the diagonal from the length of the diagonal and then subtract 1 to get the number of swaps."
    */
   //Det(L)
   double prodDetL=1;
   double prodDetU=1;
   double prodDetP=1;
   for(int i=0;i<rows;i++){
    for(int j=0;j<rows;j++){
    if(i==j){
        prodDetL*=L[i][j];
    }
    }
   }
   cout<< "Det(L)="<<prodDetL<<endl;
   //Det(U)
    for(int i=0;i<rows;i++){
    for(int j=0;j<rows;j++){
    if(i==j){
        prodDetU*=U[i][j];
    }
    }
   }
   cout<< "Det(U)="<<prodDetU<<endl;
//    Det(P)
   int sumof1=0;
   int lenofdiag=0;
    for(int i=0;i<rows;i++){
    for(int j=0;j<rows;j++){
    if(i==j ){
        lenofdiag++;
        if(P[i][j]==1)
        { sumof1++;}        
       
    }
    }
   }
   double swaps =lenofdiag- sumof1-1;
   prodDetP = pow(-1,swaps);
   cout<<"Det(P)="<<prodDetP<<endl;

//    cout<< "Det(L)="<<prodDetL<<"\tDet(U)="<<prodDetU<<"\tDet(P)="<<prodDetP<<endl;
    cout<<"Det(LUP)="<<prodDetL*prodDetU*prodDetP<<endl;
   


    return 0;
}

