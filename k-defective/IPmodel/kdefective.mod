# vim: set filetype=ampl :
set V; # Vertex Set
param e {V, V}; # e[i, j] = 1 if (i, j) 是图中的一条边
param k; # k For k-defective
var z {V, V} >= 0;
var x {V} integer >= 0, <= 1;
maximize Maximum_KDefective: sum {i in V} x[i];
subject to KDfective: sum {i in V, j in V} e[i, j] * z[i, j] <= k;
subject to EachEdge {i in V, j in V}: z[i, j] >= x[i] + x[j] - 1;

