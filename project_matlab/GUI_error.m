function varargout = GUI_error(varargin)
% GUI_ERROR MATLAB code for GUI_error.fig
%      GUI_ERROR, by itself, creates a new GUI_ERROR or raises the existing
%      singleton*.
%
%      H = GUI_ERROR returns the handle to a new GUI_ERROR or the handle to
%      the existing singleton*.
%
%      GUI_ERROR('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI_ERROR.M with the given input arguments.
%
%      GUI_ERROR('Property','Value',...) creates a new GUI_ERROR or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before GUI_error_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to GUI_error_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help GUI_error

% Last Modified by GUIDE v2.5 30-Dec-2013 11:21:00

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @GUI_error_OpeningFcn, ...
                   'gui_OutputFcn',  @GUI_error_OutputFcn, ...
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


% --- Executes just before GUI_error is made visible.
function GUI_error_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to GUI_error (see VARARGIN)

% Choose default command line output for GUI_error
handles.output = hObject;

set(handles.error_text,'String', varargin{1});

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes GUI_error wait for user response (see UIRESUME)
% uiwait(handles.gui_error);


% --- Outputs from this function are returned to the command line.
function varargout = GUI_error_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in close.
function close_Callback(hObject, eventdata, handles)
% hObject    handle to close (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(handles.gui_error);
