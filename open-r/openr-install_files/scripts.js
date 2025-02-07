function getScrollingPosition() {
	var position = [0, 0];

	if (typeof window.pageYOffset != 'undefined')
		position = [ window.pageXOffset, window.pageYOffset ];

	else if (typeof document.documentElement.scrollTop != 'undefined' && document.documentElement.scrollTop > 0)
		position = [document.documentElement.scrollLeft, document.documentElement.scrollTop ];

	else if (typeof document.body.scrollTop != 'undefined')
		position = [document.body.scrollLeft, document.body.scrollTop ];

	return position;
}

function setScrollingPosition(position) {
	window.scrollTo(position[0],position[1]);
}

function getWindowDimensions() {
	var myWidth = 0, myHeight = 0;
		if( typeof( window.innerWidth ) == 'number' ) {
		//Non-IE
		myWidth = window.innerWidth;
		myHeight = window.innerHeight;
	} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
		//IE 6+ in 'standards compliant mode'
		myWidth = document.documentElement.clientWidth;
		myHeight = document.documentElement.clientHeight;
	} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
		//IE 4 compatible
		myWidth = document.body.clientWidth;
		myHeight = document.body.clientHeight;
	}
	return [myWidth,myHeight];
}

var mainmenuOffsetTop=-1;
var mainmenuOffsetLeft=-1;
var indexOffsetTop=-1;
var indexOffsetLeft=-1;
var curpos=[-1,-1];

function getOffsetTop(el) {
	off=0;
	while(el) {
		off+=el.offsetTop;
		el=el.offsetParent;
	}
	return off;
}
function getOffsetLeft(el) {
	off=0;
	while(el) {
		off+=el.offsetLeft;
		el=el.offsetParent;
	}
	return off;
}

function setPos(el,left,elpos,scrpos,toppos) {
	if(el==null || elpos==-1)
		return;
	if(toppos<scrpos[1]) {
		el.style.top=(elpos-toppos)+"px";
		el.style.left=(left-scrpos[0])+"px";
		el.style.position="fixed";
		//el.style.border="3px solid red";
	} else {
		el.style.top=elpos+"px";
		el.style.left=left+"px";
		el.style.position="absolute";
		//el.style.border="3px solid white";
	}
}

window.onscroll=function() {
	if(mainmenuOffsetTop>=0) {
		scrpos=getScrollingPosition();
		if(scrpos[0]==curpos[0] && scrpos[1]==curpos[1])
			return;
		//bottom=Math.max(mainmenuOffsetTop-20,indexOffsetTop-20);
		//if(pos>bottom && curpos>bottom)
		//	return;
		curpos=scrpos;
		var mainmenu=document.getElementById('mainmenu');
		setPos(mainmenu,mainmenuOffsetLeft,mainmenuOffsetTop,scrpos,mainmenuOffsetTop-20);
		var index=document.getElementById('index');
		setPos(index,indexOffsetLeft,indexOffsetTop,scrpos,indexOffsetTop-20);
	}
}

window.onresize=function() {
	var index=document.getElementById('index');
	if(index!=null) {
		indexOffsetLeft=getOffsetLeft(index.parentNode)+15;
		index.style.width=(index.parentNode.clientWidth-30)+"px";
	}
	curpos=[-1,-1];
	window.onscroll();
}

window.onload=function() {
	var mainmenu=document.getElementById('mainmenu');
	if(mainmenu!=null) {
		mainmenuOffsetTop=getOffsetTop(mainmenu);
		mainmenuOffsetLeft=getOffsetLeft(mainmenu);
		mainmenu.parentNode.style.width=mainmenu.offsetWidth+"px";
		//mainmenu.parentNode.style.minwidth=mainmenu.offsetWidth+"px";
		
		//ugly hack to keep menu td from collapsing in firefox... ?
		var menuspacer=document.getElementById('menuspacer');
		menuspacer.style.width=mainmenu.offsetWidth+"px";
	}
	var index=document.getElementById('index');
	if(index!=null) {
		indexOffsetTop=getOffsetTop(index);
		indexOffsetLeft=getOffsetLeft(index);
		index.style.width=index.offsetWidth;
		//index.parentNode.style.width=index.offsetWidth+"px";
		//index.parentNode.style.minwidth=index.offsetWidth+"px";
	}
	window.onresize();
};

