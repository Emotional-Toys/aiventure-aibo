function q = inv_kin(dh,type,range,vec_proj,Pobj,endlink,Plink,second_invert,third_invert)
%INV_KIN_ERS_LEG_POS computes position inverse kinematics for an AIBO leg
%dh is the matrix representing the dh parameters
%Pobj is the target position
%endlink is the last link to move
%Plink is point on that link (and represented relative to that link's frame)

q=zeros(endlink,1);

if endlink>=2,
	if endlink>=3,
		if endlink>=4,
			q(4)=computeThirdERSLink(4,dh,type,range,q,Pobj,endlink,Plink,third_invert)
		end
		q(3)=computeSecondERSLink(3,dh,type,range,q,Pobj,endlink,Plink,second_invert)
	end
	q(2)=computeFirstERSLink(2,dh,type,range,q,Pobj,endlink,Plink)
end

%check if link 2 is maxed out
limited=find((q==range(1:size(q,1),1) | q==range(1:size(q,1),2)) & range(1:size(q,1),1)~=range(1:size(q,1),2));
if sum(limited==2)>0,
	%redo links 3 and 4 since link 2 was limited
	if endlink>=3,
		if endlink>=4,
			q(4)=computeSecondERSLink(4,dh,type,range,q,Pobj,endlink,Plink,second_invert)
		end
		q(3)=computeFirstERSLink(3,dh,type,range,q,Pobj,endlink,Plink)
	end
end

%check again, maybe now link 3 is maxed out as well
limited=find((q==range(1:size(q,1),1) | q==range(1:size(q,1),2)) & range(1:size(q,1),1)~=range(1:size(q,1),2));
if sum(limited==3)>0 && endlink>=4,
	%redo link 4 since link 3 was limited
	q(4)=computeFirstERSLink(4,dh,type,range,q,Pobj,endlink,Plink)
end

if vec_proj,
	%This method is an approximation, not entirely precise or fast as it could be
	%Something to work on some more down the road! :)
	poE=convertLink(dh,type,q,0,Pobj,endlink);
	if poE(4)~=0,
		poE=poE(1:3)/poE(4);
	else
		poE=poE(1:3);
	end
	plE=Plink(1:3);
	if Plink(4)~=0,
		plE=plE/Plink(4);
	end
	obj_comp_link=dot(plE,poE)/norm(plE)
	if obj_comp_link<norm(plE),
		obj_comp_link=obj_comp_link*.975; %.975 is a bit of fudge - accounts for joints moving Plink when adjusting
	else
		obj_comp_link=obj_comp_link/.975; %.975 is a bit of fudge - accounts for joints moving Plink when adjusting
	end
	obj_proj_link=[ obj_comp_link*plE/norm(plE) ; 1]
	q=inv_kin(dh,type,range,false,Pobj,endlink,obj_proj_link,second_invert,third_invert);
	ipvisualize(dh,type,q,endlink,Plink,'m:','ko');
else
	dhvisualize(dh,type,q,range,15);
	hold on;
	plot3(Pobj(1)/Pobj(4),Pobj(2)/Pobj(4),Pobj(3)/Pobj(4),'kx');
	hold off;
%	for f=1:endlink-1,
%		if type(f)==0 || type(f)==1,
%			p=convertLink(dh,type,q,endlink,Plink,f)
%			ipvisualize(dh,type,q,f,p,'c:','ko');
%		end
%	end
	ipvisualize(dh,type,q,endlink,Plink,'y-','ko');
end

