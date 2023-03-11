#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <math.h>

/*
Each QR code can be seen as a matrix of binary cells, meaning that each cell can be represented as a black or white pixel/square.

In order to send and store QR codes, compressing QR codes is essential to save network traffic and storage space. So, a tech start up - QR Shorter Inc - is developing a method to compress the QR codes. The method encodes the QR code as summary of its characteristics.

The encoding algorithm goes as follows:

save the information about the size of the QR Code, meaning the number of cells per line/column (N). The QR Shorter Inc. works only with squared QR Codes.
save the amount of black cells per line (lb, line blacks, array of size N)
save the amount of black cells per column (cb, column blacks, array of size N)
save the amount of color transitions per line (lt, line color transitions, array of size N)
save the amount of color transitions per column (ct, column color transitions, array of size N)
save the amount of black cells per quadrant (qb, quadrant blacks, array of size 4)
save the amount of black cells per diagonal - main and antidiagonal - (db, diagonal blacks, array of size 2)


Encoding Notes:
Consider the QR Code as a squared matrix of side N. For a matrix cell c positioned in the line line 1 ≤ l ≤ N and a column 1 ≤ c ≤ N, c belongs to:

quadrant 1 if l ≤ floor(N/2) and c > floor(N/2),
quadrant 2 if l ≤ floor(N/2) and c ≤ floor(N/2),
quadrant 3 if l > floor(N/2) and c ≤ floor(N/2),
quadrant 4 if l > floor(N/2) and c > floor(N/2),
diagonal 1 (main diagonal) if l = c
diagonal 2 (antidiagonal) if L-l+1 = c
Figure 2 shows two examples on how the encoding is performed.


From that information, it should be possible to reconstruct (decode) the QR code and save some space. The storage needed for that info is 1+4*N+4+2, which is smaller than N*N, for every N > 5.

Example:

From the encoding

N = 4
lb = 1 1 1 1
cb = 1 1 1 1
lt = 1 2 2 1
ct = 1 2 2 1
qb = 0 2 0 2
db = 4 0
the following QR code can be decoded:

Oh no, a problem!
The company found out that, in some cases, two (or more) different QR codes generate the same encoding. See the example in Figure 4.

This is a big step back in their business model. They decided that, instead of withdrawing the idea, they would implement a module that evaluates each encoding and says if it decodes into a single QR code or to multiple QR codes. If more than one QR code can be decoded from that encoding, it will be considered an invalid encoding.

Your job is to create this module. Given an encoding, find out how many QR codes can be generated from it. If only one, print the decoded QR code, if more than one, state the encoding as INVALID and tell how many QR codes it can generate.
Additional, there might be some defects in the encodings, so if you find one of those cases that cannot generate any QR coded state as DEFECT.

Input
The input starts with one line containing the number of encodings to be assessed.

Then, for each encoding, the first line contains the size of the QR code in lines/columns: 2 ≤ N ≤ 30

After that:

One line containing N numbers (each between 1 and N), corresponding to the number of black cells in each line 1 to N (lb).
One line containing N numbers (each between 1 and N), corresponding to the number of black cells in each column 1 to N (cb).
One line containing N numbers (each between 1 and N-1), corresponding to the number of color transitions in each line 1 to N (lt).
One line containing N numbers (each between 1 and N-1), corresponding to the number of color transitions in each column 1 to N (ct).
One line containing 4 numbers (each between 1 and floor(N/2+1)^2), corresponding to the number of black cells in each quadrant 1 to 4 (qb).
One line containing 2 numbers (each between 1 and N), corresponding to the number of black cells in each diagonal 1 to 2 (db).


Output
For each test case, you should print one of the following possible outputs:

Valid case: 1 QR code decoded
If only one solution can be generated from the encoding, you should print the following phrase:

VALID: 1 QR Code generated.

After that, print the QR Code generated. Print every white cell as a space character ' ' and every black cell as a cardinal '#' character. Print a frame for the QR code using the character '+' for the corners, '-' for the top and bottom borders and '|' for the left and right borders.

Invalid case: more than one QR code decoded
If more than one QR Code is decoded, you should print the phrase (being k the number of QR Codes generated):

INVALID: K QR Codes generated.

Defect case: 0 QR codes decoded
If no QR Code can be decoded from the encoding, than print the phrase:

DEFECT: No QR Code generated.

Example
Example input:

3
3
1 0 1
1 0 1
1 0 1
1 0 1
0 1 0 1
2 0
4
2 2 2 2
2 2 2 2
3 3 3 3
1 1 1 1
2 2 2 2
2 2
4
2 3 4 3
2 3 4 3
1 1 0 1
1 1 0 1
4 3 4 1
2 4
Example output:

VALID: 1 QR Code generated!
+---+
|#  |
|   |
|  #|
+---+
INVALID: 2 QR Codes generated!
DEFECT: No QR Code generated!
*/

