function thisPlotHandle = plotRoad(this, axesHande, plotSpec )
   thisPlotHandle = plot(axesHande, this.roadNodes.lons, this.roadNodes.lats);
   
   for ii= 1:numel(plotSpec)
      set( thisPlotHandle, plotSpec(ii).prop, plotSpec(ii).propVal);
   end  
end