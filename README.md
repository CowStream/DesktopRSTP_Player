# DesktopRSTP_Player

A RSTP Player based on qt-vlc win32 and QT5

## 播放器功能介绍

live555 作为服务端，通过RTSP协议访问服务端，播放WebM视频格式音视频。
视频格式WebM使用audio(Vorbis)+video(VP8)的编码方式，虽然文件体积会较大，但播放同步效果很好。

## 注意

需要自己搭建Qt5和qt-vlc编译环境，工程内不包含qt-vlc文件

## TODO LIST

 1. 实现服务器片库内容浏览
 2. 获取子窗口链接，播放目标源