var animations=new Object();
function animateStatus() {
	this.p=0;
	this.path=null;
	this.pathEnv=null;
	this.scale=null;
	this.scaleEnv=null;
	this.filter=null;
	this.out=null;
	this.prop=null;
	this.format="XXX";
	this.lifespan=1000;
	this.framerate=15;
	this.chain=null;
	return this;
}
function rangeScaleEnv(min,max) {
	this.min=min;
	this.max=max;
	return this;
}
function rangeScale(x,env) {
	var r=env['max']-env['min'];
	return r*x+env['min'];
}
function smoothScale(x,env) {
	return rangeScale(.5-Math.cos(x*Math.PI)/2,env);
}

function smoothPath(x,env) {
	return 1-(Math.cos(x*Math.PI)+1)/2;
}

function checkScroll(x) {
	window.onresize();
	return x;
}

function newAnimationID() {
	var rid;
	do {
		rid="an"+Math.random().toString().substr(2,5);
	} while(animations[rid]!=undefined || rid==0);
	return rid;
}

function animate1D(index) {
	var stat = animations[index];
	if(!stat)
		return;
	if(stat.p>1 || stat.out==null || stat.prop==null) {
		if(stat.chain==null) {
		} else if(stat.chain instanceof animateStatus) {
			animations[index]=stat.chain;
			animate1D(index);
			return;
		} else if(stat.chain instanceof Array) {
			for(x in stat.chain) {
				if(stat.chain[x] instanceof animateStatus) {
					var id = newAnimationID();
					animations[id]=stat.chain[x];
					animate1D(id);
				} else {
					with (stat) {
						eval(chain[x].toString());
					}
				}
			}
		} else {
			with (stat) {
				eval(chain.toString());
			}
		}
		delete animations[index];
		return;
	}
	var v = (stat.path!=null) ? stat.path(stat.p,stat.pathEnv) : stat.p;
	if(stat.scale!=null)
		v=stat.scale(v,stat.scaleEnv);
	if(stat.filter!=null)
		v=stat.filter(v);
	//window.document.getElementById("disp").innerHTML=v;
	stat.out[stat.prop]=stat.format.replace("XXX",v);
	var interval=1000/stat.framerate;
	stat.p+=interval/stat.lifespan;
	setTimeout('animate1D("'+index+'")',interval);
}

function makeFade(comp,start,finish,init) {
	var an=new animateStatus();
	an.out=comp.style;
	if(navigator.userAgent.indexOf("MSIE")==-1) {
		// use standard 'opacity'
		if(init)
			comp.style.opacity=start;
		an.prop='opacity';
		an.filter=function(x) { return x.toFixed(4); }
	} else {
		// use IE specific 'filter'
		if(init)
			comp.style.filter="alpha(opacity="+(start*100)+")";
		an.prop='filter';
		an.format="alpha(opacity=XXX)"
		start*=100;
		finish*=100;
		an.filter=Math.round;
	}
	an.scale=rangeScale;
	an.scaleEnv=new rangeScaleEnv(start,finish);
	return an;
}

function makePlayImage(theimg,frames,playtime,init,reverse) {
	var rot=new animateStatus();
	var first;
	var last;
	if(reverse) {
		first=theimg.height/frames-theimg.height;
		last=0;
	} else {
		first=0;
		last=theimg.height/frames-theimg.height;
	}
	if(init) {
		theimg.style.top=first+"px";
		rot.p=1/(frames-1);
	}
	rot.out=theimg.style; //window.document.getElementById("disp");
	rot.prop='top'; //'innerHTML';
	rot.scale=rangeScale;
	rot.scaleEnv=new rangeScaleEnv(first,last);
	rot.filter=Math.round;
	rot.format="XXXpx";
	rot.lifespan=playtime;
	rot.framerate=(frames-1)/rot.lifespan*1000;
	return rot;
}

