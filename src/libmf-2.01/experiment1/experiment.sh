#!/bin/sh
train=../mf-train
predict=../mf-predict

rm -f model.txt
$train -r 0.1 -f 10 -l2 0.070 -k 32 -t 200 train.txt model.txt
$predict test.txt model.txt output.txt
