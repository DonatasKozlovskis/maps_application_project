function thisPlotHandle = plotRoad(this, axesHande)
   thisPlotHandle = plot(axesHande, this.roadNodes.lons, this.roadNodes.lats, '-','LineWidth',1);
end