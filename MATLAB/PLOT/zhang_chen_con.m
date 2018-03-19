clc
clear all
close all

% data = importdata('D:\Cong\KTH\PhD2\Projects\Validation\Zhang_Chen\Experiment\ZhangandChen.xlsx');
% data = data.data;
% save

load;

con_exp = data.Concentration;
con_simulation = data.Simulation0x28particle0x290x28AVE0x29;
% con_simulation = data.Simulation0x28particle0x29;
con_simulation = con_simulation(2:end,:);

exp_pos_con = con_exp(1:5,4);

% set uniformly distributed vertical locations
z = 0:0.05:2.4;
z = z(:);

tol = 0.005;

% ---------------------------------------
% plot properties 
linewidth = 1.6;
fontsize = 9;
markersize = 28;
% -------------
% Figure properties 
xwidth = 24;    % cm - width of A4 paper
ywidth = 6;     % cm -
NUM = 6;        % number of subplots
% -------------

for i=1:NUM

    j=i-1;
    
    % get simulated concentration
    pos = con_simulation(:,1+j*4);    
    con = con_simulation(:,3+j*4);

    pos = pos(~isnan(pos));
    con = con(~isnan(pos));
    
    % get experimental concentration
    exp_con = con_exp(1+j*7:5+j*7,6);
    
    
    % construct splines for concentration
    sp_c = spaps(pos,con,tol);
    con = fnval(sp_c,z);
    
    
    % ----------------------- Particle Concentration --------------------%
    % plot concentration
    figure(1)
    
    % subplot('Position',pos) creates axes in the custom position specified by pos. 
    % specify pos as a four-element vector of the form [left bottom width height].
    gap_left = 0.042; gap_between = 0.058; gap_bottom = 0.18;
    width = 0.105; height = 0.8;
    space = gap_between + width; % make sure space <= (100%-2*gap0) / 7 = ;
    
    axs(i) = subplot(1,6,i,'Position',[gap_left+j*space, gap_bottom, width, height]);
   
    % plot smoothed simulated velocity
    points = fnplt(csapi(z, con));
    plot(points(2,:),points(1,:),'r--','LineWidth',linewidth);
    
    % set properties
    set(gca,'XMinorTick','on','YMinorTick','on');
    set(gca,'LineWidth',linewidth/2,'FontSize',fontsize);
    
    xlabel('C/C_e_x_h_a_u_s_t','FontSize',8,'FontWeight','bold','Color','k');
    ylabel('Z [m]','FontSize',8,'FontWeight','bold','Color','k');
    text(0.7,0.9,['P' num2str(i)],'FontSize',8,'FontWeight','bold','Color','k','Units','normalized')
    ax = axs(i); ax.XRuler.MinorTick = 0:0.2:3; ax.YRuler.MinorTick = 0:0.1:2.4;
    
    
    % plot measured velocity
    hold on
    scatter(exp_con, exp_pos_con, markersize, 'k', 'filled', 's');
    
    % set figure properties
    axis([0 3 0 2.4]);
    
 
    
end


figure(1)
hFig = figure(1);
set(gcf,'PaperPositionMode','auto','Units','centimeters')
set(hFig, 'Position', [10 10 xwidth ywidth])    % [left bottom width height]
for i=1:NUM 
    set(get(axs(i),'xlabel'),'Position',[0.7 -0.1],'Units','normalized');
end
% print('concentration','-dmeta')

