import cv2
import numpy as np
import dlib
from math import hypot
#loading webcam and cat-nose image and creating mask
video=cv2.VideoCapture(0)
cat_nose=cv2.imread("cat_nose.png")
_,frame=video.read()
row,col,_=frame.shape
cat_mask=np.zeros((row,col),np.uint8)
# loading face detector
detector=dlib.get_frontal_face_detector()
predictor=dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

while(True):
	_,frame=video.read()
	cat_mask.fill(0)
	gray_frame=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
	faces=detector(frame)
	for face in faces:
		landmarks=predictor(gray_frame,face)
		# nose co-ordinates (according to dlib face landmarks)
		nose_top=(landmarks.part(29).x, landmarks.part(29).y)
		#nose_center=(landmarks.part(30).x, landmarks.part(30).y)
		#nose_center2=(landmarks.part(32).x, landmarks.part(32).y)
		nose_center=(landmarks.part(32).x+(landmarks.part(30).x-landmarks.part(32).x)/2,landmarks.part(30).y-(landmarks.part(30).y-landmarks.part(32).y)/2)
		#print(nose_center,n)
		nose_left=(landmarks.part(31).x, landmarks.part(31).y)
		nose_right=(landmarks.part(35).x, landmarks.part(35).y)
		
		nose_width=int(hypot(nose_left[0]-nose_right[0],nose_left[1]-nose_right[1])*4) # width is distance between the left point and right point
		nose_height=int(nose_width*0.9)
		#new positions where we will put the cat nose
		top_left=(int(nose_center[0]-nose_width/2),int(nose_center[1]-nose_height/2))
		bottom_right=(int(nose_center[0]+nose_width/2),int(nose_center[1]+nose_height/2))
		
		# adding the cat nose
		cat_nose1=cv2.resize(cat_nose,(nose_width,nose_height))
		cat_nose1_gray=cv2.cvtColor(cat_nose1,cv2.COLOR_BGR2GRAY)
		_,cat_mask=cv2.threshold(cat_nose1_gray,25,255,cv2.THRESH_BINARY_INV)
		
		nose_area=frame[top_left[1]:top_left[1]+nose_height, top_left[0]:top_left[0]+nose_width]
		nose_area_no_nose= cv2.bitwise_and(nose_area,nose_area,mask=cat_mask)
		final_nose=cv2.add(nose_area_no_nose, cat_nose1)
		frame[top_left[1]:top_left[1]+nose_height, top_left[0]:top_left[0]+nose_width]=final_nose
		
	cv2.imshow("Frame", frame)
	
			
	if(cv2.waitKey(1) & 0xFF==ord('q')):
		break
video.release()
cv2.destroyAllWindows()

