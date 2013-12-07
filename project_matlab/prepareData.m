clc
clear
% add path of all dependencies
allDependPath = genpath('dependencies');
addpath(allDependPath);
% add database Driver
addDatabaseDriver('dependencies/postgresqlDriver/postgresql-9.3-1100.jdbc4.jar');
%connect to database
databaseName = 'schneider';
username = 'postgres';
password = 'password';
conn = database( databaseName, username, password,'Vendor','PostGreSQL');

if ~isempty( conn.Message)
    error('Database connection error')
else
    % get result from queries
    nodes = fetch(exec(conn, 'SELECT * FROM schneider.node'));
    roads = fetch(exec(conn, 'SELECT * FROM schneider.road'));
    roadNodes = fetch(exec(conn, 'SELECT * FROM schneider.road_node as rn, schneider.node as nn where rn."NodeID" = nn."ID"'));
    % take data
    nodesData = nodes.Data;
    roadData = roads.Data;
    roadNodesData = roadNodes.Data;
    %constructing nodes dataset for later usage
    nodesDataName = {'textId' 'lat' 'lon' 'numId'};
    nodesData = [nodesDataName; nodesData];
    nodesDataset = cell2dataset(nodesData);
    save('dependencies/data/nodesDataset.mat', 'nodesDataset');
    
    % delete uneeded objects
    clear username password databaseName conn nodes roads roadNodes nodesData nodesDataName;
    
    numberOfRoads= size(roadData, 1);

    allRoads = ArrayRoad(numberOfRoads);
    
    for i=1:numberOfRoads
        %finding points for current road
        findPoints = strcmpi( char(roadData(i,1)),  roadNodesData(:,2));
        roadPoints = roadNodesData(findPoints,3:8);
        sortedPoints = sortrows(roadPoints,2);
        
        numberOfRoadNodes = size(sortedPoints, 1);
        %initializing road with # numberOfRoadNodes MapNodes
        tempRoad = Road(numberOfRoadNodes);
        tempRoad.id =       char(roadData(i,1)); 
        tempRoad.name =     char(roadData(i,2));
        tempRoad.type =     char(roadData(i,3));
        tempRoad.oneWay =    strcmpi('True', roadData(i,4));
        %adding road nodes 
        tempRoad.roadNodes.textIds = sortedPoints(:,1);
        tempRoad.roadNodes.numIds = cell2mat(sortedPoints(:,6));
        tempRoad.roadNodes.lats = cell2mat(sortedPoints(:,4));
        tempRoad.roadNodes.lons = cell2mat(sortedPoints(:,5));
        
%         for rn=1:numberOfRoadNodes     
%             tempTextId = cell2mat(sortedPoints(rn,1));
%             tempNumId = cell2mat(sortedPoints(rn,6)); 
%             tempLat = cell2mat(sortedPoints(rn,4)); 
%             tempLon = cell2mat(sortedPoints(rn,5));          
%             tempRoad.roadNodes.Push(MapNode(tempLat, tempLon, tempNumId, tempTextId));
%         end
        allRoads.roads(i) = tempRoad;
    end
    save('dependencies/data/allRoads.mat', 'allRoads');
end