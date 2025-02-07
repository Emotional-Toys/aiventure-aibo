function ipvisualize(dh,type,q,endlink,p,lstyle,estyle)
%IPVISUALIZE draws a link from endlink to p
%dh is the robot construction in denavit-hartenberg convention
%type is the types of each joint (0 is rotational, 1 is prismatic, 2 is fixed (immobile))
%q is the current joint positions
%endlink is the link p is relative to
%p is the point on endlink
%lstyle is the plot style to use for line
%estyle is the plot style to use for endpoint
if(p(4)==0),
	return
end
conf=set_q(dh,type,q);
hold on
fPlink=kine(conf,endlink-1)*rot_z(q(endlink))*p;
elink=kine(conf,endlink-1)*[0 0 0 1]';
plot3(fPlink(1)/fPlink(4),fPlink(2)/fPlink(4),fPlink(3)/fPlink(4),estyle);
plot3([elink(1) ; fPlink(1)/fPlink(4)],[elink(2) ; fPlink(2)/fPlink(4)],[elink(3) ; fPlink(3)/fPlink(4)],lstyle);
hold off

function T=rot_z(t)
T=dhtransform([t 0 0 0]);
