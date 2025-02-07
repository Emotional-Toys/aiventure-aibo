function d=vector_from_point_to_vector(p,vec,o)
%VECTOR_FROM_POINT_TO_VECTOR returns the shortest line connecting p to vec
%p is the point
%vec is the vector
%[optional] o is the position of origin which p and v are relative to
if nargin==2,
	o=[0 0 0];
end

v=vec-o;
w=p-o;
c1=dot(w,v);
c2=dot(v,v);
b=c1/c2;
Pb=o+b*v;
d=norm(p-Pb);