using namespace std;

struct QR_Code
{
    int N;
    int **cells;
    int has_error = 0; // 0 = no error, 1 = error
    int nr_qr_codes = 0;

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

    // check if the values given as parameters are the same as the qr code
    int check_if_valid(vector<int> cb, vector<int> lb, vector<int> ct, vector<int> lt, int *qb, int *db)
    {
        /* make a print comparing both vector<int> cb, vector<int> lb, vector<int> ct, vector<int> lt, int *qb, int *db with qr_code.cb, qr_code.lb, qr_code.ct, qr_code.lt, qr_code.qb, qr_code.db */
        // just to print
        for (int i = 0; i < N; i++)
        {
            cout << "i: " << i << endl;
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

        cout << "............." << endl;
        printf("check res: %d %d %d %d %d %d\n", cb == this->cb, lb == this->lb, ct == this->ct, lt == this->lt, equal(qb, qb + 4, this->qb), equal(db, db + 2, this->db));
        printf("%d", cb == this->cb && lb == this->lb && ct == this->ct && lt == this->lt && equal(qb, qb + 4, this->qb) && equal(db, db + 2, this->db));
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

int start_generating_tree(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int qb[], int db[]);
int check_cell(QR_Code qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
QR_Code * loop_through_all_cells(QR_Code *qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db);
void printQR_code(QR_Code *qr_code);
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
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    }
    return 0;
}

/*
    In order to get fast output times, we'll use a binary tree to store the possible QR Codes.
    Whenever we find that our qr code is invalid according to the encoding, we'll stop the branch of the tree, print "INVALID" and return.
    If we find that our qr code is valid, we'll print "VALID", print the qr_Code and move on to the next branch.
    If we find that our qr code is a defect, we'll print "DEFECT" and move on to the next branch.

    N saves the information about the size of the QR Code, meaning the number of cells per line/column (N). The QR Shorter Inc. works only with squared QR Codes.
    lb saves the amount of black cells per line (lb, line blacks, array of size N)
    cb saves the amount of black cells per column (cb, column blacks, array of size N)
    lt saves the amount of color transitions per line (lt, line color transitions, array of size N)
    ct saves the amount of color transitions per column (ct, column color transitions, array of size N)
    qb saves the amount of black cells per quadrant (qb, quadrant blacks, array of size 4)
    db saves the amount of black cells per diagonal - main and antidiagonal - (db, diagonal blacks, array of size 2)


    i will treverse the qr_code cell by cell and i will add next to it the possible qr_codes that can be generated from the current qr_code.

                                            +--+
                                            |  | <--- blank qr_code
                                            |  |
                                            +--+
                                +--+                    +--+
                                |  |                    |# | <--- first cell of the tree, i = 0, j = 0
                                |  |                    |  |
                                +--+                    +--+
                        +--+            +--+        +--+        +--+
                        |  |            | #|        |# |        |##| <--- second cell of the tree, i = 0, j = 1
                        |  |            |  |        |  |        |  |
                        +--+            +--+        +--+        +--+
                    +--+    +--+    +--+    +--+ +--+    +--+ +--+    +--+
                    |  |    |  |    | #|    | #| |# |    |# | |##|    |##| <--- third cell of the tree, i = 1, j = 0
                    |  |    |# |    |  |    |# | |  |    |# | |  |    |# |
                    +--+    +--+    +--+    +--+ +--+    +--+ +--+    +--+
                ...................................4th cell.............................................

                quadrants in the qr_code
                    0   1
                    2   3

                if it's odd:
                    2 0 1
                    0 0 0
                    3 0 4
*/
int start_generating_tree(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{

    // first detect if the settings are valid
    if (defect_detection(N, lb, cb, lt, ct, qb, db) == true)
    {
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
    QR_Code qr_code_pre_pro = pre_processing(qr_code, lb, cb, lt, ct, qb, db);

    // qr_code_new is the qr pre processed.
    cout << "pre-processed:" << endl;
    printQR_code(&qr_code_pre_pro);

    /*
        we need to do some pre processing, set the cells that need to be certain values according to the settings lb, cb, lt, ct, qb, db
        also, we need to find the defects (find if the current lb, cb, lt, ct, qb, db are valid without generating the qr code)
        make sure to optimize the tree sicne the recursion will be very deep in some cases
    */
    // carefull this function is recursive and will run thousands of times
    QR_Code *qr2 = loop_through_all_cells(&qr_code_pre_pro, 0, 0, lb, cb, lt, ct, qb, db);
    qr2 -> cells[0][0] = 3;
    QR_Code *qr_code_new = loop_through_all_cells(qr2, 0, 0, lb, cb, lt, ct, qb, db);

    if (qr_code_new->has_error == 0)
    {
        // check the size of qr_code.qr_codes_generated and make sure its 0
        if (qr_codes_generated == 1)
        {
            std::cout << "VALID: 1 QR Code generated!" << std::endl;
            qr_code_new->print_qr_code();
            return 0;
        }

        std::cout << "INVALID: " << qr_codes_generated << " QR Codes generated!" << std::endl;
        return 0;
    }

    std::cout << "DEFECT: No QR Code generated!" << std::endl;
    return 0;
}

QR_Code * loop_through_all_cells(QR_Code *qr_code, int l, int c, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int *qb, int *db)
{
    /*
        One line containing N numbers (each between 1 and N), corresponding to the number of black cells in each line 1 to N (lb). - done
        One line containing N numbers (each between 1 and N), corresponding to the number of black cells in each column 1 to N (cb). - done
        One line containing N numbers (each between 1 and N-1), corresponding to the number of color transitions in each line 1 to N (lt). - done
        One line containing N numbers (each between 1 and N-1), corresponding to the number of color transitions in each column 1 to N (ct). - done
        One line containing 4 numbers (each between 1 and floor(N/2+1)^2), corresponding to the number of black cells in each quadrant 1 to 4 (qb). - done
        One line containing 2 numbers (each between 1 and N), corresponding to the number of black cells in each diagonal 1 to 2 (db). - done

        0 -> white (from pre-processing)
        1 -> black (from pre-processing)
        2 -> white (from tree)
        3 -> black (from tree)
    */
    if (qr_code->has_error == 1)
    {
        std::cout << "fds" << std::endl;
        return qr_code;
    }

    if (qr_code->cells[l][c] == 1 || qr_code->cells[l][c] == 0)
    {
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
        loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db);
        qr_code->cells[l][c] = 3;
        loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db);
    }

    /* final cell  */
    if (l == qr_code->N - 1 && c == qr_code->N - 1)
    {
        // REACHED THE END, check if i have a valid qr_code, if so add it to the vector of qr_codes
        if (qr_code->check_if_valid(lb, cb, lt, ct, qb, db) == 1)
        {
            qr_codes_generated++;
        }

        // if we found a solution, we'll add it to the vector of qr_codes
        return qr_code;
    }

    /* color transitions */
    if (c != qr_code->N - 1)
    {
        if (qr_code->cells[l][c] % 2 != qr_code->cells[l][c + 1] % 2)
        {
            qr_code->ct[c]++;
            if (qr_code->ct[c] > ct[c])
            {
                qr_code->has_error = 1;
                return qr_code;
            }
        }
    }

    if (l != qr_code->N - 1)
    {
        if (qr_code->cells[l][c] % 2 != qr_code->cells[l + 1][c] % 2)
        {
            qr_code->lt[l]++;
            if (qr_code->lt[l] > lt[l])
            {
                qr_code->has_error = 1;
                return qr_code;
            }
        }
    }

    /* counting */
    if (qr_code->cells[l][c] % 2 == 1)
    {
        /* black cells column */
        qr_code->cb[c]++;

        /* black cells inline */
        qr_code->lb[l]++;

        /* diagonals */
        if (l == c)
        {
            qr_code->db[0]++;
            if (qr_code->db[0] > db[0])
            {
                qr_code->has_error = 1;
                return qr_code;
            }
        }
        else if (l + c == qr_code->N - 1)
        {
            qr_code->db[1]++;
            if (qr_code->db[1] > db[1])
            {
                qr_code->has_error = 1;
                return qr_code;
            }
        }

        /* quadrants
            1 0
            2 3
        */
        if (l < qr_code->N / 2)
        {
            if (c < qr_code->N / 2)
            {
                qr_code->qb[1]++;
                if (qr_code->qb[1] > qb[1])
                {
                    qr_code->has_error = 1;
                    return qr_code;
                }
            }
            else if (c > qr_code->N / 2)
            {
                qr_code->qb[0]++;
                if (qr_code->qb[0] > qb[0])
                {
                    qr_code->has_error = 1;
                    return qr_code;
                }
            }
        }
        else if (l > qr_code->N / 2)
        {
            if (c < qr_code->N / 2)
            {
                qr_code->qb[2]++;
                if (qr_code->qb[2] > qb[2])
                {
                    qr_code->has_error = 1;
                    return qr_code;
                }
            }
            else if (c > qr_code->N / 2)
            {
                qr_code->qb[3]++;
                if (qr_code->qb[3] > qb[3])
                {
                    qr_code->has_error = 1;
                    return qr_code;
                }
            }
        }
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
    QR_Code* qr_code_n = loop_through_all_cells(qr_code, l, c, lb, cb, lt, ct, qb, db);
    printQR_code(qr_code_n);
    qr_code_n->cells[l][c] = 3;
    QR_Code* qr_code_n2 = loop_through_all_cells(qr_code_n, l, c, lb, cb, lt, ct, qb, db);
    printQR_code(qr_code_n2);

    return qr_code_n2;
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
        }
        else if (num_black == 0)
        {
            for (int j = 0; j < qr_code.N; j++)
            {
                qr_code.cells[i][j] = 0;
            }
        }
    }
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
    // first check if lb and cb make sense, floor down N / 2
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

void printQR_code(QR_Code *qr_code)
{

    // Print the qr_code info
    cout << "---------------------QR Code info----------------" << endl;
    cout << "N: " << qr_code->N << endl;
    cout << "lb - nr of black cells inline: ";
    for (int i = 0; i < qr_code->N; i++)
    {
        cout << qr_code->lb[i] << " ";
    }
    cout << endl;
    cout << "cb - nr of black cells in column: ";
    for (int i = 0; i < qr_code->N; i++)
    {
        cout << qr_code->cb[i] << " ";
    }
    cout << endl;
    cout << "lt - nr of color transitions in line: ";
    for (int i = 0; i < qr_code->N; i++)
    {
        cout << qr_code->lt[i] << " ";
    }
    cout << endl;
    cout << "ct - nr of color transitions in column:";
    for (int i = 0; i < qr_code->N; i++)
    {
        cout << qr_code->ct[i] << " ";
    }
    cout << endl;
    cout << "qb - nr of black cells in quadrant:";
    for (int i = 0; i < 4; i++)
    {
        cout << qr_code->qb[i] << " ";
    }
    cout << endl;
    cout << "db - nr of black cells in diagonal: ";
    for (int i = 0; i < 2; i++)
    {
        cout << qr_code->db[i] << " ";
    }
    cout << endl;

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