说明：
此程序立于在两个嵌入式开发办之间实现PTP时间同步协议
目前的内容主要是应用级框架，运行主要流程如下：
1.在config中设置ip、端口、主从类型等信息
2.先运行server端监听，随后使用client端请求，同步过程断续自动
3.command程序用来给client端发送指令，获得同步信息，如发送offset为取得当前同步信息，sync为马上同步并返回，sync quit使client退出。

编译：
在主目录下运行make.sh
