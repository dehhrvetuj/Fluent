clc
clear all
close all

data=importdata('Inlet.csv');
data=data.data;

% SWITCH Y AND Z
x = data(:,1); 
z = data(:,2); 
y = data(:,3);

u = data(:,4); 
w = data(:,5); 
v = data(:,6);

mag = sqrt(u.^2 + v.^2 + w.^2);

% figure
% scatter(x,y,[],mag,'.');

% GENERATE RECTANGULAR GRID (-2.1 2.1)
[xq,yq] = meshgrid(-0.3:0.005:0.3);


figure
scatter(xq(:),yq(:),'.');


% INTERPOLATE ON GIRD
uq = griddata(x,y,u,xq,yq-1.8,'nearest');
vq = griddata(x,y,v,xq,yq-1.8,'nearest');
wq = griddata(x,y,w,xq,yq-1.8,'nearest');


% mesh(xq,yq,wq);

% save inlet_profile

% ELIMINATE NaN
uq(isnan(uq)) = 0;
vq(isnan(vq)) = 0;
wq(isnan(wq)) = 0;


% RESHAPE GRID MATRIX INTO VECTOR
xq = xq(:);
yq = yq(:);
uq = uq(:);
vq = vq(:);
wq = wq(:);

zq = ones(length(xq),1)*3.2;

% TRANSLATE THE COORDINATE
xq = [xq+9.30; -xq+9.30];
yq = [yq+3.10; -yq+6.65];
zq = [zq; zq];

uq = [uq; -uq];
vq = [vq; -vq];
wq = [wq; wq];

magq = sqrt(uq.^2 + vq.^2 + wq.^2);

figure
scatter(xq,yq,[],magq,'.');


% EXPORT DATA IN SCHEME FORMAT FOR FLUENT USE
nrow = length(xq);

fileID = fopen('cor-inlet','w');

fprintf(fileID,'%s %d %s\n','((cor-inlet-profile point', nrow,')');

fprintf(fileID,'%s\n','(x');
for i=1:nrow
    fprintf(fileID,'%f\n',xq(i));
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(y');
for i=1:nrow    
    fprintf(fileID,'%f\n',yq(i));    
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(z');
for i=1:nrow
    
    fprintf(fileID,'%f\n',zq(i));  
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(u');
for i=1:nrow   
    fprintf(fileID,'%f\n',uq(i));  
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(v');
for i=1:nrow
    fprintf(fileID,'%f\n',vq(i));   
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(w');
for i=1:nrow  
    fprintf(fileID,'%f\n',wq(i));
end
fprintf(fileID,'%s\n',')');

fclose(fileID);
