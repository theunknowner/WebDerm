row=33;
xvec = 1:length(hvec);
hfit = polyfit(xvec,hvec(row,1:length(hvec)),17);
hfitval = polyval(hfit,xvec);
sfit = polyfit(xvec,svec(row,1:length(svec)),17);
sfitval = polyval(sfit,xvec);
lfit = polyfit(xvec,lvec(row,1:length(lvec)),17);
lfitval = polyval(lfit,xvec);
hfitval2 = hfitval;
sfitval2 = sfitval;
lfitval2 = lfitval;

[xmaxh,imaxh,xminh,iminh] = extrema(hfitval);
[xmaxs,imaxs,xmins,imins] = extrema(sfitval);
[xmaxl,imaxl,xminl,iminl] = extrema(lfitval);

hfitval2(iminh) = hvec(row,iminh);
hfitval2(imaxh) = hvec(row,imaxh);
lfitval2(iminl) = lvec(row,iminl);
lfitval2(imaxl) = lvec(row,imaxl);
sfitval2(imins) = svec(row,imins);
sfitval2(imaxs) = svec(row,imaxs);