import numpy as np
import csv

f = open("hvec.csv")
csv_f = csv.reader(f)
X,Y = zip(*csv_f)

X = np.array(X);
Y = np.array(Y);

X = map(float,X)
Y = map(float,Y)

p = np.polyfit(X,Y,17)
pp = np.polyval(p,X)

fp = open("hfitvals.csv","w")
csv_fp = csv.writer(fp,delimiter='\n')
csv_fp.writerows([pp])
f.close()
fp.close()

f = open("svec.csv")
csv_f = csv.reader(f)
X,Y = zip(*csv_f)

X = np.array(X);
Y = np.array(Y);

X = map(float,X)
Y = map(float,Y)

p = np.polyfit(X,Y,17)
pp = np.polyval(p,X)

fp = open("sfitvals.csv","w")
csv_fp = csv.writer(fp,delimiter='\n')
csv_fp.writerows([pp])
f.close()
fp.close()

f = open("lvec.csv")
csv_f = csv.reader(f)
X,Y = zip(*csv_f)

X = np.array(X);
Y = np.array(Y);

X = map(float,X)
Y = map(float,Y)

p = np.polyfit(X,Y,17)
pp = np.polyval(p,X)

fp = open("lfitvals.csv","w")
csv_fp = csv.writer(fp,delimiter='\n')
csv_fp.writerows([pp])
f.close()
fp.close()
