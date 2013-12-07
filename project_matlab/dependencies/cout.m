function varargout=cout(C)
    C=C(:).';
    varargout=C(1:nargout);
end