function showMore(theid,steps,skip) {
	var theimg=window.document.getElementById(theid+"_control");
	if(!theimg) {
		alert("bad id for control: "+theid); return false;
	}
	var cont = window.document.getElementById(theid+"_content");
	if(!cont) {
		alert("bad id: "+theid); return false;
	}
	theimg.onclick=function() { showLess(theid,steps); }
	if(cont.style.display=="block")
		return;
	
	if(skip) {
		var desc = window.document.getElementById(theid+"_description");
		theimg.style.top=theimg.height/steps-theimg.height;
		cont.style.display="block";
		desc.style.display="none";
		window.onresize();
		return;
	}
	
	var rid = newAnimationID();
	animations[rid]=makePlayImage(theimg,steps,80,true);
	animations[rid].chain='window.document.getElementById("'+theid+'_control").style.title="Show more information..."';
	animate1D(rid);
	
	/*var an=makeFade(cont,0,1,true);
	an.lifespan=500;
	var id = newAnimationID();
	animations[id]=an;
	
	var desc = window.document.getElementById(theid+"_description");
	if(desc) {
		animations[id]=makeFade(desc,1,0);
		animations[id].lifespan=250;
		animations[id].chain=[
			'window.document.getElementById("'+theid+'_description").style.display="none"; window.document.getElementById("'+theid+'_content").style.display="block"; window.onresize();',
			an
		]
	} else {
		cont.style.display="block";
		window.onresize();
	}*/
	
	var desc = window.document.getElementById(theid+"_description");
	if(desc) {
		desc.style.display="none"
	}
	cont.style.display="block";
	window.onresize();
		
	animate1D(id);
}

function showLess(theid,steps,skip) {
	var theimg=window.document.getElementById(theid+"_control");
	if(!theimg) {
		alert("bad id for control: "+theid); return false;
	}
	var cont = window.document.getElementById(theid+"_content");
	if(!cont) {
		alert("bad id: "+theid); return false;
	}
	theimg.onclick=function() { showMore(theid,steps); }
	if(cont.style.display=="none")
		return;
	
	var rid = newAnimationID();
	animations[rid]=makePlayImage(theimg,steps,80,true,true);
	animate1D(rid);
	
	/*var an=makeFade(cont,1,0);
	an.lifespan=250;
	var id = newAnimationID();
	animations[id]=an;
	
	var desc = window.document.getElementById(theid+"_description");
	if(desc) {
		var dan=makeFade(desc,0,1,true);
		dan.lifespan=500;
		an.chain=[
			'window.document.getElementById("'+theid+'_description").style.display="block"; window.document.getElementById("'+theid+'_content").style.display="none"; window.onresize();',
			dan
		]
	} else {
		an.chain='window.document.getElementById("'+theid+'_content").style.display="none"; window.onresize();';
	}*/
	
	var desc = window.document.getElementById(theid+"_description");
	if(desc) {
		desc.style.display="block";
	}
	cont.style.display="none";
	window.onresize();
		
	animate1D(id);
}

function gotoid(thedest) {
	var el = window.document.getElementById(thedest);
	if(!el) return;
	var y = getOffsetTop(el);
	//setTimeout('window.location=\"#'+thedest+'\"',10);
	setTimeout('window.scrollTo(0,'+y+')',1); // have to use a timeout in safari (wtf)
}

function delayedLoad(thedest,delay) {
	setTimeout('window.location="'+thedest+'"',delay);
}


/*
  This work is licensed under Creative Commons GNU GPL License
  http://creativecommons.org/licenses/GPL/2.0/
  Copyright (C) 2006 Russel Lindsay
  www.weetbixthecat.com
*/


