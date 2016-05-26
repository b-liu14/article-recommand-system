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
    data_path = '../data/';
    train_path = strcat(data_path, 'user-info-train.txt');
    test_path = strcat(data_path, 'user-info-test.txt');
    train_origin = load(train_path);
    test_origin = load(test_path);
    save(origin_data, 'train_origin', 'test_origin');
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
    test_processed = zeros(num_users, num_articles);
    for i = 1 : size(test_origin, 1)
        x = test_origin(i, 1);
        y = test_origin(i, 2);
        test_processed(x, y) = 1;
    end
    save(processed_data, ...
        'test_processed', 'train_processed', 'num_users', 'num_articles');
else
    load(processed_data);
end

%% recommend article for users
% In this part, recommend algorithm was called and save with ...
% name r_usercf, r_itemcf, r_content
fprintf('recommend\n');
k = 10;
%r_usercf  = xUserCF_recommend ...   
%    (train_processed, num_users, num_articles, k);
r_itemcf = ItemCF_recommend ...
    (train_processed, num_users, num_articles, k);
r_content = ItemContent_recommend...
    (train_processed, num_users, num_articles, k);
% TODO: sort the recommendation by the weight.
recommendation = ...
    [r_usercf(:, :, 1); r_itemcf(:, :, 1); r_content(:, :, 1)];

%% recommend test
% In this part, we will test the precision and recall of our system.
[recall, precision] = recommendation_test(recommendation, test_processed);
fprintf('recall = %f\n precision = %f\n', recall, precision);



