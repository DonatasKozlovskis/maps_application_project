% clc;
% allDependPath = genpath('dependencies');
% addpath(allDependPath);
% clear;
% % 
% load data
% load('allRoads.mat');
% load('nodesDataset.mat');
% load('sparseMatrices.mat');
% 
% allRoads.nodesInRoads;

[foundRoadIds, foundRoadNames ]= allRoads.nodePairInRoad([2533 2527 2514 2499 906 835 923 3 4 939 915])
%[foundRoadIds, foundRoadNames ]= allRoads.nodePairInRoad(shortestRoad.nodesNumIds);




