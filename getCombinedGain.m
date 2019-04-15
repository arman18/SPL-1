function [gain, gain_send,newFea, newQua,quantization,s,comp,large_rel,th] = computeBestGain(tr_fea, tr_label, ...
    remainingFeatures, selectedFeatures, quantization, max_qua_level,newqua1)
th=10000 ;    
    gain = -inf;
    gain_send=-inf;
    newFea=[];
    newQua=[];
    s=1;
    large_rel=0;
   
        [m, n] = size(tr_fea);
    
            mm=m;

        
        for i=1:length(remainingFeatures)
        ids = [selectedFeatures remainingFeatures(i)];
        features = tr_fea(:, ids);
       
    if length(unique(tr_fea(:,remainingFeatures(i))))==1
        gain=-1000;
        gain_send=-1000;
        newFea = remainingFeatures(i);
        newQua=2;
        quantization = quantization;
        s=0;
        comp=0;
        large_rel=0;
        th=10000;
    break;
    end
    
jj=newqua1;

quantization1=quantization;



for j=1:1:2


ll=quantization1;
for kk=-1:1:1
if size(quantization1,2)==1
jj=jj+((-1)^kk)*j;

quant=quantization1+kk*j;
if quant<2
    quant=quantization1;
end
quantization1=quant+1;
end

jj=jj+kk*j;
if jj<2
    jj=newqua1;
end


dof11=0;
dof22=0;
dof33=0;
qq=[quantization1 jj];
dof123=0;
rddof = 0;
%                if size(features,2)==2
                [edges,distortion, idx1,temp_mi_with_class, QuantizedFeatures,I1,I2] = MI_random(features,tr_label,2,qq);
%                 QuantizedFeatures(:,1:2) = QuantizedFeatures1;
%                 Ia = I11;
%                else
%                 [edges,distortion, idx1,temp_mi_with_class, QuantizedFeatures2,I12,I2] = MI_random(features(:,end),tr_label,2,qq(:,end));
%                 QuantizedFeatures(:,size(features,2)) = QuantizedFeatures2;
%                 Ia =[Ia I12];
%                end
             no_cls=length(unique(tr_label));
             
             uv = unique(QuantizedFeatures(:,end));
%              I1=Ia;
             
             jj=length(unique(QuantizedFeatures(:,end))); 
             
             if size(QuantizedFeatures,2)==2
             first=length(unique(QuantizedFeatures(:,1))); 
             quantization1=first;
             end
n  = histc(QuantizedFeatures(:,end),uv);
if sum((n<1))>0
    b_c = size(uv,1) - sum((n<1));
else
    b_c = size(uv,1) ;
end

                 dof11=(no_cls-1)*(b_c-1);
            I1=I1(end)-(no_cls-1)*(b_c-1)/(2*m*log(2));

                redundancy = zeros(length(selectedFeatures), 1);
                complementary = zeros(length(selectedFeatures), 1);


                for pp = 1:length(selectedFeatures)
                    data = [tr_label QuantizedFeatures(:, pp) QuantizedFeatures(:, end)];
if length(unique( QuantizedFeatures(:, end)))==1
        gain=-1000;
        gain_send=-1000;
        newFea = remainingFeatures(i);
        newQua=2;
        quantization = quantization;
        s=0;
        comp=0;
        large_rel=0;
        th=10000;
    break;
    end
                    
 uv1 = unique(QuantizedFeatures(:,pp));
n  = histc(QuantizedFeatures(:,pp),uv1);
if sum((n<1))>0
    b_c_pp = size(uv1,1) - sum((n<1));
else
    b_c_pp  = size(uv1,1) ;
end


            barti_RD =  (b_c-1) * (b_c_pp- 1);


        prob = dimensionalize(data); 
        p_zxy = prob ./ sum(prob(:)); 
if length(size(p_zxy))~=3
    p_zxy;
end
        p_xy  = marginal(p_zxy, ~ofn(1, 2+1), 1); 
        
        p_xyz = dimensionalize( [QuantizedFeatures(:, pp) QuantizedFeatures(:, end) tr_label ]); % convert to freq table
row = size(p_xyz,1)-sum(sum(p_xyz==0,2)==size(p_xyz,2))-1;
column = size(p_xyz,2)-sum([(sum([p_xyz==0]))]==size(p_xyz,1))-1;
barti_CMP = sum(row.*column);

        red_new       =  interaction_info(p_xy) - barti_RD/(2*m*log(2));
        cond_red_new  =  interaction_info(p_zxy, 1)-barti_CMP/(2*m*log(2));

                     complementary(pp) = cond_red_new-red_new;
                     redundancy(pp) = red_new; 
         
                    

dof33=dof33 -barti_RD;   
rddof=rddof + barti_RD;

dof33=dof33 +  barti_CMP;

                end
                
                dof33 = (dof33 /length(selectedFeatures)); 
                dof123=dof11+dof33;
                
      


        gain11_I1 = I1(end);
        gain1_I1 = I1(end) + mean(complementary);
         gain22_I1  =  mean(complementary);


      

            now_temp =  mean(complementary);


chi11=(chi2inv(0.99,dof11))/(2*mm*log(2));

chi33=(chi2inv(0.99,dof33))/(2*mm*log(2));

clearvars rd_t com_t r_t ;
                      if  gain1_I1> gain
                    newFea = remainingFeatures(1);
                    newQua = jj;
                    s = std(complementary);
        gain = gain1_I1;
                 comp=now_temp;

chi123=chi2inv(0.99,dof123)/(2*mm*log(2)); 
chird=(chi2inv(0.99,rddof))/(2*mm*log(2));


if gain1_I1>chi123
    th=chi123;

     gain_send = gain1_I1;


elseif gain11_I1>chi11 & mean(redundancy)>chird

th= 10000;
gain_send = 0;
elseif gain11_I1>chi11 & mean(redundancy)<chird
th= chi11;
 gain_send = gain11_I1;


elseif gain11_I1<chi11 & mean(redundancy)<chird & gain22_I1>chi33
    th= chi33;

     gain_send = gain22_I1;


end                 
clearvars neg;
                else
clearvars neg1;
                         
                end

            end
    end
    quantization= quantization1;
end