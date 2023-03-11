#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <math.h>

#define DEBUG 1
#define TIMES 1
#define DEBUG_GEN 0

using namespace std;

struct QR_Code
{
    int N;
    int **cells;

    std::vector<int> cb;
    std::vector<int> lb;
    std::vector<int> ct;
    std::vector<int> lt;

    int qb[4];
    int db[2];

    QR_Code(int N)
    {
        this->N = N;
        this->cells = new int *[N];
        for (int i = 0; i < N; i++)
        {
            this->cells[i] = new int[N];
        }
    }

    QR_Code()
    {
        this->N = 0;
        this->cells = NULL;
    }

    QR_Code copy()
    {
        QR_Code new_qr_code = QR_Code(this->N);
        for (int i = 0; i < this->N; i++)
        {
            for (int j = 0; j < this->N; j++)
            {
                new_qr_code.cells[i][j] = this->cells[i][j];
            }
        }
        return new_qr_code;
    }

    void reset_configs()
    {
        // reset the vector values to 0 but keep the size
        this->cb.assign(this->cb.size(), 0);
        this->lb.assign(this->lb.size(), 0);
        this->ct.assign(this->ct.size(), 0);
        this->lt.assign(this->lt.size(), 0);
        this->qb[0] = 0;
        this->qb[1] = 0;
        this->qb[2] = 0;
        this->qb[3] = 0;
        this->db[0] = 0;
        this->db[1] = 0;
    }

