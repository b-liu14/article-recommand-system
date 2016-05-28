#!/bin/sh
train=../mf-train
predict=../mf-predict
recommend=../mf-recommend
test=../mf-test
model=model.txt
data_train=train.txt
data_test=test.txt
data_recommend=recommend.txt
data_result=result.txt

rm -f recommend.txt ocmf_output.txt ocmf_model.txt result.txt result_list.txt
for i in 32
do
	echo $i
	$train --quiet -r 0.1 -f 10 -l2 0.070 -k 32 -t 200 $data_train $model
	$recommend -k 5 -g -1 $data_test $model $data_recommend
	$test -k 5 -g -1 $data_test $model $data_recommend result_list.txt $data_result
done
