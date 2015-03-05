row=85;
vec = hfitval;
hfit = polyfit(xvec,hvec(row,1:length(hvec)),17);
hfitval = polyval(hfit,xvec);
sfit = polyfit(xvec,svec(row,1:length(svec)),17);
sfitval = polyval(sfit,xvec);
lfit = polyfit(xvec,lvec(row,1:length(lvec)),17);
lfitval = polyval(lfit,xvec);