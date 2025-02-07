load;
clf;

p=[100 90 -110 1]';
iplstyle='c-';
ipestyle='co';

disp('LFr2xx');
%lfr_q=zeros(size(lf2xx,1),1);
%dhvisualize(lf2xx,leg_type,lfr_q,leg_range2xx,15);
lfr_q=inv_kin(lf2xx,leg_type,leg_range2xx,false,p,5,[10 10 10 1]',0,0)
%for x=1:size(IP2xxLFr,1)
%	ipvisualize(lf2xx,leg_type,lfr_q,IP2xxLFr(x,1),[IP2xxLFr(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('RFr2xx');
rfr_q=zeros(size(rf2xx,1),1);
%dhvisualize(rf2xx,leg_type,rfr_q,leg_range2xx,15);
rfr_q=inv_kin(rf2xx,leg_type,leg_range2xx,false,[p(1) -p(2) p(3) p(4)]',5,[10 10 -10 1]',0,0)
%for x=1:size(IP2xxRFr,1)
%	ipvisualize(rf2xx,leg_type,rfr_q,IP2xxRFr(x,1),[IP2xxRFr(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('LBk2xx');
lbk_q=zeros(size(lh2xx,1),1);
%dhvisualize(lh2xx,leg_type,lbk_q,leg_range2xx,15);
lbk_q=inv_kin(lh2xx,leg_type,leg_range2xx,false,[-p(1) p(2) p(3) p(4)]',5,[10 10 10 1]',0,0)
%for x=1:size(IP2xxLBk,1)
%	ipvisualize(lh2xx,leg_type,lbk_q,IP2xxLBk(x,1),[IP2xxLBk(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('RBk2xx');
rbk_q=zeros(size(rh2xx,1),1);
%dhvisualize(rh2xx,leg_type,rbk_q,leg_range2xx,15);
rbk_q=inv_kin(rh2xx,leg_type,leg_range2xx,false,[-p(1) -p(2) p(3) p(4)]',5,[10 10 -10 1]',0,0)
%for x=1:size(IP2xxRBk,1)
%	ipvisualize(rh2xx,leg_type,rbk_q,IP2xxRBk(x,1),[IP2xxRBk(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('Cam210');
cam_q=inv_kin(cam2xx,cam_type2xx,cam_range210,true,[280 -50 100 1]',5,[30 -10 50 1]',0,1)
%for x=1:size(IP2xxCamera,1)
%	ipvisualize(cam2xx,cam_type2xx,cam_q,IP2xxCamera(x,1),[IP2xxCamera(x,2:4)'; 1],iplstyle,ipestyle)
%end
