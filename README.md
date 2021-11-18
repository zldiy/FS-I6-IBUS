# FS-I6-IBUS
STC51 MCU analyze FLYSKY ia6b receiver ibus data and control LED or other channel 

1.上传整个keil文档；其中只有串口接收函数，定时器函数，GPIO函数有效，其他暂时无效
2.功能：使用STC51单片机接收IA6B的IBUS协议信号，通过通道7,控制LED灯的工作状态，通道8控制500ms暂高电平，通道9控制高低电平输出。
3.用于F450无人机指示灯控制。通道7实现LED的同步闪烁，前后闪烁，关闭的功能
4.可以解码14个通道。暂用10个通道。
