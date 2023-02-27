#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
// given 3 different points (x,y) , find if they are colinear

/*
    3 for loops is bad, O(n^3), calculate the declive of each line and compare them
    
    S = {x1, x2, x3}
    P = {(x1, x1^3), (x2, x2^3), (x3, x3^3)}
    
    ! a + b + c = 0 => (a,a^3), (b,b^3), (c,c^3) is going to be colinear 

    * (a^3 - b^3) / (a - b) = (b^3 - c^3) / (b - c)
    * (a-b)(a^2+ ab + b^2)/(a-b) = (b-c)(b^2 + bc + c^2)/(b-c)
    * (a^2 + ab + b^2) = (b^2 + bc + c^2)
    * a^2 + ab - bc - c^2 = 0
    * a (a + b) - b (b + c) = 0
    * a (a + b + c) - c (a + b + c) = 0
    * (a + b + c) (a - c) = 0
    * a + b + c = 0 or a - c = 0
*/

int main() {
    std::vector<int> Point1 = { 1, 2 };
    std::vector<int> Point2 = { 3, 4 };
    std::vector<int> Point3 = { 5, 6 };

    std::cout << "Are they coliinear? " << is_Colinear(Point1, Point2, Point3) << std::endl;
    
}

int is_Colinear(std::vector<int> Point1, std::vector<int> Point2, std::vector<int> Point3) {
    // y = mx + b
    int m = calculate_declive(Point1, Point2);




}

double calculate_declive(std::vector<int> Point1, std::vector<int> Point2) {
    return (Point2[1] - Point1[1]) / (Point2[0] - Point1[0]);
}