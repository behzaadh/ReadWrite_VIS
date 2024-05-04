%function out = read_vis(filename,startIdx,cellSize)
function out = write_vis(filename, updatedKeyword, dataIn)

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

isUpdatedKeyworkAvailable = false;
for i = 1:numel(names)
    if strcmp(names{i}, updatedKeyword)
        isUpdatedKeyworkAvailable = true;
    end
end

if ~isUpdatedKeyworkAvailable 
    error('The updated keyword does not exist in T files!'); 
end

out = [];

if ~exist('filename', 'var')
    error(['''' filename ''' does not exist']); 
end

% Open file
fclose all;

fid = fopen(filename,'r+');
if fid < 3; error 'Error while opening VIS_T file'; end

% Display the names and values
for i = 1:numel(names)
    % Skip
    %disp([names{i}, ': ', num2str(values(i))]);
    fseek(fid, values(i)-1,'bof');
    keyword = deblank(fread(fid, 50, 'uint8=>char')');
    if ~strcmp(keyword, updatedKeyword)
        continue;
    end
    dtype   = deblank(fread(fid, 11, 'uint8=>char')');

    switch dtype
        case 'Integer'
            conv =  'int32=>double';
        case 'Real'
            conv = 'single=>double';
        case 'Double'
            conv = 'double';
        case 'Logic'
            conv = 'int32';
        case 'String'
            conv = 'uint8=>char';
    end

    fseek(fid, values(i)-1+50+33+4,'bof');
    fwrite(fid, dataIn, conv, 0, 'b');

end

fclose(fid);

