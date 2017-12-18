function [ sample_data ] = ReadDPMSampleData( filename )

data = importdata(filename);
data = data(3:end);

[n, ~] = size(data);

sample_data = zeros(n, 12);

for i=1:n
    
    line = data(i);
    line = line{1};
    
    sample_data(i,:) = sscanf(line, ...
        '(( %f %f %f %f %f %f %f %f %f %f %f %f')';
    
end




end

