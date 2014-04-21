#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <queue>

using namespace std;

//The adjacent vertices
int **Em = NULL;
//The capacity matrix
int **Cm = NULL;
//Residual capacity
int **rc = NULL;
//capacity of path to node
int *cp = NULL;
//parents
int *pi = NULL;

int **new_matrix(int size) {
    int ix;
    int **m = NULL;

    //space for the rows
    m = (int**) malloc(size * sizeof(int*));
    //space for each column in the matrix
    for(ix = 0; ix < size; ++ix)
        m[ix] = (int*) calloc(size, sizeof(int));
    return m;

}

void free_matrix(int **matrix, int size) {
    int ix;
    for(ix = 0; ix < size; ++ix)
        free(matrix[ix]);
    free(matrix);
}

void fill_with_zeros(int **matrix, int size) {
    /* int i, j; */
    int i;
    for(i = 0; i < size; ++i)
        memset(matrix[i], 0, size * sizeof(int));
        /* for(j = 0; j < size; ++j) */
        /*     matrix[i][j] = 0; */
}

int EdmondsKarp(int **E, int **C, int s, int t, int n) {
    int sum = 0;
    int u, v, ix;
    //BFS queue
    queue<int> Q;
/* here: */
    while(true) {
        memset(pi, -1, sizeof(int)*n);
        pi[s] = s;
        cp[s] = INT_MAX;
        Q.push(s);
LOOP:
        while(!Q.empty()) {
            u = Q.front();
            Q.pop();
            for(ix = 0, v = E[u][ix]; (ix < n); ++ix, v = E[u][ix]) {
                // There is available capacity,
                // and v is not seen before in search
                if ((C[u][v] - rc[u][v] > 0) && (pi[v] == -1)) {
                    pi[v] = u;
                    cp[v] = min(cp[u], C[u][v] - rc[u][v]);
                    if (v != t)
                        Q.push(v);
                    else {
                        // Backtrack search, and write flow
                        while(pi[v] != v) {
                            u = pi[v];
                            rc[u][v] += cp[t];
                            rc[v][u] -= cp[t];
                            v = u;
                        }
                        goto LOOP;
                        /* break; */
                    }
                }
            }
        }
        if (pi[t] == -1) { // We did not find a path to t
            sum = 0;
            /* for (ix = 0, v = rc[s][ix]; ix < n; ++ix, v = rc[s][ix]) */
            for (ix = 0; ix < n; ++ix) {
                v = rc[s][ix];
                sum += v;
            }
            return sum;
        }
    }
}

int main(int argc, char const* argv[])
{
    int n(0), m(0), u(0), v(0), h(0), k(1);
    int ix, jx, kx, res = INT_MAX;
    //to hold the critical points
    int **critical = NULL;

    scanf("%d %d", &n, &m);

    /* Create the matrices so we can use them */
    Em = new_matrix(n);
    Cm = new_matrix(n);

    for(ix = 0; ix < m; ++ix) {
        scanf("%d %d", &u, &v);
        // add an edge that links u and v
        Em[u][v] = v;
        Em[v][u] = u;
        // set the weight of the edge
        Cm[u][v] = 1;
        Cm[v][u] = 1;

    }
    //the number of problems
    scanf("%d", &h);
    critical = (int**) malloc(h * sizeof(int*));
    for(ix = 0; ix < h; ++ix) {
        //the number of critical points
        scanf("%d ", &k);
        //Alloc k + 1 elements so we can save the number of critical points
        //for this problem in the first position
        critical[ix] = (int*) malloc((k+1) * sizeof(int));
        critical[ix][0] = k;
        for(jx = 1; jx <= k; ++jx) {
            scanf("%d ", &(critical[ix][jx]));
        }
    }

    rc = new_matrix(n);
    cp = (int*) calloc(n, sizeof(int));
    pi = (int*) calloc(n, sizeof(int));

    for(ix = 0; ix < h; ++ix) {
        for(jx = 1; jx <= critical[ix][0]; ++jx){
            for(kx = 1; kx <= critical[ix][0]; ++kx){
                if(jx != kx)
                    res = min(res, EdmondsKarp(Em, Cm, critical[ix][jx], critical[ix][kx], n));
            }
        }
        printf("%d\n", res);
        res = INT_MAX;
        fill_with_zeros(rc, n);
        memset(cp, 0, n*sizeof(int));
        /* we can free it now */
        free(critical[ix]);
    }
    /* free all the memory */
    free(critical);
    free(cp); free(pi);
    free_matrix(Em, n);
    free_matrix(Cm, n);
    free_matrix(rc, n);
    return 0;
}
