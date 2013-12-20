function roadPlotHandles = plotRoads(this, axesHande, lineSpec)
%
    roadPlotHandles = [1:this.numberOfRoads];

    hold on
    for p = 1:this.numberOfRoads 
        roadPlotHandles = [roadPlotHandles this.roads(p).plotRoad(axesHande, lineSpec)];
    end
    hold off
 %   xlabel('Longitude');
 %   ylabel('Latitude');
    lat_lon_proportions;
end
