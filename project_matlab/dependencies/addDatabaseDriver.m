function addDatabaseDriver(driverPath)
    if ~ismember(driverPath,javaclasspath)
        javaaddpath(driverPath);
    end
end