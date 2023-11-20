// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example MQTT client. It performs the following steps:
//    1. Connects to the MQTT server specified by `s_url` variable
//    2. When connected, subscribes to the topic `s_sub_topic`
//    3. Publishes message `hello` to the `s_pub_topic`
//    4. Receives that message back from the subscribed topic and closes
//    5. Timer-based reconnection logic revives the connection when it is down
//
// To enable SSL/TLS, make SSL=OPENSSL or make SSL=MBEDTLS 
#include "source/examples/mqtt-client/mongoose.h"
// broker.hivemq.com是一个免费的公共MQTT代理，可以用于MQTT测试，TCP端口为1883
static const char *s_url = "mqtt://broker.hivemq.com:1883";
static const char *s_sub_topic = "mg/+/test";
static const char *s_pub_topic = "mg/clnt/test";
static int s_qos = 1;
static struct mg_connection *s_conn;

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
  s_signo = signo;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) { //表示已创建连接
    MG_INFO(("CREATED"));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    // 错误信息通过ev_data传递进来
    MG_ERROR(("%p %s", c->fd, (char *) ev_data)); //等价于printf
  } else if (ev == MG_EV_CONNECT) { //表示已建立连接
    // If target URL is SSL/TLS, command client connection to use TLS
    // 使用mg_url_is_ssl函数用于检查给定的 URL 是否使用加密方案
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = "ca.pem"};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_MQTT_OPEN) { //这个事件是MQTT服务器接受客户端时，响应客户端用的
    // MQTT connect is successful
    struct mg_str subt = mg_str(s_sub_topic);
    struct mg_str pubt = mg_str(s_pub_topic), data = mg_str("hello");
    MG_INFO(("CONNECTED to %s", s_url));
    mg_mqtt_sub(c, subt, s_qos);
    MG_INFO(("SUBSCRIBED to %.*s", (int) subt.len, subt.ptr));

    mg_mqtt_pub(c, pubt, data, s_qos, false);
    MG_INFO(("PUBLISHED %.*s -> %.*s", (int) data.len, data.ptr,
             (int) pubt.len, pubt.ptr));
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_INFO(("RECEIVED %.*s <- %.*s", (int) mm->data.len, mm->data.ptr,
             (int) mm->topic.len, mm->topic.ptr));
    c->is_closing = 1;
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("CLOSED"));
    s_conn = NULL;  // Mark that we're closed
  }
  (void) fn_data;
}

// Timer function - recreate client connection if it is closed
// timer_fn函数的作用是，如果客户端连接关闭，则重新创建该连接
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  // mg_mqtt_opts用于指定MQTT连接选项
  struct mg_mqtt_opts opts = {.clean = true,
                              .qos = s_qos,
                              .topic = mg_str(s_pub_topic), //mg_str用于创建Mongoose字符串
                              .message = mg_str("goodbye")};
  //mg_mqtt_connet用于创建客户端MQTT连接，fn是事件处理函数
  if (s_conn == NULL) s_conn = mg_mqtt_connect(mgr, s_url, &opts, fn, NULL);
}

int main(void) {
  struct mg_mgr mgr;
  //MG_TIMER_REPEAT表示定时重复调用函数，MG_TIMER_RUN_NOW表示设置定时器后立即调用
  int topts = MG_TIMER_REPEAT | MG_TIMER_RUN_NOW;

  signal(SIGINT, signal_handler);   // Setup signal handlers - exist event
  signal(SIGTERM, signal_handler);  // manager loop on SIGINT and SIGTERM

  mg_mgr_init(&mgr);                                // Init event manager
// 调用mg_timer_add添加一个定时器到mgr事件管理器的内部定时器列表中
// mgr事件管理器将以3000毫秒的间隔调用timer_fn函数，并将参数&mgr传递给它
  mg_timer_add(&mgr, 3000, topts, timer_fn, &mgr);  // Init timer
// 进行事件循环，mg_mgr_poll遍历所有连接，接受新连接，发送和接收数据，关闭连接
// 并为各个事件调用事件处理函数。当s_signo 不为0时，也就是接收到了退出信号，则结束无限循环
  while (s_signo == 0) mg_mgr_poll(&mgr, 1000);     // Event loop, 1s timeout
  mg_mgr_free(&mgr);                                // Finished, cleanup

  return 0;
}

