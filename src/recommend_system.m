% clc; clear; close;

%% load data
% In this part, data was load and save with name test.mat
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
processed_data = 'processed_data.mat';
if ~exist(processed_data, 'file')
    num = max(train_origin);
    num_user = num(1);
    num_article = num(2);
    train_processed = zeros(num_user, num_article);
    for i = 1 : size(train_origin, 1)
        x = train_origin(i, 1);
        y = train_origin(i, 2);
        train_processed(x, y) = 1;
    end
    test_processed = zeros(num_user, num_article);
    for i = 1 : size(test_origin, 1)
        x = test_origin(i, 1);
        y = test_origin(i, 2);
        test_processed(x, y) = 1;
    end
    save(processed_data, 'test_processed', 'train_processed');
else
    load(processed_data);
end

%% recommend article for users
% In this part, recommend algorithm was called and save with ...
% name r_usercf, r_itemcf, r_content
k = 10;
r_usercf  = UserCF_recommend     (train, num_user, num_article, k);
r_item_cf = ItemCF_recommend     (train, num_user, num_article, k);
r_content = ItemContent_recommend(train, num_user, num_article, k);


