function dist = distanceToNode(this, otherNode)
%DistanceToNode distance between current and other node
%
%   DistanceToNode Calculates distance between two MapNode points 
%   expressed in lat1, lon1, lat2, lon2.
%   Assumes spherical Earth. 
if ~(isa(otherNode,'MapNode'))
    error('Wrong object provided, need MapNode class node')
end

% %
% const avg earth radius in kilometres
earthRad =  6371.009; % km
% pi value
myPi = 3.141592653589793;

diffRadLat = (this.lat-otherNode.lat)*myPi/180;
diffRadLon = (this.lon-otherNode.lon)*myPi/180;

thisRadLat = this.lat * myPi/180;
otherNodeRadLat = otherNode.lat * myPi/180;

a = sin(diffRadLat/2).^2 + sin(diffRadLon/2).^2 .* cos(thisRadLat).* cos(otherNodeRadLat); 
c = 2 .* atan2(sqrt(a), sqrt(1-a));
dist = earthRad * c;

end