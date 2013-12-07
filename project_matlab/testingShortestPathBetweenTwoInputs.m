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
allRoads.plotMap(ax, 'staticmap.png');

% choosing Walk/Car
currentSparse = sparseMatCar;
numberOfNodes = size(currentSparse, 1);

% get start coordinate from user
userInput =  ginput(1);
userNodeStart = MapNode(userInput(2), userInput(1));

% get target coordinate from user
userInput =  ginput(1);
userNodeTarget = MapNode(userInput(2), userInput(1));

% trying to get projection for userNodeStart
[clPrNodeStart, clSegNodeStart1, clSegNodeStart2] = closestProjection(userNodeStart, 0.3, allRoads, nodesDataset, currentSparse);
clPrNodeStart.numId = numberOfNodes- 1;

% trying to get projection for userNodeTarget
[clPrNodeTarget, clSegNodeTarget1, clSegNodeTarget2] = closestProjection(userNodeTarget, 0.3, allRoads, nodesDataset, currentSparse);
clPrNodeTarget.numId = numberOfNodes;

% % update nodes dataset:

% % update nodes dataset:
tempDataRow = nodesDataset(end, :);
tempDataRow.textId = cellstr(char(clPrNodeStart.textId));
tempDataRow.lat = clPrNodeStart.lat;
tempDataRow.lon = clPrNodeStart.lon;
tempDataRow.numId = clPrNodeStart.numId;
nodesDataset = [nodesDataset; tempDataRow];


tempDataRow = nodesDataset(end, :);
tempDataRow.textId = cellstr(char(clPrNodeTarget.textId));
tempDataRow.lat = clPrNodeTarget.lat;
tempDataRow.lon = clPrNodeTarget.lon;
tempDataRow.numId = clPrNodeTarget.numId;
nodesDataset = [nodesDataset; tempDataRow];

%%%%%%%%%%%%%%%%555 updating sparse
tempNodes = ArrayMapNode;
tempNodes.push(clSegNodeStart1);
tempNodes.push(clPrNodeStart);
tempNodes.push(clSegNodeStart2);

% get numId values of road nodes
nodes1 = tempNodes.numIds(1:end-1);
nodes2 = tempNodes.numIds(2:end);
% prepare coordinates 
lat1 = tempNodes.lats(1:end-1);
lat2 = tempNodes.lats(2:end);
lon1 = tempNodes.lons(1:end-1);
lon2 = tempNodes.lons(2:end);
% calculate distances between coordinates of nodes  
nodes12Dist = pointDistance(lat1, lon1, lat2, lon2);
nodes12Dist(nodes12Dist==0)= 0.0001;

% make sparse matrix
tempSpMat1 = sparse(nodes1, nodes2, nodes12Dist, numberOfNodes, numberOfNodes);
tempSpMat2 = sparse(nodes2, nodes1, nodes12Dist, numberOfNodes, numberOfNodes);
currentSparse = currentSparse + tempSpMat1 + tempSpMat2;


%%%%%%%%%%%%%%%%%%%%%%
tempNodes = ArrayMapNode;
tempNodes.push(clSegNodeTarget1);
tempNodes.push(clPrNodeTarget);
tempNodes.push(clSegNodeTarget2);

% get numId values of road nodes
nodes1 = tempNodes.numIds(1:end-1);
nodes2 = tempNodes.numIds(2:end);
% prepare coordinates 
lat1 = tempNodes.lats(1:end-1);
lat2 = tempNodes.lats(2:end);
lon1 = tempNodes.lons(1:end-1);
lon2 = tempNodes.lons(2:end);
% calculate distances between coordinates of nodes  
nodes12Dist = pointDistance(lat1, lon1, lat2, lon2);
nodes12Dist(nodes12Dist==0)= 0.0001;
% make sparse matrix
tempSpMat1 = sparse(nodes1, nodes2, nodes12Dist, numberOfNodes, numberOfNodes);
tempSpMat2 = sparse(nodes2, nodes1, nodes12Dist, numberOfNodes, numberOfNodes);
currentSparse = currentSparse + tempSpMat1 + tempSpMat2;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


hold on
%plotting projection on 
plot([clPrNodeStart.lon userNodeStart.lon], [clPrNodeStart.lat userNodeStart.lat], '-','LineWidth',2, 'Color','red'); 

plot([clPrNodeTarget.lon userNodeTarget.lon], [clPrNodeTarget.lat userNodeTarget.lat], '-','LineWidth',2, 'Color','red'); 
hold off

% calculating shortest path
[dist,myPath,pred] = graphshortestpath(currentSparse, clPrNodeStart.numId, clPrNodeTarget.numId);
%[dist,myPath,pred] = graphshortestpath(currentSparse, clSegNodeStart1.numId, clSegNodeTarget1.numId);


spLat = nodesDataset.lat(myPath(1:end));
%spLat = [clPrNodeStart.lat; spLat(:); clPrNodeTarget.lat];
spLon = nodesDataset.lon(myPath(1:end));
%spLon = [clPrNodeStart.lon; spLon(:); clPrNodeTarget.lon];

hold on

plot(spLon, spLat, '-','LineWidth',2, 'Color','green'); 


