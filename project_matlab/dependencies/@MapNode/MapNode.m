classdef MapNode
   % A MapNode class represents MapNode 
   % with lon lat coordinates, numId and textId
   
    properties
        lat = []      % double
        lon = []      % double
        numId = []    % single
        textId  = ''    % char
    end % properties
    methods
        % constructor
        function this = MapNode(varargin)
            % MapNode class constructor
            switch nargin
                case 0
                    % No argument - set default MapNode
                    this.lat = 0;
                    this.lon = 0;
                    this.numId = nan;
                    this.textId = nan;
                case 1
                    % 1 argument - if it's a MapNode object, copy to a new object
                    if (isa(varargin{1},'MapNode'))
                        this = varargin{1};
                    else
                        % otherwise display error message
                        error('Wrong number of input arguments for Class MapNode')
                    end 
                case 2
                    % 2 arguments - set only Lat and Lon
                    this.lat = varargin{1};
                    this.lon=varargin{2};
                    this.numId = nan;
                    this.textId = nan;                 
                case 4
                    % 4 arguments. Assume that they're lat lon numId texID
                    this.lat = varargin{1};
                    this.lon = varargin{2};
                    this.numId = varargin{3};
                    this.textId = varargin{4};
                otherwise
                    error('Wrong number of input arguments for Class MapNode')
            end % case 
        end % constructor

            
    end % methods 

end