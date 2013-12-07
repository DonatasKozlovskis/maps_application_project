clc
clear 
%%
addpath('openstreetmap');
addpath('openstreetmap\dependencies\gaimc');
addpath('openstreetmap\dependencies\hold');
addpath('openstreetmap\dependencies\lat_lon_proportions');
addpath('openstreetmap\dependencies\plotmd');
addpath('openstreetmap\dependencies\textmd');
addpath('openstreetmap\dependencies\xml2struct');
%%
parsed_osm = parse_openstreetmap('test.osm');
save('parsedRoads.mat', 'parsed_osm')
%%
clear
clc
load('parsed_osm.mat')
plot_way(axes, parsed_osm)
%%
[connectivity_matrix, intersection_nodes] = extract_connectivity(parsed_osm);
%%
start = 32
target = 1727
[route, dist] = route_planner(connectivity_matrix, start, target)
%%
plot_route(axes, route, parsed_osm)
 
