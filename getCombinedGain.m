function [gain_send,th,selected,new] = getCombinedGain(tr_fea, tr_label, remainingFeatures, newqua1, selected)

th=10000 ;    
gain = -inf;
gain_send=-inf;
[m, n] = size(tr_fea);
no_cls=length(unique(tr_label));
new = [];
mm=m;

ids = [selected.Features remainingFeatures];
features = tr_fea(:, ids);
if length(unique(tr_fea(:,remainingFeatures)))==1
    gain_send=-1000;
    th=10000;
    return;
end

jj=newqua1;

for j=1:1:2   
    for kk=-1:1:1
        if size(selected.Quantization,2)==1
            jj=jj+((-1)^kk)*j;

            quant=selected.Quantization+kk*j;
            if quant<2
                quant=selected.Quantization;
            end
            selected.Quantization=quant+1;
        end
        jj=jj+kk*j;
        if jj<2
            jj=newqua1;
        end
        dof33=0;
        qq=[selected.Quantization jj];
        rddof = 0;
        if size(features,2)==2
            [selected.QuantizedFeatures,selected.DF] = discritization2(features(:,1),qq(:,1));
            [p12, p1, p2] = estpab(selected.QuantizedFeatures,tr_label);
            selected.I = estmutualinfo(p12,p1,p2,selected.DF,no_cls,length(tr_label));
            
            [QuantizedFeature,DF] = discritization2(features(:,2),qq(:,2));
            [p12, p1, p2] = estpab(QuantizedFeature,tr_label);
            I = estmutualinfo(p12,p1,p2,DF,no_cls,length(tr_label));
            
        else

            [QuantizedFeature,DF] = discritization2(features(:,end),qq(:,end)); 
            [p12, p1, p2] = estpab(QuantizedFeature,tr_label);
            I = estmutualinfo(p12,p1,p2,DF,no_cls,length(tr_label));
        end
%         jj=DF; 
        
        dof11=(no_cls-1)*(DF-1);
        redundancy = 0;
        complementary = 0;


        for pp = 1:length(selected.Features)
            if DF==1
                gain=-1000;
                gain_send=-1000; 
                th=10000;
                break;
            end                
            barti_RD =  (DF-1) * (selected.DF(pp)- 1);
            barti_CMP = (barti_RD*no_cls);
            
            [p12, p1, p2] = estpab(QuantizedFeature,selected.QuantizedFeatures(:,pp));

            red_new     = estmutualinfo(p12,p1,p2,DF,selected.DF(pp),length(tr_label));

            newcondvec_xz = mergemultivariables(selected.QuantizedFeatures(:, pp),tr_label);
            cond_red_new  =  condentropy(QuantizedFeature,tr_label) - condentropy(QuantizedFeature,newcondvec_xz);
            cond_red_new  = cond_red_new - barti_CMP/(2*m*log(2));
            complementary(pp) = cond_red_new-red_new; 
            redundancy(pp) = red_new; 

            dof33=dof33 -barti_RD; 
            rddof=rddof + barti_RD;
            dof33=dof33 +  barti_CMP; 
        end
        dof33 = (dof33 /length(selected.Features)); 
        dof123=dof11+dof33;
        gain11_I1 = I;
        gain1_I1 = I + 1/(size(selected.Features,2))*complementary;
        gain22_I1  =  mean(complementary);
        chi11=(chi2inv(0.99,dof11))/(2*mm*log(2));
        chi33=(chi2inv(0.99,dof33))/(2*mm*log(2));

        clearvars rd_t com_t r_t ;
        if  gain1_I1> gain
            gain = gain1_I1;
            new.Qua = jj;
            new.I = I;
            new.DF = DF;
            new.QuantizedFeature = QuantizedFeature;
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