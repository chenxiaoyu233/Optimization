
cd glpk
CFLAGS=-O2 ./configure --disable-shared --prefix=$(pwd)
make
make check
make install
