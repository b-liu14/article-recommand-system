clc; clear;
load('origin_data.mat');
m = size(test_origin, 1);
test = [test_origin ones(m, 1)];
dlmwrite('test.txt', test, ' ');

m = size(train_origin, 1);
train = [train_origin ones(m, 1)];
dlmwrite('train.txt', train, ' ');