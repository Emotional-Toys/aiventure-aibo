function T=dhtransform(dhrow)
theta=dhrow(1);
d=dhrow(2);
alpha=dhrow(3);
a=dhrow(4);
T=[ [cos(theta) , -cos(alpha)*sin(theta) ,  sin(alpha)*sin(theta) , a*cos(theta) ];
		[sin(theta) ,  cos(alpha)*cos(theta) , -cos(theta)*sin(alpha) , a*sin(theta) ];
		[   0       ,       sin(alpha)       ,       cos(alpha)       ,      d       ];
		[   0       ,           0            ,           0            ,      1       ] ];

