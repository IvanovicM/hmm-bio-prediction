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
        this -> O[i] = new double[m];
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
    int len = y.size();

    vector<int> vect(len);
    double** V = new double* [n];
    for (int i = 0; i < n; i++)
    {
        V[i] = new double[len];
    }
    double** x = new double* [n];
    for (int i = 0; i < n; i++)
    {
        x[i] = new double[len];
    }

    // Making V and x
    for (int i = 0; i < n; i++)
    {
        V[i][0] = log(pi[i]) + log(O[i][ y[0] ]);
    }

    for (int j = 1; j < len; j++)
    {
        for (int i = 0; i < n; i++)
        {
            V[i][j] = V[0][j - 1] + log(T[0][i]) + log(O[i][ y[j] ]);
            x[i][j] = 0;
            for (int l = 1; l < n; l++)
            {
                double p = V[l][j - 1] + log(T[l][i]) + log(O[i][ y[j] ]);
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
    double maxx = V[0][len - 1];
    vect[len - 1] = 0;
    for (int i = 1; i < n; i++)
    {
        if (V[i][len - 1] > maxx)
        {
            maxx = V[i][len - 1];
            vect[len - 1] = i;
        }
    }

    if (maxx < 0 && isinf(maxx))
    {
        for (int i = 0; i < len; i++)
        {
            vect[i] = -1;
        }
    }
    else
    {
        for (int i = len - 1; i > 0; i--)
        {
            vect[i - 1] = x[ vect[i] ][i];
        }
    }

    return vect;
}

pair< pair<double**, double*>, double> HMM::fwd(vector<int> &y)
{
    int len = y.size();

    double** alpha = new double* [n];
    for (int i = 0; i < n; i++)
    {
        alpha[i] = new double[len];
    }
    double* c = new double[len];
    double L = 0.0;

    // Making alpha and c
    double sumc = 0;
    for (int i = 0; i < n; i++)
    {
        alpha[i][0] = pi[i] * O[i][ y[0] ];
        sumc += alpha[i][0];
    }
    c[0] = 1.0 / sumc;
    for (int i = 0; i < n; i++)
    {
        alpha[i][0] = alpha[i][0] * c[0];
    }

    for (int t = 1; t < len; t++)
    {
        sumc = 0;
        for (int i = 0; i < n; i++)
        {
            double sum = 0;
            for (int k = 0; k < n; k++)
            {
                sum += alpha[k][t - 1] * T[k][i];
            }

            alpha[i][t] = O[i][ y[t] ] * sum;
            sumc += alpha[i][t];
        }

        c[t] = 1.0 / sumc;

        // Making alpha hat
        for (int i = 0; i < n; i++)
        {
            alpha[i][t] = alpha[i][t] * c[t];
        }
    }

    // calculating (log)L
    for (int t = 0; t < len; t++)
    {
        L -= log(c[t]);
    }

    return make_pair(make_pair(alpha, c), L);
}

double** HMM::backward(vector<int> &y, double* c)
{
    int len = y.size();

    double** beta = new double* [n];
    for (int i = 0; i < n; i++)
    {
        beta[i] = new double[len];
    }

    // Making beta
    for (int i = 0; i < n; i++)
        beta[i][len - 1] = 1.0;

    for (int t = len - 2; t >= 0; t--)
    {
        for (int i = 0; i < n; i++)
        {
            beta[i][t] = 0.0;
            for (int k = 0; k < n; k++)
            {
                beta[i][t]+= T[i][k] * O[k][ y[t + 1] ] * beta[k][t + 1];
            }

            // Making beta hat
            beta[i][t] = beta[i][t] * c[t + 1];
        }
    }

    return beta;
}

void HMM::baumwelch(vector<int> &y, int iter, double tolerance)
{
    int len = y.size();
    double L, old_L = 0.0;

    for (int itr = 0; itr < iter; itr++)
    {
        // Making next matrix O -> OO
        double** OO = new double* [n];
        for (int i = 0; i < n; i++)
        {
            OO[i] = new double[m];
        }

        // Run the forward and backward algorithm
        pair<pair<double**, double*>, double> x = fwd(y);
        double** alpha = x.first.first;
        double* c = x.first.second;
        L = x.second;
        double** beta = backward(y, c);

        for (int i = 0; i < n; i++)
        {
            // Calculating pi
            pi[i] = alpha[i][0] * beta[i][0];

            // Calculating common denominator
            // DD = sum_gamma
            double DD = 0.0;
            for (int t = 0; t < len - 1; t++)
            {
                DD+= alpha[i][t] * beta[i][t];
            }

            // Calculating TT
            // TT[i][j] = sum_xi / sum_gamma
            for (int j = 0; j < n; j++)
            {
                double NN = 0.0;
                for (int t = 0; t < len - 1; t++)
                {
                    NN+= c[t + 1] * alpha[i][t] * O[j][ y[t + 1] ] * beta[j][t + 1];
                }
                T[i][j] *= NN / DD;
            }

            // Calculating OO
            // OO[i][j] = sum_gamma*(y[t] == j) / sum_gamma
            for (int j = 0; j < m; j++)
                OO[i][j] = 0.0;

            DD+= alpha[i][len - 1] * beta[i][len - 1];
            for (int t = 0; t < len; t++)
            {
                OO[i][ y[t] ]+= alpha[i][t] * beta[i][t];
            }
            for (int j = 0; j < m; j++)
            {
                OO[i][j]/= DD;
            }
        }

        // Cleaning: alpha, beta, c, O
        for (int i = 0; i < n; i++)
        {
            delete[] alpha[i];
            delete[] beta[i];
        }
        delete[] alpha;
        delete[] beta;
        delete[] c;

        for (int i = 0; i < n; i++)
        {
            delete[] O[i];
        }
        delete[] O;
        O = OO;

        // Checking if the tolerance is satisfied
        if (fabs(old_L - L) < tolerance) break;
        old_L = L;
    }
}

void HMM::writeParameters()
{
    printf("pi:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%lf ", pi[i]);
    }
    printf("\n\n");

    printf("O:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%lf ", O[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("T:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%lf ", T[i][j]);
        printf("\n");
    }
    printf("\n");
}
