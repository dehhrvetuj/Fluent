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

% SET UPWARD VELOCITY TO ZERO
w(w>0) = -0.001;

mag = sqrt(u.^2 + v.^2 + w.^2);

figure
scatter(x,y,[],mag,'.');

% GENERATE RECTANGULAR GRID (-2.1 2.1)
[xq,yq] = meshgrid(-2.1:0.005:2.1);

% INTERPOLATE ON GIRD
uq = griddata(x,y,u,xq,yq,'nearest');
vq = griddata(x,y,v,xq,yq,'nearest');
wq = griddata(x,y,w,xq,yq,'nearest');

% mesh(xq,yq,wq);

% save inlet_profile

% ELIMINATE NaN
uq(isnan(uq)) = 0;
vq(isnan(vq)) = 0;
wq(isnan(wq)) = 0;

% MARK POINTS WITHIN (-1.5 1.5) WITH NaN
I = abs(xq)<1.5 & abs(yq)<1.5;

xq(I) = NaN;
yq(I) = NaN;
uq(I) = NaN;
vq(I) = NaN;
wq(I) = NaN;

% RESHAPE GRID MATRIX INTO VECTOR
xq = xq(:);
yq = yq(:);
uq = uq(:);
vq = vq(:);
wq = wq(:);

% ELIMINATE POINTS MARKED BY NaN
xq = xq(~isnan(xq));
yq = yq(~isnan(yq));
uq = uq(~isnan(uq));
vq = vq(~isnan(vq));
wq = wq(~isnan(wq));

zq = ones(length(xq),1)*3.2;

% PLOT FIGURE
magq = sqrt(uq.^2 + vq.^2 + wq.^2);

figure
scatter(xq,yq,[],magq,'.');

% TRANSLATE THE COORDINATE
xq = xq + 3.50;
yq = yq + 4.46;

% EXPORT DATA IN SCHEME FORMAT FOR FLUENT USE
nrow = length(xq);

fileID = fopen('or-inlet','w');

fprintf(fileID,'%s %d %s\n','((or-inlet-profile point', nrow,')');

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
