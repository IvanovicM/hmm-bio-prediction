#include <bits/stdc++.h>
#include "../include/hmm.h"

using namespace std;

HMM::HMM (int n, int m) // constructor with random generated parameters
{
    this -> n = n;
    this -> m = m;

    // pi
    this -> pi = new double[n];
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        this -> pi[i] = 1 + rand() % 100;
        sum += this -> pi[i];
    }
    for (int i = 0; i < n; i++)
    {
        this -> pi[i] = this -> pi[i] / sum;
    }

    // T
    this -> T = new double*[n];
    for (int i = 0; i < n; i++)
    {
        this -> T[i] = new double[n];
        sum = 0;
        for (int j = 0; j < n; j++)
        {
            this -> T[i][j] = 1 + rand() % 100;
            sum += this -> T[i][j];
        }
        for (int j = 0; j < n; j++)
        {
            this -> T[i][j] = this -> T[i][j] / sum;
        }
    }

    // O
    this -> O = new double*[n];
    for (int i = 0; i < n; i++)
    {
        this -> O[i] = new double[n];
        sum = 0;
        for (int j = 0; j < m; j++)
        {
            this -> O[i][j] = 1 + rand() % 100;
            sum += this -> O[i][j];
        }
        for (int j = 0; j < m; j++)
        {
            this -> O[i][j] = this -> O[i][j] / sum;
        }
    }
}

HMM::HMM (int n, int m, double *pi, double **T, double **O) // constructor with given parameters
{
    this -> n = n;
    this -> m = m;

    // pi
    this -> pi = new double[n];
    for (int i = 0; i < n; i++)
    {
        this -> pi[i] = pi[i];
    }

    // T
    this -> T = new double*[n];
    for (int i = 0; i < n; i++)
    {
        this -> T[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            this -> T[i][j] = T[i][j];
        }
    }

    // O
    this -> O = new double*[n];
    for (int i = 0; i < n; i++)
    {
        this -> O[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            this -> O[i][j] = O[i][j];
        }
    }
}

HMM::~HMM () // destructor
{
    delete[] pi;

    for (int i = 0; i < n; i++)
    {
        delete[] T[i];
    }
    delete[] T;

    for (int i = 0; i < n; i++)
    {
        delete[] O[i];
    }
    delete[] O;
}

vector<int> HMM::viterbi(vector<int> &y)
{
    vector<int> vect(y.size());
    double** V = new double* [n];
    for (int i = 0; i < n; i++)
    {
        V[i] = new double[y.size()];
    }
    double** x = new double* [n];
    for (int i = 0; i < n; i++)
    {
        x[i] = new double[y.size()];
    }

    // Making V and x
    for (int i = 0; i < n; i++)
    {
        V[i][0] = log(pi[i]) + log(O[i][ y[0] ]);
    }

    for (int j = 1; j < y.size(); j++)
    {
        for (int i = 0; i < n; i++)
        {
            V[i][j] = V[0][j-1] + log(T[0][i]) + log(O[i][ y[j] ]);
            x[i][j] = 0;
            for (int l = 1; l < n; l++)
            {
                double p = V[l][j-1] + log(T[l][i]) + log(O[i][ y[j] ]);
                if (p > V[i][j])
                {
                    V[i][j] = p;
                    x[i][j] = l;
                }
            }
        }
    }

    // Finding the most probable sequence of conditions
    // Set of conditions if it is possible, -1 for each member, otherwise
    double maxx = V[0][y.size() - 1];
    vect[y.size() - 1] = 0;
    for (int i = 1; i < n; i++)
    {
        if (V[i][y.size() - 1] > maxx)
        {
            maxx = V[i][y.size() - 1];
            vect[y.size() - 1] = i;
        }
    }

    if (maxx < 0 && isinf(maxx))
    {
        for (int i = 0; i < y.size(); i++)
        {
            vect[i] = -1;
        }
    }
    else
    {
        for (int i = y.size() - 1; i > 0; i--)
        {
            vect[i - 1] = x[ vect[i] ][i];
        }
    }

    return vect;
}

pair< pair<double**, double*>, double> HMM::fwd(vector<int> &y)
{
    double** alpha = new double* [n];
    for (int i = 0; i < n; i++)
    {
        alpha[i] = new double[y.size()];
    }
    double* c = new double[y.size()];
    double L;

    // Making alpha and c
    double sumc = 0;
    for (int i = 0; i < n; i++)
    {
        alpha[i][0] = pi[i] * O[i][ y[0] ];
        sumc += alpha[i][0];
    }
    c[0] = 1.0 / sumc;

    for (int j = 1; j < y.size(); j++)
    {
        sumc = 0;
        for (int i = 0; i < n; i++)
        {
            double sum = 0;
            for (int k = 0; k < n; k++)
            {
                sum += alpha[k][j - 1] * T[k][i];
            }

            alpha[i][j] = O[i][ y[j] ] * sum;
            sumc += alpha[i][j];
        }

        c[j] = 1.0 / sumc;

        // Making alpha hat
        for (int i = 0; i < n; i++)
        {
            alpha[i][j] = alpha[i][j] * c[j];
        }
    }

    // calculating (log)L
    for (int i = 0; i < y.size(); i++)
    {
        L -= log(c[i]);
    }

    return make_pair(make_pair(alpha, c), L);
}

double** HMM::backward(vector<int> &y, double* c)
{
    double** beta = new double* [n];
    for (int i = 0; i < n; i++)
    {
        beta[i] = new double[y.size()];
    }

    // Making beta
    for (int i = 0; i < n; i++)
        beta[i][y.size() - 1] = 1;

    for (int j = y.size() - 2; j >= 0; j--)
    {
        for (int i = 0; i < n; i++)
        {
            beta[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                beta[i][j]+= T[i][k] * O[k][ y[j + 1] ] * beta[k][j + 1];
            }

            // Making beta hat
            beta[i][j] = beta[i][j] * c[j + 1];
        }
    }

    return beta;
}