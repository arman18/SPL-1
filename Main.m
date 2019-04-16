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

