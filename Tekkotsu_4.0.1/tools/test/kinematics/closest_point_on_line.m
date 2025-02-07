function Pb=closest_point_on_line(p,vec,o)
%closest point on line returns the point of intersection of vec and shortest line connecting p to vec
%p is the point
%vec is the vector
%[optional] o is the position of origin which p and v are relative to (assumes [0 0 0] otherwise)
if nargin==2,
	c1=p.*vec;
	c2=vec.*vec;
	b=c1/c2;
	Pb=b*vec;
else
	v=vec-o;
	w=p-o;
	c1=w.*v;
	c2=v.*v;
	b=c1/c2;
	Pb=o+b*v;
end
