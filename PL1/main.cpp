// Read a list of numbers until eof, and print them
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

/*

Problem Description
To cellebrate its success, a new company throws a really big party. Everyone who is someone in town is invited. To help people get to know each other, each guest is given a card with a number (a positive integer) upon arrival, and asked to find two other guests in the crowd to form a group of three such that:
there is at least one man and one woman in the group
the sum of the numbers held by the two women (or two men) in the group is equal to the number held by the man (or woman, respectively).
The first group to meet these conditions will win 60.000€, or so say the party organizers...
Since nobody could form such a group until the end of the party, some people demanded that all cards be collected to see whether the game was rigged so that the prize would not have to be paid. It is your job to find out whether, given the numbers handed out at the party, it was possible to win the game and claim the prize.

Input
For each test case, the number of guests, n, is given in the first line, and each guest's number is given in the next n lines. To distinguish between men and women at the party, men's numbers were recorded as negative numbers, whereas women's numbers were recorded as positive. The end of the test case is indicated by a line containing "0". A new test case may then follow.
Output
The output for each test case consists of a single line saying "Fair" if the game was fair and a winning group could indeed be found. Otherwise, the output must be "Rigged".

3 for loops it's not enough to solve the problem.

Makeout a solution and give explanations on what you did.

Input
For each test case, the number of guests, n, is given in the first line, and each guest's number is given in the next n lines. To distinguish between men and women at the party, men's numbers were recorded as negative numbers, whereas women's numbers were recorded as positive. The end of the test case is indicated by a line containing "0". A new test case may then follow.
Output
The output for each test case consists of a single line saying "Fair" if the game was fair and a winning group could indeed be found. Otherwise, the output must be "Rigged".

Example input:

8
-1
3
5
-2
6
2
-5
7
0
4
-1
-2
1
2
0
Example output:

Fair
Rigged
*/
int solve(std::vector<int> numbers);

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int num, i = 0, n;
    // create list of numbers
    std::vector<int> numbers;
    
    std::vector<int> solutions;

    // read numbers until eof
    while (true)
    {
        std::cin >> num;
        
        // if we read EOF break
        if (std::cin.eof() || std::cin.fail()) {
            break;
        }

        if (num != 0) {
            numbers.push_back(num);
        } else {
            // add to the str the solve solution
            solutions.push_back(solve(numbers));
            // clear the vector
            numbers.clear();
            
            num = 0;

        }

    }

    // if the solution is 0, print "Fair", else print "Rigged"
    for (i = 0; i < solutions.size(); i++) {
        if (solutions[i] == 0) {
            std::cout << "Fair" << std::endl;
        } else {
            std::cout << "Rigged" << std::endl;
        }
    }

    return 0;
}

// max of 3 sum ever found is O(n^2)
// function to solve the problem and returns "Rigged" or "Fair"
int solve(std::vector<int> numbers) {
    int n = numbers.size();
    int i, j, k;

    // sort the vector
    std::sort(numbers.begin(), numbers.end());

    for (i = 0; i < n-2; i++) {
        j = i + 1;
        k = n - 1;

        while (j < k) {
            int sum = numbers[i] + numbers[j] + numbers[k];

            if (sum == 0) {
                return 0;
            } else if (sum > 0) { // walk to the left
                k--;
            } else { // walk to the right
                j++;
            }
        }
    }

    return 1;
}