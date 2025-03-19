# IP기반 Capstone Design 졸업작품I #
프로젝트명 : 가정 내 영유아 사고 방지를 위한 Auto Tracking Robot

## 프로젝트 소개 ##
지식재산을 기반으로 발명된 학부과정 졸업 작품이며, 가정 내에 영유아의 사고를 방지하기 위해 
실내 CCTV의 기술과 로봇 청소기의 기술의 일부를 활용하였습니다.
영유아를 실시간으로 가정 내에서 추적하며 돌아다니는 미니 모바일 로봇에는 3가지의 기능이 있습니다.
1. OpenCV 색체 인식 기술을 활용하여 녹색을 인식하였을 때, 로봇이 움직일 수 있도록 설계
2. 가정 내에 위험한 구역(화장실, 주방 등등)에 위험 방지 시스템을 설계하였고 해당 구역에 거리에 도달 시 경고음을 울릴 수 있도록 설정
3. 스마트폰 앱 컨트롤러를 이용하여 start 일 때는 실시간으로 움직이도록 설정, 그 외에 기능은 로봇을 수동으로 제어할 수 있도록 설정  

MCU로는 ATmega128 , Arduino mega2560 , SBC(Single Board Computer)는 Raspberry Pi 4B를 사용하였습니다.


## 하드웨어 구성도 ##
![image](https://github.com/user-attachments/assets/e7b1a83d-e13b-48bd-85f6-ec2b36f29a65)



## 소프트웨어 아키텍쳐 ##
![image](https://github.com/user-attachments/assets/0749fa7a-b123-4045-a204-2fb814361951)


## 영유아를 추적하기위해 사용했던 기술 ##
![Opencv123](https://github.com/user-attachments/assets/5c65a5ab-e249-4141-b94e-d5fe56e23aa1)



## 완성품 ##
<모바일 로봇>
![image](https://github.com/user-attachments/assets/4faff440-fb3d-42ca-aa02-34ca71fd52c1)

<위험 방지 시스템>
![image](https://github.com/user-attachments/assets/d26ead5c-da56-47bc-b694-e24c19162e6f)

<앱 컨트롤러>
![image](https://github.com/user-attachments/assets/0b78a236-cf1a-4013-b0cb-9098e14bbfeb)
