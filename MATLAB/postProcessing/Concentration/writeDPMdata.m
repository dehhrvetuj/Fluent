function [ D ] = writeDPMdata(X, Y, z, C, filename)
% Write data to a text file for paraview processing
% Data C should be on X-Y plane, of a dimension M X N.
% Input z should be a scalar
% Input X and Y should be vector, whose the length is M+1 and N+1 respectively


[M, N] = size(C);

e1 = ones(M,1);
e2 = ones(N,1);

% create left and right band matrix
A = spdiags([e1 e1], 0:1, M-1, M);
B = spdiags([e2 e2], -1:0, N, N-1);

% expand one more row and one more colomn
D = zeros(M+1,N+1);

% contruct the internal elements
D(2:end-1,2:end-1) = A*C*B./4;

% extrapolate the top and bottom edge
D(1,2:end-1) = D(2,2:end-1);
D(end,2:end-1) = D(end-1,2:end-1);

% extrapolate the left and right edge
D(2:end-1,1) = D(2:end-1,2);
D(2:end-1,end) = D(2:end-1,end-1);

% process the four vertices
D(1,1) = (D(1,2) + D(2,1))./2;
D(1,end) = (D(1,end-1) + D(2,end))./2;
D(end,1) = (D(end-1,1) + D(end,2))./2;
D(end,end) = (D(end-1,end) + D(end,end-1))./2;



[X, Y] = meshgrid(X, Y);

x = X(:); 
y = Y(:); 
z = z*ones(M+1,N+1);
z = z(:);

Concentration = D(:);

% construct a table
T = table(x, y, z, Concentration);

% write to file
writetable(T, filename);



end

