function [precision, recall] = recommendation_test(recommendation, answer,num_user,k)
    precision = zeros(num_user,1);
    recall = zeros(num_user,1);
    for i = 1:num_user
        ans_all = sum(answer(i,:)==1);
        target = 0;
        for j = 1:k
            if answer(i,recommendation(i,j,2)) == 1
                target = target + 1;
            end
        end
        precision(i) = target/k;
        recall(i) = target/ans_all;
    end
end