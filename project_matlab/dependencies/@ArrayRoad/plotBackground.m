function thisPlotHandle = plotBackground(this, axesHande, imName)   
    staticBg = imread(imName);
    imgBgMirror = flipdim(staticBg,1);
    x=linspace(this.boundMin.lon, this.boundMax.lon,1000); %1000 samples
    y=linspace(this.boundMin.lat, this.boundMax.lat,1000); %1000 samples
    axesHande;
    thisPlotHandle = image(x, y, imgBgMirror);
    set(gca,'YDir','normal');
end