#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <math.h>

#define DEBUG 1
#define TIMES 0

using namespace std;

struct res
{
    vector<int> route;
    long int profit;
};

/*

As a personal rule, John never wants to have more than K shares from a company.
On each day John can perform at most one transaction. A transaction is to buy or sell as many shares as he wants from that company.
The trading exchange that John uses charges a fixed fee R for every share bought, for example, if John buy 2 shares in a transaction the exchange fee would be 2 × R. There is no fee when selling shares.

Tasks
There are three possible tasks.

For the first task (100 points), you have to print the best profit that John could have gotten by trading shares of each company.

For the second task (100 points), for each company, you have to print the best profit and an optimal trading scheme, that is, print the information of how many shares to buy or sell each day to get the best profit.

For the third task (50 points), you have to print the best profit, and how many distinct trading schemes would give that profit value.


Example Task 1
Input
1
2 5 3 10
100 20 40 200 170
10 10 40 10 50
Output
510
150

Example Task 2
Input
2
2 5 3 10
100 20 40 200 170
10 10 40 10 50
Output
510
0 3 0 -3 0
150
1 2 -3 3 -3

Example Task 3
Input
3
2 5 3 10
100 20 40 200 170
10 10 40 10 50
Output
510 1
150 4

Input
The first line of the input gives an integer 1, 2 or 3, denoting which task to solve.

The second line of the input gives four-space separated integers N, D, K, R.

The following N lines give the daily share values of each company. In particular, the daily share values of a company are given by D space-separated integers V1, V2, …, VD that correspond to the values of one share on day 1, 2, …, D, respectively.

Output
For task 1, output one line for each company with an integer that corresponds to the best profit that can be obtained trading the shares of that company.

For task 2, output two lines for each company. The first line should contain a single integer that corresponds to the best profit that can be obtained trading the shares of that company. The second line should give a trading scheme for that company that gives the best profit. If there are multiple optimal trading schemes you can print any. The trading scheme should contain D space-separated integers T1, T2, …, TN that denote the transaction operations on days 1, 2, …, N, respectively. A value Ti < 0 means that on day i John should have sold |Ti| shares, a value Ti > 0 means that John should have bough Ti shares, and a value Ti = 0 mean that John should do nothing on that day.

For task 3, output one line for each company with two space-separated integers. The first integer is the best profit that can be achieved for that company, and the second is the number of distinct trading schemes that would result in that profit value. Since this number may be too large print the result modulo 109 + 7. Take into account the following modulo property:

(a+b) mod  m = ((a mod  m)+(b mod  m)) mod  m
*/
long int bestProfit(vector<int> shareValues, int K, int R, int D);
struct res findRoutes(int D, int K, int R, vector<int> shareValues,long int maxProfit,  vector<int> bestRoute);
void route(vector<int> shareValues, int K, int R, int D);
long int calc_Profit(vector<int> route, vector<int> shareValues, int R);

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int task;
    cin >> task;

    int N, D, K, R;
    cin >> N >> D >> K >> R;

    vector<vector<int>> shareValues(N, vector<int>(D));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < D; j++)
        {
            cin >> shareValues[i][j];
        }
    }

    // select task
    switch (task)
    {
    case 1:
        // first, we need to create a table with fixed cost R and cost, the difference between prices
        for (int i = 0; i < N; i++)
        {
            cout << bestProfit(shareValues[i], K, R, D) << endl;
        }
        break;
    case 2:
        for (int i = 0; i < N; i++)
        {
            route(shareValues[i], K, R, D);
        }
        break;
    case 3:
        break;
    default:
        break;
    }

    return 0;
}

// create best profit function
long int bestProfit(vector<int> shareValues, int K, int R, int D)
{
    // i can buy less K shares in one transaction
    // i can perform at most one transaction per day
    // the fee is R for every share bought, no fee when selling

    // create a vector to store the best profit for each day
    vector<int> maxDelta(D, 0);

    return 0;
}

// For the second task (100 points), for each company, you have to print the best profit and an optimal trading scheme, that is, print the information of how many shares to buy or sell each day to get the best profit.
void route(vector<int> shareValues, int K, int R, int D)
{
    long int maxProfit = 0;
    vector<int> bestRoute(D, 0);

    // STARTING IN THE END OF THE MATRIX, select the route of values, going up and left that have the most |delta|
    res r = findRoutes(D, K, R, shareValues, maxProfit, bestRoute);
    cout << r.profit << endl;
    for (int i = 0; i < r.route.size(); i++)
    {
        cout << r.route[i] << " ";
    }

    cout << endl;
}

// DYNAMIC PROGRAMMING GRAPH
struct res findRoutes(int D, int K, int R, vector<int> shareValues,long int maxProfit,  vector<int> bestRoute)
{

    /*
        2 5 3 10 n d k r
        100 20 40 200 170
    */

    int maxDeltaX = 0;
    int maxDeltaY = 0;
    int maxDelta = 0;
    vector<vector<int>> delta(D, vector<int>(D, 0));
    for (int i = 0; i < D; i++)
    {
        for (int j = i + 1; j < D; j++)
        {
            delta[i][j] = shareValues[j] - shareValues[i];
            if (delta[i][j] > maxDelta)
            {
                maxDelta = delta[i][j];
                maxDeltaX = i;
                maxDeltaY = j;
            }
        }
    }


    /*
        delta matrix
        0 -80 -60 100 70
        0 0 20 180 150
        0 0 0 160 130
        0 0 0 0 -30
        0 0 0 0 0
    */

    // this matrix will be reserved for a trade that we want to make
    vector<int> lockedInterval = {0, 0};

    // start in the end of the matrix, in the case above, start in -30
    for (int i = D - 1; i >= 0; i--)
    {
        for (int j = D - 1; j >= i; j--)
        {
            // we start at the end to find close gap trades. we dont want to start with a trade that could potentialy trades all the profits
            


        }
    }

    // create new struct res to store best route and profit
    res r;
    r.route = bestRoute;
    r.profit = maxProfit;
    return r;
}

// debug func
void p_matrix(vector<vector<int>> matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

long int calc_Profit(vector<int> route, vector<int> shareValues, int R)
{
    long int maxProfit = 0;

    for (int i = 0; i < route.size(); i++)
    {
        if (route[i] > 0)
        {
            maxProfit += route[i] * shareValues[i];
        }
        else if (route[i] < 0)
        {
            maxProfit += route[i] * shareValues[i] - R * route[i]; // maxRoute is negative
        }
    }

    maxProfit = abs(maxProfit);

    cout << "Finding profit for route: ";
    for (int i = 0; i < route.size(); i++)
    {
        cout << route[i] << " ";
    }
    cout << endl;
    cout << "Profit: " << maxProfit << endl;

    return maxProfit;
}