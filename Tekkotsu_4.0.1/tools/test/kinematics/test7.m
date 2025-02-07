load;
clf;

p=[100 90 -110 1]';
iplstyle='c-';
ipestyle='co';

disp('LFr7');
%lfr_q=zeros(size(lf7,1),1);
%dhvisualize(lf7,leg_type,lfr_q,front_leg_range7,15);
lfr_q=inv_kin(lf7,leg_type,front_leg_range7,false,p,5,[10 10 10 1]',0,0)
%for x=1:size(IP7LFr,1)
%	ipvisualize(lf7,leg_type,lfr_q,IP7LFr(x,1),[IP7LFr(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('RFr7');
rfr_q=zeros(size(rf7,1),1);
%dhvisualize(rf7,leg_type,rfr_q,front_leg_range7,15);
rfr_q=inv_kin(rf7,leg_type,front_leg_range7,false,[p(1) -p(2) p(3) p(4)]',5,[10 10 -10 1]',0,0)
%for x=1:size(IP7RFr,1)
%	ipvisualize(rf7,leg_type,rfr_q,IP7RFr(x,1),[IP7RFr(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('LBk7');
lbk_q=zeros(size(lh7,1),1);
%dhvisualize(lh7,leg_type,lbk_q,front_leg_range7,15);
lbk_q=inv_kin(lh7,leg_type,hind_leg_range7,false,[-p(1) p(2) p(3) p(4)]',5,[10 10 10 1]',0,0)
%for x=1:size(IP7LBk,1)
%	ipvisualize(lh7,leg_type,lbk_q,IP7LBk(x,1),[IP7LBk(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('RBk7');
rbk_q=zeros(size(rh7,1),1);
%dhvisualize(rh7,leg_type,rbk_q,front_leg_range7,15);
rbk_q=inv_kin(rh7,leg_type,hind_leg_range7,false,[-p(1) -p(2) p(3) p(4)]',5,[10 10 -10 1]',0,0)
%for x=1:size(IP7RBk,1)
%	ipvisualize(rh7,leg_type,rbk_q,IP7RBk(x,1),[IP7RBk(x,2:4)'; 1],iplstyle,ipestyle)
%end

disp('Cam7');
cam_q=inv_kin(cam7,cam_type7,cam_range7,true,[300 -50 190 1]',6,[0 0 1 0]',0,1)
%for x=1:size(IP7Camera,1)
%	ipvisualize(cam7,cam_type7,cam_q,IP7Camera(x,1),[IP7Camera(x,2:4)'; 1],iplstyle,ipestyle)
%end










%p=[0 90 0 1]';  %some out of reach points aren't handled very well...
