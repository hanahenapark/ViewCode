// Question 43. Do line segments [P1, P2] and [P3, P4] intersect?
 
// Test case example 1:
// [P1, P2, P3, P4]: [(25, 26), (85, 93), (26, 35), (82, 20)]
// Output: True
 
// Test case example 2:
// [P1, P2, P3, P4]: [(36, 92), (84, 29), (62, 59), (36, 79)]
// Output: True

//Write an algorithm to check if 2 lines intersect each other.
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

double Dir(int pix, int piy, int pjx, int pjy, int pkx, int pky){
/*
//return (Pk-Pi) X (Pj-Pi) = 
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

double min(double i, double j){
    if(i<=j){
        return i;
    }
    else{
        return j;
    }
}
double max(double i, double j){
    if(i<=j){
        return j;
    }
    else{
        return i;
    }
}
bool onSeg(int pix, int piy, int pjx, int pjy, int pkx, int pky){
double minX= min(pix, pjx);
double maxX = max(pix, pjx);
double minY= min(piy, pjy);
double maxY = max(piy, pjy);

if(((minX<=pkx)&&(pkx<=maxX))&&((minY<=pky)&&(pky<=maxY))){
return true;
}
else 
{return false;}
}

void SegInt(int p1x,int p1y,int p2x,int p2y,int p3x,int p3y, int p4x,int p4y){
double d1 = Dir(p3x,p3y,p4x,p4y, p1x, p1y);
double d2 = Dir(p3x,p3y,p4x,p4y, p2x, p2y);
double d3 = Dir( p1x, p1y, p2x, p2y, p3x,p3y);
double d4 = Dir(p1x, p1y, p2x, p2y, p4x,p4y);

if(((d1>0 && d2<0)||(d1<0 && d2>0))&&((d3>0 && d4<0)||(d3<0 && d4>0))){
    cout<<"True"<<endl;
}
else if((d1==0) && (onSeg(p3x,p3y,p4x,p4y, p1x, p1y))){
   cout<<"True"<<endl;
}
else if((d2==0) && (onSeg(p3x,p3y,p4x,p4y, p2x, p2y))){
   cout<<"True"<<endl;
}
else if((d3==0) && (onSeg(p1x, p1y, p2x, p2y, p3x,p3y))){
   cout<<"True"<<endl;
}
else if((d3==0) && (onSeg(p1x, p1y, p2x, p2y, p4x,p4y))){
   cout<<"True"<<endl;
}
else{
    cout<<"False"<<endl;
}


}

// Driver program to test above function
int main()
{
    //Line 1 = P1P2
    //each line has x and y value 
    //Line 2 = P3P4
    //Test Case 1
    // int P1_x=0;
    // int P1_y=0;

    // int P2_x = 7;
    // int P2_y=7;
    
    // int P3_x = 0;
    // int P3_y = 1;

    // int P4_x=8;
    // int P4_y = 8;

    //Test Case 2
    // int P1_x=25;
    // int P1_y=26;

    // int P2_x = 85;
    // int P2_y=93;
    
    // int P3_x = 26;
    // int P3_y = 35;

    // int P4_x=82;
    // int P4_y = 20;
    //Test Case 3:
     int P1_x=36;
    int P1_y=92;

    int P2_x = 84;
    int P2_y=29;
    
    int P3_x = 62;
    int P3_y = 59;

    int P4_x=36;
    int P4_y = 79;

    SegInt(P1_x, P1_y, P2_x, P2_y, P3_x, P3_y,P4_x, P4_y);

	return 0;
}
