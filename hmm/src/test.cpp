#include <bits/stdc++.h>
#include "../include/hmm.h"

#define MAX_ITER 100
#define MAX_TOL 0.000001

using namespace std;

void readInitialParameters(int &n, int &m, double *&pi, double **&T, double **&O)
{
    freopen("initial-parameters.txt", "r", stdin);

    scanf("%d", &n); // number of nodes
    scanf("%d", &m); // number of observations

    pi = new double [n];
    T = new double* [n];
    for (int i = 0; i < n; i++)
    {
        T[i] = new double[n];
    }
    O = new double* [n];
    for (int i = 0; i < n; i++)
    {
        O[i] = new double[m];
    }

    // Example where pi[i] is same for every i
    for (int i = 0; i < n; i++)
    {
       pi[i] = 1.0 / n;
    }

    // Reading matrix T
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &T[i][j]);

    // Reading matrix O
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%lf", &O[i][j]);
}

void readY(vector<int> &y)
{
    freopen("y-array.txt", "r", stdin);

    y.clear();

    int len;
    scanf("%d", &len);
    for (int i = 0; i < len; i++)
        {
            int u;
            scanf("%d", &u);
            y.push_back(u);
        }
}

void writeAlphaCL(int n, int len, double **alpha, double *c, double L)
{
    printf("Alpha:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < len; j++)
            printf("%lf ", alpha[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("c:\n");
    for (int i = 0; i < len; i++)
    {
        printf("%lf ", c[i]);
    }
    printf("\n");

    printf("\nlog(L): %lf\n", L);
}

void writeBeta(int n, int len, double **beta)
{
    printf("\nBeta:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < len; j++)
            printf("%lf ", beta[i][j]);
        printf("\n");
    }
    printf("\n");
}

void writeViterbi(int len, vector<int> &vect)
{
    printf("vector (viterbi):\n");
    for (int i = 0; i < len; i++)
        printf("%d ", vect[i]);
    printf("\n");
}

void test()
{
    int n;
    int m;
    double *pi;
    double **T;
    double **O;
    readInitialParameters(n, m, pi, T, O);

    vector<int> y;
    readY(y);

    HMM *obj = new HMM(n, m, pi, T, O);

    // testing forward and backward
    pair<pair<double**, double*>, double> x = obj -> fwd(y);
    double** alpha = x.first.first;
    double* c = x.first.second;
    double L = x.second;
    double** beta = obj -> backward(y, c);
    writeAlphaCL(n, y.size(), alpha, c, L);
    writeBeta(n, y.size(), beta);

    // testing baumwelch
    int iter = MAX_ITER;
    double tolerance = MAX_TOL;
    obj -> baumwelch(y, iter, tolerance);
    obj -> writeParameters();

    // testing viterbi
    vector<int> vect = obj -> viterbi(y);
    writeViterbi(y.size(), vect);
}

int main()
{
    test();
    return 0;
}

