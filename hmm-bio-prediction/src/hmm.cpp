#include <bits/stdc++.h>
#include "../include/hmm.h"

using namespace std;

HMM::HMM () // constructor with random generated parameters
{
    this -> n = 133;
    this -> m = 20;

    // tags
    /*
    1 - i (in)
    2 - m (membrane)
    3 - o (out)
    */
    this -> tags = new int[n];
    int k = 0;
    // inloop 1-10 - i
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 1;
    // inglob - i
    this -> tags[k++] = 1;
    // inloop 11-20 - i
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 1;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;
    // helix - m
    for (int i = 0; i < 25; i++)
        this -> tags[k++] = 2;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;
    // short loop 1-10 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // outglob - o
    this -> tags[k++] = 3;
    // short loop 11-20 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // long loop 1-10 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // out glob - o
    this -> tags[k++] = 3;
    // long loop 11-20 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;
    // helix - m
    for (int i = 0; i < 25; i++)
        this -> tags[k++] = 2;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;

    // pi; always starts with a loop
    this -> pi = new double[n];
    for (int i = 0; i < n; i++)
    {
        this -> pi[i] = 0.0;
    }
    pi[0] = 1 + rand() % 100;
    pi[56] = 1 + rand() % 100;
    pi[77] = 1 + rand() % 100;
    double sum = pi[0] + pi[56] + pi[77];
    pi[0] /= sum;
    pi[56] /= sum;
    pi[77] /= sum;

    // T; it is known in advance which field of matrix is 0
    this -> T = new double*[n];
    for (int i = 0; i < n; i++)
    {
        this -> T[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            this -> T[i][j] = 0.0;
        }
        this -> T[i][(i+1)%n] = 1 + rand() % 100;
    }
    this -> T[76][98] = this -> T[76][77];
    this -> T[76][77] = 0.0;
    this -> T[55][77] = 1 + rand() % 100;

    // inloop 1-10 - i
    for (int i = 0; i < 10; i++)
    {
        this -> T[i][20 - i] = 1 + rand() % 100;
        this -> T[i][21 - i] = 1 + rand() % 100;
    }
    // inglob - i
    this -> T[10][10] = 1 + rand() % 100;
    // inloop 11-20 - i
    // cap - m
    // helix - m
    for (int i = 30; i <= 49; i++)
    {
        this -> T[28][i] = 1 + rand() % 100;
    }
    // cap - m
    // short loop 1-10 - o
    for (int i = 0; i < 10; i++)
    {
        this -> T[i + 56][56 + 20 - i] = 1 + rand() % 100;
        this -> T[i + 56][56 + 21 - i] = 1 + rand() % 100;
    }
    this -> T[56][98] = this -> T[56][77];
    this -> T[56][77] = 0.0;
    // outglob - o
    this -> T[66][66] = 1 + rand() % 100;
    // short loop 11-20 - o
    // long loop 1-10 - o
    // out glob - o
    this -> T[87][87] = 1 + rand() % 100;
    // long loop 11-20 - o
    // cap - m
    // helix - m
    for (int i = 107; i <= 126; i++)
    {
        this -> T[105][i] = 1 + rand() % 100;
    }
    // cap - m

    sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        sum = 0.0;
        for (int j = 0; j < n; j++)
            sum+= this -> T[i][j];
        for (int j = 0; j < n; j++)
            this -> T[i][j] /= sum;
    }

    // O
    this -> O = new double*[n];
    for (int i = 0; i < n; i++)
    {
        this -> O[i] = new double[m];
        sum = 0.0;
        for (int j = 0; j < m; j++)
        {
            this -> O[i][j] = 1 + rand() % 100;
            sum += this -> O[i][j];
        }
        for (int j = 0; j < m; j++)
        {
            this -> O[i][j] /= sum;
        }
    }
}

HMM::HMM (double *pi, double **T, double **O) // constructor with given parameters
{
    this -> n = 133;
    this -> m = 20;

    // tags
    /*
    1 - i (in)
    2 - m (membrane)
    3 - o (out)
    */
    this -> tags = new int[n];
    int k = 0;
    // inloop 1-10 - i
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 1;
    // inglob - i
    this -> tags[k++] = 1;
    // inloop 11-20 - i
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 1;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;
    // helix - m
    for (int i = 0; i < 25; i++)
        this -> tags[k++] = 2;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;
    // short loop 1-10 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // outglob - o
    this -> tags[k++] = 3;
    // short loop 11-20 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // long loop 1-10 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // out glob - o
    this -> tags[k++] = 3;
    // long loop 11-20 - o
    for (int i = 0; i < 10; i++)
        this -> tags[k++] = 3;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;
    // helix - m
    for (int i = 0; i < 25; i++)
        this -> tags[k++] = 2;
    // cap - m
    for (int i = 0; i < 5; i++)
        this -> tags[k++] = 2;

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

bool HMM::match(int a, int b)
{
    if (a == 0) // a == .
        return true;
    return (a == b);
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

pair< pair<double**, double*>, double> HMM::fwd(vector< pair<int, int> > &y)
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
        if (match(y[0].second, tags[i]))
            alpha[i][0] = pi[i] * O[i][ y[0].first ];
        else
            alpha[i][0] = 0;

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

            if (match(y[t].second, tags[i]))
                alpha[i][t] = O[i][ y[t].first ] * sum;
            else
                alpha[i][t] = 0;

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

double** HMM::backward(vector< pair<int, int> > &y, double* c)
{
    int len = y.size();

    double** beta = new double* [n];
    for (int i = 0; i < n; i++)
    {
        beta[i] = new double[len];
    }

    // Making beta
    for (int i = 0; i < n; i++)
    {
        if (match(y[len - 1].second, tags[i]))
            beta[i][len - 1] = 1.0;
        else
            beta[i][len - 1] = 0.0;
    }

    for (int t = len - 2; t >= 0; t--)
    {
        for (int i = 0; i < n; i++)
        {
            beta[i][t] = 0.0;

            if (match(y[t].second, tags[i]))
            {
                for (int k = 0; k < n; k++)
                {
                    beta[i][t]+= T[i][k] * O[k][ y[t + 1].first ] * beta[k][t + 1];
                }
            }

            // Making beta hat
            beta[i][t] = beta[i][t] * c[t + 1];
        }
    }

    return beta;
}

void HMM::baumwelch(vector< pair<int, int> > &y, int iterations, double tolerance)
{
    int len = y.size();
    double L, old_L = 0.0;

    for (int itr = 0; itr < iterations; itr++)
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
                    NN+= c[t + 1] * alpha[i][t] * O[j][ y[t + 1].first ] * beta[j][t + 1];
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
                OO[i][ y[t].first ]+= alpha[i][t] * beta[i][t];
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
        printf("%lf", pi[i]);
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
