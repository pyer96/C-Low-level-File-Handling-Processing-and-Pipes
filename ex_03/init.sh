#!/usr/bin/bash

for i in $(ls .);
do
 if [[ -f $i ]] &&  [[ -x $i ]] && [[ ! $i == *.sh ]]; then
	rm $i
elif [[ $i ==  *.c ]];then
	gcc $i -o ${i%.c} -Wall -Werror -fsanitize=leak
 fi
done
