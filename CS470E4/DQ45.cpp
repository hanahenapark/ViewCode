// Question 45. Given a set of line segments, {[P1, P2, P3, P4, …,P(N-1), P(N)}. 
//It represents a polygon, and the vertices are in clockwise order. 
//Please check if point [Pi, Pj] is within this polygon.
//HW 3
// Test case example:
// [P1, P2, P3, P4, P5, P6]: [(75, 55), [70, 38], (22, 1), (13, 85), (31, 93), (73, 78)]
// [Pi, Pj]: [37, 60]
// Output: True

#include<iostream>
#include <climits>
using namespace std;

struct Point {
   int x, y;
};

struct line {
   Point p1, p2;
};

bool onSegment(line l1, Point p) {        //check whether p is on the line or not
   if(p.x <= max(l1.p1.x, l1.p2.x) && p.x <= min(l1.p1.x, l1.p2.x) &&
      (p.y <= max(l1.p1.y, l1.p2.y) && p.y <= min(l1.p1.y, l1.p2.y)))
         return true;

   return false;
}

int Dir(Point a, Point b, Point c) {
   int val = (b.y-a.y)*(c.x-b.x)-(b.x-a.x)*(c.y-b.y);
   if (val == 0)
      return 0;           //colinear
   else if(val < 0)
      return -1;          //counter-clockwise direction
      return 1;          //clockwise direction
}

bool isIntersect(line l1, line l2) {
   //four direction for two lines and points of other line
    int d1 = Dir(l1.p1, l1.p2, l2.p1); //line 2 point 1 on line1
   int d2 = Dir(l1.p1, l1.p2, l2.p2); //line 2 point 2 on line1
   int d3 = Dir(l2.p1, l2.p2, l1.p1);  //line 1 point 1 on line2
   int d4 = Dir(l2.p1, l2.p2, l1.p2); //line 1 point 1 on line2

   if(d1 != d2 && d3 != d4)
      return true;           //they are intersecting
   if(d1==0 && onSegment(l1, l2.p1))         //when p2 of line2 are on the line1
      return true;
   if(d2==0 && onSegment(l1, l2.p2))         //when p1 of line2 are on the line1
      return true;
   if(d3==0 && onSegment(l2, l1.p1))          //when p2 of line1 are on the line2
      return true;
   if(d4==0 && onSegment(l2, l1.p2))         //when p1 of line1 are on the line2
      return true;
   return false;
}

bool inside(Point poly[], int n, Point p) {
   if(n < 3)
      return false;                  //when polygon has less than 3 edge, it is not polygon
   line exline = {p, {20000, p.y}};   //create a point at infinity, y is same as point p
   int count = 0;
   int i = 0;
   do {
      //side 1: point1 to point2, point 2 to point 3, etc...
      line side = {poly[i], poly[(i+1)%n]};     //forming a line from two consecutive points of poly
      if(isIntersect(side, exline)) {          //if side intersects exline
         if(Dir(side.p1, p, side.p2) == 0)      ////if colinear, check if p on side line of polygon
            return onSegment(side, p);
         count++;
      }
      i = (i+1)%n;
   } while(i != 0);
      return count&1;             //when count is odd
      //when count is odd (last bit ends with 1) & with 1 to get 1
      //count being odd indicates that the count number of sides of polygon are on
      //the right side of the point (on horizontal line)
      //odd number indicates that there is a side on the left side of the point,
      //thus encapsulating point
}


int main() {
   // line polygon = {{{0,0},{10,0}},{{10,0},{10,10}},{{10,10},{0,10}},{{0,10},{0,0}}};
   Point polygon[] = {{0, 0}, {10, 3}, {12, 10}, {0, 10}};
   Point p = {13, 3};
   int num_vertexes = 4;

      if(inside(polygon, num_vertexes, p))
      cout << "Point ("<< p.x<<", "<<p.y<<") is inside the polygon.";
   else
      cout << "Point ("<< p.x<<", "<<p.y<<") is not inside polygon.";
}
