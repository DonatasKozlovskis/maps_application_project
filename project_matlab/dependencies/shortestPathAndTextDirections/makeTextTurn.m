function whichTurn = makeTextTurn(nodeA, nodeB, nodeC)
%makeTextTurn returns text depending on which side to turn.

    angle = angleTreeNodes(nodeA, nodeB, nodeC);
    absAngle = abs(angle);
    
    side = '';
    turnPower = '';
    
    % determine side to turn
    if (angle>0)
        side = 'right';
    else
        side = 'left';
    end
    
    % determine turn (angle power)
    if (absAngle < 60)
        turnPower = 'Sharp';
    elseif (absAngle >= 60 && absAngle <120)
        turnPower = 'Med.';
    else
        turnPower = 'Slight';
    end
    
    whichTurn = [turnPower, ' turn ', side]; 
    
end
    