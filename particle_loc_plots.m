num_parts = 10;

% Open the files and read the data
fid = fopen('particle_locs.txt');
[partLocs, countPart] = fscanf(fid, '%g', [num_parts*2 inf]);

height = 800;
width = 800;

figure;
for i=1:num_parts
    cols = size(partLocs,2);
    % Make sure (0,0) ending points are not plotted
    if(partLocs(2*i-1, cols) == 0 && partLocs(2*i, cols) == 0)
        scatter(partLocs(2*i-1,1:cols-1), partLocs(2*i,1:cols-1), '.'); 
    else
        scatter(partLocs(2*i-1,:), partLocs(2*i,:), '+');
    end
    hold on;
end
%axis([-0.5,0.5,-0.5,0.5]);
axis([-width/2, width/2, -height/2, height/2])