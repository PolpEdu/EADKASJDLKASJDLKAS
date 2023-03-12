#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <math.h>

#define DEBUG 0
#define TIMES 0
#define DEBUG_GEN 0

using namespace std;

struct QR_Code
{
    int N;
    int **cells;

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

    void reset_qr_code()
    {
        for (int i = 0; i < this->N; i++)
        {
            for (int j = 0; j < this->N; j++)
            {
                this->cells[i][j] = 0;
            }
        }
    }

    // check if the values given as parameters are the same as the qr code
    int check_if_valid(vector<int> cb, vector<int> lb, vector<int> ct, vector<int> lt, int *qb, int *db, int c, int l, int b)
    {
        /* 1. count columns*/
        vector<int> bcol = vector<int>(this->N, 0); // to store the black cells in each column
        int transitions = 0;
        for (int i = 0; i < c + 1; i++)
        {
            int b_cell = 0;
            for (int j = 0; j < l + 1; j++)
            {
                b_cell += this->cells[j][i] % 2; // add 1 if the cell is black
            }
            if (b_cell > cb[i])
            {
                if (DEBUG)
                    cout << "b_cell > cb[i]";
                return false; // return immediately if the number of black cells in a column is greater than the number of black cells in the column of the qrcode
            }

            if (l == this->N - 1 && b_cell < cb[i])
            {
                if (DEBUG)
                    cout << "l == this->N && b_cell < cb[i]";
                return false; // return immediifately if the number of black cells in a column is less than the number of black cells in the column of the qrcode
            }

            bcol[i] = b_cell;
        }

        // calculate the number of transitions for the column of c
        if (this->cells[l][c] == 3)
        {
            for (int i = 0; i < c + 1; i++)
            {
                for (int j = 0; j < l; j++)
                {
                    if (this->cells[j][i] % 2 != this->cells[j + 1][i] % 2 && i == c)
                    {
                        transitions++;
                    }
                }
            }

            if (this->N - l - 1 < ct[c] - transitions - 1)
            {
                if (DEBUG)
                    cout << "this->N - l - 1 < ct[c] - transitions - 1";
                return false;
            }
            if (this->N - l - 1 < cb[c] - bcol[c])
            {
                if (DEBUG)
                    cout << "this->N - l - 1 < cb[c] - bcol[c]";
                return false;
            }
            if (cb[c] == bcol[c] && (ct[c] - transitions > 3))
            {
                if (DEBUG)
                    cout << "cb[c] == bcol[c] && (this->N - l - 1 < ct[c] - transitions - 1";
                return false;
            }
        }
        /* count columns*/

        /* 2. line transition */
        int counter_tran = 0;
        for (int i = 0; i < c; i++)
        {
            if (this->cells[l][i] % 2 != this->cells[l][i + 1] % 2)
            {
                counter_tran++;
            }
        }
        if (c == this->N - 1 && counter_tran < lt[l]) // maybe here?
        {
            if (DEBUG)
                cout << "his->N - 1 && counter_tran < l";
            return false;
        }
        if (counter_tran > lt[l])
        {
            if (DEBUG)
                cout << "counter_trann < l";
            return false;
        }
        /* line transition */

        /* 3. diagonals */
        int d = 0;
        int cd = 0;

        for (int i = 0; i < l + 1; i++)
        {
            for (int j = 0; j < c + 1; j++)
            {
                if (i == j) // i think this is ok
                {
                    d += this->cells[i][j] % 2;
                }
                if (i + j == this->N - 1)
                {
                    cd += this->cells[i][j] % 2;
                }
            }
        }

        if (d > db[0] || (cd > db[1]))
        {
            if (DEBUG)
                cout << "djlaskdlajl < l";
            return false;
        }
        if (c == this->N - 1 && l == this->N - 1 && (d < db[0] || cd < db[1]))
        {
            if (DEBUG)
                cout << "dlçaskdalkdlas < l";
            return false;
        }
        if (c == l && this->N - (c + 1) < db[0] - d)
        {
            if (DEBUG)
                cout << "daslkdlaskdalkaklal < l";
            return false;
        }
        /* ^~diagonals~^ */

        /* 4. line_blacks */
        if (c == this->N - 1 && b < lb[l] || b > lb[l])
        {
            if (DEBUG)
                cout << "aklslaaaal < l";
            return false;
        }

        /* 5. calc transistion columns */
        for (int i = 0; i < c + 1; i++)
        {
            int counter_col_trans = 0;
            for (int j = 0; j < l; j++)
            {
                if (this->cells[j][i] % 2 != this->cells[j + 1][i] % 2)
                {
                    counter_col_trans++;
                }
            }

            if (counter_col_trans > ct[i])
            {
                if (DEBUG)
                    cout << "counter_col_trans ct l";
                return false;
            }

            if (l == this->N - 1 && counter_col_trans < ct[i])
            {
                if (DEBUG)
                    cout << "estou < l";
                return false;
            }
        }

        /* 6. quadrants */
        int quadr[] = {0, 0, 0, 0};
        int quadrante = 0;
        int cell_quadrante = 0;
        int total_cells = 0;

        for (int i = 0; i < this->N; i++)
        {
            for (int j = 0; j < this->N; j++)
            {
                // N-1 since we are starting on 0
                if (i <= (this->N - 1) / 2 && j <= (this->N - 1) / 2)
                {
                    quadrante = 1;
                }
                else if (i > (this->N - 1) / 2 && j <= (this->N - 1) / 2)
                {
                    quadrante = 2;
                }
                else if (i > (this->N - 1) / 2 && j > (this->N - 1) / 2)
                {
                    quadrante = 3;
                }
                // else stays at 0
                quadr[quadrante] += this->cells[i][j] % 2;
            }
        }

        if (l + 1 <= this->N / 2 && c + 1 <= this->N / 2)
        {
            cell_quadrante = 1;
        }
        else if (l + 1 > this->N / 2 && c + 1 <= this->N / 2)
        {
            cell_quadrante = 2;
        }
        else if (l + 1 > this->N / 2 && c + 1 > this->N / 2)
        {
            cell_quadrante = 3;
        }

        if (this->N % 2 == 0)
        {
            int half_size = this->N / 2;
            // get the total cells in each quadrant
            if (cell_quadrante == 0)
                total_cells = (this->N - (c + 1)) + (half_size - (l + 1)) * half_size;
            else if (cell_quadrante == 1)
                total_cells = (half_size - (c + 1)) + (half_size - (l + 1)) * half_size;
            else if (cell_quadrante == 2)
                total_cells = (half_size - (c + 1)) + ((this->N - (l + 1))) * half_size;
            else if (cell_quadrante == 3)
                total_cells = (this->N - (c + 1)) + (this->N - (l + 1)) * half_size;
        }
        else
        {
            int half_size = this->N / 2 + 1;
            if (cell_quadrante == 0)
                total_cells = (this->N - (c + 1)) + (half_size - 1 - (l + 1)) * half_size;
            else if (cell_quadrante == 1)
                total_cells = (half_size - 1 - (c + 1)) + (half_size - 1 - (l + 1)) * (half_size - 1);
            else if (cell_quadrante == 2)
                total_cells = (half_size - 1 - (c + 1)) + ((this->N - (l + 1))) * (half_size - 1);
            else if (cell_quadrante == 3)
                total_cells = (this->N - (c + 1)) + (this->N - (l + 1)) * half_size;
        }

        if (total_cells < qb[cell_quadrante] - quadr[cell_quadrante])
        {
            if (DEBUG)
                cout << total_cells << "  ret qb" << qb[cell_quadrante] << " quadr" << quadr[cell_quadrante] << endl;
            return false;
        }

        for (int i = 0; i < 4; i++)
        {
            if (c == this->N - 1 && l == this->N - 1 && quadr[i] < qb[i] || quadr[i] > qb[i])
            {
                if (DEBUG)
                    cout << "c == this->N - 1 && l == this->N - 1 &&";
                return false;
            }
        }

        /* ^~quadrants~^*/

        return true;
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

    int calc_transitions(int l, int c)
    {
        int i = 0;
        for (int j = 0; j < c; j++)
        {
            if (this->cells[l][j] % 2 != this->cells[l][j + 1] % 2)
            {
                i++;
            }
        }
        return i;
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

void loop_through_all_cells(QR_Code &qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db, int lb_counter);
void printQR_code(QR_Code *qr_code);
int sum(std::vector<int> array, int N);
int sum2(int *array, int N);
bool defect_detection(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
void pre_processing(QR_Code *qr_code, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
int init(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int num = 0; // number of encodings to be assessed
    int N = 0;   // size of the QR code in lines/columns: 2 ≤ N ≤ 30

    std::cin >> num;

    for (int as = 0; as < num; as++)
    {
        std::cin >> N;

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

        // generate blank qr_code
        QR_Code qr_code = QR_Code(N);
        qr_code.set_all_to(2);

        if (!defect_detection(N, lb, cb, lt, ct, qb, db))
        {
            // we need to do some pre processing, set the cells that need to be certain values according to the settings lb, cb, lt, ct, qb, db
            pre_processing(&qr_code, lb, cb, lt, ct, qb, db);
            if (DEBUG)
            {
                std::cout << "PRE PROCESSING DONE" << std::endl;
                printQR_code(&qr_code);
            }

            /*
                we need to do some pre processing, set the cells that need to be certain values according to the settings lb, cb, lt, ct, qb, db
                also, we need to find the defects (find if the current lb, cb, lt, ct, qb, db are valid without generating the qr code)
                make sure to optimize the tree sicne the recursion will be very deep in some cases
            */
            loop_through_all_cells(qr_code, 0, 0, lb, cb, lt, ct, qb, db, 0);
        }

        if (qr_codes_generated == 0)
        {
            std::cout << "DEFECT: No QR Code generated!" << std::endl;
        }

        // check the size of qr_code.qr_codes_generated and make sure its 0
        else if (qr_codes_generated == 1)
        {
            std::cout << "VALID: 1 QR Code generated!" << std::endl;
            qr_to_print.print_qr_code();
        }
        else if (qr_codes_generated > 1)
        {
            std::cout << "INVALID: " << qr_codes_generated << " QR Codes generated!" << std::endl;
        }

        qr_codes_generated = 0;
    }
    return 0;
}

void loop_through_all_cells(QR_Code &qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db, int lb_counter)
{
    if (l >= qr_code.N)
    {
        if (qr_codes_generated == 0)
        {
            qr_to_print = qr_code.copy();
        }
        qr_codes_generated++;
        return;
    }

    if (DEBUG)
    {
        cout << "l: " << l << " c: " << c << endl;
        cout << qr_code.cells[l][c] << endl;
        cout << qr_code.check_if_valid(lb, cb, lt, ct, qb, db, c, l, lb_counter) << endl;
    }
    if (qr_code.cells[l][c] == 0)
    {
        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db, c, l, lb_counter))
        {
            if (c == qr_code.N - 1)
            {
                l++;
                c = 0;
                loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, 0);
            }
            else
            {
                c++;
                loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter);
            }
        }
        return;
    }
    else if (qr_code.cells[l][c] == 1)
    {
        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db, c, l, lb_counter + 1))
        {
            if (c == qr_code.N - 1)
            {
                l++;
                c = 0;
                loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, 0);
            }
            else
            {
                c++;
                loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter + 1);
            }
        }
        return;
    }

    qr_code.cells[l][c] = 3;
    int t = qr_code.calc_transitions(l, c);
    if (DEBUG)
        cout << "trans: " << t << endl;

    if (c != qr_code.N - 1)
    {
        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db, c, l, lb_counter + 1))
        {
            if (
                !(lb_counter + 1 == lb[l] && lt[l] - t > 3) && !(qr_code.N - c - 1 < lt[l] - t - 1) && !(lb[l] - lb_counter - 1 > qr_code.N - c - 1) &&
                !(t == lt[l] && (qr_code.N - c - 1 < lb[l] - lb_counter - 1)))
            {
                loop_through_all_cells(qr_code, l, c + 1, lb, cb, lt, ct, qb, db, lb_counter + 1);
            }
        }

        qr_code.cells[l][c] = 2; // go back to the previous cell

        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db, c, l, lb_counter))
        {
            if (!(lb[l] - lb_counter - 1 > qr_code.N - c - 1))
            {
                loop_through_all_cells(qr_code, l, c + 1, lb, cb, lt, ct, qb, db, lb_counter);
            }
        }
    }
    else
    {
        // end of the line, go onw line downards
        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db, c, l, lb_counter + 1))
        {
            loop_through_all_cells(qr_code, l + 1, 0, lb, cb, lt, ct, qb, db, 0);
        }

        qr_code.cells[l][c] = 2; // go back to the previous cell

        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db, c, l, lb_counter))
        {
            loop_through_all_cells(qr_code, l + 1, 0, lb, cb, lt, ct, qb, db, 0);
        }
    }
}

