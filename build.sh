if [[ "$1" == "debug" ]]; then
    clang++ -std=c++23 -g -O0 -Wall -Wextra -Wconversion -Wsign-conversion -Iinclude src/*.cpp -o hermes
else
    clang++ -std=c++23 -Wall -Wextra -Wconversion -Wsign-conversion -Iinclude src/*.cpp -o hermes
fi
