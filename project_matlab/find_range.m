function output = find_range(class, lat, lon, distance, handles)

    if handles.ShortestPathPlotted == 1
    delete(handles.shortPathPlot);
    handles.ShortestPathPlotted = 0;
    end

    if handles.ShortestPathPlotted2 == 1
        delete(handles.shortPathPlot2);
        handles.ShortestPathPlotted2 = 0;
    end
    
    filteredClass = FilterClass(class);
    
        
    [length UnusedVariable] = size(filteredClass);
    
    for i=length:-1:1
        
        lat2 = cell2mat(filteredClass(i,1));
        lon2 = cell2mat(filteredClass(i,2));

        if (lat && lon && lat2 && lon2)>0
            handles.dataUser.mapNodeStart = MapNode(lat, lon);
            handles.dataUser.mapNodeTarget = MapNode(lat2, lon2);

            if handles.walk_car == 1
                [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatWalk);
            else
                [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatCar);
            end

            shortestPathLineSpec = handles.dataBack.config.plotSpec(strcmp ( {handles.dataBack.config.plotSpec.roadType}, shortestRoad.type));
            
            if (dist*1000) > distance
                filteredClass(i,:) = [];
            end
          
        else
            GUI_error('One or more points are invalid, please recheck it');
        end
    end
    
    output = filteredClass;

end

 