function d=dist_point_to_vector(p,vec,o)
%DIST_POINT_TO_VECTOR returns distance of shortest line connecting p to vec
%p is the point
%vec is the vector
%[optional] o is the position of origin which p and v are relative to (assumes [0 0 0] otherwise)
if nargin==2,
	c1=dot(p,vec);
	c2=dot(vec,vec);
	b=c1/c2;
	Pb=b*vec;
else
	v=vec-o;
	w=p-o;
	c1=dot(w,v);
	c2=dot(v,v);
	b=c1/c2;
	Pb=o+b*v;
end
d=norm(p-Pb);
