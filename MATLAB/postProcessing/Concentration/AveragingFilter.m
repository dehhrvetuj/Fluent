function [ target, source ] = AveragingFilter( target, source, maxN )

critical_value = 3;

[m,n] = size(source);

I = ones(m,n);

N = ceil(maxN/2);

temp_s = zeros(m, n, N);
temp_t = zeros(m, n, N);

temp_s(:,:,1) = source;
temp_t(:,:,1) = target;


for i=2:N
    
    order = 2*i-1;
    
    temp_s(:,:,i) = filter2(fspecial('average',[order,order]), source) ... 
                    *order^2;
    
    temp_t(:,:,i) = filter2(fspecial('average',[order,order]), target);
    
    I = I + (temp_s(:,:,i)<=critical_value);

end




for i=1:m
    for j=1:n
        
        source(i,j) = temp_s(i,j,I(i,j));
        target(i,j) = temp_t(i,j,I(i,j));
        
    end
end
