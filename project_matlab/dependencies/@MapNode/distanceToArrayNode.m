function  distances = distanceToArrayNode(this, nodesArray)
%distanceToNodesArray distance between current and all nodes in
%nodesArray of class ArrayMapNode
%
%   distanceToNodesArray Calculates distance between two MapNode points 
%   expressed in lat1, lon1, lat2, lon2.
%   Assumes sperical Earth. 
    if ~(isa(nodesArray,'ArrayMapNode'))
        error('Wrong object provided, need MapNode class node')
    end

    % %
    % const avg earth radius in kilometers
    earthRad =  6371.009; % km
    % pi value
    myPi = 3.141592653589793;

    diffRadLat = (this.lat-nodesArray.lats)*myPi/180;
    diffRadLon = (this.lon-nodesArray.lons)*myPi/180;

    thisRadLat = this.lat * myPi/180;
    nodesArrayRadLat = nodesArray.lats * myPi/180;

    a = sin(diffRadLat/2).^2 + sin(diffRadLon/2).^2 .* cos(thisRadLat).* cos(nodesArrayRadLat); 
    c = 2 .* atan2(sqrt(a), sqrt(1-a));
    distances = earthRad * c;
end
