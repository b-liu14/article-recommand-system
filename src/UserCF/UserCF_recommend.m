% recommend(num_user,k,2),the page 1 is the recommendation's value,
% the page 2 is the recommendation's serial number
function recommend = UserCF_recommend(train, num_user, num_article, k)
%     userMat = zeros(num_user,num_user);
%     userNumEachItem = zeros(num_article,1);
%     itemEachUser = zeros(num_user,num_article);
    %% calculate items' similarity
%     userEachItem = zeros(num_article,num_user);
%     itemNumEachUser = zeros(num_user,1);
%     for i = 1 : num_article
%         for j = 1 : num_user
%             if train(j,i) == 1
%                 userNumEachItem(i) = userNumEachItem(i) + 1;
%                 itemNumEachUser(j) = itemNumEachUser(j) + 1;
%                 userEachItem(i,userNumEachItem(i)) = j;
%                 itemEachUser(j,itemNumEachUser(j)) = i;
%             end
%         end
%         for m = 1 : userNumEachItem(i)
%             for n = m + 1 : userNumEachItem(i)
%                 userMat(userEachItem(i,m),userEachItem(i,n)) = userMat(userEachItem(i,m),userEachItem(i,n)) + 1;
%             end
%         end
%     end         
%     for i = 1 : num_user
%         for j = 1 : num_user
%             if itemNumEachUser(i) * itemNumEachUser(j) ~= 0
%                 userMat(i,j) = userMat(i,j) / sqrt(itemNumEachUser(i)*itemNumEachUser(j));
%             end
%             userMat(j,i) = userMat(i,j);
%         end
%     end
    %% save and load data
%     save('userInfo','userMat','itemEachUser','itemNumEachUser','-v7.3');
    load('userInfo');
    %% calculate the recommend items for each user
    recommend(:,:,1) = zeros(num_user,k);
    recommend(:,:,2) = zeros(num_user,k);
    for i = 1:num_user
        rank = zeros(num_article,1);
        [temp,ind] = sort(userMat(i,:),'descend');
        for m = 1:k
            for j = 1:itemNumEachUser(ind(m))               
                if ismember(itemEachUser(ind(m),j),itemEachUser(i,:)) 
                    continue;
                else
                    rank(itemEachUser(ind(m),j)) = rank(itemEachUser(ind(m),j)) + temp(m);
                end
            end
        end
        [a,loc] = sort(rank,'descend');
        recommend(i,:,1) = a(1:k);
        recommend(i,:,2) = loc(1:k);
    end        
end