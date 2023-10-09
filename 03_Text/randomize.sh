#!/bin/sh

input=$(tee)

#echo $input | head -n 1 -c 1 - | tail -n 1 -c 1 -
#list of all chars
i=1
nl=
while read -r line;
do
	length=${#line};
	j=1;
	while [ $j -le ${length} ];
	do
		chars=$(/bin/echo -e "${chars}${nl}$i $j")
		j=$(($j + 1));
		nl="\n"
	done;
	i=$(($i + 1));
done <<< $input


chars=$(echo "${chars}" | shuf)
#echo "${chars}"

tput clear


while read -r idx;
do
	arr=(${idx});
	tput cup $idx;
	#${arr[0]} $((${arr[1]} + 1))
	#echo ${arr[0]};
	echo "$input" | head -n "${arr[0]}" -c "${arr[1]}" - | tail -n 1 -c 1 -;
	tput cup $LINES 0;
	sleep 0.$1;
done <<< ${chars}