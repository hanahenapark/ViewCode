// Question 44: Given a set of line segments, 
//{[P1, P2], [P3, P4], …,[P(N-1), P(N)]}, 
//determine whether any pair of segments intersects.
//Look at 43
// Test case example 1:
// [P1, P2]: [(49, 92), (58, 80)]
// [P3, P4]: [(40, 78), (68, 50)]
// [P5, P6]: [(99, 91), (37, 72)]
// [P7, P8]: [(97, 34), (55, 68)]
// [P9, P10]:       	[(74, 89), (93, 4)]
// [P11, P12]:     	[(63, 66), (49, 11)]
// [P13, P14]:     	[(53, 14), (24, 73)]
// [P15, P16]:     	[(46, 27), (29, 10)]
// Output: True
 
// Test case example 2:
// [P1, P2]: [(32, 41), (12, 45)]
// [P3, P4]: [(87, 11), (27, 7)]
// [P5, P6]: [(94, 37), (65, 77)]
// [P7, P8]: [(65, 98), (55, 94)]
// [P9, P10]:       	[(64, 78), (48, 63)]
// [P11, P12]:     	[(100, 10), (58, 64)]
// [P13, P14]:     	[(73, 14), (59, 19)]
// [P15, P16]:     	[(46, 27), (29, 10)]
// Output: False

#include <cstring>
#include <iostream>
#include <vector>
#include <stdbool.h>
using namespace std;
struct Point {
   int p1x, p1y,p2x,p2y;
};

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

bool SegInt(int p1x,int p1y,int p2x,int p2y,int p3x,int p3y, int p4x,int p4y){
double d1 = Dir(p3x,p3y,p4x,p4y, p1x, p1y);
double d2 = Dir(p3x,p3y,p4x,p4y, p2x, p2y);
double d3 = Dir( p1x, p1y, p2x, p2y, p3x,p3y);
double d4 = Dir(p1x, p1y, p2x, p2y, p4x,p4y);

if(((d1>0 && d2<0)||(d1<0 && d2>0))&&((d3>0 && d4<0)||(d3<0 && d4>0))){
  //  cout<<"True1"<<endl;
    return true;
}
else if((d1==0) && (onSeg(p3x,p3y,p4x,p4y, p1x, p1y))){
  // cout<<"True2"<<endl;
    return true;
}
else if((d2==0) && (onSeg(p3x,p3y,p4x,p4y, p2x, p2y))){
 //  cout<<"True3"<<endl;
    return true;
}
else if((d3==0) && (onSeg(p1x, p1y, p2x, p2y, p3x,p3y))){
  // cout<<"True4"<<endl;
     return true;
}
else if((d3==0) && (onSeg(p1x, p1y, p2x, p2y, p4x,p4y))){
  // cout<<"True5"<<endl;
     return true;
}
else{
    //cout<<"False"<<endl;
      return false;
}


}

// Driver program to test above function
int main()
{
 
    Point polygon1[] = {{49, 92,58, 80},
                        {40, 78,68, 50},
                        {99, 91,37, 72},
                        {97, 34,55, 68},
      	                {74, 89,93, 4},
  	                    {63, 66,49, 11},
   	                    {53, 14,24, 73},
     	                {46, 27,29, 10}};

    //  Point polygon1[] =   
    //                 {{32, 41,12, 45},
    //                 {87, 11,27, 7},
    //                 {94, 37,65, 77},
    //                 {65, 98,55, 94},
   	//                 {64, 78,48, 63},
	//                 {100, 10,58, 64},
   	//                 {73, 14,59, 19},
   	//                 {46, 27,29, 10}};
    int num_Seg=8;
    //polygon1[(i+1)%num_Seg].ptx
    int interSect=0;
    for(int j=0;j<num_Seg;j++){
        if(interSect==0){
           // cout<<"top"<<endl;
        for(int i=0;i<num_Seg;i++){
            if(i!=j){
        int P1_x=polygon1[i%num_Seg].p1x;
        int P1_y=polygon1[i%num_Seg].p1y;

        int P2_x = polygon1[i%num_Seg].p2x;
        int P2_y=polygon1[i%num_Seg].p2y;

        int P3_x = polygon1[j%num_Seg].p1x;
        int P3_y = polygon1[j%num_Seg].p1y;

        int P4_x= polygon1[j%num_Seg].p2x;
        int P4_y = polygon1[j%num_Seg].p2y;
        if(SegInt(P1_x, P1_y, P2_x, P2_y, P3_x, P3_y,P4_x, P4_y)){
           // cout<<"lines intersect"<<endl;
            interSect=1;
            break;
        }
            }
        }
        }
    }


    if(interSect==1){
        cout<<"True\n";
    }
    else{
        cout<<"False";
    }

	return 0;
}