/**
  Binary search
  1. Array MUST be sorted already
  2. If the array has multiple elements with the same value the first instance 
     in the array is returned e.g.
       [1,2,3,3,3,4,5].binarySearch(3);  // returns 2
     This means slightly more loops than other binary searches, but I figure 
     it's worth it, as the worst case searchs on a 1 million array is around 20
  3. The return value is the index of the first matching element, OR 
     if not found, a negative index of where the element should be - 1 e.g.
       [1,2,3,6,7].binarySearch(5);      // returns -4
       [1,2,3,6,7].binarySearch(0);      // returns -1
     To insert at this point do something like this:
       var array = [1,2,3,6,7];
       var index = array.binarySearch(5);
       if(index < 0)
         array.splice(Math.abs(index)-1, 0, 5); // inserted the number 5
  4. mid calculation from 
     http://googleresearch.blogspot.com/2006/06/extra-extra-read-all-about-it-nearly.html
*/
Array.binarySearch = function(arr,item)
{
  var left = -1, 
      right = arr.length, 
      mid;
  
  while(right - left > 1)
  {
    mid = (left + right) >>> 1;
    if(arr[mid] < item)
      left = mid;
    else
      right = mid;
  }
  
  if(arr[right] != item)
    return -(right + 1);
  
  return right;
}

Array.contains = function(arr,item) {
	return Array.binarySearch(arr,item)>=0;
}



function open_display(el) {
	if ( el.style.display == 'none' ) {
		el.style.display = 'block';
		var vid = document.getElementById(el.id+"-video");
		if(vid!=null) {
			//if(typeof vid.currentTime != 'undefined')
			//	vid.currentTime=0;
			if(typeof vid.play =='function')
				vid.play();
		}
	}
}
function close_display(el) {
	if ( el.style.display != 'none' ) {
		var vid = document.getElementById(el.id+"-video");
		if(vid!=null) {
			if(typeof vid.pause == 'function')
				vid.pause();
		}
		el.style.display = 'none';
	}
	return true;
}
function blanket_size(blanket) {
	return;
	if(blanket!=null) {
		// width is set by css 100%, no need to script
		blanket.style.height = getWindowDimensions()[1] + 'px';
	}
}
function window_pos(popUpDiv) {
	dims = getWindowDimensions();
	// now using position:fixed, don't need to manually offset by scroll...
	// scr = getScrollingPosition();
	popUpDiv_left=dims[0]/2-popUpDiv.offsetWidth/2;
	popUpDiv.style.left = popUpDiv_left + 'px';
	popUpDiv_top=dims[1]/2-popUpDiv.offsetHeight/2;
	popUpDiv.style.top = popUpDiv_top + 'px';
}
function open_popup(windowname) {
	var blanket = document.getElementById('blanket');
	if(typeof blanket.lastPopup == 'undefined') 
		blanket.lastPopup=null;
	if(blanket.lastPopup==null) {
		blanket_size(blanket);
		open_display(blanket);
	} else {
		close_display(blanket.lastPopup.cur);
	}
	var el = document.getElementById(windowname);
	blanket.lastPopup = { prev:blanket.lastPopup, cur:el };
	open_display(el); // has to go before window_pos so it will update the size for positioning
	window_pos(el);
}
function close_popup() {
	var blanket = document.getElementById('blanket');
	if(typeof blanket.lastPopup == 'undefined')
		return;
	close_display(blanket.lastPopup.cur);
	blanket.lastPopup = blanket.lastPopup.prev;
	if(blanket.lastPopup==null) {
		close_display(blanket);
	} else {
		open_display(blanket.lastPopup.cur);
	}
}
function rescale_popup(windowname) {
	var windowDiv = document.getElementById(windowname);
	var popUpDiv = document.getElementById(windowname+"-img");
	dims = getWindowDimensions();
	// now using position:fixed, don't need to manually offset by scroll...
	// scr = getScrollingPosition();
	windowDiv.style.width="80%";
	windowDiv.style.height="auto";
	if(popUpDiv!=null) {
		popUpDiv.style.width="100%";
		popUpDiv.style.height="auto";
	}
	windowDiv_left=dims[0]/2-windowDiv.offsetWidth/2;
	windowDiv_top=dims[1]/2-windowDiv.offsetHeight/2;
	
	if(windowDiv_top<30) {
		if(popUpDiv!=null) {
			// leave room for a caption, if any
			var h = popUpDiv.offsetHeight / windowDiv.offsetHeight
			popUpDiv.style.height=(h*100)+"%";
			popUpDiv.style.width="auto";
		}
		windowDiv.style.width="auto";
		windowDiv.style.height="80%";
	}
	window_pos(windowDiv);
}

function lt(a,b) { return a<b; }
function gt(a,b) { return a>b; }