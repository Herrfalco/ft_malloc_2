#!/bin/bash

mkdir correction
make -C rusage
cp rusage/rusage correction
make fclean -C rusage

cp script/ld_env.sh correction

make -C srcs
cp srcs/test* correction
rm -rf correction/*.c correction/*.o
make fclean -C srcs
