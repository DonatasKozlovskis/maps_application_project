function textDirections = makeDirections(shortestRoad, allRoads)
%makeDirections makes text output for directions
%

%if shortest Road contains less then two nodes
        textDirections = '';

    if (numel(shortestRoad.nodes)<2)
        textDirections = '';
    else
        [indivDist, totalDist] = shortestRoad.roadNodes.distanceBetweenNodes;
        [foundRoadIds, foundRoadNames]= allRoads.nodePairInRoad(shortestRoad.nodesNumIds);
        %replacing empty street names with string 'unnamed':
        foundRoadNames (strcmp(foundRoadNames, '')) = {'unnamed'};
        %initializing temp distance in One street 
        tempStreetDist = indivDist(1);
        
        for k=2:numel(indivDist)
            if strcmp( foundRoadNames(k-1), foundRoadNames(k) )
                tempStreetDist= tempStreetDist+ indivDist(k);
                if k == numel(indivDist)
                    textDirections = [ textDirections, 'Continue ', num2str(tempStreetDist), ' km on ', ...
                            char(foundRoadNames(k-1)), ' road.\n '];                    
                end
                
            else
                textDirections = [ textDirections, 'Continue ', num2str(tempStreetDist), ' km on ', ...
                char(foundRoadNames(k-1)), ' road.\n '];
                
                turnText = makeTextTurn(shortestRoad.nodes(k-1), shortestRoad.nodes(k), shortestRoad.nodes(k+1));
                
                textDirections = [ textDirections, turnText, ' to ', char(foundRoadNames(k)), ' road.\n '];
                tempStreetDist = indivDist(k);
            end
        end


    end
    

end