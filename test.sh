# !/bin/bash

prefixs=("spec" "in")
suffixs=("" ".txt")



len=${#prefixs[@]}

for (( j = 0; j < $len; ++j)) 
do
    tmp1=${prefixs[j]}
    fname=$tmp1

for (( i = 1;i < 32; i++))
do
   
    fname=${prefixs[j]}$i${suffixs[j]}
    if [ ! -f testFile/$fname ];then
        continue
    fi

    echo "process testFile/$fname"
    ./HuffCompressor testFile/$fname
    if [ $? -ne 0 ];then
        echo $fname " not process success"
        exit 0
    fi
    is_same=`cmp list.nst testFile/$fname` 
    echo $is_same
    if [ -z is_same ];then
        echo "list.nst" "not same to" "testFile/$fname"
        exit 0
    fi

done
done
