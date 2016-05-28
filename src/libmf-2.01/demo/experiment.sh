#!/bin/sh
train=../mf-train
predict=../mf-predict
data_train=train.txt
data_test=test.txt
##########################################################################
# One-class matrix factorization (OCMF)
##########################################################################
echo "------------------------------"
echo "one-class matrix factorization"
echo "------------------------------"
# In-memory training with holdout validation
$train -f 10 -l2 0.01 -k 32 -t 200 -p $data_test $data_train ocmf_model.txt
# Do prediction and show row-oriented AUC
$predict -e 6 $data_test ocmf_model.txt ocmf_output.txt
