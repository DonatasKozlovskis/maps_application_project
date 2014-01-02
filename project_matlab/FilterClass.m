function output = FilterClass(item)
    load('POIdata');
    load('POIClasses');
    
    %for all the points we set the default data to all the data
    if item == 1
        output = POIdata;
    else
        
        class = POIClasses(item);
        FilteredClass = {};
        [sizeData unusedVariable] = size(POIdata);
        for i=1:sizeData
            %in the next line we use setdiff to look for diferences between
            %two cells. If the words are equal it will return an empty cell
            %that we will check with isempte
            if strcmp(POIdata(i,4), class)
                %we add every element that has the same 'class' with cat()
                FilteredClass = cat(1,FilteredClass,POIdata(i,:));
            end
        end
        output = FilteredClass;
    end
end