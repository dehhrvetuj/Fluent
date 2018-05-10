clc
clear all
close all

data=importdata('D:\Cong\KTH\PhD2\Classification\Journals\PaperI\Figures.xlsx');
data = data.data;
data = data.Blad2_4;

OT = data(1:9,[6, 3]);


close all

opengl hardware

set(0,'DefaultLineLineSmoothing','on');
set(0,'DefaultPatchLineSmoothing','on');
opengl('OpenGLLineSmoothingBug',1);


% plot properties 
linewidth = 2;
fontsize = 14;
markersize = 96;
% -------------
% Figure properties 
xwidth = 42;    % cm - width of A4 paper
ywidth = 14;     % cm -
NUM = 3;        % number of subplots
% -------------


figure(1)
hFig = figure(1);
set(gcf,'PaperPositionMode','auto','Units','centimeters')
set(hFig, 'Position', [4 4 xwidth ywidth])    % [left bottom width height]


gap_left = 0.045; gap_between = 0.099; gap_bottom = 0.18;
width = 0.25; height = 0.75;
space = gap_between + width; % make sure space <= (100%-2*gap0) / 7 = ;


    
    
for i=1:NUM
    axs(i) = subplot(1,3,i,'Position',[gap_left+(i-1)*space, gap_bottom, width, height]);
    hold on
    
    set(gca,'YScale','log','XScale','log')
    set(gca,'XMinorTick','on','YMinorTick','on');
    set(gca,'LineWidth',linewidth/2,'FontSize',fontsize);
    

    xlabel('Airflow Rate [m^3/s]','FontSize',12,'FontWeight','bold','Color','k');
    ylabel('CFU / m^3','FontSize',12,'FontWeight','bold','Color','k');
    box on

    
    
    scatter(OT(1:3,1),OT(1:3,2), markersize,'k','filled','s','LineWidth',6);
    scatter(OT(4:6,1),OT(4:6,2), markersize,'k','filled','^')
    scatter(OT(7:9,1),OT(7:9,2), markersize,'k','filled','d','LineWidth',12)
  
    plot(Q,C,'k--','LineWidth',linewidth)
    
%     axis([0.8 4 0.01 100]);
    ax = axs(i); ax.XRuler.MinorTick = 0.4:0.1:4;
    set(gca,'XLim',[0.8 4],'XTick',[1 2 3 4]);
%     xticks([1 2 3 4])


end


    
    
% 
%     
% axs = subplot(1,3,1,'Position',[gap_left, gap_bottom, width, height]);
% 
% % subplot(1,3,1)
% xlabel('Airflow Rate [m^3/s]','FontSize',8,'FontWeight','bold','Color','k');
% ylabel('CFU / m^3','FontSize',8,'FontWeight','bold','Color','k');
% box on
% 
% subplot(1,3,2,'Position',[gap_left+space, gap_bottom, width, height])
% xlabel('Airflow Rate','FontSize',8,'FontWeight','bold','Color','k');
% ylabel('Z [m]','FontSize',8,'FontWeight','bold','Color','k');
% box on
% 
% subplot(1,3,3,'Position',[gap_left+space*2, gap_bottom, width, height])
% xlabel('Airflow Rate','FontSize',8,'FontWeight','bold','Color','k');
% ylabel('Z [m]','FontSize',8,'FontWeight','bold','Color','k');
% box on
%    
