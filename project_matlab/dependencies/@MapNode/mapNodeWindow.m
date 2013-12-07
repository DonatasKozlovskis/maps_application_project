function [nodeMin, nodeMax] = mapNodeWindow(this, windowSize, boundNodeMin, boundNodeMax)
%MapNodeWindow square window around MapNode of [lat lon]
%   MapNodeWindow Calculates square window around geoPoint of [lat lon] 
%   by adding ws window size in kilometers to lat and lon
%   Assumes sperical Earth. 
%   boundNodeMin and boundNodeMax should contain bounds= [minLat maxLat minLon maxLon]

if ~(isa(boundNodeMin,'MapNode')&&isa(boundNodeMax,'MapNode'))
    error('Wrong bounds provided, need MapNode class')
end

% %
% const earth circumference in kilometers
earthCirc = 40075.04; % km
% pi value
myPi = 3.141592653589793;


% express latitude in Radians
thisLatRadians = this.lat * myPi/180;

% calculate deltas
dLat = 360*windowSize / earthCirc;
dLon = 360*windowSize / ( earthCirc *cos(thisLatRadians) ) ;

%calculate new window

nodeMin = MapNode(this.lat - dLat, this.lon - dLon);
nodeMax = MapNode(this.lat + dLat, this.lon + dLon);


% checking if new window not exceeds 
% min bounds
if nodeMin.lat<boundNodeMin.lat
    nodeMin.lat=boundNodeMin.lat;
end
if nodeMin.lon<boundNodeMin.lon
    nodeMin.lon=boundNodeMin.lon;
end
% max bounds
if nodeMax.lat>boundNodeMax.lat
    nodeMax.lat=boundNodeMax.lat;
end
if nodeMax.lon>boundNodeMax.lon
    nodeMax.lon=boundNodeMax.lon;
end

% checking if calculated window if outside bounds window

% min bounds
if nodeMax.lat<boundNodeMin.lat
    nodeMax.lat=boundNodeMin.lat;
end
if nodeMax.lon<boundNodeMin.lon
    nodeMax.lon=boundNodeMin.lon;
end
% max bounds
if nodeMin.lat>boundNodeMax.lat
    nodeMin.lat=boundNodeMax.lat;
end
if nodeMin.lon>boundNodeMax.lon
    nodeMin.lon=boundNodeMax.lon;
end



end