function qd=computeThirdERSLink(curlink,dh,type,range,q,Pobj,endlink,Plink,invert)
disp('computeThirdERSLink');
ITOL=1e-6;
%We'll compute the knee angle first, using the
%  length of the thigh
%  distance from knee to Plink
%  distance from shoulder to Pobj
%use law of cosines to find angle at knee of Pobj and Plink, and the difference is the amount to move
pp=convertFrame(set_q(dh,type,q),curlink-1,[0 0 0 1]',curlink);
pp(3)=0;
previous_to_cur_len=homog_norm(pp) %shoulder_to_knee
pl=convertLink(dh,type,q,endlink,Plink,curlink)
if (abs(pl(1))<ITOL && abs(pl(2))<ITOL),
	%Plink is along axis of rotation, nothing we do is going to move it, so don't bother
	qd=q(curlink);
	return;
end
if Plink(4)==0,
	%We're dealing with an infinite ray
	disp('out of reach (positive infinity)');
	tl=atan2(pl(2),pl(1))
	tp=atan2(pp(2),pp(1))
	qd=normalize_angle(tp-tl)
else
	cur_to_plink_xyz_len=homog_norm(pl)
	plz=pl(3);
	pl(3)=0;
	cur_to_plink_len=homog_norm(pl)
	prev_to_pobj=convertFrame(set_q(dh,type,q),0,Pobj,curlink-1)
	prev_to_pobj_xyz_len=homog_norm(prev_to_pobj);
	prev_to_pobj(3)=0;
	tgt_len=sqrt(prev_to_pobj_xyz_len^2-plz^2)
	aor=(cur_to_plink_len^2+previous_to_cur_len^2-tgt_len^2)/(2*cur_to_plink_len*previous_to_cur_len)
	tl=atan2(pl(2),pl(1))
	tp=atan2(pp(2),pp(1))
	al=normalize_angle(tp-tl)
	%have to check if Pobj is within reach
	if isinf(aor), %qd should be set to whatever it currently is, plink is along cur's z axis
	elseif aor<-1, %Pobj is too far away - straighten out
		disp('out of reach (negative)');
		qd=pi+al;
	elseif aor>1, %Pobj is too close to center of rotation - fold up
		disp('out of reach (positive)');
		qd=al;
	else
		ao=-acos(aor)
		if invert,
			ao=-ao;
		end
		qd=ao+al;
		%qd=(pi-(pi-ao)*cur_to_plink_len/cur_to_plink_xyz_len)+al;
	end
	qd=normalize_angle(qd)
end
qd=limit_angle(qd,range(curlink,1),range(curlink,2))


function qd=computeSecondERSLink(curlink,dh,type,range,q,Pobj,endlink,Plink,invert)
disp('computeSecondERSLink');
ITOL=1e-6;
%Convert Pobj, Plink, z3 to be frame 'curlink' relative
po=convertFrame(set_q(dh,type,q),0,Pobj,curlink)
pl=convertLink(dh,type,q,endlink,Plink,curlink)
pp=convertFrame(set_q(dh,type,q),curlink-1,[0 0 0 1]',curlink)
zp=convertFrame(set_q(dh,type,q),curlink-1,[0 0 1 0]',curlink);
zp(4)=1
ao=pi/2-acos(dot(dehomog(zp),dehomog(po))/homog_norm(zp)/homog_norm(po))
%alternative method for ao: (Doesn't quite work... but the idea is there...)
%pop=convertFrame(set_q(dh,type,q),0,Pobj,curlink-1)
%pox=norm(pop(1:2)/pop(4))-norm(pp(1:2)/pp(4))
%poy=pop(3)/pop(4)+pp(3)/pp(4)
%ao=atan2(poy,pox)
if (abs(pl(1))<ITOL && abs(pl(2))<ITOL), %Plink is along axis of rotation, can't move it
	qd=q(curlink);
else
	if pl(4)==0,
		qd=computeFirstERSLink(curlink,dh,type,range,q,Pobj,endlink,Plink)
	else
		r=norm(pl(1:2)/pl(4)) %radius of link point around axis of rotation
		tors=(r^2-(pl(3)*tan(ao))^2)/(r^2+(r*tan(ao))^2)
		if tors<0,
			disp('out of reach')
			if xor(dot(zp(1:end-1),po(1:end-1))>0,dot(zp(1:end-1),pp(1:end-1))<0)
				qd=-pi/2;
			else
				qd=pi/2;
			end
		else
			to=acos(sqrt(tors))
			if xor(dot(zp(1:end-1),po(1:end-1))>0,dot(zp(1:end-1),pp(1:end-1))<0)
				to=-to;
			end
			if invert,
				to=pi-to
			end
			tl=atan2(pl(2),pl(1))
			qd=normalize_angle(to-tl);
		end
		qd=limit_angle(qd,range(curlink,1),range(curlink,2))
	end
end

function qd=computeFirstERSLink(curlink,dh,type,range,q,Pobj,endlink,Plink)
disp('computeFirstERSLink');
ITOL=1e-6;
%Convert Pobj and Plink to be frame 'curlink' relative
po=convertFrame(set_q(dh,type,q),0,Pobj,curlink)
pl=convertLink(dh,type,q,endlink,Plink,curlink)
if (abs(pl(1))<ITOL && abs(pl(2))<ITOL), %Plink is along axis of rotation, can't move it
	qd=q(curlink);
else
	to=atan2(po(2),po(1))
	tl=atan2(pl(2),pl(1))
	qd=normalize_angle(to-tl)
	qd=limit_angle(qd,range(curlink,1),range(curlink,2));
end

function v=convertFrame(dh,cur,A,dest)
v=inv(kine(dh,dest-1))*kine(dh,cur-1)*A;

function v=convertLink(dh,type,q,cur,A,dest)
if cur==0
	v=rotz(-q(dest))*convertFrame(set_q(dh,type,q),cur,A,dest);
else
	v=rotz(-q(dest))*convertFrame(set_q(dh,type,q),cur,rotz(q(cur))*A,dest);
end

function w=dehomog(v)
w=v(1:end-1)/v(end);

function n=homog_norm(x)
n=norm(dehomog(x));

function r=normalize_angle(x)
r=x-round(x/(pi*2))*pi*2;

function T=rotz(t)
T=dhtransform([t 0 0 0]);
