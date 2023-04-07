//Hana Park
//CS470 Exam 4 Question 1
//Handles both convex and concave polygons by seeing it lines intersect

#include <cstring>
#include <iostream>
#include <vector>
#include <stdbool.h>
using namespace std;
struct Point {
   int p1x, p1y;
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
    //{{84, 97}, {97, 84}, (94, 70}, (80, 53}, (55, 74}, (60, 91}};
    //{{39, 70}, (80, 63}, (78, 19}, (34, 37}, (32, 55}};

    //cout<<"Test Case 1"<<endl;
    // Point polygon1[] = {{61,100},{99,84},{76,78},{62,64}};
    // Point polygon2[]={{8,58},{74,55},{71,27},{64,11},{4,21}};
    // cout<<"Test Case 2"<<endl;
    // Point polygon1[] = {{59,94},{78,100},{100,59},{58,68},{51,73}};
    // Point polygon2[]={{31,48},{69,39},{52,10},{16,19},{13,35}};
    cout<<"Test Case 3"<<endl;
    Point polygon1[] =  {{31,63},{74,96},{62,33},{27,20},{29,54}};
    Point polygon2[]={{32,63},{73,70},{67,55},{54,37},{30,9},{2,19}};

    //NOTE: MUST CHANGE
    int num_Seg_p1=5;
    int num_Seg_p2=6;
    //polygon1[(i+1)%num_Seg].ptx
    int interSect=0;
    for(int j=0;j<num_Seg_p1;j++){
        if(interSect==0){
           // cout<<"top"<<endl;
        for(int i=0;i<num_Seg_p2;i++){
            
       
        int P1_x=polygon1[i%num_Seg_p1].p1x;
        int P1_y=polygon1[i%num_Seg_p1].p1y;

        int P2_x = polygon1[(i+1)%num_Seg_p1].p1x;
        int P2_y=polygon1[(i+1)%num_Seg_p1].p1y;

        int P3_x = polygon2[(j)%num_Seg_p2].p1x;
        int P3_y = polygon2[(j)%num_Seg_p2].p1y;

        int P4_x= polygon2[(j+1)%num_Seg_p2].p1x;
        int P4_y = polygon2[(j+1)%num_Seg_p2].p1y;

        if(SegInt(P1_x, P1_y, P2_x, P2_y, P3_x, P3_y,P4_x, P4_y)){
           // cout<<"lines intersect"<<endl;
            interSect=1;
            break;
        }
             }
            
        }
        
    }


    if(interSect==1){
        cout<<"Polygons Intersect\n";
    }
    else{
        cout<<"Polygons do not intersect";
    }

	return 0;
}

