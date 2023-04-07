// Question 41: Given two directed segments [P0, P1] and [P0, P2], 
//is [P0, P1] clockwise from [P0, P2] with respect to their common endpoint P0?
// Test case example 1:
// [P0, P1, P2]: [(45, 76), (65, 28), (71, 68)]
/*(p1 - p0) x (p2 - p0) = (x1 - x0) (y2 - y0) - (x2 - x0) (y1 - y0)
if positive, P0P1 is clockwise, negative counter clock-wise*/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;
double Dir(int pix, int piy, int pjx, int pjy, int pkx, int pky){
/*
//return (Pk-Pi) X (Pj-Pi) = (p2 - p0) x (p2 - p0)
//       (Pk-Pi) X (Pj-Pi) = (Xk-Xi)( Yj-Yi) - (Xj-Xi)(Yk-Yi)

double A = Xk-Xi;
double B = Yj-Yi;
double C = Xj-Xi;
double D = Yk-Yi;

Pj-Pi =  P’2 = (X2-X0, Y2-Y0)
*/
//X1-X0 = Pk-Pi
        //pix,        pjx,    pkx
// int Dir(Point a, Point b, Point c) { //O(1)
//    int val = (b.y-a.y)*(c.x-b.x)-  (b.x-a.x)*(c.y-b.y);
//    int val = B(pjy-piy)*A(pkx-pjx)-C(pjx-pix)*D(pky-pjy);
double A = pkx-pix;
double B = pjy-piy;
double C = pjx-pix;
double D = pky-piy;

return (A*B)-(C*D);

}
int main(){
  //  Test Case 1:
    int p0x = 45; //common end point
    int p0y =76; 
    int p1x=65;
    int p1y=28;
    int p2x=71;
    int p2y=68;
    //Test Case 2
    // int p0x = 66; //common end point
    // int p0y =50; 
    // int p1x=12;
    // int p1y=35;
    // int p2x=17;
    // int p2y=96;

    if(Dir(p0x,p0y,p2x,p2y,p1x,p1y)>0){
        cout<<"TRUE\n";
    }
    else if(Dir(p0x,p0y,p2x,p2y,p1x,p1y)<0){
        cout<<"FALSE\n";
    }
    else
    {
        cout<<"Collinear\n";
    }
    return 0;
}