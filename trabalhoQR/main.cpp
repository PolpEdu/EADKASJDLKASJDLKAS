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
    int check_if_valid(vector<int> cb, vector<int> lb, vector<int> ct, vector<int> lt, int *qb, int *db, int c, int l, int b)
    {
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

        /*
        bool calcTransLine(vector<int> &array, int y, int x)
    {
        int counter = 0;
        for (int i = 0; i < x; ++i)
        {
            if (array[i] % 2 != array[i + 1] % 2)
                counter++;
        }
        if (x == array.size() - 1 && counter < lt[y])
            return false;
        else if (counter > lt[y])
        {
            return false;
        }
        return true;
    }

    bool dbverify(vector<vector<int>> &qrcode, int y, int x)
    {
        int diag = 0;
        int cdiag = 0;

        for (int i = 0; i < y + 1; ++i)
        {
            for (int j = 0; j < x + 1; j++)
            {
                if (i == j && qrcode[i][j] % 2)
                    diag++;

                if (tam - i - 1 == j && qrcode[i][j] % 2)
                    cdiag++;
            }
        }

        // print qrcode
        //  for (size_t i = 0; i < tam; i++){
        //      for (size_t j = 0; j < tam; j++){
        //          cout << qrcode[i][j] << " ";
        //      }
        //      cout << endl;
        //  }

        if (diag > db[0] || cdiag > db[1])
            return false;
        if (x == tam - 1 && y == tam - 1 && (diag < db[0] || cdiag < db[1]))
            return false;

        if ((x == y && tam - (x + 1) < db[0] - diag))
        {
            return false;
        }

        return true;
    }

    bool calclineblacks(int blacks, int y, int x)
    {
        if (x == tam - 1 && blacks < lb[y] || blacks > lb[y])
            return false;

        return true;
        //???????? return !(x == tam - 1 && blacks < lb[y] || blacks > lb[y]);
    }

    bool quadrantes(vector<vector<int>> &qrcode, int y, int x)
    {
        int quad[] = {0, 0, 0, 0};
        int q = 0;

        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {
                // primeiro quadrante
                if (i <= tam / 2 && j > tam / 2)
                    quad[0] += qrcode[i - 1][j - 1] % 2;
                // segundo quadrante
                else if (i <= tam / 2 && j <= tam / 2)
                    quad[1] += qrcode[i - 1][j - 1] % 2;
                // terceiro quadrante
                else if (i > tam / 2 && j <= tam / 2)
                    quad[2] += qrcode[i - 1][j - 1] % 2;
                // quarto quadrante
                else if (i > tam / 2 && j > tam / 2)
                {
                    quad[3] += qrcode[i - 1][j - 1] % 2;
                }
            }
        }

        // primeiro quadrante
        if (y + 1 <= tam / 2 && x + 1 > tam / 2)
        {
        }
        q = 0;
        // segundo quadrante
        if (y + 1 <= tam / 2 && x + 1 <= tam / 2)
            q = 1;
        // terceiro quadrante
        if (y + 1 > tam / 2 && x + 1 <= tam / 2)
            q = 2;
        // quarto quadrante
        if (y + 1 > tam / 2 && x + 1 > tam / 2)
            q = 3;

        int aux = 0, count = 0;

        if (tam % 2 == 0)
        {
            aux = tam / 2;

            if (q == 0)
                count = (tam - (x + 1)) + (aux - (y + 1)) * aux;
            else if (q == 1)
                count = (aux - (x + 1)) + (aux - (y + 1)) * aux;
            else if (q == 2)
                count = (aux - (x + 1)) + ((tam - (y + 1))) * aux;
            else if (q == 3)
                count = (tam - (x + 1)) + (tam - (y + 1)) * aux;
        }

        else
        {
            aux = tam / 2 + 1;

            if (q == 0)
                count = (tam - (x + 1)) + (aux - 1 - (y + 1)) * aux;
            else if (q == 1)
                count = (aux - 1 - (x + 1)) + (aux - 1 - (y + 1)) * (aux - 1);
            else if (q == 2)
                count = (aux - 1 - (x + 1)) + ((tam - (y + 1))) * (aux - 1);
            else if (q == 3)
                count = (tam - (x + 1)) + (tam - (y + 1)) * aux;
        }

        if (count < qb[q] - quad[q])
        {

            return false;
        }

        for (int i = 0; i < 4; ++i)
        {
            if (x == tam - 1 && y == tam - 1 && quad[i] < qb[i] || quad[i] > qb[i])
                return false;
        }
        return true;
    }

    bool calcTransCol(vector<vector<int>> &qrcode, int y, int x)
    {

        for (int j = 0; j < x + 1; ++j)
        {
            int counter = 0;
            for (int i = 0; i < y; i++)
            {
                if (qrcode[i][j] % 2 != qrcode[i + 1][j] % 2)
                {
                    counter++;
                }
            }
            if (y == tam - 1 && counter < ct[j])
                return false;

            else if (counter > ct[j])
            {
                return false;
            }
        }

        return true;
    }

    bool countcol(vector<vector<int>> &qrcode, int y, int x)
    {
        vector<int> bcol;
        for (int i = 0; i < tam; ++i)
        {
            bcol.push_back(0);
        }
        int trans = 0;
        for (int j = 0; j < x + 1; ++j)
        {
            int counter = 0;
            for (int i = 0; i < y + 1; ++i)
            {
                counter += qrcode[i][j] % 2;
            }
            if (y == tam - 1 && counter < cb[j])
                return false;
            else if (counter > cb[j])
            {
                return false;
            }
            bcol[j] = counter;
        }

        // calcular transições para a coluna de x
        // apenas preciso de fazer isto de o pixel em que estiver for um 1
        if (qrcode[y][x])
        {
            for (int j = 0; j < x + 1; ++j)
            {
                for (int i = 0; i < y; ++i)
                {
                    if (qrcode[i][j] % 2 != qrcode[i + 1][j] % 2 && j == x)
                    {
                        trans++;
                    }
                }
            }
            if (tam - y - 1 < cb[x] - bcol[x])
            {
                return false;
            }
            if (cb[x] == bcol[x] && (ct[x] - trans - 1 > 2))
            {
                return false;
            }
            if (tam - y - 1 < ct[x] - trans - 1)
            {
                return false;
            }
        }
        // if(!(blacksl+1 == lb[y] && lt[y]-transitions -1 > 2) && !(tam-x-1 < lt[y]-transitions-1) && !(lb[y] - blacksl -1 > tam-x-1) && !(transitions == lt[y] && (tam-x-1 < lb[y]-blacksl-1))){
        return true;
    }

        */
        bool trans_l = true;
        int counter = 0;
        for (int i = 0; i < c; i++)
        {
            if (this->cells[l][i] % 2 != this->cells[l][i + 1] % 2)
            {
                counter++;
            }
        }

        trans_l = 
        

        bool line_blacks = c == this->N - 1 &&  b < this->lb[l] || b > this->lb[l];
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
            if (j != 0)
            {
                if (this->cells[l][j] % 2 != this->cells[l][j - 1] % 2)
                {
                    i++;
                }
            }
        }
        return i;
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

