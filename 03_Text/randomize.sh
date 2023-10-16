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

input="$( echo "$input" | iconv -t ucs-2 - )"
#tput clear

echo "$input"

while read -r idx;
do
	arr=(${idx})
	#tput cup $idx
	#${arr[0]} $((${arr[1]} + 1))
	#echo "$((2*arr[1]))"
	hex=$(echo "$input" | head -n "${arr[0]}" -c "$((2*arr[1]))" - | tail -n 1 -c 2 - | hexdump | cut -c 9- | head -n 1 -c 4 - )
	echo $hex
	/bin/echo -e "\u$hex"
	#tput cup $LINES 0
	sleep 0.$1
done <<< ${chars}