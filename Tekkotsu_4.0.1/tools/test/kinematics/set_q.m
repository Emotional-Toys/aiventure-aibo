function dh = set_q(origdh,type,q)
% this function returns a Denavit-Hartenberg parameter set with the
% specified joint position, handling both rotational and prismatic joints
% origdh - the DH parameterization, which may contain a position offset
% type - indicates 0 for rotational or 1 for prismatic for each joint
% q - the vector of joint positions, relative to position specified in origdh
% 
% the idea is the origdh position is the hardware zero point, and
% q is the position relative to that zero point, so the position in q
% corresponds to the same position on the actual hardware

len=min([size(origdh,1) size(q,1)]);
rot_idxs=find(type(1:len)==0);
prism_idxs=find(type(1:len)==1);

dh=origdh;
dh(rot_idxs,1)=dh(rot_idxs,1)+q(rot_idxs,1);
dh(prism_idxs,2)=dh(prism_idxs,2)+q(prism_idxs,1);
