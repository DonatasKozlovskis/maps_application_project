function  foundRoad = GetRoad(this, idValue)
% returns Road with id value given in textId long
% if not found returns []
    if (nargin==2)
        if (ischar(idValue)| iscell(idValue))       
            foundNumId = find(strcmp(idValue, this.allRoadIds), 1, 'first' );
            if ~isempty(foundNumId)
                foundRoad = this.roads(foundNumId);
            else
                foundRoad = [];
            end
        else
            % otherwise display error message
            size(nargin)
            error('Wrong input arguments for getRoad')        
        end
    else
        % otherwise display error message
        error('Wrong number of input arguments for getRoad')
    end
end