function y=limit_angle(x,mn,mx)
if x<mn || x>mx
	mn_dist=abs(normalize_angle(mn-x));
	mx_dist=abs(normalize_angle(x-mx));
	if mn_dist<mx_dist
		y=mn;
	else
		y=mx;
	end
else
	y=x;
end

function r=normalize_angle(x)
r=x-round(x/(pi*2))*pi*2;

