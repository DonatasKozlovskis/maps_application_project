% conn = database('schneider','postgres','password','Vendor','PostGreSQL');
% 
% POI = exec(conn,'SELECT poi.* FROM schneider.poi');
% POI = fetch(POI);
% close(POI);
% 
% POIdata = POI.Data;
clc;
clear;
allDependPath = genpath('dependencies');
addpath(allDependPath);

GUI();