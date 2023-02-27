#include <vector>
#include <iostream>
#include <sstream>
#include <vector>

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

int start_generating_tree(int N, std::vector<int> lb, std::vector<int> cb, std::vector<int> lt, std::vector<int> ct, int qb[], int db[]);

struct QR_Code
{
    int N;
    int **cells;

    QR_Code(int N) {
        this->N = N;
        this->cells = new int*[N];
        for (int i = 0; i < N; i++) {
            this->cells[i] = new int[N];
        }
    }
};

// create a binary tree with QR_Code as data. every node has a square on or off (1 or 0)
struct Node
{
    QR_Code code;
    Node * left;
    Node * right;

    Node(QR_Code data) {
        this->code = data;
        this->left = nullptr;
        this->right = nullptr;
    }
};

struct BinaryTree {
    Node * root;

    BinaryTree() {
        this->root = nullptr;
    }

    void insert(QR_Code data) {
        Node * node = new Node(data);
        if (this->root == nullptr) {
            this->root = node;
        } else {
            Node * current = this->root;
            while (true) {
                if (data.cells[0][0] == 1) {
                    if (current->left == nullptr) {
                        current->left = node;
                        break;
                    } else {
                        current = current->left;
                    }
                } else {
                    if (current->right == nullptr) {
                        current->right = node;
                        break;
                    } else {
                        current = current->right;
                    }
                }
            }
        }
    }

    void print() {
        print(this->root);
    }

    void print(Node * node) {
        if (node == nullptr) {
            return;
        }
        print(node->left);
        print(node->right);
    }
};

// insert a new node in the binary tree

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

    std::cin >> N;
    if (N < 2 || N > 30)
    {
        std::cout << "N must be between 2 and 30" << std::endl;
        return 0;
    }
    
    for (int i = 0; i < num; i++)
    {
        // create an int vector of size N
        // without this way int lb[N] it will not work
        std::vector<int> lb(N);
        std::vector<int> cb(N);
        std::vector<int> lt(N);
        std::vector<int> ct(N);
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

        start_generating_tree(N, lb, cb, lt, ct, qb, db);
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

    I will create a tree that contains every possible generation of qr codes. I will start with a root node that contains an empty qr code, a blank one N by N.
    the first line of the tree will have just one node, the root node, the blank qr code.

    i will treverse the qr_code cell by cell and i will add next to it the possible qr_codes that can be generated from the current qr_code.

                                            +--+
                                            |  | <--- root node, blank qr_code
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
    //  start generating a tree with all the qr_codes of size N

    // create a root node an empty qr code with N lines and N columns
    QR_Code qr_code_root;
    qr_code_root.cells = new int *[N];
    for (int i = 0; i < N; i++)
    {
        qr_code_root.cells[i] = new int[N];
    }

    return 0;
}

int printQR_code(QR_Code qr_code)
{
    cout << "+";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << "-";
    }
    cout << "+" << endl;

    // Print the QR code

    // Print the bottom border
    cout << "+";
    for (int i = 0; i < qr_code.N; i++)
    {
        cout << "-";
    }
    cout << "+" << endl;
}