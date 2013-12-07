clc;
clear;
allDependPath = genpath('dependencies');
addpath(allDependPath);
% % test MapNode constructor
testNode0 = MapNode;
testNode1 = MapNode(1,2);
testNode2 = MapNode(testNode1);
testNode2 = MapNode(1, 2, 100, 'aaaa');
testNode1 = MapNode(3, 4, 200, 'bbbb');
testNodeMin = MapNode(2,2);
testNodeMax = MapNode(3,4);
% % test MapNode window function
[aa, bb] =testNode1.mapNodeWindow(30, testNodeMin, testNodeMax);
% % test MapNode distance function
aaa = testNodeMin.distanceToNode(testNodeMax);
 
% % test ArrayMapNode constructor and functions
tarray = ArrayMapNode(2);
tarray.nodes(1) = testNode1;
tarray.nodes(2) = testNode2;
tarray.nodes(3) = testNode2;
tarray.GetMapNode('bbbb')
tarray.mapNodesDistance(100, 200)
tarray.push(MapNode)

tarray.lats = [1 2 3 4]
tarray.textIds
tarray.textIds = {'a' 'b' 'c' 'd'}

%%
% % window plots

% wLat = [aa.lat aa.lat bb.lat bb.lat aa.lat];
% wLon = [aa.lon bb.lon bb.lon aa.lon aa.lon];
% %plotting window
% cla
% plot(wLon, wLat, '-','LineWidth',2, 'Color','red');
% hold on
% plot(testNode1.lon, testNode1.lat, 'x','LineWidth',2, 'Color','red');
