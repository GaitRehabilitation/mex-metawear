function metawear_connect(varagin)

    switch nargin
        case 1
            mexMetaWear('connect',varargin{1})
        otherwise
            error('metawear_connect: incorrect number of arguments, check the documentation!')
    end
end