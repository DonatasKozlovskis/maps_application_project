clc;
allDependPath = genpath('dependencies');
addpath(allDependPath);
clear;

nodeA = MapNode(0,0);
nodeB = MapNode(1,1);
nodeC = MapNode(2,1);


angleTreeNodes(nodeA, nodeB, nodeC)
