function [closestProjectedNode, closestSegmentNode1, closestSegmentNode2] = closestProjectionToSegment(userNode, wSize, allRoads, nodesDataset, currentSparseMatrix)

closestProjectedNode = MapNode.empty;
closestSegmentNode1 = MapNode.empty;
closestSegmentNode2 = MapNode.empty;
shortestDist = inf;

    % creating window nodes limits
    [userNodeMin, userNodeMax] = userNode.mapNodeWindow(wSize, allRoads.boundMin, allRoads.boundMax);

    %subseting all nodes from window
    nodesInWindow = nodesDataset.lat > userNodeMin.lat & ...
                    nodesDataset.lon > userNodeMin.lon & ...
                    nodesDataset.lat < userNodeMax.lat & ...
                    nodesDataset.lon < userNodeMax.lon;
    %leaving only those nodes which are in current sparse matrix as well.
    nodesInSparse =0;
    %getting nodes ID in window
    nodesInWindowNumId = nodesDataset.numId(nodesInWindow);

    % extracting all connected nodes from sparse
    [cPair1, cPair2] = find(currentSparseMatrix);
    % reforming to 2x array
    jointNodeNumId = [cPair1 cPair2];

    %getting paired nodes Id in my window
    %cpair2 is not used since the roads can be only one way and this allows
    %to take only right roads
    
    pairInWindow = ismember(cPair1, nodesInWindowNumId); % | ismember(cPair2, nodesInWindowNumId);
    myPairs = jointNodeNumId(pairInWindow,:);

    tempProjectedNodes = ArrayMapNode.empty;
    if (size(myPairs, 1)>0)
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

        tempProjectedNodes = userNode.nodeOnArraySegment(segmentNodes1, segmentNodes2, allRoads.latLonAdjCoef);
    end


    % if projection found
    if (~isempty(tempProjectedNodes))
        tempDistances = userNode.distanceToArrayNode(tempProjectedNodes);
        minIndex = find(min(tempDistances) == tempDistances, 1);
        %assign new shortest dist and node
        shortestDist=tempDistances(minIndex);
        closestProjectedNode =  tempProjectedNodes.nodes(minIndex);
        closestSegmentNode1 =segmentNodes1.nodes(closestProjectedNode.numId);
        closestSegmentNode2 =segmentNodes2.nodes(closestProjectedNode.numId);
    end

%%%%%%%%%%%
    windowNodes = ArrayMapNode.empty;
    if (size(myPairs, 1)> 0)
        tempNodeData = nodesDataset(myPairs(:,1),:);
        windowNodes = ArrayMapNode( size(myPairs, 1) );
        windowNodes.lats = tempNodeData.lat;
        windowNodes.lons = tempNodeData.lon;
        windowNodes.numIds = tempNodeData.numId;
        windowNodes.textIds = tempNodeData.textId;
    end
    
    if (~isempty(windowNodes))

        tempDistances = userNode.distanceToArrayNode(windowNodes);
        minIndex = find(min(tempDistances) == tempDistances, 1);
        %assign new shortest dist and node
        tempShortestDist=tempDistances(minIndex);
        if tempShortestDist<shortestDist
            closestProjectedNode =  windowNodes.nodes(minIndex);
            closestSegmentNode1 =windowNodes.nodes(minIndex);
            closestSegmentNode2 =windowNodes.nodes(minIndex); 
        end

    end

end