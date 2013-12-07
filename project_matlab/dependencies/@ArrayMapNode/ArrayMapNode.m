classdef ArrayMapNode < handle
   % A ArrayMapNode class represents array of MapNode objects 
    properties
        nodes = MapNode.empty        % array of MapNode
    end % properties
    properties (Dependent)
        lats = []        % double array
        lons = []        % double array
        numIds = []      % single array
        textIds      % string array
        boundMin       % MapNode
        boundMax       % MapNode
    end
    methods
        % constructor
        function this = ArrayMapNode(varargin)
            % MapNode class constructor
            switch nargin
                case 0
                    % No argument - set default MapNode
                case 1
                    % 1 argument - if it's a MapNode object, copy to a new object
                    if (isa(varargin{1},'ArrayMapNode'))
                        this = varargin{1};
                    elseif (isa(varargin{1},'numeric'))
                        if (varargin{1}>0)
                            this.nodes(varargin{1}) = MapNode;
                        end
                    else
                        % otherwise display error message
                        error('Wrong input argument for Class ArrayMapNode')
                    end 
                otherwise
                    error('Wrong number of input arguments for Class ArrayMapNode')
            end % case 
        end % constructor
        function this = set.lats(this, newValues)
            [this.nodes(:).lat] = vout(newValues);            
        end
        function this = set.lons(this, newValues)
            [this.nodes(:).lon] = vout(newValues);            
        end
        function this = set.numIds(this, newValues)
            [this.nodes(:).numId] = vout(newValues);            
        end
        function this = set.textIds(this, newValues)
            if (isa(newValues,'cell'))
                [this.nodes(:).textId] = cout(newValues);
            else
                 error('Wrong input argument for setting textIds in ArrayMapNode')
            end
        end
        % % getters;
        function value = get.lats(this)
            value = [this.nodes(:).lat];            
        end
        function value = get.lons(this)
            value = [this.nodes(:).lon];            
        end
        function value = get.numIds(this)
            value = [this.nodes(:).numId];            
        end
        function value = get.textIds(this)
            value = {this.nodes(:).textId};            
        end
        function value = get.boundMin(this)
            value = MapNode( min([this.nodes(:).lat]), min([this.nodes(:).lon]));            
        end
        function value = get.boundMax(this)
            value = MapNode( max([this.nodes(:).lat]), max([this.nodes(:).lon]));            
        end
    end % methods 

end