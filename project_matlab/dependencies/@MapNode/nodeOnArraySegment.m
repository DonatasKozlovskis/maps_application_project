function projectedNodes = nodeOnArraySegment(this, segmentNodes1, segmentNodes2, coefAdj)
%mapNodeOnSegment myPoint projection on line between segmentNodes1 & segmentNodes2
%
%   mapNodeOnSegment function projects this mapNode 
%   on line segment between segmentNode1 and segmentNode1
%   if imposibble to project, returns MapNode.empty

% % copy node to avoid original object changes
thisNodeCopy = this;
segmentNodes1.lats = 1/coefAdj *segmentNodes1.lats;
segmentNodes2.lats = 1/coefAdj *segmentNodes2.lats;
thisNodeCopy.lat = 1/coefAdj * thisNodeCopy.lat;


% % % put segment nodes to array
% segmentArray = ArrayMapNode(2);
% segmentArray.nodes(1) = segmentNodes1;
% segmentArray.nodes(2) = segmentNodes2;

isEqual = (segmentNodes1.lats==segmentNodes2.lats & segmentNodes1.lons==segmentNodes1.lons); 
segmentNodes1.lats(isEqual) = segmentNodes1.lats(isEqual) + 0.0000000000001;

u = ((thisNodeCopy.lat - segmentNodes1.lats) .* (segmentNodes2.lats - segmentNodes1.lats)) ... 
     + ((thisNodeCopy.lon - segmentNodes1.lons) .* (segmentNodes2.lons - segmentNodes1.lons));
uDenom = power(segmentNodes2.lats - segmentNodes1.lats, 2) + power(segmentNodes2.lons - segmentNodes1.lons, 2);

u = u ./ uDenom;
 
pLats = segmentNodes1.lats + (u .* (segmentNodes2.lats - segmentNodes1.lats));
pLons = segmentNodes1.lons + (u .* (segmentNodes2.lons - segmentNodes1.lons));

boundMinLats = min(segmentNodes1.lats, segmentNodes2.lats);
boundMinLons = min(segmentNodes1.lons, segmentNodes2.lons);
boundMaxLats = max(segmentNodes1.lats, segmentNodes2.lats);
boudnMaxLons = max(segmentNodes1.lons, segmentNodes2.lons);

isValid = (pLats >= boundMinLats & pLats <= boundMaxLats) & (pLons >= boundMinLons & pLons <= boudnMaxLons);

pLats = pLats*coefAdj;
if sum(isValid)
    projectedNodes = ArrayMapNode(sum(isValid));
    projectedNodes.lats = pLats(isValid);
    projectedNodes.lons = pLons(isValid);
    projectedNodes.numIds = find(isValid);
else
    projectedNodes = ArrayMapNode.empty;
end

% since passed by reference (handle class), restore values.
segmentNodes1.lats = coefAdj *segmentNodes1.lats;
segmentNodes2.lats = coefAdj *segmentNodes2.lats;

