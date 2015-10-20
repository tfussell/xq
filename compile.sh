export XLNT_DIR=/Users/thomas/Development/xlnt
clang++ -std=c++14 -I $XLNT_DIR/include -lxlnt -lminiz -L $XLNT_DIR/lib main.cpp -o xq