int check_cell(QR_Code qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
void loop_through_all_cells(QR_Code &qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db, int lb_counter);
void printQR_code(QR_Code qr_code);
int sum(std::vector<int> array, int N);
int sum2(int *array, int N);
bool defect_detection(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
void pre_processing(QR_Code qr_code, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
int init(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);

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

        init(N, lb, cb, lt, ct, qb, db);

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        if (TIMES)
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    }
    return 0;
}

int init(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{

    // first detect if the settings are valid
    if (defect_detection(N, lb, cb, lt, ct, qb, db))
    {
        if (DEBUG)
            std::cout << "DEFECT: No QR Code generated DEFECT!" << std::endl;
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
    pre_processing(qr_code, lb, cb, lt, ct, qb, db);

    /*
        we need to do some pre processing, set the cells that need to be certain values according to the settings lb, cb, lt, ct, qb, db
        also, we need to find the defects (find if the current lb, cb, lt, ct, qb, db are valid without generating the qr code)
        make sure to optimize the tree sicne the recursion will be very deep in some cases
    */
    loop_through_all_cells(qr_code, 0, 0, lb, cb, lt, ct, qb, db, 0);

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

void loop_through_all_cells(QR_Code &qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db, int lb_counter)
{
    if (l == qr_code.N)
    {
        qr_codes_generated++;
        qr_to_print = qr_code;
        return;
    }

    if (qr_code.cells[l][c] == 0)
    {
        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db))
        {
            if (c == qr_code.N - 1)
            {
                l++;
                c = 0;
                lb_counter = 0;
            }
            else
            {
                c++;
            }
        }
        loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter);

        return;
    }
    else if (qr_code.cells[l][c] == 1)
    {
        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db))
        {
            if (c == qr_code.N - 1)
            {
                l++;
                c = 0;
                lb_counter = 0;
            }
            else
            {
                c++;
                lb_counter++;
            }
            loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter);
        }
        return;
    }

    qr_code.cells[l][c] = 1;
    int t = qr_code.calc_transitions(l, c);

    if (c == qr_code.N - 1)
    {
        l++;
        c = 0;
        lb_counter = 0;

        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db))
        {
            loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter);
        }

        qr_code.cells[l - 1][c] = 2; // go back to the previous cell

        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db))
        {
            loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter);
        }
    }
    else
    {
        c++;
        lb_counter++;
        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db))
        {
            if (!(lb_counter + 1 == lb[l] && lt[l] - t > 3) && !(qr_code.N - c - 1 < lt[l] - t - 1) && !(lb[l] - lb_counter - 1 > qr_code.N - c - 1) && !(t == lt[l] && (qr_code.N - c - 1 < lb[l] - lb_counter - 1)))
            {
                loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter);
            }
        }

        qr_code.cells[l][c - 1] = 2; // go back to the previous cell

        if (qr_code.check_if_valid(lb, cb, lt, ct, qb, db))
        {
            if (!(lb[l] - lb_counter - 1 > qr_code.N - c - 1))
            {
                loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db, lb_counter);
            }
        }
    }
}

