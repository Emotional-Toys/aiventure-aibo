function dhvisualize(origdh,type,q,range,axlen)
dh=set_q(origdh,type,q);
hold on;
lp=zeros(3,size(dh,1)+1);
for link=0:size(dh,1),
	lp(:,link+1)=dehomog(kine(dh,link)*[0 0 0 1]');
end
plot3(lp(1,:),lp(2,:),lp(3,:),'g-');
plot3(lp(1,:),lp(2,:),lp(3,:),'ko');
limited=find((q==range(1:size(q,1),1) | q==range(1:size(q,1),2)) & range(1:size(q,1),1)~=range(1:size(q,1),2));
for link=limited',
	plot3(lp(1,link:link+1),lp(2,link:link+1),lp(3,link:link+1),'m--');
end
axis equal

for link=0:size(dh,1),
	axx=dehomog(kine(dh,link)*[[0 0 0 1];[axlen 0 0 1]]');
	axy=dehomog(kine(dh,link)*[[0 0 0 1];[0 axlen 0 1]]');
	axz=dehomog(kine(dh,link)*[[0 0 0 1];[0 0 axlen 1]]');
	plot3(axx(1,:),axx(2,:),axx(3,:),'r-');
	plot3(axy(1,:),axy(2,:),axy(3,:),'b-');
	plot3(axz(1,:),axz(2,:),axz(3,:),'k-');
end

%hilight origin
plot3(lp(1,1),lp(2,1),lp(3,1),'m*');

hold off;

function p=dehomog(ph)
for c=1:size(ph,2)
	p(:,c)=ph(1:3,c)/ph(4,c);
end
