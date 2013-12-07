clc;
allDependPath = genpath('dependencies');
addpath(allDependPath);
clear;

load('allRoads.mat')
unique([allRoads.roads(:).nodesNumIds]);

ax =axes;
cla(ax);
allRoads.plotBackground(ax, 'staticmap.png');
allRoads.plotRoads(ax);


%get(ax, 'YDir')