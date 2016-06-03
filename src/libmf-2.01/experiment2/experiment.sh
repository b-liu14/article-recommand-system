#!/bin/sh
train=../mf-train
predict=../mf-predict
test=../mf-test

for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1
do
	echo $i
	rm -f model.txt resultlist.txt result.txt recommend.txt
	$train --quiet -r $i -f 10 -l2 0.070 -k 1024 -t 2000 tr.txt model.txt
	$predict te.txt model.txt recommend.txt
	$test -g -1 an.txt model.txt recommend.txt resultlist.txt result.txt
done