    // check if the values given as parameters are the same as the qr code
    int check_if_valid(vector<int> cb, vector<int> lb, vector<int> ct, vector<int> lt, int *qb, int *db)
    {
        // loop through qr and update the values
        for (int l = 0; l < this->N; l++)
        {
            for (int c = 0; c < this->N; c++)
            {
                /* color transitions */
                if (c != 0)
                {
                    /* cout << "yoyo that's ya boi, c: " << c << " l: " << l << endl;
                    printQR_code(qr_code); */
                    if (this->cells[l][c] % 2 != this->cells[l][c - 1] % 2)
                    {
                        this->lt[l]++;
                        if (this->lt[l] > lt[l])
                        {
                            return 0;
                        }
                    }
                }

                if (l != 0)
                {
                    if (this->cells[l][c] % 2 != this->cells[l - 1][c] % 2)
                    {
                        this->ct[c]++;

                        if (this->ct[c] > ct[c])
                        {
                            // cout << "ct fds" << endl;
                            return 0;
                        }
                    }
                }

                /* counting */
                if (this->cells[l][c] % 2 == 1)
                {
                    /* black cells column */
                    this->cb[c]++;
                    if (this->cb[c] > cb[c])
                    {
                        return 0;
                    }

                    /* black cells inline */
                    this->lb[l]++;
                    if (this->lb[l] > lb[l])
                    {
                        return 0;
                    }

                    /* diagonals */
                    if (l == c)
                    {
                        this->db[0]++;
                        if (this->db[0] > db[0])
                        {
                            return 0;
                        }
                    }

                    if (l + c == this->N - 1)
                    {
                        this->db[1]++;
                        if (this->db[1] > db[1])
                        {
                            return 0;
                        }
                    }

                    /* quadrants
                        1 0
                        2 3
                    */
                    if (l < (this->N - 1) / 2.0)
                    {
                        if (c < (this->N - 1) / 2.0)
                        {
                            this->qb[1]++;
                            if (this->qb[1] > qb[1])
                            {
                                return 0;
                            }
                        }
                        else if (c > (this->N - 1) / 2.0)
                        {
                            this->qb[0]++;
                            if (this->qb[0] > qb[0])
                            {
                                return 0;
                            }
                        }
                    }
                    else if (l > (this->N - 1) / 2.0)
                    {
                        if (c < (this->N - 1) / 2.0)
                        {
                            this->qb[2]++;
                            if (this->qb[2] > qb[2])
                            {
                                return 0;
                            }
                        }
                        else if (c > (this->N - 1) / 2.0)
                        {
                            this->qb[3]++;
                            if (this->qb[3] > qb[3])
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }

        if (DEBUG)
        {
            for (int i = 0; i < N; i++)
            {
                cout << "cb e this: " << i << endl;
                cout << "cb: " << cb[i] << " " << this->cb[i] << endl;
                cout << "lb: " << lb[i] << " " << this->lb[i] << endl;
                cout << "ct: " << ct[i] << " " << this->ct[i] << endl;
                cout << "lt: " << lt[i] << " " << this->lt[i] << endl;
            }

            cout << "...." << endl;

            for (int i = 0; i < 4; i++)
            {
                cout << "qb: " << qb[i] << " " << this->qb[i] << endl;
            }

            cout << "...." << endl;

            for (int i = 0; i < 2; i++)
            {
                cout << "db: " << db[i] << " " << this->db[i] << endl;
            }

            cout << "gives:" << endl;
            cout << "cb == this->cb: " << (cb == this->cb) << endl;
            cout << "lb == this->lb: " << (lb == this->lb) << endl;
            cout << "ct == this->ct: " << (ct == this->ct) << endl;
            cout << "lt == this->lt: " << (lt == this->lt) << endl;
            cout << "equal(qb, qb + 4, this->qb): " << equal(qb, qb + 4, this->qb) << endl;
            cout << "equal(db, db + 2, this->db): " << equal(db, db + 2, this->db) << endl;
            cout << "............." << endl;
        }

        return cb == this->cb && lb == this->lb && ct == this->ct && lt == this->lt && equal(qb, qb + 4, this->qb) && equal(db, db + 2, this->db);
    }

    void set_all_to(int value)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                this->cells[i][j] = value;
            }
        }
    }

    void print_qr_code()
    {
        cout << "+";
        for (int i = 0; i < N; i++)
        {
            cout << "-";
        }
        cout << "+" << endl;
        for (int i = 0; i < N; i++)
        {
            cout << "|";
            for (int j = 0; j < N; j++)
            {
                if (this->cells[i][j] % 2 == 0)
                {
                    cout << " ";
                }
                else
                {
                    cout << "#";
                }
            }
            cout << "|" << endl;
        }
        cout << "+";
        for (int i = 0; i < N; i++)
        {
            cout << "-";
        }
        cout << "+" << endl;
    }
};

int qr_codes_generated;
QR_Code qr_to_print;

int start_generating_tree(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int qb[], int db[]);
int check_cell(QR_Code qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
void loop_through_all_cells(QR_Code *qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
void printQR_code(QR_Code qr_code);
int sum(std::vector<int> array, int N);
int sum2(int *array, int N);
bool defect_detection(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
QR_Code pre_processing(QR_Code qr_code, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int i = 0;
    int num = 0; // number of encodings to be assessed
    int N = 0;   // size of the QR code in lines/columns: 2 ≤ N ≤ 30

    if (i == 0)
    {
        std::cin >> num;
    }

    for (int i = 0; i < num; i++)
    {
        qr_codes_generated = 0;

        std::cin >> N;
        qr_to_print = QR_Code(N);

        if (N < 2 || N > 30)
        {
            std::cout << "N must be between 2 and 30" << std::endl;
            return 0;
        }

        std::vector<int> lb(N, 0);
        std::vector<int> cb(N, 0);
        std::vector<int> lt(N, 0);
        std::vector<int> ct(N, 0);
        int qb[4];
        int db[2];

        for (int i = 0; i < N; i++)
        {
            std::cin >> lb[i];
        }

        for (int i = 0; i < N; i++)
        {
            std::cin >> cb[i];
        }

        for (int i = 0; i < N; i++)
        {
            std::cin >> lt[i];
        }

        for (int i = 0; i < N; i++)
        {
            std::cin >> ct[i];
        }

        for (int i = 0; i < 4; i++)
        {
            std::cin >> qb[i];
        }

        for (int i = 0; i < 2; i++)
        {
            std::cin >> db[i];
        }
        // count time
        auto start = std::chrono::high_resolution_clock::now();

        start_generating_tree(N, lb, cb, lt, ct, qb, db);

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        if (TIMES)
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    }
    return 0;
}

int start_generating_tree(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{

    // first detect if the settings are valid
    if (defect_detection(N, lb, cb, lt, ct, qb, db) == true)
    {
        if (DEBUG) 
            std::cout << "DEFECT: No QR Code generated DEFECT DETECTED!" << std::endl;
        else 
            std::cout << "DEFECT: No QR Code generated!" << std::endl;
        return 0;
    }

    // generate blank qr_code
    QR_Code qr_code = QR_Code(N);
    qr_code.set_all_to(2);

    // reset the qr_code.cb to vector of N size to zero
    qr_code.cb = std::vector<int>(N, 0);
    qr_code.lb = std::vector<int>(N, 0);
    qr_code.ct = std::vector<int>(N, 0);
    qr_code.lt = std::vector<int>(N, 0);

    qr_code.qb[0] = 0;
    qr_code.qb[1] = 0;
    qr_code.qb[2] = 0;
    qr_code.qb[3] = 0;

    qr_code.db[0] = 0;
    qr_code.db[1] = 0;

    // we need to do some pre processing, set the cells that need to be certain values according to the settings lb, cb, lt, ct, qb, db
    QR_Code qr_code_new = pre_processing(qr_code, lb, cb, lt, ct, qb, db);

    /*
        we need to do some pre processing, set the cells that need to be certain values according to the settings lb, cb, lt, ct, qb, db
        also, we need to find the defects (find if the current lb, cb, lt, ct, qb, db are valid without generating the qr code)
        make sure to optimize the tree sicne the recursion will be very deep in some cases
    */
    // carefull this function is recursive and will run thousands of times
    // cout << "branch:" << endl;
    loop_through_all_cells(&qr_code_new, 0, 0, lb, cb, lt, ct, qb, db);
    qr_code_new.cells[0][0] = 3;
    loop_through_all_cells(&qr_code_new, 0, 0, lb, cb, lt, ct, qb, db);

    if (qr_codes_generated == 0)
    {
        std::cout << "DEFECT: No QR Code generated!" << std::endl;
        return 0;
    }

    // check the size of qr_code.qr_codes_generated and make sure its 0
    if (qr_codes_generated == 1)
    {
        std::cout << "VALID: 1 QR Code generated!" << std::endl;
        qr_to_print.print_qr_code();
        return 0;
    }

    if (qr_codes_generated > 1)
    {
        std::cout << "INVALID: " << qr_codes_generated << " QR Codes generated!" << std::endl;
        return 0;
    }

    return 0;
}

void loop_through_all_cells(QR_Code* qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{

    // cout << "l: " << l << " c: " << c << endl;
    if (qr_code->cells[l][c] == 1 || qr_code->cells[l][c] == 0)
    {
        if (l == qr_code->N - 1 && c == qr_code->N - 1)
        {
            if (qr_code->check_if_valid(lb, cb, lt, ct, qb, db))
            {
                if (qr_codes_generated == 0)
                {
                    qr_to_print = qr_code->copy();
                }
                qr_codes_generated++;
            }
            return;
        }

        if (c == qr_code->N - 1)
        {
            l++;
            c = 0;
        }
        else
        {
            c++;
        }
        loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db);

        return;
    }



    if (l == qr_code->N - 1 && c == qr_code->N - 1)
    {
        if (qr_code->check_if_valid(lb, cb, lt, ct, qb, db))
        {
            if (qr_codes_generated == 0)
            {
                qr_to_print = qr_code->copy();
            }
            qr_codes_generated++;
        }

        return;
    }

    if (c == qr_code->N - 1)
    {
        l++;
        c = 0;
    }
    else
    {
        c++;
    }

    // if it is valid, we'll add the next possible qr_codes to the tree
    // cout << "going left branch -> "
    //      << "c: " << c << " l: " << l << endl;
    loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db);

    qr_code->cells[l][c] = 3;
    // cout << "going right branch -> "
    //      << "c: " << c << " l: " << l << endl;
    loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db);

    qr_code->cells[l][c] = 2;
}

// this function will pre_process all the cell that fill the criteria of those parameters and
QR_Code pre_processing(QR_Code qr_code, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{
    // i have a qr_code with all cells set to 2
    // i'll set the cells that are certain to be black or white according to the given info
    // we'll mark each cell with a number 0 or 1 that is certain to appear in the qr_code
    // with this we'll reduce the number of possible qr_codes and automatically eliminate some of them before starting the backtracking algorithm

    for (int i = 0; i < qr_code.N; i++)
    {
        int num_black = lb[i];
        int num_transitions = lt[i];

        // process rows, if the number of black cells is equal to the number of black cells in the given info, then we'll set all cells to 1
        if (num_black == qr_code.N)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[i][j] = 1;
            }
            qr_code.lb[i] = qr_code.N;
            qr_code.lt[i] = 0;
        }
        else if (num_black == 0)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[i][j] = 0;
            }
            qr_code.lb[i] = 0;
            qr_code.lt[i] = 0;
        }
    }

