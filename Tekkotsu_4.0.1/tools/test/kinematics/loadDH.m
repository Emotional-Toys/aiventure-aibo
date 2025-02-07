function dh = loadDH(file)
%LOADDH loads Denavit-Hartenberg parameters from a text file
%The only thing this does over matlab's own 'load' is convert
%angles from degrees to radians
%Data columns should be theta, d, alpha, a
dh=load(file);
dh(:,[1 3])=dh(:,[1 3])/180*pi;
