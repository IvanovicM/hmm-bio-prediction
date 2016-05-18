#include <bits/stdc++.h>

class HMM
{
    private:
        int n; // number of nodes
        int m; // number of observations
        double *pi; // start-state probability vector
        double **T; // transition probability matrix
        double **O; // output probability matrix

    public:
        HMM(int n, int m);
        HMM (int n, int m, double *pi, double **T, double **O);
        ~HMM();

        std::vector<int> viterbi(std::vector<int> &y);
        std::pair< std::pair<double**, double*>, double> fwd(std::vector<int> &y);
        double** backward(std::vector<int> &y, double* c);
        void baumwelch(std::vector<int> &y, int iter, double tolerance);

        void writeParameters();
};
