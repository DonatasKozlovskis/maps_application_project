classdef Road
   % A road class that represents road with lon lat coordinates
   
    properties
        id = ''      % char
        name = ''    % char
        type = ''    % char
        oneWay = false   % logical
        roadNodes  = ArrayMapNode % 
    end % properties
    properties (Dependent)
        boundMin       % MapNode
        boundMax       % MapNode
        nodes  % real array of MapNodes
        nodesTextIds %
        nodesNumIds %
        nodesLats %
        nodesLons %
    end    
    methods
        function this = Road(varargin)
            % MapNode class constructor
            switch nargin
                case 0
                    % No argument - set default MapNode
                    this.roadNodes = ArrayMapNode;
                case 1
                    % 1 argument - if it's a MapNode object, copy to a new object
                    if (isa(varargin{1},'Road'))
                        this = varargin{1};
                    elseif (isa(varargin{1},'numeric'))
                        this.roadNodes  = ArrayMapNode(varargin{1});                      
                    else
                        % otherwise display error message
                        error('Wrong input argument for Class Road')
                    end 
                otherwise
                    error('Wrong number of input arguments for Class Road')
            end % case 
        end % constructor
        function value = get.boundMin(this)
            value = MapNode( min(this.roadNodes.lats), min(this.roadNodes.lons));            
        end
        function value = get.boundMax(this)
            value = MapNode( max(this.roadNodes.lats), max(this.roadNodes.lons));            
        end
        function value = get.nodes(this)
            value = this.roadNodes.nodes;           
        end
        function value = get.nodesTextIds(this)
            value = this.roadNodes.textIds;           
        end
        function value = get.nodesNumIds(this)
            value = this.roadNodes.numIds;           
        end
        function value = get.nodesLats(this)
            value = this.roadNodes.lats;           
        end
        function value = get.nodesLons(this)
            value = this.roadNodes.lons;           
        end
    end % methods 

end
