#include <iostream>
using namespace std;
const int N = 5;

/*Given matrix
Iterate through rows and coloms

cofactor(mat, cofactorMat, 0, firstRow, dimension);
given matrix, and smaller cofactor matrix
If row does not equal row value p, and colomn does not equal value firstRow (row index value)
--> Basically create smaller matrix that does not use the associate value's row and colomn that will be multiplied
-->dwindle down to [1x1 matrix] which will return mat[0][0]

*/
void cofactor(int mat[N][N], int temp[N][N], int p,int q, int n){
   int i = 0, j = 0;
   for (int row = 0; row < n; row++){
      for (int column = 0; column < n; column++){
         if (row != p && column != q){
            temp[i][j++] = mat[row][column];
            if (j == n - 1){
                  j = 0;
                  i++;
            }
         }
      }
   }
}
/*If dimension =1, it is a single value mat[0][0]
for all the rows 
Determinant = sum of all determinants in values in row * appropriate sign * determinant of smaller matrix
Det = determinant + sign * mat[0][row index]* determinant of cofactorMat

flip the sign 
Recursively call determinant on smaller cofactor matrix until dimension is 1. 
*/
int determinantOfMatrix(int mat[N][N], int dimension){
   int D = 0;
   if (dimension == 1)
      return mat[0][0];
   int cofactorMat[N][N];
   int sign = 1;
   for (int firstRow = 0; firstRow < dimension; firstRow++){ //0-> row size 
      cofactor(mat, cofactorMat, 0, firstRow, dimension);
      D += sign * mat[0][firstRow] * determinantOfMatrix(cofactorMat, dimension - 1);
      sign = -sign;
   }
   return D;
}
void show(int mat[N][N], int row, int col){
   for (int i = 0; i < row; i++){
      for (int j = 0; j < col; j++)
         cout<<mat[i][j]<<" ";
         cout<<endl;
   }
   cout<<endl;
}
int main(){
 //N = size of colomn and row
   int mat[N][N] = {
      { 4, 3, 3,1,2},
      { 3, 6, 2,7,8},
      { 2, 1, 4,13,2},
      {5,2,4,5,9},
      {9,12,4,5,3}};
   cout<<"The matrix is "<<endl;
   show(mat,N,N);
   cout<<"Determinant of the matrix is "<<determinantOfMatrix(mat, N);
   return 0;
}