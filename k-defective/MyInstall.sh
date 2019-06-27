
cd glpk
CFLAGS="-O2 -fPIC" ./configure --disable-shared --prefix=$(pwd)
make
make check
make install