// this function will pre_process all the cell that fill the criteria of those parameters and
void pre_processing(QR_Code *qr_code, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{
    vector<int> white_line = vector<int>(qr_code->N, 0);
    vector<int> black_line = vector<int>(qr_code->N, 0);

    int number_of_white_lines = 0;
    int number_of_black_lines = 0;

    for (int i = 0; i < qr_code->N; i++)
    {
        // simple case, everything filled
        if (lb[i] == qr_code->N)
        {
            black_line[i] = 1;
            number_of_black_lines++;
            for (int j = 0; j < qr_code->N; j++)
            {
                qr_code->cells[i][j] = 1;
            }
        }
        else if (lb[i] == 0)
        {
            white_line[i] = 1;
            number_of_white_lines++;
            for (int j = 0; j < qr_code->N; j++)
            {
                qr_code->cells[i][j] = 0;
            }
        }
        // check for 1 transition while there is just a black cell, this means that
        // the cell is in the corners of the line (we don't know what corner, left or right)
        if ((lb[i] == 1) && (lt[i] == 1))
        {
            for (int j = 1; j < qr_code->N - 1; j++)
            {
                // middle of the line, filled with white
                qr_code->cells[i][j] = 0;
            }
        }
        // the opposite of the previous case, where we have still one transition but this time
        // we have lots of black cells
        if ((lb[i] == qr_code->N - 1) && (lt[i] == 1))
        {
            // fill the middle of the line with black
            for (int j = 1; j < qr_code->N - 1; j++)
            {
                qr_code->cells[i][j] = 1;
            }
        }

        // check for "chess pattern"
        if (lt[i] == qr_code->N - 1 && lb[i] == qr_code->N / 2 + 1)
        {
            for (int j = 0; j < qr_code->N; j++)
            {
                if (j % 2 == 0)
                {
                    qr_code->cells[i][j] = 1;
                }
                else
                {
                    qr_code->cells[i][j] = 0;
                }
            }
        }
    }
    /* ##################################################*/

    // Loop through each line of the QR code and fill the line with black cells where there is only one color transition (lt[i] == 1) across the entire line
    for (int i = 0; i < qr_code->N; i++)
    {
        // we only have one color transition in the line
        if (lt[i] == 1)
        {
            // we'll fill the line with black cells
            for (int j = lb[i]; j < qr_code->N - lb[i]; j++)
            {
                qr_code->cells[i][j] = 0;
            }
        }
    }
    /* ##################################################*/

    if (DEBUG)
    {
        cout << "number of white lines: " << number_of_white_lines << endl;
        cout << "number of black lines: " << number_of_black_lines << endl;
        qr_code->print_qr_code();
    }

    /* ##################################################*/
    for (int l = 0; l < qr_code->N; l++)
    {
        // fill columns
        if (cb[l] == qr_code->N)
        {
            for (int j = 0; j < qr_code->N; j++)
            {
                qr_code->cells[j][l] = 1;
            }
        }
        else if (cb[l] == 0)
        {
            for (int j = 0; j < qr_code->N; j++)
            {
                qr_code->cells[j][l] = 0;
            }
        }

        // fill the rest of the qr_code with black cells
        if (qr_code->N - number_of_white_lines == cb[l])
        {
            for (int j = 0; j < qr_code->N; j++)
            {
                if (white_line[j] == 0)
                {
                    qr_code->cells[j][l] = 1;
                }
            }
        }

        // fill the rest of the qr_code with white cells
        if (qr_code->N - number_of_black_lines == 0)
        {
            for (int j = 0; j < qr_code->N - number_of_black_lines; j++)
            {
                if (black_line[j] == 0)
                {
                    qr_code->cells[j][l] = 0;
                }
            }
        }
    }
    /* ##################################################*/

    // check for a bulls eye pattern (target pattern)
    vector<int> diagonal = vector<int>(qr_code->N, 0);
    vector<int> inv_diagonal = vector<int>(qr_code->N, 0);
    int n_crossed = 0;         // number of crossed cells
    int n_counter_crossed = 0; // number of crossed cells in the diagonal
    for (int i = 0; i < qr_code->N; i++)
    {
        for (int j = 0; j < qr_code->N; j++)
        {
            if (i == j)
            {
                // we are in the diagonal, check if we have the same number of black cells in the line and in the column
                if (cb[i] == qr_code->N && lb[j] == qr_code->N)
                {
                    // if so, we have a bulls eye pattern, store the diagonal and the number of marks
                    diagonal[i] = 1;
                    n_crossed++;
                }
            }
            if (i + j == qr_code->N - 1)
            {
                // we are in the counter diagonal, check if we have the same number of black cells in the line and in the column
                if (cb[i] == qr_code->N && lb[j] == qr_code->N)
                {
                    // if so, we have a bulls eye pattern, store the diagonal and the number of marks
                    inv_diagonal[i] = 1;
                    n_counter_crossed++;
                }
            }
        }
    }

    // if the number of marks is the same of the number of black cells in the diagonal, fill the diagonal with white cells
    if (n_crossed == db[0])
    {
        for (int i = 0; i < qr_code->N; i++)
        {
            for (int j = 0; j < qr_code->N; j++)
            {
                if (i == j && diagonal[i] == 0)
                {
                    qr_code->cells[i][j] = 0;
                }
            }
        }
    }

    // if the number of marks is the same of the number of black cells in the counter diagonal, fill the counter diagonal with white cells
    if (n_counter_crossed == db[1])
    {
        for (int i = 0; i < qr_code->N; i++)
        {
            for (int j = 0; j < qr_code->N; j++)
            {
                if (i + j == qr_code->N - 1 && inv_diagonal[i] == 0)
                {
                    qr_code->cells[i][j] = 0;
                }
            }
        }
    }

    /* ##################################################*/
    if (DEBUG)
    {
        cout << "Crossed cells: " << n_crossed << endl;
        cout << "Counter crossed cells: " << n_counter_crossed << endl;
        cout << "Diagonal: ";
        for (int i = 0; i < qr_code->N; i++)
        {
            cout << diagonal[i] << " ";
        }
        cout << endl;
        qr_code->print_qr_code();
    }
    /* ##################################################*/

    // check if the diagonal is the same of N, if so that means that the diagonal is filled with black cells
    if (db[0] == qr_code->N)
    {
        // fill the diagonal with black cells
        for (int i = 0; i < qr_code->N; i++)
        {
            for (int k = 0; k < qr_code->N; k++)
            {
                // fill the diagonal as normal
                if (i == k)
                {
                    qr_code->cells[i][k] = 1;
                }
            }
        }

        for (int i = 0; i < qr_code->N; i++)
        {
            // check if the line contains just one black cell
            if (lb[i] == 1)
            {
                // since we have just one cell (the cell of the diagonal), that means the other cells of the square are white
                for (int j = 0; j < qr_code->N; j++)
                {
                    if (j != i)
                    {
                        qr_code->cells[i][j] = 0;
                    }
                }
            }
        }
    }
    // check if we have an empty diagonal, if so that means that the diagonal is filled with white cells
    else if (db[0] == 0)
    {
        // fill the diagonal with white cells
        for (int i = 0; i < qr_code->N; i++)
        {
            for (int k = 0; i < qr_code->N; k++)
            {
                // fill the diagonal as normal
                if (i == k)
                {
                    qr_code->cells[i][k] = 0;
                }
            }
        }

        for (int i = 0; i < qr_code->N; i++)
        {
            // check if the line contains just one white cell
            if (lb[i] == qr_code->N - 1)
            {
                // since we have just one white cell (the cell of the diagonal), that means the other cells of the square are black
                for (int j = 0; j < qr_code->N; j++)
                {
                    if (j != i)
                    {
                        qr_code->cells[i][j] = 1;
                    }
                }
            }
        }
    }
    if (DEBUG)
    {
        cout << "Diagonal filled" << endl;
        qr_code->print_qr_code();
    }

    // same logic as above, but for the inverse diagonal
    if (db[1] == 0)
    {
        for (int i = 0; i < qr_code->N; i++)
        {
            for (int k = 0; k < qr_code->N; k++)
            {
                if (i + k == qr_code->N - 1)
                {
                    qr_code->cells[i][qr_code->N - 1 - i] = 0;
                }
            }
        }

        for (int i = 0; i < qr_code->N; i++)
        {
            if (lb[i] == qr_code->N - 1)
            {
                for (int j = 0; j < qr_code->N; j++)
                {
                    if (i + j != qr_code->N - 1)
                    {
                        qr_code->cells[i][j] = 1;
                    }
                }
            }
        }
    }
    else if (db[1] == qr_code->N)
    {
        for (int i = 0; i < qr_code->N; i++)
        {
            for (int k = 0; k < qr_code->N; k++)
            {
                if (i + k == qr_code->N - 1)
                {
                    qr_code->cells[i][qr_code->N - 1 - i] = 1;
                }
            }
        }

        for (int i = 0; i < qr_code->N; i++)
        {
            if (lb[i] == 1)
            {
                for (int j = 0; j < qr_code->N; j++)
                {
                    if (i + j != qr_code->N - 1)
                    {
                        qr_code->cells[i][j] = 0;
                    }
                }
            }
        }
    }

    if (DEBUG)
    {
        cout << "Inverse diagonal filled" << endl;
        printQR_code(qr_code);
    }
    /* ##################################################*/
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

    for (int i = 0; i < N; ++i)
    {
        // check if its possible to create a qrcode with the given transitions and blacks
        if (lt[i] == 0 && (lb[i] > 0 && lb[i] < N))
        {
            return true;
        }
        if (ct[i] == 0 && (cb[i] > 0 && cb[i] < N))
        {
            return true;
        }

        if (N - 1 < lt[i])
        {
            return true;
        }

        if (lt[i] == N - 1 && N / 2 > lb[i])
        {
            return true;
        }

        if (N - 1 < ct[i])
        {
            return true;
        }

        if (ct[i] == N - 1 && N / 2 > cb[i])
        {
            return true;
        }
        if (lb[i] == N / 2 && lb[i] * 2 < lt[i])
        {
            return true;
        }

        int sumCol = 0;
        int sumRow = 0;
        int sumQuads = 0;

        for (int i = 0; i < N; ++i)
        {
            sumCol += cb[i];
            sumRow += lb[i];
        }
        for (int i = 0; i < 4; ++i)
        {
            sumQuads += qb[i];
        }

        if (sumCol != sumRow || sumCol != sumQuads)
        {
            return true;
        }

        int upperRows = 0;
        int lowerRows = 0;
        int leftCols = 0;
        int rightCols = 0;

        for (int i = 0; i < N / 2; ++i)
        {
            upperRows += lb[i];
            leftCols += cb[i];
        }

        for (int i = N / 2; i < N; ++i)
        {
            lowerRows += lb[i];
            rightCols += cb[i];
        }

        if (((qb[0] + qb[1]) != upperRows) || ((qb[2] + qb[3]) != lowerRows) || ((qb[1] + qb[2]) != leftCols) || ((qb[0] + qb[3]) != rightCols))
        {
            return true;
        }
    }
    return false;
}

void printQR_code(QR_Code *qr_code)
{

    // Print the qr_code info
    cout << "---------------------QR Code info----------------" << endl;
    cout << "N: " << qr_code->N << endl;

    cout << "+";
    for (int i = 0; i < qr_code->N; i++)
    {
        cout << "-";
    }
    cout << "+" << endl;

    // Print the QR code
    for (int i = 0; i < qr_code->N; i++)
    {
        cout << "|";
        for (int j = 0; j < qr_code->N; j++)
        {
            cout << qr_code->cells[i][j];
        }
        cout << "|" << endl;
    }

    // Print the bottom border
    cout << "+";
    for (int i = 0; i < qr_code->N; i++)
    {
        cout << "-";
    }

    cout << "+" << endl;
    cout << "------------------------------------------------" << endl;
}