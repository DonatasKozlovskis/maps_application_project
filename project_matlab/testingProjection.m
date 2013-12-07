clc;
clear;
allDependPath = genpath('dependencies');
addpath(allDependPath);

load('allRoads.mat');
load('nodesDataset.mat');
load('sparseMatrices.mat');

%plotting le map
ax =axes;
cla(ax);
allRoads.plotMap(ax, 'staticmap.png');
% get coordinate from user
userInput =  ginput(1);
myNode = MapNode(userInput(2), userInput(1));

% HARDCODED current sparse
currentSparseMat = sparseMatCar; % = sparseMatWalk


% creating window nodes limits
[myNodeMin, myNodeMax] = myNode.mapNodeWindow(0.2, allRoads.boundMin, allRoads.boundMax);

%subseting nodes from window
nodesInWindow = nodesDataset.lat > myNodeMin.lat & ...
                nodesDataset.lon > myNodeMin.lon & ...
                nodesDataset.lat < myNodeMax.lat & ...
                nodesDataset.lon < myNodeMax.lon;
%getting nodes ID in window
nodesInWindowNumId = nodesDataset.numId(nodesInWindow);

% extracting all connected nodes from sparse
[cPair1, cPair2] = find(currentSparseMat);
% reforming to 2x array
jointNodeNumId = [cPair1 cPair2];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%getting paired nodes Id in my window
pairInWindow = ismember(cPair1, nodesInWindowNumId); % | ismember(cPair2, nodesInWindowNumId);
myPairs = jointNodeNumId(pairInWindow,:);

tempNodeData1 = nodesDataset(myPairs(:,1),:);
segmentNodes1 = ArrayMapNode(size(myPairs, 1));
segmentNodes1.lats = tempNodeData1.lat;
segmentNodes1.lons = tempNodeData1.lon;
segmentNodes1.numIds = tempNodeData1.numId;
segmentNodes1.textIds = tempNodeData1.textId;

tempNodeData2 = nodesDataset(myPairs(:,2),:);
segmentNodes2 = ArrayMapNode(size(myPairs, 1));
segmentNodes2.lats = tempNodeData2.lat;
segmentNodes2.lons = tempNodeData2.lon;
segmentNodes2.numIds = tempNodeData2.numId;
segmentNodes2.textIds = tempNodeData2.textId;

tempProjectedNodes = myNode.nodeOnArraySegment(segmentNodes1, segmentNodes2, allRoads.latLonAdjCoef);

shortestDist = inf;
closestProjectedNode = MapNode.empty;
% if projection found
if (~isempty(tempProjectedNodes))
    tempDistances = myNode.distanceToArrayNode(tempProjectedNodes);
    minIndex = find(min(tempDistances) == tempDistances, 1);
    %assign new shortest dist and node
    shortestDist=tempDistances(minIndex);
    closestProjectedNode =  tempProjectedNodes.nodes(minIndex);
    closestSegmentNode1 =segmentNodes1.nodes(minIndex);
    closestSegmentNode2 =segmentNodes2.nodes(minIndex);
end

%%%%%%%%%%%

tempNodeData = nodesDataset(nodesInWindowNumId,:);
windowNodes = ArrayMapNode(size(nodesInWindowNumId, 1));
windowNodes.lats = tempNodeData.lat;
windowNodes.lons = tempNodeData.lon;
windowNodes.numIds = tempNodeData.numId;
windowNodes.textIds = tempNodeData.textId;

if (~isempty(windowNodes))
    
    tempDistances = myNode.distanceToArrayNode(windowNodes);
    minIndex = find(min(tempDistances) == tempDistances, 1);
    %assign new shortest dist and node
    tempShortestDist=tempDistances(minIndex);
    if tempShortestDist<shortestDist
        closestProjectedNode =  windowNodes.nodes(minIndex);
        closestSegmentNode1 =windowNodes.nodes(minIndex);
        closestSegmentNode2 =windowNodes.nodes(minIndex); 
    end
    
    

end


%making square coords for window plot
wLat = [myNodeMin.lat myNodeMin.lat myNodeMax.lat myNodeMax.lat myNodeMin.lat];
wLon = [myNodeMin.lon myNodeMax.lon myNodeMax.lon myNodeMin.lon myNodeMin.lon];

%plotting window
hold on
plot(wLon, wLat, '-','LineWidth',2, 'Color','red'); 

hold on
%plotting projection on 
plot([closestProjectedNode.lon myNode.lon], [closestProjectedNode.lat myNode.lat], '-','LineWidth',2, 'Color','red'); 

hold off

