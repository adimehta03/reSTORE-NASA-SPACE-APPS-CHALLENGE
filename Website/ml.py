import pandas as pd
import numpy as np
from kmodes.kprototypes import KPrototypes
import sys

df = pd.read_csv('dataset.txt',sep=";")

df_array = df.values

df_array[:, 2] = df_array[:, 2].astype(float)

kproto = KPrototypes(n_clusters=3, verbose=2,max_iter=20)
clusters = kproto.fit_predict(df_array, categorical=[0,1,3,4])

cluster_dict=[]
for c in clusters:
    cluster_dict.append(c)

df['cluster'] = cluster_dict


c0 = df[df['cluster']== 0]
c1 = df[df['cluster']== 1]
c2 = df[df['cluster']== 2]


# c0 = df[df['cluster']== 0].applymap(lambda s:s.lower() if type(s) == str else s)
# c1 = df[df['cluster']== 1].applymap(lambda s:s.lower() if type(s) == str else s)
# c2 = df[df['cluster']== 2].applymap(lambda s:s.lower() if type(s) == str else s)
# # print(c0['cost range'])
# # print(c1,c2,c0)
# # print("In py")

# poll = 'Benzene'#sys.argv[1]
# mincost,maxcost = 100,600#int(sys.argv[2].split('-')[0]),int(sys.argv[2].split('-')[1])
# moist = 'Normal'#sys.argv[3]
# temp = 'Normal'#sys.argv[4]


# c00 = c0[c0['pollutant']==poll.lower()]
# c01 = c00[c00['cost range']>mincost]
# c02 = c01[c01['cost range']<maxcost]
# c03 = c02[c02['Moisture']==moist.lower()]
# c04 = c03[c03['Temperature']==temp.lower()]

# c05 = c00.merge(c01,how="inner")
# c06 = c05.merge(c02,how="inner")
# c07 = c06.merge(c03,how="inner")
# c08 = c07.merge(c04,how="inner")
# print(c05,c06,c07,c08)

# print(c00,c01,c02,c03,c04)

# print(df['cost range'])
# df = df[df['pollutant']=='Benzene' & c0['cost range']>mincost]# & c0['cost range']<maxcost & c0['Moisture']==moist.lower() &  c0['Temperature']==temp.lower()]
# print(df)
