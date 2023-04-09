#include <iostream>
#include <vector>
#include <valarray>
#include <bitset>
#include <ctime> // include library

using namespace std;

int total_qrcodes = 0;
vector<vector<int>> toprint;

int tam;
int lb[31];
int lt[31];
int qb[4];
int cb[31];
int ct[31];
int db[2];

class Tree
{
public:
    void create_tree(vector<vector<int>> &qrcode, int x, int y, int blacksl)
    {

        if (y >= tam)
        {
            total_qrcodes++;
            toprint = qrcode;
            return;
        }
        if (qrcode[y][x] == 4)
        {
            if (x == tam - 1)
            {
                if (calclineblacks(blacksl, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
                {

                    create_tree(qrcode, 0, y + 1, 0);
                }
                return;
            }
            else
            {
                if (calclineblacks(blacksl, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
                {
                    create_tree(qrcode, x + 1, y, blacksl);
                }
                return;
            }
        }
        else if (qrcode[y][x] == 3)
        {
            if (x == tam - 1)
            {
                if (calclineblacks(blacksl + 1, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
                {
                    create_tree(qrcode, 0, y + 1, 0);
                }
                return;
            }
            else
            {
                if (calclineblacks(blacksl + 1, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
                {
                    create_tree(qrcode, x + 1, y, blacksl + 1);
                }
                return;
            }
        }

        qrcode[y][x] = 1;

        int transitions = sumtranslines(qrcode[y], x);
        if (x == tam - 1)
        {

            // gerar o qrcode
            if (calclineblacks(blacksl + 1, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
            {
                create_tree(qrcode, 0, y + 1, 0);
            }

            qrcode[y][x] = 0;

            if (calclineblacks(blacksl, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
            {
                create_tree(qrcode, 0, y + 1, 0);
            }
        }
        else
        {
            if (calclineblacks(blacksl + 1, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
            {
                if (!(blacksl + 1 == lb[y] && lt[y] - transitions - 1 > 2) && !(tam - x - 1 < lt[y] - transitions - 1) && !(lb[y] - blacksl - 1 > tam - x - 1) && !(transitions == lt[y] && (tam - x - 1 < lb[y] - blacksl - 1)))
                {

                    create_tree(qrcode, x + 1, y, blacksl + 1);
                }

                // se o numero de pretos que faltam for maior que o numero de espaços por preenche
            }

            qrcode[y][x] = 0;

            if (calclineblacks(blacksl, y, x) && calcTransLine(qrcode[y], y, x) && countcol(qrcode, y, x) && calcTransCol(qrcode, y, x) && quadrantes(qrcode, y, x) && dbverify(qrcode, y, x))
            {
                if (!(lb[y] - blacksl - 1 > tam - x - 1))
                {
                    create_tree(qrcode, x + 1, y, blacksl);
                }
            }
        }
    }

    bool calcTransLine(vector<int> &array, int y, int x)
    {
        int counter = 0;
        cout << "y: " << y << " x: " << x << endl;
        for (int i = 0; i < x; ++i)
        {
            cout << "" << array[i] % 2 << " ";
            if (array[i] % 2 != array[i + 1] % 2)
                counter++;
        }
        cout << endl;
        if (x == array.size() - 1 && counter < lt[y])
            return false;
        else if (counter > lt[y])
        {
                cout << "counter_trann < l: " << counter << " " << lt[y] << endl;
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
            if (counter > cb[j])
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

    int sumtranslines(vector<int> &line, int x)
    {
        int counter = 0;
        for (int i = 0; i < x; ++i)
        {
            if (line[i] % 2 != line[i + 1] % 2)
                counter++;
        }
        return counter;
    }

    void print_qrcode()
    {

        cout << '+';
        for (int i = 0; i < tam; ++i)
            cout << '-';
        cout << '+' << '\n';
        for (int i = 0; i < tam; ++i)
        {
            cout << '|';
            for (int j = 0; j < tam; ++j)
            {
                if (toprint[i][j] % 2 == 0)
                    cout << ' ';
                else if (toprint[i][j] % 2 == 1)
                    cout << '#';
            }
            cout << '|';
            cout << endl;
        }
        cout << '+';
        for (int i = 0; i < tam; ++i)
            cout << '-';
        cout << '+' << '\n';
    }
};
void preproc(vector<vector<int>> &qrcode)
{
    int proclinhasbranco = 0;
    int proclinhaspreto = 0;
    vector<int> lineb;
    vector<int> linep;
    for (int i = 0; i < tam; ++i)
    {
        lineb.push_back(0);
        linep.push_back(0);
    }
    for (int i = 0; i < tam; i++)
    {
        if (lt[i] == 1)
        {
            for (int j = lb[i]; j < tam - lb[i]; j++)
            {
                qrcode[i][j] = 4;
            }
        }
    }

    for (int i = 0; i < tam; ++i)
    {

        if (lt[i] == tam - 1 && lb[i] == tam / 2 + 1)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (j % 2 == 0)
                    qrcode[i][j] = 3;
                else
                    qrcode[i][j] = 4;
            }
        }

        if (lb[i] == tam)
        {
            for (int j = 0; j < tam; ++j)
            {
                qrcode[i][j] = 3;
            }
            proclinhaspreto++;
            linep[i] = 1;
        }
        if (lb[i] == 0)
        {
            for (int j = 0; j < tam; ++j)
            {
                qrcode[i][j] = 4;
            }
            proclinhasbranco++;
            lineb[i] = 1;
        }
        if ((lb[i] == 1) && (lt[i] == 1))
        {
            for (int j = 1; j < tam - 1; ++j)
            {
                qrcode[i][j] = 4;
            }
        }
        if ((lb[i] == tam - 1) && (lt[i] == 1))
        {
            for (int j = 1; j < tam - 1; ++j)
            {
                qrcode[i][j] = 3;
            }
        }
    }

    for (int j = 0; j < tam; j++)
    {
        if (cb[j] == tam)
        {
            for (int i = 0; i < tam; ++i)
            {
                qrcode[i][j] = 3;
            }
        }
        if (cb[j] == 0)
        {
            for (int i = 0; i < tam; ++i)
            {
                qrcode[i][j] = 4;
            }
        }

        if (tam - proclinhasbranco == cb[j])
        {
            for (int i = 0; i < tam; ++i)
            {
                if (!lineb[i])
                {
                    qrcode[i][j] = 3;
                }
            }
        }
        if (tam - proclinhaspreto == 0)
        {
            for (int i = 0; i < tam - proclinhaspreto; ++i)
            {
                if (!linep[i])
                {
                    qrcode[i][j] = 4;
                }
            }
        }
    }

    vector<int> diagonal;
    vector<int> cdiagonal;
    for (int i = 0; i < tam; ++i)
    {
        diagonal.push_back(0);
        cdiagonal.push_back(0);
    }

    if (db[0] == tam)
    {
        for (int i = 0; i < tam; ++i)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (i == j)
                {
                    qrcode[i][j] = 3;
                }
            }
        }
        for (int i = 0; i < tam; ++i)
        {
            if (lb[i] == 1)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (!(i == j))
                    {
                        qrcode[i][j] = 4;
                    }
                }
            }
        }
    }

    if (db[0] == 0)
    {
        for (int i = 0; i < tam; ++i)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (i == j)
                {
                    qrcode[i][j] = 4;
                }
            }
        }
        for (int i = 0; i < tam; ++i)
        {
            if (lb[i] == tam - 1)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (!(i == j))
                    {
                        qrcode[i][j] = 3;
                    }
                }
            }
        }
    }

    if (db[1] == tam)
    {
        for (int i = 0; i < tam; ++i)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (i == tam - j - 1)
                {
                    qrcode[i][tam - i - 1] = 3;
                }
            }
        }
        for (int i = 0; i < tam; ++i)
        {
            if (lb[i] == 1)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (!(i == tam - j - 1))
                    {
                        qrcode[i][j] = 4;
                    }
                }
            }
        }
    }

    if (db[1] == 0)
    {
        for (int i = 0; i < tam; ++i)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (i == tam - j - 1)
                {
                    qrcode[i][tam - i - 1] = 4;
                }
            }
        }
        for (int i = 0; i < tam; ++i)
        {
            if (lb[i] == tam - 1)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (!(i == tam - j - 1))
                    {
                        qrcode[i][j] = 3;
                    }
                }
            }
        }
    }
    int cruzado = 0;
    int ccruzado = 0;
    for (int i = 0; i < tam; ++i)
    {
        for (int j = 0; j < tam; ++j)
        {
            if (i == j)
            {
                if (cb[i] == tam && lb[i] == tam)
                {
                    diagonal[i] = 1;
                    cruzado++;
                }
            }
            if (tam - 1 - i == j)
            {
                if (cb[i] == tam && lb[i] == tam)
                {
                    cdiagonal[i] = 1;
                    ccruzado++;
                }
            }
        }
    }
    if (cruzado == db[0])
    {
        for (int i = 0; i < tam; ++i)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (i == j && !diagonal[i])
                {
                    qrcode[i][j] = 4;
                }
            }
        }
    }

    if (ccruzado == db[1])
    {
        for (int i = 0; i < tam; ++i)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (i == tam - j - 1 && !cdiagonal[i])
                {
                    qrcode[i][tam - i - 1] = 4;
                }
            }
        }
    }

    // preencher os quadrantes
    int size;
    bool q1 = false;
    bool q2 = false;
    bool q3 = false;
    bool q4 = false;

    if (tam % 2 == 0)
    {
        size = tam / 2;
        if (qb[0] == size * size)
        {
            q1 = true;
        }
        if (qb[1] == size * size)
        {
            q2 = true;
        }
        if (qb[2] == size * size)
        {
            q3 = true;
        }
        if (qb[3] == size * size)
        {
            q3 = true;
        }
    }
    else
    {
        size = tam / 2 + 1;
        if (qb[0] == size * (size - 1))
        {
            q1 = true;
        }
        if (qb[1] == (size - 1) * (size - 1))
        {
            q2 = true;
        }
        if (qb[2] == (size) * (size - 1))
        {
            q3 = true;
        }
        if (qb[3] == (size) * (size))
        {
            q3 = true;
        }
    }
    if (q1 || q2 || q3 || q4)
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i <= tam / 2 && j > tam / 2 && q1)
                {
                    qrcode[i - 1][j - 1] == 3;
                }
                // segundo quadrante
                else if (i <= tam / 2 && j <= tam / 2 && q2)
                {
                    qrcode[i - 1][j - 1] == 3;
                }
                // terceiro quadrante
                else if (i > tam / 2 && j <= tam / 2 && q3)
                {
                    qrcode[i - 1][j - 1] == 3;
                }
                // quarto quadrante
                else if (i > tam / 2 && j > tam / 2 && q4)
                {
                    qrcode[i - 1][j - 1] == 3;
                }
            }
        }
    }

    q1 = false;
    q2 = false;
    q3 = false;
    q4 = false;
    if (qb[0] == 0)
    {
        q1 = true;
    }
    if (qb[1] == 0)
    {
        q2 = true;
    }
    if (qb[2] == 0)
    {
        q3 = true;
    }
    if (qb[3] == 0)
    {
        q3 = true;
    }
    if (q1 || q2 || q3 || q4)
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i <= tam / 2 && j > tam / 2 && q1)
                {
                    qrcode[i - 1][j - 1] == 4;
                }
                // segundo quadrante
                else if (i <= tam / 2 && j <= tam / 2 && q2)
                {
                    qrcode[i - 1][j - 1] == 4;
                }
                // terceiro quadrante
                else if (i > tam / 2 && j <= tam / 2 && q3)
                {
                    qrcode[i - 1][j - 1] == 4;
                }
                // quarto quadrante
                else if (i > tam / 2 && j > tam / 2 && q4)
                {
                    qrcode[i - 1][j - 1] == 4;
                }
            }
        }
    }

    for (int i = 0; i < tam; i++)
    {
        if (lb[i] * 2 == lt[i])
        {
            qrcode[i][0] = 4;
            qrcode[i][tam - 1] = 4;
        }
        if ((tam - lb[i]) * 2 == lt[i])
        {
            qrcode[i][0] = 3;
            qrcode[i][tam - 1] = 3;
        }
        if (cb[i] * 2 == ct[i])
        {
            qrcode[0][i] = 4;
            qrcode[tam - 1][i] = 4;
        }
        if ((tam - cb[i]) * 2 == ct[i])
        {
            qrcode[0][i] = 3;
            qrcode[tam - 1][i] = 3;
        }
    }

    // -------------------------------------------------
    for (int i = 0; i < tam; i++)
    {
        int contatres = 0;
        int contaquatro = 0;
        int contatresdiagonal = 0;
        int contaquatrodiagonal = 0;
        int ccontatresdiagonal = 0;
        int ccontaquatrodiagonal = 0;
        for (int i = 0; i < tam; ++i)
        {
            for (int j = 0; j < tam; ++j)
            {
                if (qrcode[i][j] == 3)
                {
                    contatres++;
                }
                if (qrcode[i][j] == 4)
                {
                    contaquatro++;
                }
                if (i == j && qrcode[i][j] == 3)
                {
                    contatresdiagonal++;
                }
                if (i == j && qrcode[i][j] == 4)
                {
                    contaquatrodiagonal++;
                }
                if (i == tam - j - 1 && qrcode[i][j] == 3)
                {
                    ccontatresdiagonal++;
                }
                if (i == tam - j - 1 && qrcode[i][j] == 4)
                {
                    ccontaquatrodiagonal++;
                }
            }
            if (lt[i] % 2 == 0)
            {
                if (qrcode[i][0])
                {
                    qrcode[i][tam - 1] = qrcode[i][0];
                }
                else if (qrcode[i][tam - 1])
                {
                    qrcode[i][0] = qrcode[i][tam - 1];
                }
            }

            if (ct[i] % 2 == 0)
            {
                if (qrcode[0][i])
                {
                    qrcode[tam - 1][i] = qrcode[0][i];
                }
                else if (qrcode[tam - 1][i])
                {
                    qrcode[0][i] = qrcode[tam - 1][i];
                }
            }
        }

        if (ccontatresdiagonal == db[1])
        {
            for (int i = 0; i < tam; ++i)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (i == tam - j - 1 && qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 4;
                    }
                }
            }
        }

        if (ccontaquatrodiagonal == tam - db[1])
        {
            for (int i = 0; i < tam; ++i)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (i == tam - j - 1 && qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 3;
                    }
                }
            }
        }

        if (contatresdiagonal == db[0])
        {
            for (int i = 0; i < tam; ++i)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (i == j && qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 4;
                    }
                }
            }
        }

        if (contaquatrodiagonal == tam - db[0])
        {
            for (int i = 0; i < tam; ++i)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (i == j && qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 3;
                    }
                }
            }
        }

        if (contatres == qb[0] + qb[1] + qb[2] + qb[3])
        {
            for (int i = 0; i < tam; ++i)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 4;
                    }
                }
            }
        }
        if (contaquatro == tam - (qb[0] + qb[1] + qb[2] + qb[3]))
        {
            for (int i = 0; i < tam; ++i)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 3;
                    }
                }
            }
        }

        for (int i = 0; i < tam; ++i)
        {
            int linhatres = 0;
            int linhaquatro = 0;
            for (int j = 0; j < tam; j++)
            {
                if (qrcode[i][j] == 3)
                {
                    linhatres++;
                }
                if (qrcode[i][j] == 4)
                {
                    linhaquatro++;
                }
            }

            if (linhatres == lb[i])
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 4;
                    }
                }
            }
            if (tam - lb[i] == linhaquatro)
            {
                for (int j = 0; j < tam; ++j)
                {
                    if (qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 3;
                    }
                }
            }
        }

        for (int j = 0; j < tam; ++j)
        {
            int colunatres = 0;
            int colunaquatro = 0;

            for (int i = 0; i < tam; ++i)
            {
                if (qrcode[i][j] == 3)
                {
                    colunatres++;
                }
                if (qrcode[i][j] == 4)
                {
                    colunaquatro++;
                }
            }
            if (colunatres == cb[j])
            {
                for (int i = 0; i < tam; ++i)
                {
                    if (qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 4;
                    }
                }
            }
            if (tam - cb[j] == colunaquatro)
            {
                for (int i = 0; i < tam; ++i)
                {
                    if (qrcode[i][j] == 0)
                    {
                        qrcode[i][j] = 3;
                    }
                }
            }
        }
    }

    int totalq1 = 0;
    int totalq2 = 0;
    int totalq3 = 0;
    int totalq4 = 0;

    int brancasq1 = 0;
    int brancasq2 = 0;
    int brancasq3 = 0;
    int brancasq4 = 0;

    for (int i = 1; i < tam + 1; ++i)
    {
        for (int j = 1; j < tam + 1; ++j)
        {

            // primeiro quadrante
            if (i <= tam / 2 && j > tam / 2 && qrcode[i - 1][j - 1] == 3)
            {
                totalq1++;
            }
            if (i <= tam / 2 && j > tam / 2 && qrcode[i - 1][j - 1] == 4)
            {
                brancasq1++;
            }
            // segundo quadrante
            else if (i <= tam / 2 && j <= tam / 2 && qrcode[i - 1][j - 1] == 3)
            {
                totalq2++;
            }
            else if (i <= tam / 2 && j <= tam / 2 && qrcode[i - 1][j - 1] == 4)
            {
                brancasq2++;
            }
            // terceiro quadrante
            else if (i > tam / 2 && j <= tam / 2 && qrcode[i - 1][j - 1] == 3)
            {
                totalq3++;
            }
            else if (i > tam / 2 && j <= tam / 2 && qrcode[i - 1][j - 1] == 4)
            {
                brancasq3++;
            }
            // quarto quadrante
            else if (i > tam / 2 && j > tam / 2 && qrcode[i - 1][j - 1] == 3)
            {
                totalq4++;
            }
            else if (i > tam / 2 && j > tam / 2 && qrcode[i - 1][j - 1] == 4)
            {
                brancasq4++;
            }
        }
    }
    if (totalq1 == qb[0])
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i <= tam / 2 && j > tam / 2 && qrcode[i - 1][j - 1] == 0)
                {
                    qrcode[i - 1][j - 1] = 4;
                }
            }
        }
    }

    if (brancasq1 == tam - qb[0])
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i <= tam / 2 && j > tam / 2 && qrcode[i - 1][j - 1] == 0)
                {
                    qrcode[i - 1][j - 1] = 3;
                }
            }
        }
    }

    if (totalq2 == qb[1])
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i <= tam / 2 && j <= tam / 2 && qrcode[i - 1][j - 1] == 0)
                {
                    qrcode[i - 1][j - 1] = 4;
                }
            }
        }
    }
    if (brancasq2 == tam - qb[1])
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i <= tam / 2 && j <= tam / 2 && qrcode[i - 1][j - 1] == 0)
                {
                    qrcode[i - 1][j - 1] = 3;
                }
            }
        }
    }

    if (totalq3 == qb[2])
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i > tam / 2 && j <= tam / 2 && qrcode[i - 1][j - 1] == 0)
                {
                    qrcode[i - 1][j - 1] = 4;
                }
            }
        }
    }

    // if(brancasq3 == tam-qb[2]){
    //     for (int i = 1; i < tam+1; ++i) {
    //         for (int j = 1; j < tam+1; ++j) {

    //             //primeiro quadrante
    //             if (i > tam / 2 && j <= tam / 2 && qrcode[i-1][j-1] == 0){
    //                 qrcode[i-1][j-1] = 3;
    //             }

    //         }
    //     }
    // }

    if (totalq4 == qb[3])
    {
        for (int i = 1; i < tam + 1; ++i)
        {
            for (int j = 1; j < tam + 1; ++j)
            {

                // primeiro quadrante
                if (i > tam / 2 && j > tam / 2 && qrcode[i - 1][j - 1] == 0)
                {
                    qrcode[i - 1][j - 1] = 4;
                }
            }
        }
    }
    // if(brancasq4 == tam-qb[3]){
    //     for (int i = 1; i < tam+1; ++i) {
    //         for (int j = 1; j < tam+1; ++j) {

    //             //primeiro quadrante
    //             if (i > tam / 2 && j > tam / 2  && qrcode[i-1][j-1] == 0){
    //                 qrcode[i-1][j-1] = 3;
    //             }

    //         }
    //     }
    // }
}

