function thisPlotHandle = plotRoad(this, axesHande, plotSpec )
    thisPlotHandle = plot(axesHande, this.roadNodes.lons, this.roadNodes.lats);
   
    for ii= 1:numel(plotSpec)
        setProperty = plotSpec(ii).prop;
        setPropertyValue = plotSpec(ii).propVal;
        if  sum(isstrprop(setPropertyValue, 'digit'))>0
            setPropertyValue = str2num(setPropertyValue);
        end
        set( thisPlotHandle, setProperty, setPropertyValue);
    end  
end