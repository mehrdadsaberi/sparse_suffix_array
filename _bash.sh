
# random ordering
g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 3 1
g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 5 1
g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 10 1
g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 20 1
g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 40 1
g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 80 1

# lexicographical ordering
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 3 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 5 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 10 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 20 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 40 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 80 0

# non-sparse 
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 1 1 1

# random, k = 6
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 6 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 10 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 20 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 30 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 40 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 80 1

# lexi, k = 6
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 6 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 10 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 20 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 30 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 40 0
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 6 80 0

# using prefix lookup table
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 3 1 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 5 1 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 10 1 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 20 1 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 40 1 1
# g++ -std=c++17 suffix-array.cpp -o a.out && ./a.out 3 80 1 1
