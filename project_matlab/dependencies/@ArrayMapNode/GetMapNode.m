function  foundNode = GetMapNode(this, idValue)
% returns MapNode with idValue given in textId or numId
% if not found returns []
%
    if (nargin==2)
        if (isa(idValue,'numeric'))       
            foundNumId = find(this.numIds==idValue, 1, 'first');
            if ~isempty(foundNumId)
                foundNode = this.nodes(foundNumId);
            else
                foundNode = [];
            end
        elseif (ischar(idValue))       
            foundNumId = find(strcmp(idValue, this.textIds), 1, 'first' );
            if ~isempty(foundNumId)
                foundNode = this.nodes(foundNumId);
            else
                foundNode = [];
            end
        else
            % otherwise display error message
            size(nargin)
            error('Wrong input arguments for getMapNode')        
        end
    else
        % otherwise display error message
        error('Wrong number of input arguments for getMapNode')
    end
end