function projectedNode = nodeOnSegment(this, segmentNode1, segmentNode2, coefAdj)
%mapNodeOnSegment myPoint projection on line between segmentNode1 & segmentNode2
%
%   mapNodeOnSegment function projects this mapNode 
%   on line segment between segmentNode1 and segmentNode1
%   if imposibble to project, returns MapNode.empty

% % copy node to avoid original object changes
thisNodeCopy = this;
segmentNode1.lat = 1/coefAdj *segmentNode1.lat;
segmentNode2.lat = 1/coefAdj *segmentNode2.lat;
thisNodeCopy.lat = 1/coefAdj * thisNodeCopy.lat;


% % put segment nodes to array
segmentArray = ArrayMapNode(2);
segmentArray.nodes(1) = segmentNode1;
segmentArray.nodes(2) = segmentNode2;


if (segmentNode1.lat==segmentNode2.lat && segmentNode1.lon==segmentNode1.lon)
    segmentNode1.lat = segmentNode1.lat + 0.0000000000001;
end

u = ((thisNodeCopy.lat - segmentNode1.lat) * (segmentNode2.lat - segmentNode1.lat)) + ((thisNodeCopy.lon - segmentNode1.lon) * (segmentNode2.lon - segmentNode1.lon));
uDenom = power(segmentNode2.lat - segmentNode1.lat, 2) + power(segmentNode2.lon - segmentNode1.lon, 2);

u = u / uDenom;

pLat = segmentNode1.lat + (u * (segmentNode2.lat - segmentNode1.lat));
pLon = segmentNode1.lon + (u * (segmentNode2.lon - segmentNode1.lon));

isValid = (pLat >= segmentArray.boundMin.lat && pLat <= segmentArray.boundMax.lat) && (pLon>= segmentArray.boundMin.lon && pLon<= segmentArray.boundMax.lon);

if isValid
    projectedNode = MapNode(pLat*coefAdj, pLon);
else
    projectedNode = MapNode.empty;
end

    
    