bool validationcheck()
{

    // check if is possible to create a qrcode with the transitions and blacks
    for (int i = 0; i < tam; ++i)
    {
        if (lt[i] == 0 && (lb[i] > 0 && lb[i] < tam))
        {
            return false;
        }
        if (ct[i] == 0 && (cb[i] > 0 && cb[i] < tam))
        {
            return false;
        }

        if (tam - 1 < lt[i])
        {
            return false;
        }
        // if(lt[i] == tam -1 && tam/2 < lb[i]){
        //     return false;
        // }
        if (lt[i] == tam - 1 && tam / 2 > lb[i])
        {
            return false;
        }
        if (tam - 1 < ct[i])
        {
            return false;
        }
        // if(ct[i] == tam -1 && tam/2 < cb[i]){
        //     return false;
        // }

        if (ct[i] == tam - 1 && tam / 2 > cb[i])
        {
            return false;
        }
        if (lb[i] < tam / 2 && lb[i] * 2 < lt[i])
        {
            return false;
        }
    }

    int somacolunas = 0;
    int somalinhas = 0;
    int somaquadrantes = 0;
    for (int i = 0; i < tam; ++i)
    {
        somacolunas += cb[i];
        somalinhas += lb[i];
    }
    for (int i = 0; i < 4; ++i)
    {
        somaquadrantes += qb[i];
    }
    if (somacolunas != somalinhas || somacolunas != somaquadrantes || somalinhas != somaquadrantes)
    {
        return false;
    }
    int linhascima = 0;
    int linhasbaixo = 0;
    int colunasesq = 0;
    int colunasdir = 0;
    // somar as primeiras linhas
    for (int i = 0; i < tam / 2; ++i)
    {
        linhascima += lb[i];
        colunasesq += cb[i];
    }

    for (int i = tam / 2; i < tam; ++i)
    {
        linhasbaixo += lb[i];
        colunasdir += cb[i];
    }

    if (((qb[0] + qb[1]) != linhascima) || ((qb[2] + qb[3]) != linhasbaixo) || ((qb[1] + qb[2]) != colunasesq) || ((qb[0] + qb[3]) != colunasdir))
    {
        return false;
    }

    return true;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int number_qrcodes;
    cin >> number_qrcodes;

    for (int i = 0; i < number_qrcodes; ++i)
    {

        cin >> tam;

        for (int j = 0; j < tam; j++)
            cin >> lb[j];
        for (int j = 0; j < tam; j++)
            cin >> cb[j];
        for (int j = 0; j < tam; j++)
            cin >> lt[j];
        for (int j = 0; j < tam; j++)
            cin >> ct[j];
        cin >> qb[0] >> qb[1] >> qb[2] >> qb[3];
        cin >> db[0] >> db[1];

        clock_t begin = clock();

        vector<int> checkquad = {0, 0, 0, 0};
        vector<vector<int>> qrcode;
        for (int j = 0; j < tam; ++j)
        {
            vector<int> line;
            for (int k = 0; k < tam; k++)
            {
                line.push_back(0);
            }
            qrcode.push_back(line);
        }

        Tree tree;
        if (validationcheck())
        {
            preproc(qrcode);
            // print qrcode
            //  for (int i = 0; i < tam; i++){
            //      for (int j = 0; j < tam; j++){
            //          cout << qrcode[i][j] << " ";
            //      }
            //      cout << endl;
            //  }
            tree.create_tree(qrcode, 0, 0, 0);
        }

        if (total_qrcodes == 0)
            cout << "DEFECT: No QR Code generated!\n";
        else if (total_qrcodes > 1)
            cout << "INVALID: " << total_qrcodes << " QR Codes generated!\n";
        else if (total_qrcodes == 1)
        {
            cout << "VALID: 1 QR Code generated!\n";
            tree.print_qrcode();
        }
        clock_t end = clock();
        // cout << "Time: " << double(end - begin) / CLOCKS_PER_SEC << "s\n";
        total_qrcodes = 0;
    }
    return 0;
}

/*Input teste
1
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

*/