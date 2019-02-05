#!/usr/bin/env bash

aWords="\<[aA]"			# Find words that being with A 
ddStart="\<[dD]"		# Find words beginning with D 
ddEnd="[dD]\>"			# Find words ending with D
alphaNum="[0-9][a-zA-Z]*$"	# Find words that are alphanumeric
num="^[0-9]*$"			# Find pure numbers in file
normWord="^[a-zA-Z]*$"      # Normal, all letter word


aCount=0		# Number of words starting with A
ddCount=0		# Number of words starting/ending with D
alphaNumCount=0		# Number of words that include both letters and numbers
numCount=0		# Number of totally numeric words

filename=${1}

lineCount=($(wc -l $filename)) #${a[0]}
wordCount=($(wc -w $filename)) #${a[1]}

# Translate punctuation to ' ', caps to lower, squeeze repeats, spaces to new lines
# Sort a-z, remove repeats, sort in reverse order, get first item
a=($(cat $filename | tr '[:punct:]' ' ' | tr 'A-Z' 'a-z' | tr -s ' ' | tr ' ' '\n' | sort | uniq -c | sort -k1,1nr -k2 | sed q))
mostCount=${a[0]}		# Number of times the most repetitive word is counted
mostWord=${a[1]}		# Most repetitve word

# Translate punctuation to ' ', caps to lower, squeeze repeats, spaces to new lines
# Sort a-z, remove repeats, sort in order, get first item
b=($(cat $filename | tr '[0-9]' ' ' | tr '[:punct:]' ' ' | tr 'A-Z' 'a-z' | tr -s ' ' | tr ' ' '\n' | sort | uniq -c | sort -k1,1n -k2 | sed '2 q'))
leastCount=${b[0]}		# Number of times the most repetitive word is counted
leastWord=${b[1]}		# Most repetitve word

while read -r line
do
   words=${line}
   #echo $words
   for word in $words
   do 
	if [[ $word =~ $aWords ]]; then
	    aCount=$((aCount+1))
	    #echo ' Matching A-word: ['$aCount'] '$word' --in '$words
	    #echo ''
	fi
	if [[ $word =~ $ddStart ]] && [[ $word =~ $ddEnd ]]; then
	    ddCount=$((ddCount+1))
	    #echo ' Matching DD-Word: ['$ddCount'] '$word' --in '$words
	    #echo '' 
	fi
	if [[ $word =~ $alphaNum ]] && ! [[ $word =~ $num ]]; then
	    alphaNumCount=$((alphaNumCount+1))
	    #echo 'Matching AlphaNumeric Word: ['$alphaNumCount'] '$word' --in '$words
	    #echo ''
	fi
	if [[ $word =~ $num ]]; then
	    numCount=$((numCount+1))
	    #echo 'Matching Pure Number: ['$numCount'] '$word' --in '$words
	    #echo ''
	fi
	
   done 
done < ${filename}
echo "Total lines in this file: $lineCount"
echo "Total words in this file: $wordCount" 
echo "Most repetitive word in the file is [$mostWord] counted [$mostCount] times."
echo "Least repretitve word in the files is [$leastWord] counted [$leastCount] times."
echo "Words beginning and ending with D: $ddCount"
echo "Words beginnng with A: $aCount"
echo "Total pure numbers: $numCount"
echo "Total alphanumeric words: $alphaNumCount"



b=($(cat $filename | tr '[0-9]' ' ' | tr '[:punct:]' ' ' | tr 'A-Z' 'a-z' | tr -s ' ' | tr ' ' '\n' | sort | uniq -c | sort -k1,1nr -k2 | head))

mNum=${b[0]}
mWord=${b[1]}

i=1
while ! [[ $mWord =~ $normWord ]]; do
  echo $mWord' START OF IF '$i
  i=$((i+1))
  
  mWord=${b[i]}
  if [[ $mWord =~ $normWord ]]; then
    mNum=${b[i-1]}
  fi
  echo 'Word: '$mWord 'Count: '$mNum

done

