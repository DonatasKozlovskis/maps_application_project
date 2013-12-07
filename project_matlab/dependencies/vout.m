function varargout=vout(C)
    if isnumeric(C), 
        C=num2cell(C); 
    end
    C=C(:).';
    varargout=C(1:nargout);
end