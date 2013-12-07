function dist = pointDistance(lat1, lon1, lat2, lon2)
%pointDistance distance between two points
%
%   pointDistance Calculates distance between two points 
%   expressed in lat1, lon1, lat2, lon2.
%   Assumes sperical Earth. 

myPi = 3.141592653589793;
R =  6371.009; % km

dLat = (lat2-lat1)*myPi/180;
dLon = (lon2-lon1)*myPi/180;
lat1 = lat1 * myPi/180;
lat2 = lat2 * myPi/180;

a = sin(dLat/2).^2 + sin(dLon/2).^2 .* cos(lat1).* cos(lat2); 
c = 2 .* atan2(sqrt(a), sqrt(1-a));
dist = R * c;