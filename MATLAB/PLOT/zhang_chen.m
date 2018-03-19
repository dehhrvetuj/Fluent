clc
clear all
close all

% data = importdata('D:\Cong\KTH\PhD2\Projects\Validation\Zhang_Chen\Experiment\ZhangandChen.xlsx');
% data = data.data;
% save

load;

vel_temp_exp = data.Vel_Temp;
vel_temp_simulation = data.Simulation0x28vel0x2Btemp0x29;
vel_temp_simulation = vel_temp_simulation(3:end,1:35);

exp_pos = vel_temp_exp(1:7,4);


% set uniformly distributed vertical locations
z = 0:0.1:2.4;
z = z(:);

tol = 0.0;

% ---------------------------------------
% plot properties 
linewidth = 1.6;
fontsize = 9;
markersize = 28;
% -------------
% Figure properties 
xwidth = 24;    % cm - width of A4 paper
ywidth = 8;     % cm -
% -------------

for i=1:7

    j=i-1;
    
    % get simulated velocity and temperature
    pos = vel_temp_simulation(:,1+j*5);
    vel = vel_temp_simulation(:,2+j*5);
    tmp = vel_temp_simulation(:,4+j*5);
    
    pos = pos(~isnan(pos));
    vel = vel(~isnan(vel));
    tmp = tmp(~isnan(tmp));
    
    % get experimental velocity and temperature
    exp_vel = vel_temp_exp(1+j*9:7+j*9,6);
    exp_tmp = vel_temp_exp(1+j*9:7+j*9,7);
    
    
    %
    sp_v = spaps(pos,vel,tol);
    vel = fnval(sp_v,z);
    
    sp_t = spaps(pos,tmp,tol);
    tmp = fnval(sp_t,z);
    tmp(end) = 0.98;
    
    
    % -------------------------------- Velocity --------------------------%
    % plot velocity
    figure(1)
    % subplot('Position',pos) creates axes in the custom position specified by pos. 
    % specify pos as a four-element vector of the form [left bottom width height].
    gap_left = 0.042; gap_between = 0.058; gap_bottom = 0.17;
    width = 0.080; height = 0.8;
    space = gap_between + width; % make sure space <= (100%-2*gap0) / 7 = ;
    
    subplot(1,7,i,'Position',[gap_left+j*space, gap_bottom, width, height]);
   
    % plot smoothed simulated velocity
    points = fnplt(csapi(z, vel));
    plot(points(2,:),points(1,:),'r--','LineWidth',linewidth);
    
    % set properties
    set(gca,'XMinorTick','on','YMinorTick','on');
    set(gca,'LineWidth',linewidth/2,'FontSize',fontsize);
    xlabel('V [m/s]','FontSize',8,'FontWeight','bold','Color','k');
    ylabel('Z [m]','FontSize',8,'FontWeight','bold','Color','k');
    text(0.7,0.1,['V' num2str(i)],'FontSize',8,'FontWeight','bold','Color','k','Units','normalized')
    ax = gca; ax.XRuler.MinorTick = 0:0.02:0.2; ax.YRuler.MinorTick = 0:0.1:2.4;
    
    % plot measured velocity
    hold on
    scatter(exp_vel, exp_pos, markersize, 'k', 'filled', 's');
    
    % set figure properties
    axis([0 0.2 0 2.4]);
     
    % -------------------------------- Temperature -----------------------%
    % plot temperature
    figure(2)
    subplot(2,7,i,'Position',[gap_left+j*space, gap_bottom, width, height]);
    
    % plot smoothed simulated temperature
    points = fnplt(csapi(z, tmp));
    plot(points(2,:),points(1,:),'r--','LineWidth',linewidth);
    
    % set properties
    set(gca,'XMinorTick','on','YMinorTick','on');
    set(gca,'LineWidth',linewidth/2,'FontSize',fontsize);
    xlabel('T','FontSize',8,'FontWeight','bold','Color','k');
    ylabel('Z [m]','FontSize',8,'FontWeight','bold','Color','k');
    text(0.7,0.1,['V' num2str(i)],'FontSize',8,'FontWeight','bold','Color','k','Units','normalized');
    ax = gca; ax.XRuler.MinorTick = 0:0.1:1.5; ax.YRuler.MinorTick = 0:0.1:2.4;
   
    
    % plot measured velocity    
    hold on
    scatter(exp_tmp, exp_pos, markersize, 'k', 'filled', 's');
    
    % set figure properties
    axis([0 1.5 0 2.4]);
    
end


figure(1)
hFig = figure(1);
set(gcf,'PaperPositionMode','auto','Units','centimeters')
set(hFig, 'Position', [10 10 xwidth ywidth])    % [left bottom width height]
print('velocity','-dmeta')


hFig = figure(2);
set(gcf,'PaperPositionMode','auto','Units','centimeters')
set(hFig, 'Position', [10 10 xwidth ywidth])    % [left bottom width height]
print('temperature','-dmeta')