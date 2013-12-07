function roadPlotHandles = plotRoads(this, axesHande)
%
    roadPlotHandles = [1:this.numberOfRoads];

    hold on
    for p = 1:this.numberOfRoads 
        roadPlotHandles = [roadPlotHandles this.roads(p).plotRoad(axesHande)];
    end
    hold off
    xlabel('Longitude');
    ylabel('Latitude');
    lat_lon_proportions;
end
