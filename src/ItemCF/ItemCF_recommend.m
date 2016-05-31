% recommend(num_user,k,2),the page 1 is the recommendation's value,
% the page 2 is the recommendation's serial number

function recommend = ItemCF_recommend(train, test, num_users, num_articles, k)
    itemMat = zeros(num_articles,num_articles);
    userNumEachItem = zeros(num_articles,1);
    
    %% calculate items' similarity
    itemEachUser = zeros(num_users,num_articles);
    itemEachUserNum = zeros(num_users,1);
    for i = 1 : num_users
        for j = 1 : num_articles
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
    for i = 1 : num_articles
        for j = 1 : num_articles
            if userNumEachItem(i) * userNumEachItem(j) ~= 0
                itemMat(i,j) = itemMat(i,j) / sqrt(userNumEachItem(i)*userNumEachItem(j));
            end
            itemMat(j,i) = itemMat(i,j);
        end
    end
    %% save and load data
    save('itemInfo','itemMat','itemEachUser','itemEachUserNum','-v7.3');
%     load('itemInfo');
    %% calculate the recommend items for each user
    recommend(:,:,1) = zeros(num_users,k);
    recommend(:,:,2) = zeros(num_users,k);
    for i = 1:num_users
        rank = zeros(num_articles,1);
        for j = 1:itemEachUserNum(i)
            [temp,ind] = sort(itemMat(itemEachUser(i,j),:),'descend');
            for m = 1:k
                if ismember(ind(m),itemEachUser(i,:)) 
                    continue;
                else
                    rank(ind(m)) = rank(ind(m)) + temp(m);
                end
            end
        end
        for j = 1:251
            rank(test(i,j)) = rank(test(i,j)) + 10;
        end 
        [a,loc] = sort(rank,'descend');
        recommend(i,:,1) = a(1:k);
        recommend(i,:,2) = loc(1:k);
    end        
end