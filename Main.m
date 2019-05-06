clear all; close all; clc;


addpath('Libsvm/matlab');   % Libsvm package is used
addpath('D:\MI_FINAL\mDSMwithout-MI-editing\mi');

cc = power(2,-5);
number_neighbours=5;


%paramteres
max_qua_level = 50;
no_of_fold=10;
ts_num_max = 5000;
size_features = 0;
LOO = 0;
stability = 0;
similarity_ratio = 0;
ss=0;
c_acc =0;
% ------1------
% nclass = 6;
% clabel = [1 2 3 4 5 6];
% data = dlmread('dermatology_formatted.txt');
% file = 'dermatology_formatted.txt';
% [pathstr,name,ext] = fileparts(file);
% -------2----- 
% nclass = 2;
% clabel = [1 2];
% data = dlmread('sonar data lebel first10fold.txt');
% file = 'sonar data lebel first10fold.txt';
% [pathstr,name,ext] = fileparts(file);
% -------3-----
% nclass = 6;
% clabel = [1 2 3 4 5 6];
% data = dlmread('glass.txt');
% file = 'glass.txt';
% [pathstr,name,ext] = fileparts(file);
% ------4------
% dim=13 ;%
% nclass = 3;
% clabel = [1 2 3];
% data = dlmread('wine.data');
% file = 'wine.txt';
% [pathstr,name,ext] = fileparts(file);
% -------5-----
% dim=14 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('australian.dat');
% file = 'australian.txt';
% [pathstr,name,ext] = fileparts(file);
% ------6------
% dim=9;
% nclass = 6;
% clabel = [1 2 3 4 5 6];
% data = dlmread('BreastTissue.txt');
% file = 'BreastTissue.txt';
% [pathstr,name,ext] = fileparts(file);
% ------7------
% dim=8 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('pima-indians-diabetes.data');
% file = 'pima-indians-diabetes.txt';
% [pathstr,name,ext] = fileparts(file);
% -----8-------
% dim=34 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('ionosphere.data');
% file = 'ionosphere.txt';
% [pathstr,name,ext] = fileparts(file);
% ------9------
% dim=8 ;
% nclass = 10;
% clabel = [1 2 3 4 5 6 7 8 9 10];
% data = dlmread('yeast.data');
% file = 'yeast.txt';
% [pathstr,name,ext] = fileparts(file);
% ------10------
dim=90 ;
nclass = 15;
clabel = [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15];
data = dlmread('movement_libras.data');
file = 'movement_libras.txt';
[pathstr,name,ext] = fileparts(file);
% ------11------
% dim=4 ;
% nclass = 3;
% clabel = [1 2 3];
% data = dlmread('iris.data');
% file = 'iris.txt';
% [pathstr,name,ext] = fileparts(file);
% ------12------
% dim=22 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('Parkinsons.txt');
% file = 'Parkinsons.txt';
% [pathstr,name,ext] = fileparts(file);
% -----13-------
% dim=20 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('German.txt');
% file = 'German.txt';
% [pathstr,name,ext] = fileparts(file);
% -----14-------
% dim=278 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('arrhythmia_formatted.txt');
% file = 'arrhythmia_formatted.txt';
% [pathstr,name,ext] = fileparts(file);
% -----15-------
% dim=6 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('Liver.txt');
% file = 'Liver.txt';
% [pathstr,name,ext] = fileparts(file);
% -----16-------
% dim=56 ;
% nclass = 3;
% clabel = [1 2 3];
% data = dlmread('Lung_Cancer.txt');
% file = 'Lung_Cancer.txt';
% [pathstr,name,ext] = fileparts(file);
% -----17-------
% dim=9703 ;
% nclass = 9;
% clabel = [1 2 3 4 5 6 7 8 9];
% data = dlmread('nci.txt');
% file = 'nci.txt';
% [pathstr,name,ext] = fileparts(file);
% -----18-------
% dim=57 ;
% nclass = 2;
% clabel = [1 2];
% data = dlmread('Spambase.txt');
% file = 'Spambase.txt';
% [pathstr,name,ext] = fileparts(file);
% -----19-------
% nclass = 2;
% clabel = [1 2];
% data = dlmread('breast.txt');
% file = 'breast.txt';
% [pathstr,name,ext] = fileparts(file);
% -----20-------
% nclass = 2;
% clabel = [1 2];
% data = dlmread('heart.txt');
% file = 'heart.txt';
% [pathstr,name,ext] = fileparts(file);
% -----21-------
% nclass = 2;
% clabel = [1 2];
% data = dlmread('madelon.txt');
% file = 'madelon.txt';
% [pathstr,name,ext] = fileparts(file);
% -----22-------
% nclass = 7;
% clabel = [1 2 3 4 5 6 7];
% data = dlmread('steel.txt');
% file = 'steel.txt';
% [pathstr,name,ext] = fileparts(file);
% -----23-------
% nclass = 10;
% clabel = [1 2 3 4 5 6 7 8 9 10];
% data = dlmread('Semeion.txt');
% file = 'Semeion.txt';
% [pathstr,name,ext] = fileparts(file);
% ------24------
% nclass = 3;
% clabel = [1 2 3];
% data = dlmread('waveform.txt');
% file = 'waveform.txt';
% [pathstr,name,ext] = fileparts(file);
% ------25------
% nclass = 2;%LOO
% clabel = [1 2];
% data = dlmread('dbworld_subjects.txt');
% file = 'dbworld_subjects.txt';
% [pathstr,name,ext] = fileparts(file);
% -----26-------
% nclass = 2; %LOO
% clabel = [1 2];
% data = dlmread('dbworld_bodies.txt');
% file = 'dbworld_bodies.txt';
% [pathstr,name,ext] = fileparts(file);
% ----27--------
% nclass = 10;
% clabel = [1 2 3 4 5 6 7 8 9 10];
% data = dlmread('Cardio.txt');
% file = 'Cardio.txt';
% [pathstr,name,ext] = fileparts(file);
% -----28-------
% nclass = 9; %LOO
% clabel = [1 2 3 4 5 6 7 8 9];
% data = dlmread('lymphoma.txt');
% file = 'lymphoma.txt';
% [pathstr,name,ext] = fileparts(file);
% -----29-------
% nclass = 7;
% clabel = [1 2 3 4 5 6 7];
% data = dlmread('Segment.txt');
% file = 'Segment.txt';
% [pathstr,name,ext] = fileparts(file);
% ------30------
% nclass = 2;
% clabel = [1 2];
% data = dlmread('musk.txt');
% file = 'musk.txt';
% [pathstr,name,ext] = fileparts(file);
% -----31-------
% nclass = 2; %LOO
% clabel = [1 2];
% data = dlmread('leukemia.txt');
% file = 'leukemia.txt';
% [pathstr,name,ext] = fileparts(file);
% -----32-------
% nclass = 2; %LOO
% clabel = [1 2];
% data = dlmread('colon.txt');
% file = 'colon.txt';
% [pathstr,name,ext] = fileparts(file);
% -----33-------
% nclass = 5;
% clabel = [1 2 3 4 5];
% data = dlmread('Lung.txt');
% file = 'Lung.txt';
% [pathstr,name,ext] = fileparts(file);
%--------complete data segment-----------

