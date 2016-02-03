#include <vector>

class HMM
{
    private:
        int n; // number of nodes
        int m; // number of observations
        double *pi; // start-state probability vector
        double **T; // transition probability matrix
        double **O; // output probability matrix
    public:
        std::vector<int> viterbi(std::vector<int> &Y);
};
