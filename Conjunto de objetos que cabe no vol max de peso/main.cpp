#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;
// conjunto de pizas e queremos dividir as pizzas pelos fornos.
// cada forno tem um volume maximo de peso e um volume maximo de pizzas
/*
    O máximo de tempo que demora é 1+2+1+3.

*/

/*
    Pizzas:
    id | tempo
    1  | 1
    2  | 2
    3  | 1
    4  | 3

    Create a table that conatins:

    N Pizzas \ Pizza Time |     0     |     1     |     2     |     3     |     4     |     5     |     6     |     7     |     8     |     9     |     10    |     11    |     12    |     13    |     14    |     15    |     16    |     17    |     18    |     19    |     20    |     21    |     22    |     23    |     24    |     25    |     26    |     27    |     28    |     29    |     30    |     31    |     32    |     33    |     34    |     35    |     36    |     37    |     38    |     39    |     40    |     41    |     42    |     43    |     44    |     45    |     46    |     47    |     48    |     49    |     50    |     51    |     52    |     53    |     54    |     55    |     56    |     57    |     58    |     59    |     60    |     61    |     62    |     63    |     64    |     65    |     66    |     67    |     68    |     69    |     70    |     71    |     72    |     73    |     74    |     75    |     76    |     77    |     78    |     79    |     80    |     81    |     82    |     83    |     84    |     85    |     86    |     87    |     88    |     89    |     90    |     91    |     92    |     93    |     94    |     95    |     96    |     97    |     98    |     99    |     100   |
    ----------------------|-----------|-----------|-------------------------
            0  -> Pizzas  |    True   |    False  |   False   |    False -> Can't make 0 pizzas in *exactly* 3 minutes.
            1  -> Pizzas  |    True   |    True   |   False   |    False -> Can't make 1 pizzas in *exactly* 3 minutes.
            2  -> Pizzas  |    True   |    True   |   True    |     True -> We can make 2 pizzas in *exactly* 3 minutes. Pizza 1 and Pizza 2.

            since we have a subconjunto that can produce all the times, from now on we can fill the table with True.

            3  -> Pizzas  |    True   |    True   |   True    |     True
            4  -> Pizzas  |    True   |    True   |   True    |     True
            5  -> Pizzas  |    True   |    True   |   True    |     True
                                            ...
*/

vector<vector<int>> dp;
vector<int> pizzas_time;

void pizza(int n, int s);
void print_dp(int n, int s);

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int num_pizzas;
    int max_sum = 20;
    // The first line gives the number of pizzas (N). The next N lines gives the cooking time for each pizza as an integer number. Other tests cases may follow.
    // 1 ≤ N ≤ 100. Maximum cooking time of a pizza is 200 minutes.
    cin >> num_pizzas;

    // initilize the vector of pizzas
    for (int i = 0; i <= num_pizzas; i++)
    {
        vector<int> temp;
        for (int j = 0; j <= max_sum; j++)
        {
            temp.push_back(0);
        }
        dp.push_back(temp);
    }

    for (int as = 0; as < num_pizzas; as++)
    {
        int cooking_time;
        cin >> cooking_time;
        pizzas_time.push_back(cooking_time);

        if (as == num_pizzas - 1)
        {
            pizza(num_pizzas, max_sum);
            print_dp(num_pizzas, max_sum);
            
            // check if is eof
            if (cin.eof())
            {
                break;
            }
            cin >> num_pizzas;
            as = -1;


            // reset vectors
            dp.clear();
            pizzas_time.clear();
        }
    }

    return 0;
}

// DYNAMIC PROGRAMMING GRAPH, está mal.
void pizza(int n, int s)
{
    for (int i = 0; i <= n; i++)
    {
        dp[i][0] = true;
    }
    for (int j = 1; j <= floor(s / 2); j++)
    {
        dp[0][j] = false;
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= floor(s / 2); j++)
        {
            if (j >= pizzas_time[i])
            {
                dp[i][j] = dp[i - 1][j] || dp[i - 1][j - pizzas_time[i]];
            }
            else
            {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
}

// print dp with the resect values of n pizzas and pzizas time on the sides
void print_dp(int n, int s)
{
    cout << "    |";
    for (int i = 0; i <= floor(s / 2); i++)
    {
        cout << " " << i << "  |";
    }
    cout << endl;
    cout << "----|";
    for (int i = 0; i <= floor(s / 2); i++)
    {
        if (i >= 10)
        {
            cout << "-----|";
        }
        else
        {
            cout << "----|";
        }
    }
    cout << endl;
    for (int i = 0; i <= n; i++)
    {
        cout << " " << i << "  |";
        for (int j = 0; j <= floor(s / 2); j++)
        {
            if (dp[i][j])
            {
                if (j >= 10)
                {
                    cout << "  T  |";
                }
                else
                {
                    cout << " T  |";
                }
            }
            else
            {
                if (j >= 10)
                {
                    cout << "  F  |";
                }
                else
                {
                    cout << " F  |";
                }
            }
        }
        cout << endl;
    }
    cout << "#########################################################" << endl;
}