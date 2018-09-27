clc
clear all
close all

NEW_READ = false;
NEW_COMPUTE = true;
NEW_WRITE = false;
PLOT_CON = false;

if NEW_COMPUTE

clear NEW_COMPUTE;

% dpm sample file name
filename = './5000/table_upper.dpm';
filename = './new/ACH60/instrument_upper.dpm';
% filename = './13200/table_upper.dpm';


% total number of released streams per each particle tracking
N = 5541000; %859200; %845600;

% number of repeats
NR = 1;

% total mass flow rate
M = 1E-20;                      % kg/s

% particle diameter, density and mass
Dp = 12E-6;                 % m
rho = 1000;                 % kg/m3
Mp = rho*(pi/6*Dp^3);       % kg

% source strengh
Sr = 40;                   % 5CFU/s X 8 persons

% sampling time
T = 3600;                   % second/hour

% Read DPM text file
sample_data = ReadDPMSampleData(filename);



% number of CFU
sum(sample_data(:,9))/M*Sr*T/NR


end