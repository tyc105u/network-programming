編譯
gcc -o cli -pthread client.c
gcc -o sev -pthread server.c

先執行 
./sev 255 127.0.0.1
在執行
./cli 255 127.0.0.1
每個cli會有自己的ＩＤ 從0開始

查看線上使用者
list

私訊
sercet,ID,想說的話

傳送檔案
send,ID,檔名

接收方要輸入兩次NO 或 YES 來接收
接收會得到一個複製的檔案
download_username.txt

檔名,ID如果不存在程式會掛掉


