#include "mongoose.h"

//127.0.0.1是一个特殊IP地址，表示本机地址
static const char *url = "127.0.0.1:8001";
// static const char *url = "http://www.baidu.com";
static int exit_flag = 0;

static void ev_handler(struct mg_connection *conn, int ev, void *ev_data)
{
	struct http_message *hm = (struct http_message *) ev_data;
	int connect_status;
	switch (ev) {
	case MG_EV_CONNECT:
		connect_status = *(int *)ev_data;
		if (connect_status != 0) {	//若连接失败
			printf("Error connecting to server, error code: %d\n", connect_status);
			exit_flag = 1;
		}
		printf("******************connect OK*****************");
		break;
	case MG_EV_HTTP_REPLY: //建立http服务器并收到数据
		{
			conn->flags |= MG_F_SEND_AND_CLOSE;
			//同理，hm->message.len, hm->message.p存放着服务端发送过来的信息，包括post，Host（http地址）,Content-Length(信息的长度)，以及信息本身。
    		//通过std::string rsp = std::string(hm->body.p, hm->body.len);
    		fwrite(hm->message.p, 1, (int)hm->message.len, stdout);
    		putchar('\n');
			exit_flag = 1; // 每次收到请求后关闭本次连接，重置标记
			//TODO. 处理响应数据
		}
		printf("******************reply OK*****************");
		break;
	case MG_EV_CLOSE:	//表示当前连接断开
		if (exit_flag == 0)
		{
			printf("Server closed connection\n");
			exit_flag = 1;
		};
		break;
	}
}

int main(int argc, char* argv[])
{
	struct mg_mgr mgr;    // Event manager
	mg_mgr_init(&mgr, NULL);    // Initialise event manager

    //创建outbound HTTP连接
	//参数5为发送给服务端的信息
	mg_connect_http(&mgr, ev_handler, url, NULL, NULL);

	while (exit_flag == 0) {
		//1000ms监听不到连接就结束
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);


	return 0;
}
