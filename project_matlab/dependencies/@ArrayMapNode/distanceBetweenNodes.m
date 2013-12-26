function [ pairDistances, totalDistance ] = distanceBetweenNodes( this )
%distanceBetweenNodes Distance between each pair of nodes
%   Calculates distance between each pair of nodes in array of class ArrayMapNodeconsecutively 
%   Assumes sperical Earth. 
%   returns array with distance between each pair

    % %
    % const avg earth radius in kilometers
    earthRad =  6371.009; % km
    % pi value
    myPi = 3.141592653589793;

    diffRadLat = ( this.lats(1:end-1)-this.lats(2:end) ).*myPi/180;
    diffRadLon = ( this.lons(1:end-1)-this.lons(2:end) ).*myPi/180;

    thisRadLat = this.lats(1:end-1) .* myPi/180;
    otherNodeRadLat = this.lats(2:end) * myPi/180;

    a = sin(diffRadLat/2).^2 + sin(diffRadLon/2).^2 .* cos(thisRadLat).* cos(otherNodeRadLat); 
    c = 2 .* atan2(sqrt(a), sqrt(1-a));
    
    pairDistances = earthRad * c;
    totalDistance = sum(pairDistances);
end

