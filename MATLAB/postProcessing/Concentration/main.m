clc
clear all
close all

NEW_READ = true;
NEW_COMPUTE = true;
NEW_WRITE = true;
PLOT_CON = true;

if NEW_COMPUTE

clear NEW_COMPUTE;

% dpm sample file name
filename = '../../LAF/plane-23.dpm';

% total number of released streams per each particle tracking
N = 5541000; %859200; %845600;

% number of repeats
NR = 1;

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


% dimension of the plane
% xmin = 7.00; xmax = 13.5;
% ymin = 1.35; ymax = 7.75;
% zmin = 1.0; zmax = 1.00;

xmin = 0.0; xmax = 6.4;
ymin = 0.0; ymax = 6.3;
zmin = 1.0; zmax = 1.0;

% xmin = 2.2; xmax = 4.2;
% ymin = 2.88; ymax = 3.42;
% zmin = 1.0; zmax = 1.0;

% Space 
space = 0.01;   % 1 centimeter

% scale
scale = Sr/(NR*M/Mp);

% read/load DPM sample data
if NEW_READ
    clear NEW_READ;
    sample_data = ReadDPMSampleData(filename);
    save sample_data;
else
    load sample_data;
end

compute;

else
    
load; 
    
end

% ---- filtering the data like an image -----
A = particle_concen;
B = medfilt2(A, [3 3]); 
C = filter2(fspecial('average',[3,3]),A);
D = imgaussfilt(A, 2);



% ---- plot the image ---------------------
if PLOT_CON
    plot_con(A,B,C,D);
end


% ------ write to text file ----------------

if NEW_WRITE
    
% x_ = (X(1:end-1)+X(2:end))/2;
% y_ = (Y(1:end-1)+Y(2:end))/2;


writeDPMdata(X, Y, zmin, D, [filename '.txt']);


end




