function [shortestRoad, dist] = twoInputShortestPath(mapNodeStart, mapNodeTarget, allRoads, nodesDataset, currentSparse)
% this function calculates shortest path between two random MapNodes

%number of Nodes
numberOfNodes = size(currentSparse, 1);

% trying to get projection for mapNodeStart
[clPrNodeStart, clSegNodeStart1, clSegNodeStart2] = closestProjectionToSegment(mapNodeStart, 0.3, allRoads, nodesDataset, currentSparse);
clPrNodeStart.numId = numberOfNodes- 1;

% trying to get projection for mapNodeTarget
[clPrNodeTarget, clSegNodeTarget1, clSegNodeTarget2] = closestProjectionToSegment(mapNodeTarget, 0.3, allRoads, nodesDataset, currentSparse);
clPrNodeTarget.numId = numberOfNodes;

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

%%%%%%%%%%%%%%%% updating sparse
% adding start point 
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


%%%%%%%%%%%%%%%% updating sparse
% adding target point 
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



% calculating shortest path
[dist,myPath,pred] = graphshortestpath(currentSparse, clPrNodeStart.numId, clPrNodeTarget.numId);
nodesInShortestPath = numel(myPath);

%initializing road with # nodesInShortestPath MapNodes
shortestRoad = Road(nodesInShortestPath+2);
shortestRoad.id =  'ShortestPath'; 
shortestRoad.name =      'ShortestPath'; 
shortestRoad.type = 'ShortestPath';
shortestRoad.oneWay =  1;
% adding start and target node to shortest Road
 shortestRoad.roadNodes.nodes(1) = mapNodeStart;
 shortestRoad.roadNodes.nodes(end) = mapNodeTarget;
 
% adding rest of the nodes
if (nodesInShortestPath>0)
    %adding road nodes 
    shortestRoad.roadNodes.textIds(2:end-1) = nodesDataset.textId(myPath(1:end));
    shortestRoad.roadNodes.numIds(2:end-1) = nodesDataset.numId(myPath(1:end));
    shortestRoad.roadNodes.lats(2:end-1) = nodesDataset.lat(myPath(1:end));
    shortestRoad.roadNodes.lons(2:end-1) = nodesDataset.lon(myPath(1:end));
end 

end