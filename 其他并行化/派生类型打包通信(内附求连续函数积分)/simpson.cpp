#include<iostream>
#include<cstdio>
#include<algorithm>
#define eps 1e-9
#include<cmath>
using namespace std;
typedef long double ld;
bool jian(ld a,ld b){
	return abs(a-b)<eps;
}
ld f(ld x){
}
ld simpson(ld l,ld r,ld fl,ld fr,ld fmid){
	return (fl+fr+fmid*4)*((r-l)/6);
}
ld area(ld l,ld r,ld mid,ld fl,ld fr,ld fmid,ld s){
	ld midl,midr;
	ld fmidl,fmidr,sl,sr;
	midl=(l+mid)/2,midr=(mid+r)/2;
	fmidl=f(midl),fmidr=f(midr);
	sl=simpson(l,mid,fl,fmidl,fmid),sr=simpson(mid,r,fmid,fmidr,fr);
	return jian(sl+sr,s)?s:area(l,mid,midl,fl,fmid,fmidl,sl)+area(mid,r,midr,fmid,fr,fmidr,sr);
}
ld get(ld l,ld r){
	ld mid;
	ld fl,fr,fmid;
	mid=(l+r)/2;
	fl=f(l),fr=f(r),fmid=f(mid);
	return area(l,r,mid,fl,fr,fmid,simpson(l,r,fl,fr,fmid));
}
若要加速，可以套上simpson
