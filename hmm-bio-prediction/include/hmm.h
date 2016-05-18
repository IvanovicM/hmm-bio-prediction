#include <bits/stdc++.h>

class HMM
{
    private:
        int n; // number of nodes
        int m; // number of observations
        double *pi; // start-state probability vector
        double **T; // transition probability matrix
        double **O; // output probability matrix
        int *tags; // state tags

    public:
        HMM();
        HMM (double *pi, double **T, double **O);
        ~HMM();

        bool match(int a, int b);
        std::vector<int> viterbi(std::vector<int> &y);
        std::pair< std::pair<double**, double*>, double> fwd(std::vector< std::pair<int, int> > &y);
        double** backward(std::vector< std::pair<int, int> > &y, double* c);
        void baumwelch(std::vector< std::pair<int, int> > &y, int iterations, double tolerance);

        void writeParameters();
};
