function roadPlotHandles = plotRoads(this, axesHande, lineSpec)
%
    roadPlotHandles = [1:this.numberOfRoads];
    
    hold on
    for p = 1:this.numberOfRoads 
        %subsetting only this road plot config
        thisRoadLineSpecIndex = strcmp ( {lineSpec.roadType}, this.roads(p).type);
        thisRoadLineSpec = lineSpec(thisRoadLineSpecIndex);
        %determine is spec contains numeric values:

        
        % plotting road and returning handle
        roadPlotHandles = [roadPlotHandles this.roads(p).plotRoad(axesHande, thisRoadLineSpec)];
    end
    hold off
 %   xlabel('Longitude');
 %   ylabel('Latitude');
    lat_lon_proportions;
end
