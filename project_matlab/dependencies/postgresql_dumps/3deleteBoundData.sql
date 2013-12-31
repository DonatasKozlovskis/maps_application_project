--
-- Checking limits
--
SELECT DISTINCT "RoadID" FROM schneider.road_node as rn, schneider.node AS t 
where t."Latitude" < 46.782
AND rn."NodeID" = t."ID";

SELECT DISTINCT "RoadID" FROM schneider.road_node as rn, schneider.node AS t 
where t."Latitude" > 46.813
AND rn."NodeID" = t."ID";

SELECT DISTINCT "RoadID" FROM schneider.road_node as rn, schneider.node AS t 
where t."Longitude" < 4.403
AND rn."NodeID" = t."ID";

SELECT DISTINCT "RoadID" FROM schneider.road_node as rn, schneider.node AS t 
where t."Longitude" > 4.475
AND rn."NodeID" = t."ID";

--
-- deleting unneeded nodes
--

Delete FROM schneider.node 
where "Latitude" < 46.782;

Delete FROM schneider.node 
where "Latitude" > 46.813;

Delete FROM schneider.node 
where "Longitude" < 4.403;

Delete FROM schneider.node 
where "Longitude" > 4.475;