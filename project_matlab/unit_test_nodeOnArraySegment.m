clc;
clear;
allDependPath = genpath('dependencies');
addpath(allDependPath);

bb =MapNode(2, 1);

s1=ArrayMapNode(5);
s1.lats = [0 0 0 0 0];
s1.lons = [0 0 0 0 0];

s2=ArrayMapNode(5);
s2.lats = [2 1 3 4 5];
s2.lons = [1 2 3 4 5];

pNodes = bb.nodeOnArraySegment(s1, s2, 1);
bb.distanceToArrayNode(pNodes);