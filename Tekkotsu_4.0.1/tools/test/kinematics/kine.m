function T=kine(dh,link)

T=eye(4,4);

for x=1:link,
	T=T*dhtransform(dh(x,:));
end
