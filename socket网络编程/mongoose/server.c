#include "mongoose.h"  // Include Mongoose API definitions

#define PORT_NUM "1234"
static const char *url = "127.0.0.1:8001";
// static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

// Define an event handler function
//mg_mgr是mongoose中进行事件管理的结构体，事件分为5种类型，共享同一个回调函数，事件类型通过传参区分
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  //描述数据缓冲区（接收或发送的数据）
  struct mbuf *io = &nc->recv_mbuf;
  struct http_message *hm = (struct http_message *)ev_data;

  switch (ev) 
  {
  case MG_EV_RECV:                     // MG_EV_RECV 表示事件接收
       // 该事件处理程序实现简单的TCP回显服务器
       mg_send(nc, io->buf, io->len);  // 接收到的数据
       mbuf_remove(io, io->len);       // 丢弃来自recv缓冲区的数据
       break;
  case MG_EV_HTTP_REQUEST:
      //我们收到一个HTTP请求。解析后的请求包含在“hm”中。
      //向显示完整原始请求的客户端发送HTTP回复。
      //(int)hm->message.len, hm->message.p 中存放客户端发过来的信息，包括post，Host（http地址）,Content-Length(信息的长度)，以及信息本身。
      //通过 std::string url = std::string(hm->uri.p, hm->uri.len); 可以得到url
      //通过 std::string body = std::string(hm->body.p, hm->body.len);可以得到body中 存储的从客户端发送过来的信息
       mg_serve_http(nc, hm, s_http_server_opts);
       break;
  default:
       break;
  }
}

int main(void) {
  //保存所有活动连接的事件管理器  
  struct mg_mgr mgr;
  //描述连接
  struct mg_connection *nc;

  //初始化事件管理器
  mg_mgr_init(&mgr, NULL);

  // 一个事件管理器可以添加多个连接
  // 连接可以在任何点创建，例如在事件处理函数中
  // 创建监听端口连接并将其添加到事件管理器
  // 参数2为端口号
  nc = mg_bind(&mgr, url, ev_handler);
  // nc = mg_bind(&mgr, s_http_port, ev_handler);

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {  // Start infinite event loop
    //遍历所有socket,接受新连接，发送和接受数据，关闭连接并调用对应的事件处理函数。 
		//1000ms监听不到连接就结束
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);
  return 0;
}








