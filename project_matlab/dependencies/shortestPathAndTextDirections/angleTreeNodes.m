function angle = angleTreeNodes(nodeA, nodeB, nodeC)
%calculates angle betwenn nodeA-nodeB-nodeC
    nodeAB = MapNode( nodeA.lat - nodeB.lat, nodeA.lon - nodeB.lon);
    nodeCB = MapNode( nodeC.lat - nodeB.lat, nodeC.lon - nodeB.lon);
    
    angleAB = atan2 (nodeAB.lat, nodeAB.lon);
    angleCB = atan2 (nodeCB.lat, nodeCB.lon);


    if (nodeAB==MapNode() || nodeCB==MapNode())
        angle = 0;
    else
        angle = rad2deg(angleCB-angleAB);  
        %converting angle to -180::+180 range
        if (angle>180)
            angle = angle-360;
        end
    end
end