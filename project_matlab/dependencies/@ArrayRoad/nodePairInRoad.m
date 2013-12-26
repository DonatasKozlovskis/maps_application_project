function [foundRoadIds, foundRoadNames] = nodePairInRoad(this, MapNodesNumIDs)
% find pair of nodes in road and returns road name
    pairsToCompare = numel(MapNodesNumIDs)-1;
    if (pairsToCompare== 0)
        %if only one numID given, return [];
        % since can not determine the road for ID
        foundRoadIds = [];
    else
        foundRoadIds(1:pairsToCompare) =  {''};
        foundRoadNames(1:pairsToCompare) =  {''};
        %else look for pairs in structure;
        pairedNodesId = [this.nodesInRoads.nodeId(1:end-1), ...
                        this.nodesInRoads.nodeId(2:end)];
        %loop throug each pair           

        pairsToFind = [MapNodesNumIDs(1:pairsToCompare)', MapNodesNumIDs(2:pairsToCompare+1)'];
        [findLeft, indexLeft] = ismember( pairsToFind, pairedNodesId,'rows');
        [findRight, indexRight] = ismember( pairsToFind(:,end:-1:1), pairedNodesId,'rows');

        %findBoth = findLeft | findRight;
        indexBoth = indexLeft +indexRight;
        foundRoadIds(indexBoth>0) = this.nodesInRoads.roadId(indexBoth( indexBoth>0));
        foundRoadNames(indexBoth>0) = this.nodesInRoads.roadName(indexBoth( indexBoth>0));
    end
end