    // column
    for (int i = 0; i < qr_code.N; i++)
    {
        int num_black_c = cb[i];
        int num_transitions_c = ct[i];

        // process rows, if the number of black cells is equal to the number of black cells in the given info, then we'll set all cells to 1
        if (num_black_c == qr_code.N)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[j][i] = 1;
            }
            qr_code.cb[i] = qr_code.N;
            qr_code.ct[i] = 0;
        }
        else if (num_black_c == 0)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[j][i] = 0;
            }
            qr_code.cb[i] = 0;
            qr_code.ct[i] = 0;
        }
    }

    // deduct the cell in each quadrant

    return qr_code;
}

int sum2(int *array, int N)
{
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += array[i];
    }
    return sum;
}

int sum(std::vector<int> array, int N)
{
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += array[i];
    }
    return sum;
}

bool defect_detection(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{
    // first check if lb and cb make any sense, floor down N / 2
    int max_transitions = floor(N / 2) + 1;

    for (int i = 0; i < N; i++)
    {
        // case 1 and 2: if lb[i] is 0 or N lt must be 0
        if ((lb[i] == 0 || lb[i] == N) && lt[i] != 0)
        {
            return true;
        }

        // case 3 and 4: if cb[i] is 0 or N ct must be 0
        if ((cb[i] == 0 || cb[i] == N) && ct[i] != 0)
        {
            return true;
        }

        // case 5: max transitions
        if (lt[i] > max_transitions || ct[i] > max_transitions)
        {
            return true;
        }

        // case 6: if one diagonal is full of black cells, then each line must have at least 1 black cell
        //         if both diagonals are full of black cells, then each line must have at least 2 black cells
        if (N % 2 == 0)
        {
            if (db[i] == N && lb[i] == 0 || (db[0] == N && db[1] == N) && lb[i] < 2)
            {
                return true;
            }
            // same for columns
            if (db[i] == N && cb[i] == 0 && (db[0] == N && db[1] == N) && cb[i] < 2)
            {
                return true;
            }
        }

        // define Xmin and Xmax, Ymin and Ymax for the quadrants
        int Xmin = 0;
        int Xmax = 0;
        int Ymin = 0;
        int Ymax = 0;

        if (N % 2 == 0)
        {
            Xmin = N / 2;
            Xmax = N / 2;
            Ymin = N / 2;
            Ymax = N / 2;
        }
        else
        {
            Xmin = floor(N / 2);
            Xmax = floor(N / 2) + 1;
            Ymin = floor(N / 2);
            Ymax = floor(N / 2) + 1;
        }

        /*       Ymin|
                 +---+---+---+                              qb[0] = 1 Ymin * Xmax
                 | 2 | 1 | 1 |                              qb[1] = 2 Ymin * Xmin
          Xmin---+---+---+---+-Xmáx------                   qb[2] = 3 Xmin * Ymax
                 | 3 | 4 | 4 |                              qb[3] = 4 Xmax * Ymax
                 +---+---+---+
                 | 3 | 4 | 4 |
          Xmin   +---+---+---+
                     |Ymáx
        */
        // case 7: if the number of black cells in a quadrant is greater than the number of cells in that quadrant, then the qr_code is invalid
        if (qb[0] > Xmax * Ymin || qb[1] > Xmin * Ymin || qb[2] > Xmin * Ymax || qb[3] > Xmax * Ymax)
        {
            return true;
        }
        // verify the sum of black cells per line and per column
        // the sum of the values in the lb and cb arrays should be equal to the total of black cells in the QR Code
        int total_blacks_lines = sum(lb, N);
        int total_blacks_columns = sum(cb, N);
        int total_blacks_quadrants = sum2(qb, 4);

        if (!(total_blacks_lines == total_blacks_columns && total_blacks_columns == total_blacks_quadrants))
        {
            return true;
        }

        // verify that sum of black cells >= sum of black cells in the diagonal
        int total_blacks_diagonals = sum2(db, 2);

        if (total_blacks_quadrants < total_blacks_diagonals)
        {
            return true;
        }

        // verify if the sums of black cells per quadrant are the same when calculated by the sum of the lines and columns of the respective quadrants
        int sum_top_lines = 0;     // quadrants 1 and 2
        int sum_bottom_lines = 0;  // quadrants 3 and 4
        int sum_left_columns = 0;  // quadrants 1 and 4
        int sum_right_columns = 0; // quadrants 2 and 3

        for (int i = 0; i < floor(N / 2); i++)
        {
            sum_top_lines += lb[i];
            sum_left_columns += cb[i];
        }

        for (int i = floor(N / 2); i < N; i++)
        {
            sum_bottom_lines += lb[i];
            sum_right_columns += cb[i];
        }
        //  quadrants 1 and 2               |       quadrants 3 and 4              |       quadrants 1 and 4              |         quadrants 2 and 3
        if ((sum_top_lines != qb[0] + qb[1]) || (sum_bottom_lines != qb[2] + qb[3] || (sum_right_columns != qb[0] + qb[3]) || (sum_left_columns != qb[1] + qb[2])))
        {
            return true;
        }

        // verify if in an odd matrix is all black, then the anti-diagonal must have at least 1 black cell
        if ((N % 2 != 0) && ((db[0] == N && db[1] == 0) || (db[0] == 0 && db[1] == N)))
        {
            return true;
        }
    }
    return false;
}

void printQR_code(QR_Code qr_code)
{

    // Print the qr_code info
    cout << "---------------------QR Code info----------------" << endl;
    cout << "N: " << qr_code.N << endl;
    cout << "lb - nr of black cells inline: ";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << qr_code.lb[i] << " ";
    }
    cout << endl;
    cout << "cb - nr of black cells in column: ";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << qr_code.cb[i] << " ";
    }
    cout << endl;
    cout << "lt - nr of color transitions in line: ";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << qr_code.lt[i] << " ";
    }
    cout << endl;
    cout << "ct - nr of color transitions in column:";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << qr_code.ct[i] << " ";
    }
    cout << endl;
    cout << "qb - nr of black cells in quadrant:";
    for (int i = 0; i < 4; i++)
    {
        cout << qr_code.qb[i] << " ";
    }
    cout << endl;
    cout << "db - nr of black cells in diagonal: ";
    for (int i = 0; i < 2; i++)
    {
        cout << qr_code.db[i] << " ";
    }
    cout << endl;

    cout << "+";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << "-";
    }
    cout << "+" << endl;

    // Print the QR code
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << "|";
        for (int j = 0; j < qr_code.N; j++)
        {
            cout << qr_code.cells[i][j];
        }
        cout << "|" << endl;
    }

    // Print the bottom border
    cout << "+";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << "-";
    }
    cout << "+" << endl;
    cout << "------------------------------------------------" << endl;
}