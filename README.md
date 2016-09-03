# video
实现摄像头监控的设计

此项目是基于Tiny210开发板的摄像头抓帧解帧的处理（Linux系统）

catchvideo.c 是摄像头抓取视频的代码，保存到当地文件夹

v4l2grab.c是对摄像头抓取照片的代码。
imservice.c是v4l2grab.c的服务端，接受客户端发过来的图片

这些代码需要引用一些JPEG的动态库，需要将YUYV转化成RGB再转化成JPEG格式
