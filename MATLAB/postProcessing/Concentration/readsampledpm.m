clc
clear all
close all

% dpm sample file name
filename = 'plane-23.dpm';

% total number of released streams
N = 845600;

% number of repeats
NR = 3;

% total mass flow rate
M = 1;                      % kg/s

% particle diameter, density and mass
Dp = 12E-6;                 % m
rho = 1000;                 % kg/m3
Mp = rho*(pi/6*Dp^3);       % kg

% source strengh
Sr = 4*4;                   % 4CFU/s X 4 persons

% sampling time
T = 3600;                   % second

% scale
scale = Sr/(NR*M/Mp);

% read/load DPM sample data
sample_data = ReadDPMSampleData(filename);

[n, ~] = size(sample_data);



% dimension of the plane
% xmin = min(sample_data(:,1));
% ymin = min(sample_data(:,2));
% zmin = min(sample_data(:,3));

xmin = 7.00; xmax = 13.5;
ymin = 1.35; ymax = 7.75;
zmin = 1.0; zmax = 1.00;

xmin = 8.00; xmax = 12.5;
ymin = 2.35; ymax = 6.75;
zmin = 1.0; zmax = 1.00;

% Space 
space = 0.01;   % 1 centimeter

nX = floor((xmax-xmin)/space);
nY = floor((ymax-ymin)/space);
nZ = floor((zmax-zmin)/space);

X = linspace(xmin, xmax, nX+1)';
Y = linspace(ymin, ymax, nY+1)';
Z = linspace(zmin, zmax, nZ+1)';



dX = X(2)-X(1);
dY = Y(2)-Y(1);
%dZ = Z(2)-Z(1);

dA = dX*dY;

% particle number
particle_number = zeros(nY, nX);

% particle number density
particle_concen = zeros(nY, nX);

% trajectory number
trajectory_number = zeros(nY, nX);

temp = zeros(n,1);

for i=1:n
    x = sample_data(i,1);
    y = sample_data(i,2);
    z = sample_data(i,3);
    
    
    u = sample_data(i,4);
    v = sample_data(i,5);
    w = sample_data(i,6);
    
    
    mfp = sample_data(i,9);     % mass flow rate of the i-th stream
    mp = sample_data(i,10);     % mass of particle at the i-th stream
    
    if (x<=xmin || x>=xmax || y<=ymin || y>=ymax)
        continue;
    end
        
    I = floor((y-ymin)/dY)+1;
    J = floor((x-xmin)/dX)+1;
    
    % counting the particle number
    particle_number(I,J) = particle_number(I,J) + mfp/mp;
    
    % counting the trajectory
    trajectory_number(I,J) = trajectory_number(I,J) + 1;
    
%     if (I==204 && J==642)
%         disp('%%%%%%%%%%%%%%%%%%%%%%%');
%     end
    
    % compute particle volume concentration
    
    dI = fix(v/w);      % round towards zero
    dJ = fix(u/w);      % round towards zero
    
    dm = mfp/((abs(dI)+1)*(abs(dJ)+1));       % equally distribute the concentration from the i-th stream
    
    I_ = min(max(I+dI,0),nY);
    J_ = min(max(J+dJ,0),nX);
    
%     temp(i,1) = (dm/mp) / (dA*abs(w)) * S /(NR*M/Mp);
    
%     if ( temp(i,1) > 20)
%         disp(temp(i,1));
%     end
   
    
    particle_concen(I:I_, J:J_) = particle_concen(I:I_, J:J_) + (dm/mp) / (dA*abs(w)); %(dm/mp) / (dA*abs(w));
    
    %particle_concen(I,J) = particle_concen(I,J) + (mfp/mp) / (dA*sqrt(w^2+0*u^2+0*v^2));

end

clear x y z u v w I J dI dJ I_ J_ mp mfp dm;

% Area of Plane
A = (xmax-xmin)*(ymax-ymin);

% CFU/m2 per hour: number of particles deposit
disp('BCP depoist per hour: CFU/m2');
% deposit = sum(sum(particle_number)) * scale * T / A;
particle_number = particle_number*scale;
disp(sum(sum(particle_number))*T/A);

% CFU/m3: renormalize particle concentration
particle_concen = particle_concen * scale;



x_ = (X(1:end-1)+X(2:end))/2;
y_ = (Y(1:end-1)+Y(2:end))/2;


writeDPMdata(X, Y, zmin, particle_concen, 'paraview.txt');


% figure
% H2=imagesc(x_, y_, particle_concen);
% colormap jet
% colorbar
% caxis([0 10]);

A = particle_concen;
B = medfilt2(A, [3 3]); 
C = filter2(fspecial('average',[3,3]),A);
D = imgaussfilt(B, 2);
% 
% close all
% clc
% 
% [E, F] = AveragingFilter(B, trajectory_number, 9);
% 
% figure
% imagesc(x_, y_, A);
% colormap jet
% colorbar
% 
% 
% figure
% imagesc(x_, y_, B);
% colormap jet
% colorbar
% 
% figure
% imagesc(x_, y_, C);
% colormap jet
% colorbar
% 
% figure
% imagesc(x_, y_, D);
% colormap jet
% colorbar




% x = sample_data(:,1);
% y = sample_data(:,2);
% z = sample_data(:,3);
% Dp = sample_data(:,7);
% 
% scale = S/(NR*M/Mp);
% C = sample_data(:,9)./abs(sample_data(:,6))./Mp*scale;
% 
% T = table(x, y, z, Dp, C);

% writetable(T,'trajectories.txt');



