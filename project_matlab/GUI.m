function varargout = GUI(varargin)
% GUI MATLAB code for GUI.fig
%      GUI, by itself, creates a new GUI or raises the existing
%      singleton*.
%
%      H = GUI returns the handle to a new GUI or the handle to
%      the existing singleton*.
%
%      GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI.M with the given input arguments.
%
%      GUI('Property','Value',...) creates a new GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before GUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to GUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help GUI

% Last Modified by GUIDE v2.5 20-Dec-2013 20:02:35

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @GUI_OpeningFcn, ...
                   'gui_OutputFcn',  @GUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before GUI is made visible.
function GUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to GUI (see VARARGIN)

% Choose default command line output for GUI
handles.output = hObject;


% handles.dataBack = all background data, not "accesible" for user
% handles.dataUser = data which obtained from or can be modified by user

% loading all nodes data and data for shortest path 
load('allRoads.mat');
load('nodesDataset.mat');
load('sparseMatrices.mat');
% adding needed data to handles;

handles.dataBack.allRoads = allRoads;
handles.dataBack.nodesDataset = nodesDataset;
handles.dataBack.sparseMatCar = sparseMatCar;
handles.dataBack.sparseMatWalk = sparseMatWalk;
% loading plot line specifications
handles.dataBack.config.plotSpec = dataset2struct(dataset('File','plotConfig.csv', 'Delimiter',';'));

% updating axes1 UI, plotting map on axes1:
handles.dataBack.allRoads.plotMap(handles.axes1, 'staticmap.png', handles.dataBack.config.plotSpec);

% default start-target points (from-to)
% empty class object
handles.dataUser.mapNodeStart = MapNode();
handles.dataUser.mapNodeTarget = MapNode();

%In this handles we will store the three points that we can select on our
%GUI. 1 and 2 will be used in the main GUI while the third one will be used
%in the itinerary panel
handles.lat1 = 0;
handles.lon1 = 0;
handles.lat2 = 0;
handles.lon2 = 0;
handles.lat3 = 0;
handles.lon3 = 0;

%This values will be used when plotting the points in the map. We will use
%them because the procedure is first delete the last point plotted and
%second plot the new point. But the first time we have no points plotted
%and we have to skip this
handles.a = 0;
handles.b = 0;
handles.c = 0;
handles.d = 0;
handles.e = 0;
handles.f = 0;
handles.ShortestPathPlotted = 0;
handles.ShortestPathPlotted2 = 0;

%This handle will store the distance in the itinerary
handles.distance = 0;
%maxDistance is the maximum distance for doing an itinerary. It will be
%compared with distance
handles.maxDistance = 0;

load('POIdata');
handles.ActualPOIs1 = POIdata;
handles.ActualPOIs2 = POIdata;
handles.ActualPOIs3 = POIdata;

%We set some initial parameters in order to see at the begining all the
%lists filled
set(handles.from, 'String', handles.ActualPOIs1(:,3));
set(handles.popupmenu2, 'String', handles.ActualPOIs2(:,3));
set(handles.popupmenu3, 'String', handles.ActualPOIs3(:,3));

load('POIClasses.mat');
set(handles.class1, 'String', POIClasses);
set(handles.class2, 'String', POIClasses);
set(handles.class3, 'String', POIClasses);
set(handles.class4, 'String', POIClasses);

%Setting initially visible only the POI panel. We will update the view of
%each panel when pushing the corresponding button.
set(handles.poi_panel, 'Visible', 'on');
set(handles.itinerary_panel, 'Visible', 'off');
set(handles.find_panel, 'Visible', 'off');
set(handles.instructions_panel, 'Visible', 'off');

% set walk mode by default
set(handles.walk, 'Value', 0);
set(handles.car, 'Value', 1);

