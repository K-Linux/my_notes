
<font size="7"><center>**mongoose**</center></font>

___


[TOC]

___

## <font color="1E90FF">一、简介</font>

Mongoose[^NFS]是一个用C语言编写的网络库，它是一把用于嵌入式网络编程的瑞士军刀。它为TCP、UDP、HTTP、WebSocket、CoAP、MQTT实现了事件驱动的非阻塞API，用于客户机和服务器模式功能包括：

1. 跨平台：适用于linux/unix、macos、qnx、ecos、windows、android、iphone、freertos
1. 自然支持PicoTCP嵌入式TCP/IP堆栈，LWIP嵌入式TCP/IP堆栈
1. 适用于各种嵌入式板：ti cc3200、ti msp430、stm32、esp8266；适用于所有基于linux的板，如Raspberry PI, BeagleBone等
1. 单线程、异步、无阻塞核心，具有简单的基于事件的api

内置协议：

1. 普通TCP、普通UDP、SSL/TLS（单向或双向）、客户端和服务器。
1. http客户端和服务器。
1. WebSocket客户端和服务器。
1. MQTT客户机和服务器。
1. CoAP客户端和服务器。
1. DNS客户端和服务器。
1. 异步DNS解析程序。

connetions可以是listening, outbound 和 inbound。outbound连接由mg_connect()调用创建的。listening连接由mg_bind()调用创建的。inbound连接是侦听连接接受的连接。每个connetion都由struct mg_connection结构描述，该结构有许多字段，如socket、事件处理函数、发送/接收缓冲区、标志等。

每个连接都有一个发送和接收缓存。分别是struct mg_connection::send_mbuf 和 struct mg_connection::recv_mbuf 。当数据接收后，Mongoose将接收到的数据加到recv_mbuf后面，并触发一个MG_EV_RECV事件。用户可以使用其中一个输出函数将数据发送回去，如mg_send() 或 mg_printf()。输出函数将数据追加到send_mbuf。当Mongoose成功地将数据写到socket后，它将丢弃struct mg_connection::send_mbuf里的数据，并发送一个MG_EV_SEND事件。当连接关闭后，发送一个MG_EV_CLOSE事件。

**<font size="4" color="1E90FF">事件处理函数</font>**

每个连接都有一个与之关联的事件处理函数。这些函数必须由用户实现。事件处理器是Mongoose程序的核心元素，因为它定义程序的行为。以下是一个处理函数的样子：

```C
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    /* Event handler code that defines behavior of the connection */
    ...
  }
}
```

struct mg_connection *nc : 接收事件的连接。

int ev : 事件编号，定义在mongoose.h。比如说，当数据来自于一个inbound连接，ev就是MG_EV_RECV。

void *ev_data : 这个指针指向event-specific事件，并且对不同的事件有不同的意义。举例说，对于一个MG_EV_RECV事件，ev_data是一个int *指针，指向从远程另一端接收并保存到接收IO缓冲区中的字节数。ev_data确切描述每个事件的意义。Protocol-specific事件通常有ev_data指向保存protocol-specific信息的结构体。

注意：struct mg_connection有void *user_data，他是application-specific的占位符。Mongoose并没有使用这个指针。事件处理器可以保存任意类型的信息。

事件

Mongoose接受传入连接、读取和写入数据，并在适当时为每个连接调用指定的事件处理程序。典型的事件顺序是：

对于出站连接：MG_EV_CONNECT -> (MG_EV_RECV, MG_EV_SEND, MG_EV_POLL ...) -> MG_EV_CLOSE

对于入站连接：MG_EV_ACCEPT -> (MG_EV_RECV, MG_EV_SEND, MG_EV_POLL ...) -> MG_EV_CLOSE

以下是Mongoose触发的核心事件列表（请注意，除了核心事件之外，每个协议还触发特定于协议的事件）：

MG_EV_ACCEPT: 当监听连接接受到一个新的服务器连接时触发。void *ev_data是远程端的union socket_address。

MG_EV_CONNECT: 当mg_connect()创建了一个新出站链接时触发，不管成功还是失败。void *ev_data是int *success。当success是0，则连接已经建立，否则包含一个错误码。查看mg_connect_opt()函数来查看错误码示例。

MG_EV_RECV：心数据接收并追加到recv_mbuf结尾时触发。void *ev_data是int *num_received_bytes。通常，时间处理器应该在nc->recv_mbuf检查接收数据，通过调用mbuf_remove()丢弃已处理的数据。如果必要，请查看连接标识nc->flags(see struct mg_connection)，并通过输出函数（如mg_send()）写数据到远程端。

警告：Mongoose使用realloc()展开接收缓冲区，用户有责任从接收缓冲区的开头丢弃已处理的数据，请注意上面示例中的mbuf_remove()调用。

MG_EV_SEND: Mongoose已经写数据到远程，并且已经丢弃写入到mg_connection::send_mbuf的数据。void *ev_data是int *num_sent_bytes。

注意：Mongoose输出函数仅追加数据到mg_connection::send_mbuf。它们不做任何socket的写入操作。一个真实的IO是通过mg_mgr_poll()完成的。一个MG_EV_SEND事件仅仅是一个关于IO完成的通知。

MG_EV_POLL：在每次调用mg_mgr_poll()时发送到所有连接。该事件被用于做任何事情，例如，检查某个超时是否已过期并关闭连接或发送心跳消息等。

MG_EV_TIMER: 当mg_set_timer()调用后，发送到连接。

## <font color="1E90FF">TCP服务器示例</font>

```C
#include "mongoose.h"  // Include Mongoose API definitions

// Define an event handler function
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct mbuf *io = &nc->recv_mbuf;

  switch (ev) {
    case MG_EV_RECV:
      // This event handler implements simple TCP echo server
      mg_send(nc, io->buf, io->len);  // Echo received data back
      mbuf_remove(io, io->len);      // Discard data from recv buffer
      break;
    default:
      break;
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr, NULL);  // Initialize event manager object

  // Note that many connections can be added to a single event manager
  // Connections can be created at any point, e.g. in event handler function
  mg_bind(&mgr, "1234", ev_handler);  // Create listening connection and add it to the event manager

  for (;;) {  // Start infinite event loop
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);
  return 0;
}
```


















[^NFS]:[mongoose开源HTTP库](https://www.cnblogs.com/gardenofhu/p/6961343.html)