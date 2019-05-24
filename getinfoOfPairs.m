function info = getInfoOfPairs(tr_fea, tr_label, max_qua_level)

[m,n] = size(tr_fea);
info = [];
no_cls = length(unique(tr_label));
    
for i=1:n
    mi =  -inf;
    info(i).qua = 2;
    info(i).mi = 0;
    info(i).feature = i;

    for j=2:1:max_qua_level
                
        [QuantizedFeatures,df] = discritization2(tr_fea(:, i),j);
        
        [p12, p1, p2] = estpab(QuantizedFeatures,tr_label,j+1,no_cls+1);
        temp_mi = estmutualinfo(p12,p1,p2,df,no_cls,length(tr_label));
        
        if temp_mi>mi 

            mi = temp_mi;
            dof = (df-1)*(no_cls-1);
            if temp_mi > chi2inv(0.99,dof)/(2*m *log(2)) 

                info(i).qua = j;
                info(i).mi = temp_mi;
                info(i).feature = i;

                break;
             end
        end         
    end
end


