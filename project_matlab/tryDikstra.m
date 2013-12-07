clc;
load('allRoads.mat');
load('nodesDataset.mat');
load('sparseMatrices.mat');

[dist,myPath,pred] = graphshortestpath(sparseMatCar,200,3200);

spLat = nodesDataset.lat(myPath(:));
spLon = nodesDataset.lon(myPath(:));

hold on

plot(spLon, spLat, '-','LineWidth',2, 'Color','red'); 