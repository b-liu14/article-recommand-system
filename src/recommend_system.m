%% Preprocess
% Add the current folder & subfolders to the top of the search path of MATLAB
pwdstr = pwd;
p = genpath(pwdstr);
addpath(p, '-begin');
savepath;
% clc; clear; close;

%% load origin data
% In this part, data was load and save with name test.mat
fprintf('load origin data\n');
origin_data = 'origin_data.mat';
if ~exist(origin_data, 'file')
    train_origin = load('tr.txt');
    test_origin = load('te.txt');
    ans_origin = load('an.txt');
    save(origin_data, 'train_origin', 'test_origin', 'ans_origin');
else
    load(origin_data);
end

%% process the data
% In this part, data was processed and num_user, num_article, ...
% train, test will be calculate.
fprintf('processe the data\n');
processed_data = 'processed_data.mat';
if ~exist(processed_data, 'file')
    num = max(train_origin);
    num_users = num(1);
    num_articles = num(2);
    train_processed = zeros(num_users, num_articles);
    for i = 1 : size(train_origin, 1)
        x = train_origin(i, 1);
        y = train_origin(i, 2);
        train_processed(x, y) = 1;
    end
    test_processed = zeros(num_users, 251);
    for i = 1:num_users
        for j = 1:251
            test_processed(i,j) = test_origin((i-1)*251+j,2);
        end
    end 
    ans_processed = zeros(num_users,num_articles);
    for i = 1:size(ans_origin,1)
        x = ans_origin(i,1);
        y = ans_origin(i,2);
        ans_processed(x,y) = 1;
    end
    save(processed_data,'test_processed', 'train_processed', 'num_users', 'num_articles', 'ans_processed');
else
    load(processed_data);
end

%% recommend article for users
% In this part, recommend algorithm was called and save with ...
% name r_usercf, r_itemcf, r_content
fprintf('recommend\n');
k = 5;
r_usercf  = UserCF_recommend ...   
   (train_processed, test_processed, num_users, num_articles, k);
% r_itemcf = ItemCF_recommend ...
%     (train_processed, test_processed, num_users, num_articles, k);
recommendation = r_usercf;

%% recommend test
% In this part, we will test the precision and recall of our system.
[precision, recall] = recommendation_test(recommendation, ans_processed,num_users, k);
% fprintf('recall = %f\n precision = %f\n', recall, precision);