%if handles.walk_car=1 we will go by walk, otherwise by car (handles.walk_car=0)
handles.walk_car = 0;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes GUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = GUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in from.
function from_Callback(hObject, eventdata, handles)
% hObject    handle to from (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns from contents as cell array
%        contents{get(hObject,'Value')} returns selected item from from

contents = get(hObject,'Value');

latmat = cell2mat(handles.ActualPOIs1(:,1));
longmat = cell2mat(handles.ActualPOIs1(:,2));

handles.lat1 = latmat(contents);
handles.lon1 = longmat(contents);

hold on;
if handles.b == 1; 
    delete(handles.a);
end
handles.a = plot ( handles.lon1, handles.lat1, 'ro',...
                                               'LineWidth',2,...
                                               'MarkerEdgeColor','k',...
                                               'MarkerFaceColor',[1 .49 .63],...
                                               'MarkerSize',10);
handles.b = 1;


% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function from_CreateFcn(hObject, eventdata, handles)
% hObject    handle to from (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu2.
function popupmenu2_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu2

contents = get(hObject,'Value');

latmat = cell2mat(handles.ActualPOIs2(:,1));
longmat = cell2mat(handles.ActualPOIs2(:,2));

handles.lat2 = latmat(contents);
handles.lon2 = longmat(contents);

hold on;
if handles.d == 1; 
        delete(handles.c);
end
handles.c = plot ( handles.lon2, handles.lat2, 'mo',...
                                               'LineWidth',2,...
                                               'MarkerEdgeColor','k',...
                                               'MarkerFaceColor',[.49 .63 1],...
                                               'MarkerSize',10);
handles.d = 1;

% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in go.
function go_Callback(hObject, eventdata, handles)
% hObject    handle to go (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%refresh_Callback(hObject, eventdata, handles);

if handles.ShortestPathPlotted == 1
    delete(handles.shortPathPlot);
    handles.ShortestPathPlotted = 0;
end

if handles.ShortestPathPlotted2 == 1
    delete(handles.shortPathPlot2);
    handles.ShortestPathPlotted2 = 0;
end

handles.dataUser.mapNodeStart = MapNode(handles.lat1, handles.lon1);
handles.dataUser.mapNodeTarget = MapNode(handles.lat2, handles.lon2);

if handles.walk_car == 1
    [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatWalk);
else
    [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatCar);
end

shortestPathLineSpec = handles.dataBack.config.plotSpec(strcmp ( {handles.dataBack.config.plotSpec.roadType}, shortestRoad.type));
hold on
    handles.shortPathPlot = shortestRoad.plotRoad(handles.axes1, shortestPathLineSpec);
hold off

handles.ShortestPathPlotted = 1;

% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in export_data.
function export_data_Callback(hObject, eventdata, handles)
% hObject    handle to export_data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in modify_POI.
function modify_POI_Callback(hObject, eventdata, handles)
% hObject    handle to modify_POI (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
modify_POI();
refresh_Callback(hObject, eventdata, handles);

% --- Executes on button press in new_poi.
function new_poi_Callback(hObject, eventdata, handles)
% hObject    handle to new_poi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
new_POI();


% --- Executes during object creation, after setting all properties.
function axes1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
%handles.allRoads.plotMap(hObject, 'staticmap.png', handles.plotSpec);
% Hint: place code in OpeningFcn to populate axes1

% --- Executes on button press in refresh.
function refresh_Callback(hObject, eventdata, handles)
% hObject    handle to refresh (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
load('POIdata');

handles.ActualPOIs1 = POIdata;
handles.ActualPOIs2 = POIdata;
handles.ActualPOIs3 = POIdata;

set(handles.from, 'String', handles.ActualPOIs1(:,3));
set(handles.popupmenu2, 'String', handles.ActualPOIs2(:,3));
set(handles.popupmenu3, 'String', handles.ActualPOIs3(:,3));

set(handles.from, 'Value', 1);
set(handles.popupmenu2, 'Value', 1);
set(handles.popupmenu3, 'Value', 1);

set(handles.class1, 'Value', 1);
set(handles.class2, 'Value', 1);
set(handles.class3, 'Value', 1);

if handles.ShortestPathPlotted == 1
    delete(handles.shortPathPlot);
    handles.ShortestPathPlotted = 0;
end

if handles.ShortestPathPlotted2 == 1
    delete(handles.shortPathPlot2);
    handles.ShortestPathPlotted2 = 0;
end

if handles.b == 1; 
    delete(handles.a);
end

if handles.d == 1; 
    delete(handles.c);
end

% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in getlocation1.
function getlocation1_Callback(hObject, eventdata, handles)
% hObject    handle to getlocation1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[handles.lon1,handles.lat1] = ginput(1);

hold on;
if handles.b == 1; 
    delete(handles.a);
end
handles.a = plot ( handles.lon1, handles.lat1, 'ro',...
                                               'LineWidth',2,...
                                               'MarkerEdgeColor','k',...
                                               'MarkerFaceColor',[1 .49 .63],...
                                               'MarkerSize',10);
handles.b = 1;

set(handles.from, 'value', 1);
set(handles.from, 'String', 'User location');
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in getlocation2.
function getlocation2_Callback(hObject, eventdata, handles)
% hObject    handle to getlocation2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% get start coordinate from user
[handles.lon2,handles.lat2] = ginput(1);

hold on;
if handles.d == 1; 
    delete(handles.c);
end
handles.c = plot ( handles.lon2, handles.lat2, 'mo',...
                                               'LineWidth',2,...
                                               'MarkerEdgeColor','k',...
                                               'MarkerFaceColor',[.49 .63 1],...
                                               'MarkerSize',10);
handles.d = 1;

set(handles.popupmenu2, 'value', 1);
set(handles.popupmenu2, 'String', 'User location');
% Update handles structure
guidata(hObject, handles);

% --- Executes on selection change in class1.
function class1_Callback(hObject, eventdata, handles)
% hObject    handle to class1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns class1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from class1
contents = get(hObject,'Value');

handles.ActualPOIs1 = FilterClass(contents);
set(handles.from, 'String', handles.ActualPOIs1(:,3));

%this will avoid the popupmenu to disappear because the index it greater
%than the elements to be shown
set(handles.from, 'value', 1);

% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function class1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to class1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in class2.
function class2_Callback(hObject, eventdata, handles)
% hObject    handle to class2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: contents = cellstr(get(hObject,'String')) returns class2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from class2
contents = get(hObject,'Value');

handles.ActualPOIs2 = FilterClass(contents);
set(handles.popupmenu2, 'String', handles.ActualPOIs2(:,3));

%this will avoid the popupmenu to disappear because the index it greater
%than the elements to be shown
set(handles.popupmenu2, 'value', 1);

% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function class2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to class2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in poi.
function poi_Callback(hObject, eventdata, handles)
% hObject    handle to poi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.poi_panel, 'Visible', 'on');
set(handles.itinerary_panel, 'Visible', 'off');
set(handles.find_panel, 'Visible', 'off');
set(handles.instructions_panel, 'Visible', 'off');
set(handles.go, 'Visible', 'on');

% Update handles structure
guidata(hObject, handles)


% --- Executes on button press in itinerary.
function itinerary_Callback(hObject, eventdata, handles)
% hObject    handle to itinerary (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.poi_panel, 'Visible', 'of');
set(handles.itinerary_panel, 'Visible', 'on');
set(handles.find_panel, 'Visible', 'off');
set(handles.instructions_panel, 'Visible', 'off');
set(handles.go, 'Visible', 'off');

% Update handles structure
guidata(hObject, handles)


% --- Executes on button press in range.
function range_Callback(hObject, eventdata, handles)
% hObject    handle to range (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.poi_panel, 'Visible', 'off');
set(handles.itinerary_panel, 'Visible', 'off');
set(handles.find_panel, 'Visible', 'on');
set(handles.instructions_panel, 'Visible', 'off');
set(handles.go, 'Visible', 'on');

% Update handles structure
guidata(hObject, handles)

% --- Executes on button press in instructions.
function instructions_Callback(hObject, eventdata, handles)
% hObject    handle to instructions (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.poi_panel, 'Visible', 'off');
set(handles.itinerary_panel, 'Visible', 'off');
set(handles.find_panel, 'Visible', 'off');
set(handles.instructions_panel, 'Visible', 'on');
set(handles.go, 'Visible', 'on');

% Update handles structure
guidata(hObject, handles)


function output_instructions_Callback(hObject, eventdata, handles)
% hObject    handle to output_instructions (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: get(hObject,'String') returns contents of output_instructions as text
%        str2double(get(hObject,'String')) returns contents of output_instructions as a double


% --- Executes during object creation, after setting all properties.
function output_instructions_CreateFcn(hObject, eventdata, handles)
% hObject    handle to output_instructions (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function distance_range_Callback(hObject, eventdata, handles)
% hObject    handle to distance_range (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: get(hObject,'String') returns contents of distance_range as text
%        str2double(get(hObject,'String')) returns contents of distance_range as a double


% --- Executes during object creation, after setting all properties.
function distance_range_CreateFcn(hObject, eventdata, handles)
% hObject    handle to distance_range (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in class4.
function class4_Callback(hObject, eventdata, handles)
% hObject    handle to class4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: contents = cellstr(get(hObject,'String')) returns class4 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from class4
contents = get(hObject,'Value');

handles.ActualPOIs = FilterClass(contents);
set(handles.popupmenu3, 'String', handles.ActualPOIs(:,3));

% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function class4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to class4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in getlocation3.
function getlocation3_Callback(hObject, eventdata, handles)
% hObject    handle to getlocation3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[handles.lon3,handles.lat3] = ginput(1);

hold on;
if handles.f == 1; 
    delete(handles.e);
end
handles.e = plot ( handles.lon3, handles.lat3, 'mo',...
                                               'LineWidth',2,...
                                               'MarkerEdgeColor','k',...
                                               'MarkerFaceColor',[.49 1 .63],...
                                               'MarkerSize',10);
handles.f = 1;

set(handles.popupmenu3, 'value', 1);
set(handles.popupmenu3, 'String', 'User location');
% Update handles structure
guidata(hObject, handles);

% --- Executes on selection change in popupmenu3.
function popupmenu3_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu3 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu3
contents = get(hObject,'Value');

latmat = cell2mat(handles.ActualPOIs3(:,1));
longmat = cell2mat(handles.ActualPOIs3(:,2));

handles.lat3 = latmat(contents);
handles.lon3 = longmat(contents);

hold on;
if handles.f == 1; 
    delete(handles.e);
end
handles.e = plot ( handles.lon3, handles.lat3, 'mo',...
                                               'LineWidth',2,...
                                               'MarkerEdgeColor','k',...
                                               'MarkerFaceColor',[.49 1 .63],...
                                               'MarkerSize',10);
handles.f = 1;


% Update handles structure
guidata(hObject, handles);



% --- Executes during object creation, after setting all properties.
function popupmenu3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in class3.
function class3_Callback(hObject, eventdata, handles)
% hObject    handle to class3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: contents = cellstr(get(hObject,'String')) returns class3 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from class3
contents = get(hObject,'Value');

handles.ActualPOIs3 = FilterClass(contents);
set(handles.popupmenu3, 'String', handles.ActualPOIs3(:,3));

%this will avoid the popupmenu to disappear because the index it greater
%than the elements to be shown
set(handles.from, 'value', 1);

% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function class3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to class3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function max_distance_Callback(hObject, eventdata, handles)
% hObject    handle to max_distance (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: get(hObject,'String') returns contents of max_distance as text
%        str2double(get(hObject,'String')) returns contents of max_distance as a double
handles.maxDistance = str2double(get(hObject,'String'));

% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function max_distance_CreateFcn(hObject, eventdata, handles)
% hObject    handle to max_distance (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in go_itinerary.
function go_itinerary_Callback(hObject, eventdata, handles)
% hObject    handle to go_itinerary (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.ShortestPathPlotted == 1
    delete(handles.shortPathPlot);
    handles.ShortestPathPlotted = 0;
end

if handles.ShortestPathPlotted2 == 1
    delete(handles.shortPathPlot2);
    handles.ShortestPathPlotted2 = 0;
end

handles.dataUser.mapNodeStart = MapNode(handles.lat1, handles.lon1);
handles.dataUser.mapNodeTarget = MapNode(handles.lat2, handles.lon2);

if handles.walk_car == 1
    [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatWalk);
else
    [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatCar);
end

handles.distance = dist;

shortestPathLineSpec = handles.dataBack.config.plotSpec(strcmp ( {handles.dataBack.config.plotSpec.roadType}, shortestRoad.type));
hold on
    handles.shortPathPlot = shortestRoad.plotRoad(handles.axes1, shortestPathLineSpec);
hold off
handles.ShortestPathPlotted = 1;

handles.dataUser.mapNodeStart = MapNode(handles.lat2, handles.lon2);
handles.dataUser.mapNodeTarget = MapNode(handles.lat3, handles.lon3);

if handles.walk_car == 1
    [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatWalk);
else
    [shortestRoad, dist]= twoInputShortestPath(handles.dataUser.mapNodeStart, handles.dataUser.mapNodeTarget, handles.dataBack.allRoads, handles.dataBack.nodesDataset, handles.dataBack.sparseMatCar);
end

handles.distance = handles.distance + dist;

shortestPathLineSpec = handles.dataBack.config.plotSpec(strcmp ( {handles.dataBack.config.plotSpec.roadType}, shortestRoad.type));
hold on
    handles.shortPathPlot2 = shortestRoad.plotRoad(handles.axes1, shortestPathLineSpec);
hold off

handles.ShortestPathPlotted2 = 1;

%We need the distance computed in meters since we ask user for meters.
handles.distance = handles.distance * 1000;

if handles.distance > handles.maxDistance
    TooLong();
end


% Update handles structure
guidata(hObject, handles);

% --- Executes on button press in go_range.
function go_range_Callback(hObject, eventdata, handles)
% hObject    handle to go_range (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in walk.
function walk_Callback(hObject, eventdata, handles)
% hObject    handle to walk (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.walk_car = 1;
set(handles.car, 'Value', 0);
%set(handles.walk, 'Value', 1);
% Hint: get(hObject,'Value') returns toggle state of walk

% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in car.
function car_Callback(hObject, eventdata, handles)
% hObject    handle to car (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.walk_car = 0;
%set(handles.car, 'Value', 1');
set(handles.walk, 'Value', 0.0);
% Hint: get(hObject,'Value') returns toggle state of car

% Update handles structure
guidata(hObject, handles);
