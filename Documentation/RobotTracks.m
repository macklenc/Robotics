close all; clc, clear;
R=1.25/12;
r=1/12;
W=15;
theta=10:0.001:89;
d = (4.5-(1.5./tand(theta)))/12;
baseInch=(12*d)*2;
M=0:701;
M=R*((W.*d.*sind(theta).*tand(theta/2))./(2.*d.*tand(theta/2)+r.*cosd(theta)));

plot(theta,d,'red')
hold;
plot(theta,M)
xlabel('Track Slope (deg)');
legend('show')
hleg1 = legend('Torque Reqd','d');
set(hleg1,'Location','NorthWest')
grid on

figure

plot(theta,baseInch)