// this function will pre_process all the cell that fill the criteria of those parameters and
void pre_processing(QR_Code qr_code, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{
    vector<int> white_line = vector<int>(qr_code.N, 0);
    vector<int> black_line = vector<int>(qr_code.N, 0);

    int number_of_white_lines = 0;
    int number_of_black_lines = 0;

    for (int i = 0; i < qr_code.N; i++)
    {
        // simple case, everything filled
        if (lb[i] == qr_code.N)
        {
            black_line[i] = 1;
            number_of_black_lines++;
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[i][j] = 1;
            }
        }
        else if (lb[i] == 0)
        {
            white_line[i] = 1;
            number_of_white_lines++;
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[i][j] = 0;
            }
        }
        // check for 1 transition while there is just a black cell, this means that
        // the cell is in the corners of the line (we don't know what corner, left or right)
        if ((lb[i] == 1) && (lt[i] == 1))
        {
            for (int j = 1; j < qr_code.N - 1; j++)
            {
                // middle of the line, filled with white
                qr_code.cells[i][j] = 0;
            }
        }
        // the opposite of the previous case, where we have still one transition but this time
        // we have lots of black cells
        if ((lb[i] == qr_code.N - 1) && (lt[i] == 1))
        {
            // fill the middle of the line with black
            for (int j = 1; j < qr_code.N - 1; j++)
            {
                qr_code.cells[i][j] = 1;
            }
        }

        // check for "chess pattern"
        if (lt[i] == qr_code.N - 1 && lb[i] == qr_code.N / 2 + 1)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                if (j % 2 == 0)
                {
                    qr_code.cells[i][j] = 1;
                }
                else
                {
                    qr_code.cells[i][j] = 0;
                }
            }
        }
    }
    /* ##################################################*/

    // Loop through each line of the QR code and fill the line with black cells where there is only one color transition (lt[i] == 1) across the entire line
    for (int i = 0; i < qr_code.N; i++)
    {
        // we only have one color transition in the line
        if (lt[i] == 1)
        {
            // we'll fill the line with black cells
            for (int j = lb[i]; j < qr_code.N - lb[i]; j++)
            {
                qr_code.cells[i][j] = 0; // this
            }
        }
    }
    /* ##################################################*/

    for (int l = 0; l < qr_code.N; l++)
    {
        // fill columns
        if (cb[l] == qr_code.N)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[j][l] = 1;
            }
        }
        else if (cb[l] == 0)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[j][l] = 0;
            }
        }

        // fill the rest of the qr_code with black cells
        if (qr_code.N - number_of_white_lines == cb[l])
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                if (white_line[j] == 0)
                {
                    qr_code.cells[j][l] = 1;
                }
            }
        }

        // fill the rest of the qr_code with white cells
        if (qr_code.N - number_of_black_lines == 0)
        {
            for (int j = 0; j < qr_code.N - number_of_black_lines; j++)
            {
                if (black_line[j] == 0)
                {
                    qr_code.cells[j][l] = 0;
                }
            }
        }
    }
    /* ##################################################*/

    // check for a bulls eye pattern (target pattern)
    vector<int> diagonal = vector<int>(qr_code.N, 0);
    vector<int> inv_diagonal = vector<int>(qr_code.N, 0);
    int n_crossed = 0;         // number of crossed cells
    int n_counter_crossed = 0; // number of crossed cells in the diagonal
    for (int i = 0; i < qr_code.N; i++)
    {
        for (int j = 0; j < qr_code.N; j++)
        {
            if (i == j)
            {
                // we are in the diagonal, check if we have the same number of black cells in the line and in the column
                if (cb[i] == qr_code.N && lb[j] == qr_code.N)
                {
                    // if so, we have a bulls eye pattern, store the diagonal and the number of marks
                    diagonal[i] = 1;
                    n_crossed++;
                }
            }
            if (i + j == qr_code.N - 1)
            {
                // we are in the counter diagonal, check if we have the same number of black cells in the line and in the column
                if (cb[i] == qr_code.N && lb[j] == qr_code.N)
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
        for (int i = 0; i < qr_code.N; i++)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                if (i == j && diagonal[i] == 0)
                {
                    qr_code.cells[i][j] = 0;
                }
            }
        }
    }

    // if the number of marks is the same of the number of black cells in the counter diagonal, fill the counter diagonal with white cells
    if (n_counter_crossed == db[1])
    {
        for (int i = 0; i < qr_code.N; i++)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                if (i + j == qr_code.N - 1 && inv_diagonal[i] == 0)
                {
                    qr_code.cells[i][j] = 0;
                }
            }
        }
    }

    /* ##################################################*/

    // check if the diagonal is the same of N, if so that means that the diagonal is filled with black cells
    if (db[0] == qr_code.N)
    {
        // fill the diagonal with black cells
        for (int i = 0; i < qr_code.N; i++)
        {
            for (int k = 0; i < qr_code.N; k++)
            {
                // fill the diagonal as normal
                if (i == k)
                {
                    qr_code.cells[i][k] = 1;
                }
            }
        }

        for (int i = 0; i < qr_code.N; i++)
        {
            // check if the line contains just one black cell
            if (lb[i] == 1)
            {
                // since we have just one cell (the cell of the diagonal), that means the other cells of the square are white
                for (int j = 0; j < qr_code.N; j++)
                {
                    if (j != i)
                    {
                        qr_code.cells[i][j] = 0;
                    }
                }
            }
        }
    }
    // check if we have an empty diagonal, if so that means that the diagonal is filled with white cells
    else if (db[0] == 0)
    {
        // fill the diagonal with white cells
        for (int i = 0; i < qr_code.N; i++)
        {
            for (int k = 0; i < qr_code.N; k++)
            {
                // fill the diagonal as normal
                if (i == k)
                {
                    qr_code.cells[i][k] = 0;
                }
            }
        }

        for (int i = 0; i < qr_code.N; i++)
        {
            // check if the line contains just one white cell
            if (lb[i] == qr_code.N - 1)
            {
                // since we have just one white cell (the cell of the diagonal), that means the other cells of the square are black
                for (int j = 0; j < qr_code.N; j++)
                {
                    if (j != i)
                    {
                        qr_code.cells[i][j] = 1;
                    }
                }
            }
        }
    }

    // same logic as above, but for the inverse diagonal
    if (db[1] == 0)
    {
        for (int i = 0; i < qr_code.N; i++)
        {
            for (int k = 0; i < qr_code.N; k++)
            {
                if (i + k == qr_code.N - 1)
                {
                    qr_code.cells[i][qr_code.N - 1 - i] = 0;
                }
            }
        }

        for (int i = 0; i < qr_code.N; i++)
        {
            if (lb[i] == qr_code.N - 1)
            {
                for (int j = 0; j < qr_code.N; j++)
                {
                    if (i + j != qr_code.N - 1)
                    {
                        qr_code.cells[i][j] = 1;
                    }
                }
            }
        }
    }
    else if (db[1] == qr_code.N)
    {
        for (int i = 0; i < qr_code.N; i++)
        {
            for (int k = 0; i < qr_code.N; k++)
            {
                if (i + k == qr_code.N - 1)
                {
                    qr_code.cells[i][qr_code.N - 1 - i] = 1;
                }
            }
        }

        for (int i = 0; i < qr_code.N; i++)
        {
            if (lb[i] == 1)
            {
                for (int j = 0; j < qr_code.N; j++)
                {
                    if (i + j != qr_code.N - 1)
                    {
                        qr_code.cells[i][j] = 0;
                    }
                }
            }
        }
    }
    /* ##################################################*/

    // todo: more stuff to do probably
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