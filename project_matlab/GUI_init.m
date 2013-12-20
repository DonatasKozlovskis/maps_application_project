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

load('POIdata');
load('POIClasses');
POIname = POIdata(:,3);
%POInameFrom = cat(1, 'Go from...', POIname);
%POInameTo = cat(1, 'To...', POIname);
POIlatitude = cell2mat(POIdata(:,1));
%POIlatitude = cat(1,0,POIlatitude);
POIlongitude = cell2mat(POIdata(:,2));
%POIlongitude = cat(1,0,POIlongitude);



%GUI(POInameFrom, POInameTo, POIlatitude, POIlongitude, POIClasses);
GUI(POIname, POIname, POIlatitude, POIlongitude, POIClasses);