# PetMediaPlayer

基于 Qt6 + C++ 开发的桌宠音乐播放器项目。  
融合了桌宠互动、音乐播放、歌词气泡、播放列表与设置系统。

## 功能

- 桌宠状态机（Idle / Sleep / Angry / Listen 等）
- 音乐播放器（播放、暂停、切歌、进度、音量、播放模式）
- 歌词系统（LRC 解析、歌词高亮、歌词气泡）
- 独立播放列表窗口
- 设置窗口 + QSettings 持久化
- 托盘菜单控制
- 桌宠缩放 / 透明度 / 始终置顶
- 自动恢复上次播放状态

## 技术栈

- Qt6
- C++
- QWidget
- QMediaPlayer
- QSettings
- Signal / Slot

## 项目结构

```text
PetWidget          桌宠系统
MusicPlayerManager 音乐播放器
LyricManager       歌词解析
TalkBubble         歌词气泡
PlaylistWindow     播放列表
SettingsWindow     设置窗口
UIStyleHelper      UI 样式管理
TrayManager        系统托盘
