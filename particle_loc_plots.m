% A file for plotting the Brownian Motion of 2D particles

%%%% EDIT THESE ACCORDING TO C++ CODES
num_parts = 10;             % In serial.cpp
escape_wall_width = 5;      % In Particle.h
height = 800;
width = 800;

% Open the files and read the data
fid = fopen('particle_locs.txt');
[partLocs, countPart] = fscanf(fid, '%g', [num_parts*2 inf]);

figure;
for i=1:num_parts
    cols = size(partLocs,2);
    % Make sure (0,0) ending points are not plotted
    if(partLocs(2*i-1, cols) == 0 && partLocs(2*i, cols) == 0)
        scatter(partLocs(2*i-1,1:cols-1), partLocs(2*i,1:cols-1), '.'); 
    % Find which one escaped
    elseif(partLocs(2*i-1, cols) > width/2)
        escapee_index = 2*i-1;
        scatter(partLocs(2*i-1,:), partLocs(2*i,:), '+');
    else 
        scatter(partLocs(2*i-1,:), partLocs(2*i,:), '+');
    end
    hold on;
end
%axis([-0.5,0.5,-0.5,0.5]);
axis([-width/2, width/2, -height/2, height/2]);
%axis([width/2, width/2+5, -escape_wall_width, escape_wall_width]);

% Plot the path of escaped particle and the wall
figure;
plot(partLocs(escapee_index,:), partLocs(escapee_index+1,:));
hold on;
x=[width/2, width/2];
y=[-height/2, -escape_wall_width];
plot(x, y, 'r')
hold on;
y=[escape_wall_width, height/2];
plot(x, y, 'r')
axis([width/2-50, width/2+50, -escape_wall_width-20, escape_wall_width+20]);