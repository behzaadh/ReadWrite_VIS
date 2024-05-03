%function out = read_vis(filename,startIdx,cellSize)
function out = read_vis(filename)

T_SPEC_filename = strrep(filename, '.VIS_T', '.VIS_TSPEC_');
% Open the file for reading
fileID = fopen(T_SPEC_filename, 'r');
if fileID < 3; error 'Error while opening VIS_TSPEC file'; end

% Define the format of the data in the file
formatSpec = '%s%f';

% Read the data
data = textscan(fileID, formatSpec);

% Close the file
fclose(fileID);

% Extract the names and values
names = data{1};
values = data{2};

out = [];

if ~exist('filename', 'var')
    error(['''' filename ''' does not exist']); end

% Open file
fclose all;

fid = fopen(filename);
if fid < 3; error 'Error while opening VIS_T file'; end

% Display the names and values
for i = 1:numel(names)
    % Skip
    %disp([names{i}, ': ', num2str(values(i))]);
    fseek(fid, values(i)-1,'bof');
    keyword = deblank(fread(fid, 50, 'uint8=>char')');
    dtype   = deblank(fread(fid, 11, 'uint8=>char')');
    variable    = deblank(fread(fid, 11, 'uint8=>char')');
    VariableType = deblank(fread(fid, 11, 'uint8=>char')');
    bb = fread(fid, 1, 'int32=>double')';
    if (strcmp(keyword,'END'))
        break;
    else
        cellSize = values(i+1)-values(i)-87;
    end
    switch dtype
        case 'Integer'
            conv =  'int32=>double';
            wsize = 4;
        case 'Real'
            conv = 'single=>double';
            wsize = 4;
        case 'Double'
            conv = 'double';
            wsize = 8;
        case 'Logic'
            conv = 'int32';
            wsize = 4;
        case 'String'
            conv = 'uint8=>char';
            wsize = 1;
    end

    data = fread(fid, cellSize/wsize, conv, 0, 'b');

    % Add array to struct. If keyword already exists, append data.
    if isfield(out, keyword)

    else
        out.(keyword).('Data_type') = dtype;
        out.(keyword).('Variable_class') = variable;
        out.(keyword).('Variable_type') = VariableType;
        if strcmp(dtype, 'String')
            data = string(data');
        end
        if strcmp(VariableType,'GaussPoint')
            data = reshape(data,[size(data,1)/8, 8]);
        end
        out.(keyword).('Data') = data;
    end
end

fclose(fid);

