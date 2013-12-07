clc;
clear;
allDependPath = genpath('dependencies');
addpath(allDependPath);

load('allRoads.mat');
load('nodesDataset.mat');

%max number of nodes;
numberOfNodes = size(nodesDataset, 1);
%add two extra nodes to increase sparse matrix just to have
% option to add random points;
numberOfNodes = numberOfNodes+2;
numberOfRoads = allRoads.numberOfRoads;
sparseMatCar = NaN;
sparseMatWalk = NaN;

for i=1:numberOfRoads
    %%%%%%%%%%%%%%%%%%%%
    %work with one road
    tempRoad = allRoads.roads(i);

    % get numId values of road nodes
    nodes1 = tempRoad.roadNodes.numIds(1:end-1);
    nodes2 = tempRoad.roadNodes.numIds(2:end);
    % prepare coordinates 
    lat1 = tempRoad.roadNodes.lats(1:end-1);
    lat2 = tempRoad.roadNodes.lats(2:end);
    lon1 = tempRoad.roadNodes.lons(1:end-1);
    lon2 = tempRoad.roadNodes.lons(2:end);

    % calculate distances between coordinates of nodes  
    nodes12Dist = pointDistance(lat1, lon1, lat2, lon2);
    
    % make sparse matrix
    tempSpMat1 = sparse(nodes1, nodes2, nodes12Dist, numberOfNodes, numberOfNodes);
    tempSpMat2 = sparse(nodes2, nodes1, nodes12Dist, numberOfNodes, numberOfNodes);
    % create temp sparse matrix for cars
    if tempRoad.oneWay==0
        sparseMatCarTemp = tempSpMat1 + tempSpMat2;
    else
        sparseMatCarTemp = tempSpMat1;
    end
    % create sparse matrix for cars
    if isnan(sparseMatCar)
        sparseMatCar = sparseMatCarTemp;
    else
        sparseMatCar = sparseMatCar + sparseMatCarTemp;
    end
    % create sparse matrix for walking    
    if isnan(sparseMatWalk)
        sparseMatWalk = tempSpMat1 + tempSpMat2;
    else
        sparseMatWalk = sparseMatWalk + tempSpMat1 + tempSpMat2;
    end
end % end for
save('dependencies/data/sparseMatrices.mat', 'sparseMatCar', 'sparseMatWalk');