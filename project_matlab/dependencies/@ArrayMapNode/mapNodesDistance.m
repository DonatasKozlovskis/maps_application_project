function  distance = mapNodesDistance(this, idVal1, idVal2)
    node1 = GetMapNode(this, idVal1);
    node2 = GetMapNode(this, idVal2);
    
    if (isempty(node1)|| isempty(node2))
        % otherwise display error message
        error('Node(s) not found')
    else
        distance = node1.distanceToNode(node2);
    end
end