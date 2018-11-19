clc
clear all
close all


% GENERATE RECTANGULAR GRID (-0.5 0.5)
[x,y] = meshgrid(-0.5:0.005:0.5);

[m,n] = size(x);

u = zeros(m,n);
v = zeros(m,n);
w = zeros(m,n);


magXY = sqrt(x.^2 + y.^2);
magZ = 1.0;
magZ = magZ/10;

theta = atan(magXY./magZ);

% w = cos(theta);
% u = x.*sin(theta)./magXY;
% v = y.*sin(theta)./magXY;

w = ones(m,n);
u = x.*tan(theta)./magXY;
v = y.*tan(theta)./magXY;


% ELIMINATE NaN (e.g. center of origin)
u(isnan(u)) = 0;
v(isnan(v)) = 0;



% RESHAPE GRID MATRIX INTO VECTOR
x = x(:);
y = y(:);
u = u(:);
v = v(:);
w = w(:);

z = zeros(length(x),1);


% PLOT DATA
maguv = sqrt(u.^2 + v.^2);
maguvw = sqrt(u.^2 + v.^2 + w.^2);

figure
scatter(x,y,[],maguv,'.');
figure
scatter(x,y,[],maguvw,'.');


% RESCALE VELOCITY TO HAVE DESIRED AIRFLOW RATE
V = 0.15; % [m^3/s] since the area is 1 m^2;

u = u./mean(w)*V;
v = v./mean(w)*V;
w = w./mean(w)*V;

% TRANSLATE/SCALE THE COORDINATE
L = 0.6;
W = 0.6;

x = x*L;
y = y*W;

% x = x + 9.30;
% y = y + 3.10;
% z = z + 3.20;

% x = x + 9.30;
% y = y + 6.65;
% z = z + 3.20;

x = [x + 9.30; x + 9.30];
y = [y + 3.10; y + 6.65];
z = [z + 3.20; z + 3.20];

u = [u; u];
v = [v; v];
w = [w; w];

w = -1.0*w;


% EXPORT DATA IN SCHEME FORMAT FOR FLUENT USE
nrow = length(x);

% fileID = fopen('data.txt','w');
% fprintf(fileID,'%s\n','x,y,z,u,v,w');
% for i=1:nrow
%     fprintf(fileID,'%f,%f,%f,%f,%f,%f\n',x(i),y(i),z(i),u(i),v(i),w(i));
% end
% 
% fclose(fileID);


fileID = fopen('cor-inlet','w');

fprintf(fileID,'%s %d %s\n','((cor-inlet-profile point', nrow,')');

fprintf(fileID,'%s\n','(x');
for i=1:nrow
    fprintf(fileID,'%f\n',x(i));
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(y');
for i=1:nrow    
    fprintf(fileID,'%f\n',y(i));    
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(z');
for i=1:nrow
    
    fprintf(fileID,'%f\n',z(i));  
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(u');
for i=1:nrow   
    fprintf(fileID,'%f\n',u(i));  
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(v');
for i=1:nrow
    fprintf(fileID,'%f\n',v(i));   
end
fprintf(fileID,'%s\n',')');

fprintf(fileID,'%s\n','(w');
for i=1:nrow  
    fprintf(fileID,'%f\n',w(i));
end
fprintf(fileID,'%s\n',')');

fclose(fileID);
