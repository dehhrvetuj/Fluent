clc
clear all
close all


% The outline must be closed curve

filepath = 'C:\Users\congwang\Desktop\Dummy.jpg';

% Read the image
im=imread(filepath);                
figure,imshow(im,[]);title('Raw'); 

% Convert to gray
im=rgb2gray(im);
figure,imshow(im,[]);title('Gray'); 

% Convert to binary
im=im2bw(im, 0.9);                      
figure,imshow(im,[]),title('BW');   

% Fill holes
im2=imfill(~im,'holes');
figure,imshow(im2,[]); title('')  

% Extract outline
im3=bwperim(im2);   

% Display
figure,imshow(~im3,[]);

rowsum = sum(im3,2);
colsum = sum(im3,1);

I = rowsum>0;
J = colsum>0;




[~,upper] = max(diff(I));
[~,lower] = min(diff(I));
upper = upper+1;

[~,left] = max(diff(J));
[~,right] = min(diff(J));
left = left+1;

middle = floor((left+right)/2);

x=[];
y=[];

data=im3(upper:lower, left:middle);
[m,n] = size(data);

for i=1:m
    for j=1:n
        if data(i,j)>0
            x=[x;i];
            y=[y;j];
        end
    end
end

figure
plot(x,y,'.')
            
xx = x(diff(x)>0);
yy = y(diff(x)>0);

% 1.7 m height
scale = 1.7/(max(xx)-min(xx));

xx=xx.*scale;
yy=yy.*scale;

yy=yy-max(yy);
yy(end)=0;

group=ones(length(xx),1);
number=(1:length(xx))';
z=zeros(length(xx),1);

points=[group number xx yy z];


    
    
    
    