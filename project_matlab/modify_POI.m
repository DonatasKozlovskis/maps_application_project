function varargout = modify_POI(varargin)
% MODIFY_POI MATLAB code for modify_POI.fig
%      MODIFY_POI, by itself, creates a new MODIFY_POI or raises the existing
%      singleton*.
%
%      H = MODIFY_POI returns the handle to a new MODIFY_POI or the handle to
%      the existing singleton*.
%
%      MODIFY_POI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MODIFY_POI.M with the given input arguments.
%
%      MODIFY_POI('Property','Value',...) creates a new MODIFY_POI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before modify_POI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to modify_POI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help modify_POI

% Last Modified by GUIDE v2.5 29-Dec-2013 16:33:09

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @modify_POI_OpeningFcn, ...
                   'gui_OutputFcn',  @modify_POI_OutputFcn, ...
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


% --- Executes just before modify_POI is made visible.
function modify_POI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to modify_POI (see VARARGIN)

% Choose default command line output for modify_POI
handles.output = hObject;

load('POIdata');
c = POIdata(:,3);
set(handles.listbox1,'String', c);

handles.index = 0;
handles.name = 0;
handles.lat = 0;
handles.lon = 0;
handles.adress = 0;
handles.class = 0;




% Update handles structure
guidata(hObject, handles);

% UIWAIT makes modify_POI wait for user response (see UIRESUME)
% uiwait(handles.figure3);


% --- Outputs from this function are returned to the command line.
function varargout = modify_POI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in listbox1.
function listbox1_Callback(hObject, eventdata, handles)
% hObject    handle to listbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns listbox1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from listbox1

load('POIdata.mat');
handles.index = {get(hObject,'Value')};
handles.index = cell2mat(handles.index);
name = POIdata(handles.index,3);
set(handles.text_name,'String', name);
lat = POIdata(handles.index,1);
lon = POIdata(handles.index,2);
class = POIdata(handles.index,4);
adress = POIdata(handles.index,5);
set(handles.text_latitude,'String',lat);
set(handles.text_longitude,'String',lon);
set(handles.text_class, 'String', class);
set(handles.text_adress, 'String', adress);

% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function listbox1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to listbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called



% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function get_name_Callback(hObject, eventdata, handles)
% hObject    handle to get_name (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of get_name as text
%        str2double(get(hObject,'String')) returns contents of get_name as a double
handles.name = get(hObject,'String');
% Update handles structure
guidata(hObject, handles);



% --- Executes during object creation, after setting all properties.
function get_name_CreateFcn(hObject, eventdata, handles)
% hObject    handle to get_name (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function get_latitude_Callback(hObject, eventdata, handles)
% hObject    handle to get_latitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of get_latitude as text
%        str2double(get(hObject,'String')) returns contents of get_latitude as a double
handles.lat = str2double(get(hObject,'String'));
% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function get_latitude_CreateFcn(hObject, eventdata, handles)
% hObject    handle to get_latitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


%
function get_longitude_Callback(hObject, eventdata, handles)
% hObject    handle to get_longitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of get_longitude as text
%        str2double(get(hObject,'String')) returns contents of get_longitude as a double
handles.lon = str2double(get(hObject,'String'));
% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function get_longitude_CreateFcn(hObject, eventdata, handles)
% hObject    handle to get_longitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in modify.
function modify_Callback(hObject, eventdata, handles)
% hObject    handle to modify (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
load('POIdata');
POIdata{handles.index,1} = handles.lat;
POIdata{handles.index,2} = handles.lon;
POIdata{handles.index,3} = handles.name;
POIdata{handles.index,4} = handles.class;
POIdata{handles.index,5} = handles.adress;
save('.\dependencies\data\POIdata.mat', 'POIdata');

% --- Executes on button press in close.
function close_Callback(hObject, eventdata, handles)
% hObject    handle to close (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(handles.figure3);



function get_adress_Callback(hObject, eventdata, handles)
% hObject    handle to get_adress (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: get(hObject,'String') returns contents of get_adress as text
%        str2double(get(hObject,'String')) returns contents of get_adress as a double
handles.adress = get(hObject,'String');

% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function get_adress_CreateFcn(hObject, eventdata, handles)
% hObject    handle to get_adress (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in get_class.
function get_class_Callback(hObject, eventdata, handles)
% hObject    handle to get_class (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: contents = cellstr(get(hObject,'String')) returns get_class contents as cell array
%        contents{get(hObject,'Value')} returns selected item from get_class
load('POIClasses.mat');

contents = get(hObject,'Value');
handles.class = POIClasses(contents);

% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function get_class_CreateFcn(hObject, eventdata, handles)
% hObject    handle to get_class (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
load('POIClasses.mat');
set(hObject, 'String', POIClasses);
% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
