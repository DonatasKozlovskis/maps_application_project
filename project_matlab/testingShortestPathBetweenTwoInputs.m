clc;
allDependPath = genpath('dependencies');
addpath(allDependPath);
clear;
% % 
% load data
load('allRoads.mat');
load('nodesDataset.mat');
load('sparseMatrices.mat');

%plotting le map
ax =axes;
cla(ax);
lineSpecMap = dataset2struct(dataset('File','plotConfig.csv', 'Delimiter',';'));
allRoads.plotMap(ax, 'staticmap.png', lineSpecMap);


% get start coordinate from user
userInput =  ginput(1);
mapNodeStart = MapNode(userInput(2), userInput(1));

% get target coordinate from user
userInput =  ginput(1);
mapNodeTarget = MapNode(userInput(2), userInput(1));

% choosing Walk/Car
currentSparse = sparseMatCar;
numberOfNodes = size(currentSparse, 1);

% calculating shortest path
[shortestRoad, dist]= twoInputShortestPath(mapNodeStart, mapNodeTarget, allRoads, nodesDataset, currentSparse);
%plotting shortestPath


shortestPathLineSpec = lineSpecMap(strcmp ( {lineSpecMap.roadType}, shortestRoad.type));
hold on
    aa = shortestRoad.plotRoad(ax, shortestPathLineSpec);
hold off

sprintf( makeDirections(shortestRoad, allRoads))


