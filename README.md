#简介
本项目是基于gSOAP的一个即时通讯服务框架(Instant Message Service),项目的编译使用Makefile，文档分层清晰、
书写规范。
#使用
cd InstantMessageService
make all

注意编译之前请先安装gSOAP
一旦项目编译成功，就可以使用script下的脚本进行数据库的初始化。
首先要执行的就是：
scripts/sql/db-ims.sql

然后登陆服务端：
cd bin
./server <port> <db_user> <db_pass>

然后登陆客户端：
./client <url>:<port>
如果是在本地运行，url可以使用localhost