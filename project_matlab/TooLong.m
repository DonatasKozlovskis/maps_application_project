function varargout = TooLong(varargin)
% TOOLONG MATLAB code for TooLong.fig
%      TOOLONG, by itself, creates a new TOOLONG or raises the existing
%      singleton*.
%
%      H = TOOLONG returns the handle to a new TOOLONG or the handle to
%      the existing singleton*.
%
%      TOOLONG('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TOOLONG.M with the given input arguments.
%
%      TOOLONG('Property','Value',...) creates a new TOOLONG or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before TooLong_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to TooLong_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help TooLong

% Last Modified by GUIDE v2.5 21-Dec-2013 15:39:52

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @TooLong_OpeningFcn, ...
                   'gui_OutputFcn',  @TooLong_OutputFcn, ...
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


% --- Executes just before TooLong is made visible.
function TooLong_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to TooLong (see VARARGIN)

% Choose default command line output for TooLong
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes TooLong wait for user response (see UIRESUME)
% uiwait(handles.figure4);


% --- Outputs from this function are returned to the command line.
function varargout = TooLong_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in close_error.
function close_error_Callback(hObject, eventdata, handles)
% hObject    handle to close_error (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(handles.figure4);
