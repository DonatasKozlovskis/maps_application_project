CREATE SCHEMA IF NOT EXISTS Schneider;

DROP TABLE IF EXISTS Schneider.node;
CREATE TABLE Schneider.node (
  "ID" varchar(50) NOT NULL,
  "Latitude" DOUBLE PRECISION DEFAULT NULL,
  "Longitude" DOUBLE PRECISION DEFAULT NULL,
  PRIMARY KEY ("ID")
);

DROP TABLE IF EXISTS Schneider.poi;
CREATE TABLE Schneider.poi (
  "ID" varchar(50) NOT NULL,
  "Latitude" DOUBLE PRECISION NOT NULL,
  "Longitude" DOUBLE PRECISION NOT NULL,
  "Name" varchar(200) NOT NULL,
  "Type" varchar(50) NOT NULL,
  "PicturePath" varchar(200) DEFAULT NULL,
  "NearestRoadID" varchar(50) DEFAULT NULL,
  PRIMARY KEY ("ID")
);

DROP TABLE IF EXISTS Schneider.road;
CREATE TABLE Schneider.road (
  "ID" varchar(50) NOT NULL,
  "RoadName" varchar(200) DEFAULT NULL,
  "RoadType" varchar(50) DEFAULT NULL,
  "OneWay" varchar(50) DEFAULT NULL,
  PRIMARY KEY ("ID")
);

DROP TABLE IF EXISTS Schneider.road_node;
CREATE TABLE Schneider.road_node (
  "ID" varchar(50) NOT NULL,
  "RoadID" varchar(50) NOT NULL,
  "NodeID" varchar(50) NOT NULL,
  "ContourOrder" NUMERIC(20) DEFAULT NULL,
  PRIMARY KEY ("ID"),
  FOREIGN KEY ("NodeID") REFERENCES Schneider.node("ID")  ON DELETE CASCADE ON UPDATE RESTRICT,  
  FOREIGN KEY ("RoadID") REFERENCES Schneider.road("ID")  ON DELETE CASCADE ON UPDATE RESTRICT  
);