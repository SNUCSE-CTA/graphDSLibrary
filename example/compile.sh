#compile test program
g++ -std=c++11 -O3 main.cpp util.cpp ../libgraph.a ../libdag.a -o program
