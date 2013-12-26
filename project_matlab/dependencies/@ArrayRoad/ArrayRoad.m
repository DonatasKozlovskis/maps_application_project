classdef ArrayRoad < handle
   % A ArrayMapNode class represents array of MapNode objects 
    properties
        roads = Road.empty        % array of MapNode
    end % properties
    properties (Dependent)
        numberOfRoads
        boundMin       % MapNode
        boundMax       % MapNode
        latLonAdjCoef  % double
        allRoadIds %char
        nodesInRoads
    end
    methods
        % constructor
        function this = ArrayRoad(varargin)
            % MapNode class constructor
            switch nargin
                case 0
                    % No argument - set default MapNode
                case 1
                    % 1 argument - if it's a MapNode object, copy to a new object
                    if (isa(varargin{1},'ArrayRoad'))
                        this = varargin{1};
                    elseif (isa(varargin{1},'numeric'))
                        this.roads(varargin{1}) = Road;
                    else
                        % otherwise display error message
                        error('Wrong input argument for Class ArrayRoad')
                    end 
                otherwise
                    error('Wrong number of input arguments for Class ArrayRoad')
            end % case 
        end % constructor
        function value = get.numberOfRoads(this)
            value = size(this.roads, 2);
        end
        function value = get.boundMin(this)
            minNodes =[this.roads(:).boundMin];
            value = MapNode( min([minNodes(:).lat]), min([minNodes(:).lon]));            
        end
        function value = get.boundMax(this)
            maxNodes =[this.roads(:).boundMax];
            value = MapNode( max([maxNodes(:).lat]), max([maxNodes(:).lon]));            
        end
        function value = get.latLonAdjCoef(this)
            value = cosd( mean([this.boundMin.lat this.boundMax.lat]) );          
        end
        function value = get.allRoadIds(this)
            value = {this.roads(:).id};            
        end
        function  value = get.nodesInRoads(this)
        % returns hash table for roadID and nodeIDs
            roadId = [this.roads.idRepeated];
            nodeId = [this.roads.nodesNumIds];
            roadName = [this.roads.nameRepeated];
            value= dataset({nodeId', 'nodeId'}, {roadId', 'roadId'}, {roadName', 'roadName'});
        end
    end % methods 

end