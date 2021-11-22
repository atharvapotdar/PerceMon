import json
import cv2
f=open('mot16_6_test.json', 'r')
details=json.load(f)
details=json.loads(details)
h={}
percep={}
for i in details:
    if i["frame_idx"] not in h:
        h[i["frame_idx"]]=[i["track"]]
    else:
        h[i["frame_idx"]].append(i["track"])
frames=list(h.keys())
for i in range(len(frames)-10):
    for j in h[frames[i]]:
        flag=1
        interval=frames[i+1:i+10]
        for k in range(len(interval)):
            if j not in h[interval[k]] and flag==1:
                flag=2
                temp=interval[k]-1
            elif j in h[interval[k]]:
                if flag==2:
                    percep[(j,temp,interval[k])]=1
                    break
y=percep.keys()
#img = cv2.imread('MOT16/test/MOT16-06/img1/000014.jpg')
for i in list(y):
    for j in details:
        if j["frame_idx"]==i[1] and j["track"]==i[0]:
            print("For object with id: ",j["track"]," Perception Failure Occurs in frame intervals"," ",i[1],"-",i[2]," ",j["bbox"])
            #cv2.imwrite("im2.jpg",cv2.rectangle(img,(j["bbox"][0],j["bbox"][1]),(j["bbox"][2],j["bbox"][3]), (255,0,0), 2))
            #break
    #break
f.close()
