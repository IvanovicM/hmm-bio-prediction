#include <bits/stdc++.h>
#include "../include/hmm.h"

#define MAX_ITER 100
#define MAX_TOL 0.000001

using namespace std;

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

void test()
{
    // testing hmm-bio-prediction parameters
    HMM *obj = new HMM();
    obj -> writeParameters();
}

int main()
{
    test();
    return 0;
}
