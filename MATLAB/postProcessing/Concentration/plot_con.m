function plot_con( varargin )


for i=1:nargin
    
    
    figure
    imagesc(varargin{i});
    colormap jet
    colorbar
    caxis([0 10]);

    
end



end

