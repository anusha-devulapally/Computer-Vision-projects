import cv2
import numpy as np
import dlib
#loading webcam and rabbit ears image and creating mask
video=cv2.VideoCapture(0)
_,frame=video.read()
row,col,_=frame.shape
# loading face detector
detector=dlib.get_frontal_face_detector()
while(True):
	ret,frame=video.read()
	faces=detector(frame)
	for face in faces:
		x1=face.left()
		y1=face.top()
		x2=face.right()
		y2=face.bottom()
		k=max(abs(y2-y1),abs(x1-x2))
		center=(x2-(abs(x2-x1)//2),y2-(abs(y2-y1)//2))
		#cv2.circle(frame,center,k//2,(0,0,255),2,2) # for circle
		# for rectangle
		cv2.rectangle (img=frame, pt1=(x1,y1), pt2=(x2,y2), color=(0,255,255), thickness=4)
		cv2.imshow(winname='face',mat=frame)
	if(cv2.waitKey(1) & 0xFF==ord('q')):
		break
video.release()
cv2.destroyAllWindows()

