#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <math.h>

#define DEBUG 1
#define TIMES 0

using namespace std;

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

struct res
{
    vector<int> route;
    long long int profit;
};

long long int travelThroughOptions(int company);
res route(int company);
long long int calc_Profit(vector<int> route, vector<int> shareValues, int R);

int N, D, K, R;
vector<vector<long long int>> shareValues;
vector<vector<long long int>> buysSells;



int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int task;
    cin >> task;

    cin >> N >> D >> K >> R;

    // initialzie shareValues with (N, vector<int>(D));
    shareValues.resize(N, vector<long long int>(D));
    buysSells.resize(N, vector<long long int>(D));

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
            cout << travelThroughOptions(i) << endl;
        }
        break;
    case 2:
        for (int i = 0; i < N; i++)
        {
            struct res r = route(i);
            cout << r.profit << endl;
            for (int j = 0; j < r.route.size(); j++)
            {
                if (j == r.route.size() - 1)
                {
                    cout << r.route[j] << endl;
                    continue;
                }
                cout << r.route[j] << " ";
            }
        }
        break;
    case 3:
        break;
    default:
        break;
    }

    return 0;
}

void p_matrix(vector<vector<long long int>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

// modify the travelThroughOptions(int company) function to also save each action in a vector K, -K + R, 0
res route(int company)
{
    // will keep track of buys and cells
    vector<vector<long long int>> dailyProfit = vector<vector<long long int>>(D, vector<long long int>(2));
    vector<vector<int>> transactions = vector<vector<int>>(D, vector<int>(2));
    vector<int> bestRoute = vector<int>(D);

    for (int i = 0; i < D; i++)
    {
        long long int currentShares = shareValues[company][i];
        if (i == 0)
        {
            // can only buy
            dailyProfit[i][1] -= (currentShares + R) * K;
            transactions[i][1] = K;

            /* cout << "i: " << i << endl;
            for (int j = 0; j < dailyRoute[i].size(); j++)
            {
                cout << dailyRoute[i][j] << " ";
            }
            cout << endl; */
            continue;
        }
        // (dailyProfit[i - 1][1] + (currentShares * K) is the sell option
        if (dailyProfit[i - 1][0] > (dailyProfit[i - 1][1] + (currentShares * K)))
        {
            dailyProfit[i][0] = dailyProfit[i - 1][0];
            transactions[i][0] = 0;
        }
        else
        {
            dailyProfit[i][0] = dailyProfit[i - 1][1] + (currentShares * K);
            transactions[i][0] = -K;
        }

        // dailyProfit[i][1] = max(dailyProfit[i - 1][1], dailyProfit[i - 1][0] - (currentShares + R) * K); // buy
        if (dailyProfit[i - 1][1] > (dailyProfit[i - 1][0] - (currentShares + R) * K))
        {
            dailyProfit[i][1] = dailyProfit[i - 1][1];
            transactions[i][1] = 0;
        }
        else
        {
            dailyProfit[i][1] = dailyProfit[i - 1][0] - (currentShares + R) * K;
            transactions[i][1] = K;
        }
    }
    // find best route by starting at the end and going backwards until finding a 0
    // when we do switch the buys and cells because that means profit.
    int i = D - 1;
    int j = 0;
    while (i >= 0)
    {
        if (transactions[i][j] == 0)
        {
            i--;
            continue;
        }
        bestRoute[i] = transactions[i][j];
        if (j == 0)
        {
            j = 1;
        }
        else
        {
            j = 0;
        }
        i--;
    }

    struct res r;
    r.profit = dailyProfit[D - 1][0];
    r.route = bestRoute;
    return r;
}

// generate tree with buys in the left branch and sells in the right branch. Recursively, we can find the best route
long long int travelThroughOptions(int company)
{
    // will keep track of buys and cells
    vector<vector<long long int>> dailyProfit = vector<vector<long long int>>(D, vector<long long int>(2));
    for (int i = 0; i < D; i++)
    {
        long long int currentShares = shareValues[company][i];
        if (i == 0)
        {
            // can only buy
            dailyProfit[i][1] -= (currentShares + R) * K;

            /* cout << "i: " << i << endl;
            p_matrix(dailyProfit); */
            continue;
        }
        dailyProfit[i][0] = max(dailyProfit[i - 1][0], dailyProfit[i - 1][1] + (currentShares * K));     // sell
        dailyProfit[i][1] = max(dailyProfit[i - 1][1], dailyProfit[i - 1][0] - (currentShares + R) * K); // buy

        /* cout << "i: " << i << endl;
        p_matrix(dailyProfit); */
    }

    return dailyProfit[D - 1][0];
}

// DYNAMIC PROGRAMMING GRAPH
struct res findRoutes(int D, int K, int R, vector<int> shareValues, long long int maxProfit, vector<int> bestRoute)
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
