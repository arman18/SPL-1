function [selectedFeatures, selectedQuantization]= selectFeatures(tr_fea, tr_label, max_qua_level)
[m, n] = size(tr_fea);

info = getInfoOfPairs(tr_fea, tr_label, max_qua_level);

selected = [];
sortedInfo = sortStructArray(info);
selected.Features = sortedInfo(1).feature;
selected.Quantization = sortedInfo(1).qua;
selected.QuantizedFeatures = [];
selected.I = [];
selected.DF = [];
remainingFeatures = setdiff(1:n, selected.Features);
count=1;
to = length(remainingFeatures);
for i=2:to    
    [g,threshold, selected,new] = getCombinedGain(tr_fea, tr_label,sortedInfo(i).feature,sortedInfo(i).qua, selected); 

    if g > threshold
        selected.Quantization = [selected.Quantization new.Qua];
        selected.Features = [selected.Features sortedInfo(i).feature]; 
        selected.QuantizedFeatures = [selected.QuantizedFeatures new.QuantizedFeature];
        selected.I = [selected.I new.I];
        selected.DF = [selected.DF new.DF];  
        count =  count + 1;  
    end
     remainingFeatures = setdiff(remainingFeatures,sortedInfo(i).feature);
end

selectedFeatures = selected.Features;
selectedQuantization = selected.Quantization;
end
