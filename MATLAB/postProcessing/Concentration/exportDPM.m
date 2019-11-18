clc
clear all
close all

M = 500;
N = 7;

data = importdata('C:\Users\congwang\Desktop\file.his');

[m,~] = size(data);

ID = data(:,8);

blocks = zeros(m+M,N);

I = 0;

for i=0:M-1
    
    block = data(ID==i,1:N);
    
    if ~isempty(block)
        ii = length(block(:,1));
        line = ones(1,N)*ii;
        blocks(I+1,:) = line;
        blocks(I+2:I+2+ii-1,:) = block;
        I = I+ii+1;
    end

end

save dpm.txt -ascii blocks