fid = fopen('result_new.txt', 'a');
fprintf(fid,'\nDataset: %s\n', name);
fclose(fid);
xa = data(:, 2:end);
X_tr=xa;
% tic
count=1;
for ii=1:size(xa,2)
    if length(unique(xa(:,ii)))==1
        temp1(count)=ii;
        count=count+1;
    end
end

if count>1
    othersData(:,temp1)=[];
end
clearvars temp1 count ;

classData = data(:, 1);
[m, dim] = size(othersData);

opts= struct;
opts.att_1split= 2;
opts.quaztization_level= 5;
opts.dim= dim;

if LOO==1
    acc=0;
    
    
    for i = 1 : size(othersData,1)
        ind=1:size(othersData,1);
        ind=ind(ind~=i);
        tr_fea = othersData(ind,:);
        tr_label = classData(ind,:);
        ts_fea = othersData(i,:);
        ts_label = classData(i,:);
        
        [selectedFeatures, qua]= selectFeatures(tr_fea, tr_label, max_qua_level);
        aa{i}=[selectedFeatures ; qua];
        
        sorted_fea = sort(selectedFeatures,2);
        featidx(i,:)=sorted_fea(1:1);
        
         newtr_fea = zeros((size(othersData,1)-1), size(selectedFeatures,2));
        newts_fea = zeros(1,size(selectedFeatures,2));
        
        for j = 1:length(selectedFeatures)
            [newtr_fea(:, j), edges] = nowquantizeMI_random(tr_fea(:, selectedFeatures(j)), qua(j));
            newts_fea(:, j) = testquantizeMI_random(ts_fea(:, selectedFeatures(j)), edges);
        end
        
        tr_fea=newtr_fea;
        ts_fea= newts_fea;
