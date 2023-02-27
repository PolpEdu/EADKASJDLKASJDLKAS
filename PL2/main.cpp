#include <vector>
#include <iostream>
#include <sstream>

/*
Description
You are working on a new chess computer game with an infinite chessboard and only knights. Of course, you cannot have an infinite chessboard because you don't have infinite RAM!
But you can compute the largest chessboard that you need since you know the current location of the knights and how many moves they can do.
The goal is to know the number of distinct chessboard cells that can be visited by n knights after a given number of moves of each knight.
You can assume that two or more knights can be in the same cell at the same time.
We remind you that a knight can perform 8 different moves, as shown in the picture below.

Input
The first line indicates the number of knights (1 <= n <= 30). Then, n lines follow. Each line gives the position (x,y) of a knight in the chessboard as well as the number of moves m (0 <= m <= 7) that it is allowed to perform.
You may assume that knights never travel beyond coordinates -200 to 200.

Output
For each test case, print the number of distint cells that can be visited by the n knights.


Example input:
4
-1 -1 2
2 2 1
3 3 3
4 4 3

Example output:
155
*/

int chess(std::vector<int *> knights, int num_knights);
int calulate_jumps(int x, int y, int moves);

// visited contains a 400 by 400 array of bools
// visited = {{true, false, false, ...}, {false, true, false, ...}, ...}
std::vector<bool *> visited;

// debug command
// g++ -g -Wall -std=c++11 main.cpp -o main && gdb main
// in gdb run with: run < input.txt

// release command
// g++ -O2 -Wall -std=c++11 main.cpp -o main && ./main

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int num_knights, i = 0;

    // list of knights
    std::vector<int *> knights;

    // alocate memory for the visited array
    for (int i = 0; i < 400; i++)
    {
        bool *row = new bool[400];
        visited.push_back(row);
    }

    while (true)
    {
        if (i == 0)
        {
            std::cin >> num_knights;
            i++;
            continue;
        }

        if (i > num_knights)
        {
            break;
        }

        // read input normally and store the values in a vector int[3]
        int *knight = new int[3];
        std::cin >> knight[0] >> knight[1] >> knight[2];
        knights.push_back(knight);

        // if we read EOF break
        if (std::cin.eof() || std::cin.fail())
        {
            break;
        }
        i++;
    }

    int final_value = chess(knights, num_knights);

    std::cout << final_value << std::endl;

    return 0;
}

// knights is {{x,y,moves}, {x,y,moves}, ...}
// get the number of distint cells that can be visited by the n knights stored in the knight array
int chess(std::vector<int *> knights, int num_knights)
{
    int num_moves = 0;

    for (int i = 0; i < num_knights; i++)
    {
        int *knight = knights[i];
        int x = knight[0];
        int y = knight[1];
        int moves = knight[2];

        if (moves < 0)
        {
            continue;
        }

        num_moves += calulate_jumps(x, y, moves);
    }

    return num_moves;
}

int calulate_jumps(int x, int y, int moves)
{
    if (moves < 0 || x > 200 || x < -200 || y > 200 || y < -200)
    {
        return 0;
    }

    int num_moves = 0;

    // mark the current position as visited
    if (visited[x + 200][y + 200] == false)
    {
        num_moves++;
        visited[x + 200][y + 200] = true;
    }


    // up left
    num_moves += calulate_jumps(x + 1, y + 2, moves - 1);

    // left up
    num_moves += calulate_jumps(x + 2, y + 1, moves - 1);

    // up right
    num_moves += calulate_jumps(x - 1, y + 2, moves - 1);

    // right up
    num_moves += calulate_jumps(x - 2, y + 1, moves - 1);

    // down left
    num_moves += calulate_jumps(x + 1, y - 2, moves - 1);

    // left down
    num_moves += calulate_jumps(x + 2, y - 1, moves - 1);

    // down right
    num_moves += calulate_jumps(x - 1, y - 2, moves - 1);

    // right down
    num_moves += calulate_jumps(x - 2, y - 1, moves - 1);

    return num_moves;
}