function recommend = ItemCF_recommend(train, num_user, num_article, k)
    itemMat = zeros(num_article,num_article);
    userNumEachItem = zeros(num_article,1);
    
    %% calculate items' similarity
    itemEachUser = zeros(num_user,num_article);
    itemEachUserNum = zeros(num_user,1);
    for i = 1 : num_user
        for j = 1 : num_article
            if train(i,j) == 1
                userNumEachItem(j) = userNumEachItem(j) + 1;
                itemEachUserNum(i) = itemEachUserNum(i) + 1;
                itemEachUser(i,itemEachUserNum(i)) = j;
            end
        end
        for m = 1 : itemEachUserNum(i)
            for n = m + 1 : itemEachUserNum(i)
                itemMat(itemEachUser(i,m),itemEachUser(i,n)) = itemMat(itemEachUser(i,m),itemEachUser(i,n)) + 1;
            end
        end
    end         
    for i = 1 : num_article
        for j = 1 : num_article
            itemMat(i,j) = itemMat(i,j) / sqrt(userNumEachItem(i)*userNumEachItem(j));
            itemMat(j,i) = itemMat(i,j);
        end
    end
    %% save and load data
    save('itemInfo','itemMat','itemEachUser','itemEachUserNum','-v7.3');
    load('itemInfo');
    %% calculate the recommend items for each user
%     for i = 1:num_user
%         for j = 1:
end