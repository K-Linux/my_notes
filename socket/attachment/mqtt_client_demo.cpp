#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTAsync.h"

// #define RECONNECT 1
// #define DEBUG 1
// #define CREATE_OPTIONS_CLIENT 1
// #define URIS 1
// #define HTTPHEADERS 1

#define CA_FILE             "/home/k/samba/demo/ssl/ca.crt" //CA证书
#define CLIENT_CRT_FILE     "/home/k/samba/demo/ssl/ca.crt" //客户端公钥证书
#define CLIENT_PRI_KEY      "/home/k/samba/demo/ssl/ca.crt" //客户端私钥

// #define ADDRESS     "ws://localhost:6003/mqtt"       //websocket 协议
// #define ADDRESS     "wss://localhost:6004"      //websockets 协议
// #define ADDRESS     "tcp://broker.hivemq.com:1883"   // TCP 协议
// #define ADDRESS     "tcp://localhost:6001"           // TCP 协议
// #define ADDRESS     "tcp://127.0.0.1:6001"           // TCP 协议
// #define ADDRESS     "127.0.0.1:6001"                 // TCP 协议
// #define ADDRESS     "ssl://127.0.0.1:6002"          // TLS 协议
#define ADDRESS     "ssl://localhost:6002"           // TLS 协议

#define CLIENTID    "LinuxARM_C_Async_Client"   //每个客户端ID必须唯一
#define PUBLISH_TOPIC         "/publish/topic"
#define SUBSCRIBE_TOPIC       "/subscribe/topic"
#define PAYLOAD     "Hello I'm client main"
#define PAYLOAD_SIZE     1024
#define QOS         1                           // qos 是消息传递到服务器的保证等级，0,1,2
#define TIMEOUT     10000L

volatile int connected = 0;
volatile int subscribed = 0;
volatile int unsubscribed = 0;
volatile int finished = 0;
volatile int ssl_flag = 1;
char *payload = NULL;

void traceCallback(enum MQTTASYNC_TRACE_LEVELS level, char* message);
void onSend(void* context, MQTTAsync_successData* response);
void onSendFailure(void* context, MQTTAsync_failureData* response);
int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message);
void onSubscribe(void* context, MQTTAsync_successData* response);
void onUnSubscribe(void* context, MQTTAsync_successData* response);
void onDisconnect(void* context, MQTTAsync_successData* response);
void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
void onUnSubscribeFailure(void* context, MQTTAsync_failureData* response);
void onConnect(void* context, MQTTAsync_successData* response);
void onReConnect(void* context, char* cause);
void onConnectFailure(void* context, MQTTAsync_failureData* response);
void onConnectionLost(void* context, char* cause);

#if 0
typedef struct {
    /* 当发布消息时，库会分配一个 token 给这条消息, 当消息发布成功(失败)时会执行 onSend(onFailure) 回调
    回调中会带上这个 Token，告知哪条消息发送成功或失败了*/
    MQTTAsync_token token;      //消息令牌
    union
    {
        int qos;  // 订阅时服务器授予的 QoS
        int* qosList; // 多主题订阅时的 QoS 列表 (MQTT 5.0 版本才有)
        struct
        {
            MQTTAsync_message message;  // 发布的消息
            char* destinationName;      // 主题名
        } pub;
        struct
        {
            char* serverURI;    // 连接的服务器地址
            int MQTTVersion;    // MQTT 协议版本
            int sessionPresent; // 会话是否存在
        } connect;
    } alt;
} MQTTAsync_successData;

// 发送端需要配置 MQTTAsync_message， 接收端即能看到这些配置
typedef struct {
    char struct_id[4];       // 结构标识，必须是 "MQTM"
    int struct_version;      // 结构版本，通常是 0 或 1
    int payloadlen;          // 负载长度（字节数）
    void* payload;           // 指向负载数据的指针
    int qos;                 // QoS等级（0, 1, 2）
    int retained;            // 保留标志，0 = 非保留，1 = 保留
    int dup;                 // 是否为重复消息，接收时有效
    int msgid;               // 接收消息的ID，用于唯一标识一条消息，用于判断该消息是否重发; 当消息重发时 dup 会设置为 1
    MQTTProperties properties; // MQTT 5.0 属性（可选）
} MQTTAsync_message;

typedef struct
{
    MQTTAsync_token token;   // 失败请求的 token
    int code;               // 错误码 使用 MQTTAsync_strerror(code) 获取错误描述字符串
    const char *message;    // 可选的错误消息字符串，可能为 NULL
} MQTTAsync_failureData;

#endif


// Trace回调函数，用于接收库内部日志
void traceCallback(enum MQTTASYNC_TRACE_LEVELS level, char* message) {
    const char* levelStr = NULL;
    switch (level) {
        case MQTTASYNC_TRACE_MAXIMUM: levelStr = "MAXIMUM"; break;
        case MQTTASYNC_TRACE_MEDIUM:  levelStr = "MEDIUM";  break;
        case MQTTASYNC_TRACE_MINIMUM: levelStr = "MINIMUM"; break;
        case MQTTASYNC_TRACE_PROTOCOL:levelStr = "PROTOCOL";break;
        case MQTTASYNC_TRACE_ERROR:   levelStr = "ERROR";   break;
        default:                      levelStr = "UNKNOWN"; break;
    }
    printf("[MQTT TRACE][%s] %s\n", levelStr, message);
}

void onSend(void* context, MQTTAsync_successData* response) {
    printf("token: %d\n", response->token);
    printf("Published topic: %s\n", response->alt.pub.destinationName);
    printf("Payload length: %d\n", response->alt.pub.message.payloadlen);
    printf("Message sent successfully  ----------------------\n");
    finished = 1;
}

void onSendFailure(void* context, MQTTAsync_failureData* response) {
    printf("Token: %d\n", response->token);
    printf("Error code: %d\n", response->code);
    if (response->message)
        printf("Error message: %s\n", response->message);
    printf("Failed to send message  ----------------------\n");
}

// 发送端需要配置 MQTTAsync_message ， 接收端即能看到这些配置
int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message) {
    printf("topic:[%s], message:[%.*s]\n", topicName, message->payloadlen, (char*)message->payload);
    printf("QoS:%d, Retained:%d dup:%d, msgid:%d\n", message->qos, message->retained, message->dup, message->msgid);
    printf("accept succeeded  ----------------------\n");
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    finished = 1;  // 标记消息处理完成
    return 1;
}

void onSubscribe(void* context, MQTTAsync_successData* response) {
    printf("Granted QoS: %d\n", response->alt.qos);
    printf("Subscribe succeeded  ----------------------\n");
    subscribed = 1;
}

void onUnSubscribe(void* context, MQTTAsync_successData* response) {
    printf("UnSubscribe succeeded  ----------------------\n");
    unsubscribed = 1;
    MQTTAsync client = (MQTTAsync)context;
    // 断开连接
    MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	disc_opts.onSuccess = onDisconnect;
    disc_opts.timeout = 10000;  //毫秒
	disc_opts.context = client;
    MQTTAsync_disconnect(client, &disc_opts);
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
    printf("Disconnect success  ----------------------\n");
	finished = 1;
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response) {
    printf("Token: %d\n", response->token);
    printf("Error code: %d\n", response->code);
    if (response->message)
        printf("Error message: %s\n", response->message);
    printf("Subscribe failed  ----------------------\n");
    finished = 1;
}

void onUnSubscribeFailure(void* context, MQTTAsync_failureData* response) {
    printf("Token: %d\n", response->token);
    printf("Error code: %d\n", response->code);
    if (response->message)
        printf("Error message: %s\n", response->message);
    printf("UnSubscribe failed  ----------------------\n");
    finished = 1;
}

void onReConnect(void* context, char* cause)
{
    printf("Successfully onReConnect  ----------------------\n");
}

void onConnect(void* context, MQTTAsync_successData* response) {
    printf("Server URI: %s\n", response->alt.connect.serverURI);
    printf("MQTT Version: %d\n", response->alt.connect.MQTTVersion);
    printf("Session Present: %d\n", response->alt.connect.sessionPresent);
    printf("Successfully connected  ----------------------\n");
    connected = 1;

    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_responseOptions sub_opts = MQTTAsync_responseOptions_initializer;
    sub_opts.onSuccess = onSubscribe;
    sub_opts.onFailure = onSubscribeFailure;
    sub_opts.context = client;

    // 一次性批量订阅主题
	// int qoss[2] = {2, 2};
	// char* topics[2] = {willTopic, test_topic};
	// rc = MQTTAsync_subscribeMany(client, 2, topics, qoss, &sub_opts);//一次性批量订阅主题
	// rc = MQTTAsync_unsubscribeMany(c, count, topics, &response); //一次性批量取消订阅主题

    int rc = MQTTAsync_subscribe(client, SUBSCRIBE_TOPIC, QOS, &sub_opts);
    if (rc != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start subscribe, return code %d\n", rc);
    }
}

void onConnectFailure(void* context, MQTTAsync_failureData* response) {
    printf("Token: %d\n", response->token);
    printf("Error code: %d\n", response->code);
    if (response->message)
        printf("Error message: %s\n", response->message);
    printf("Connect failed  ----------------------\n");
    finished = 1;
}

void onConnectionLost(void* context, char* cause)
{
    if (cause)
        printf("cause:%s\n", cause);

    connected = 0;

    printf("Connection lost, Trying to reconnect...  ----------------------\n");
    // 若 automaticReconnect = true, 则库会自动处理重连，一般不需要手动调用 MQTTAsync_reconnect() 函数
#ifdef RECONNECT
    MQTTAsync client = (MQTTAsync)context;
    int rc = MQTTAsync_reconnect(client); //异常断线后手动重连
    if (rc != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start reconnect, return code %d\n", rc);
    }
#endif
}

int sslErrorCallback(const char *str, size_t len, void *u)
{
    // u 为 ssl_error_context 传递的自定义上下文
    char *contextInfo = (char *)u;
    printf("[SSL ERROR][%s] %.*s\n", contextInfo, (int)len, str);
    return 0; // 返回0表示继续处理后续错误
}


int main(int argc, char* argv[])
{
#ifdef DEBUG
    // 设置日志级别为 VERBOSE，输出所有调试信息 (必须在连接之前设置)
    // MQTTAsync_setTraceLevel(MQTTASYNC_TRACE_ERROR);
    // 设置日志回调函数
    // MQTTAsync_setTraceCallback(traceCallback);
#endif

    int rc;
    MQTTAsync client;

    // 获取版本信息
	MQTTAsync_nameValue* ver_info = MQTTAsync_getVersionInfo();
	while (ver_info->name)
	{
        fprintf(stderr, "mqtt info:%s, %s\n", ver_info->name, ver_info->value);
	    ver_info++;
	}

#ifdef CREATE_OPTIONS_CLIENT
	MQTTAsync_createOptions createOptions = MQTTAsync_createOptions_initializer;
	createOptions.sendWhileDisconnected = 1; //1: 连接断开后，允许发送消息存入缓存，等待重新连接后发送; MQTTAsync_sendMessage() 函数依旧返回 true (cleansession 控制接收离线消息， sendWhileDisconnected 控制发送离线消息)
    createOptions.maxBufferedMessages = 50;   // 最大缓存消息数，超过后会拒绝新消息或删除旧消息
    createOptions.struct_version = 2;          // 用于标识结构体的版本，不同版本支持不同的字段。// 0：最基础版本，不支持 MQTTVersion 等字段  1：支持 sendWhileDisconnected maxBufferedMessages  2： 支持 persistQoS0 字段。 版本 3：支持更多高级选项。
    createOptions.MQTTVersion = MQTTVERSION_DEFAULT;
    createOptions.allowDisconnectedSendAtAnyTime = 0; // 1: 即使客户端从未连接成功，调用 MQTTAsync_sendMessage() 也不会失败，消息会被缓存等待连接建立后发送  0: 反之
    createOptions.deleteOldestMessages = 1;   // 1: 缓存满时删除最旧消息，0: 缓存满时拒绝新消息
    createOptions.restoreMessages = 1;        // 1: 断线重连时恢复未发送或未确认的消息。即断线时未完成的 QoS 1 或 QoS 2 消息会被保存  0: 反之
    createOptions.persistQoS0 = 0;             // 不持久化 QoS 0 的消息 一般设置为0
#endif

    // MQTTCLIENT_PERSISTENCE_NONE 表示持久化机制(persistence ) 使用内存来保存数据，不持久化到文件
#ifdef CREATE_OPTIONS_CLIENT
	rc = MQTTAsync_createWithOptions(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL, &createOptions);
#else
    rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
#endif
	if (rc != MQTTASYNC_SUCCESS)
	{
        fprintf(stdout, "\033[41;30mlinuxlog:\033[0m [%s]:[%s]:%d [rc:%d]\n", __FILE__, __func__, __LINE__, rc);
		MQTTAsync_destroy(&client);
		return -1;
	}

    // 参数2:用户自定义的上下文指针context，会传递给回调函数 参数3:连接断开时调用 参数4:收到订阅的消息时调用 参数5:当QoS 1或QoS 2的消息成功发送时调用
    MQTTAsync_setCallbacks(client, client, onConnectionLost, messageArrived, NULL);

    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;
	MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
#ifdef HTTPHEADERS
    MQTTAsync_nameValue httpHeaders[] = {
        {"Authorization", "Bearer your_token_here"}, // 示例头部，按需修改
        {"X-Custom-Header", "CustomValue"},          // 自定义头部
        {NULL, NULL}                                  // 结束标志
    };
#endif

#ifdef URIS
	// char* uris[3] = {"ssl://localhost:18883", "mqtts://localhost:18884", "tcp://127.0.0.1:1888"};
#endif

	conn_opts.connectTimeout = 5;            //5秒未连接成功则超时
    conn_opts.keepAliveInterval = 20;   //客户端会每隔20秒向服务器发送心跳包
    //Clean Session设置为0，表示服务器会保存会话状态和订阅的主题，客户端断开后重新连接还能自动订阅之前的主题(离线消息也会收到)
    //Clean Session设置为1，表示客户端每次连接时，服务器不会保留之前的会话状态和订阅信息，断开重连后，离线消息也不会收到
    conn_opts.cleansession = 0; // 0：客户端断开后，服务器会保存该客户端的订阅信息和未发送的消息, 客户端重新连接时可以继续接收离线时的消息; 1：反之
    conn_opts.onSuccess = onConnect;        // 仅第一次连接成功时调用该回调函数(自动重连成功后不会调用)
    conn_opts.onFailure = onConnectFailure;
	conn_opts.username = "admin";
	conn_opts.password = "admin";
	// conn_opts.binarypwd.data = "admin&%^_";    // 用于存储特殊符号的密码
	// conn_opts.binarypwd.len = (int)strlen(conn_opts.binarypwd.data);
	conn_opts.MQTTVersion = MQTTVERSION_DEFAULT;
    // 遗嘱设置。客户端异常断开连接时，服务器会发布遗嘱消息
	conn_opts.will = &will_opts;
	conn_opts.will->message = "will message";   //遗嘱消息
	conn_opts.will->qos = 1;
	conn_opts.will->retained = 0;               //0:表示遗嘱消息不会被服务器保留,针对本身
	conn_opts.will->topicName = "will topic";   //遗嘱主题
    conn_opts.context = client;
#ifdef URIS
	// conn_opts.serverURIs = uris;     //优先连接这些服务器地址。若都连接失败则连接创建客户端时默认配置的地址
	// conn_opts.serverURIcount = 3;    //表示服务器 URI 数量，客户端会根据这个数量遍历 serverURIs 数组中的地址
#endif
#ifdef HTTPHEADERS
    conn_opts.httpHeaders = httpHeaders; // 自定义HTTP头部, 以便在通过WebSocket连接时使用
#endif
    // 异常断开自动重连机制
	conn_opts.automaticReconnect = true; //true:开启断线后的自动重连功能   false:不启用自动重连功能
    conn_opts.minRetryInterval = 3;  // 自动重连时的最小重连间隔(秒)。每次重连失败后，重连间隔会以指数方式增长
    conn_opts.maxRetryInterval = 6; // 自动重连时的最大重连间隔(秒)。重连间隔时间不会超过这个值

    // SSL(使用ssl时要确保设备时间和和服务器时间是一样的)
    if (ssl_flag)
    {
        //指定SSL/TLS协议版本为 TLS 1.2
		ssl_opts.sslVersion = MQTT_SSL_VERSION_TLS_1_2;

        // SSL 错误回调; 证书验证失败等错误时会调用该回调函数
        ssl_opts.ssl_error_cb = sslErrorCallback;
        ssl_opts.ssl_error_context = (void *)"Client SSL Module";// 错误回调自定义上下文

        // 括号{}内不要定义局部变量，否则出了括号后赋值的指针会指向无效内存。所以需要定义静态变量static
        // ALPN (应用层协议协商) 配置
        // 配置 ALPN 协议列表, 声明客户端支持的协议类型, 服务器会选择其中一个
        // ALPN 机制能确保客户端和服务器在 TLS 握手阶段达成合适的应用层协议
        // 协议列表格式：每个协议前加长度字节，如 "mqtt" 长度为4，所以长度字节为 x04
        static unsigned char alpnProtos[] = {
            0x04, 'm', 'q', 't', 't',                               // 协议 mqtt
            0x05, 'm', 'q', 't', 't', 's',                          // 协议 mqtts
            0x08, 'm', 'q', 't', 't', 'v', '3', '.', '1',           // 协议 mqttv3.1
            0x0A, 'm', 'q', 't', 't', 'v', '3', '.', '1', '.', '1', // 协议 mqttv3.1.1
            0x06, 'm', 'q', 't', 't', 'v', '5',                     // 协议 mqttv5
            0x06, 'm', 'q', 't', 't', 'w', 's',                     // 协议 mqttws
            0x07, 'm', 'q', 't', 't', 'w', 's', 's',                // 协议 mqttwss
            0x08, 'h', 't', 't', 'p', '/', '1', '.', '1'            // 协议 http/1.1
        };
        ssl_opts.protos = alpnProtos;
        ssl_opts.protos_len = sizeof(alpnProtos);

        // 单向认证配置
	    ssl_opts.enableServerCertAuth = 1;    //0:关闭服务器证书认证 1:开启 (控制是否验证服务器证书)(enableServerCertAuth 应该与 verify 同步)
	    ssl_opts.verify = 1;//1:客户端会验证服务器证书的合法性(客户端和服务器都有对方的证书时使用) 0:只建立加密连接但不验证服务器身份(服务器没有客户端的证书时使用)
		ssl_opts.trustStore = CA_FILE; //客户端信任的CA证书文件路径，用于验证服务器证书是否合法(需与服务器CA证书文件一致)

        //双向认证配置，则需要配置 keyStore （客户端证书）和 privateKey （客户端私钥）
	    ssl_opts.keyStore = CLIENT_CRT_FILE; //客户端用于向服务器出示的证书文件路径(服务器用于验证客户端身份)
        ssl_opts.privateKey = CLIENT_PRI_KEY;//客户端的私钥(用于解密服务器响应)
		// ssl_opts.privateKeyPassword = "123456"; //客户端的私钥密码(若客户端私钥有设置密码的话)

	    ssl_opts.enabledCipherSuites = "DEFAULT";//启用的加密套件，使用默认套件
	    // ssl_opts.enabledCipherSuites = "PSK-AES128-CBC-SHA";
	    // ssl_opts.ssl_psk_cb = onPSKAuth; //TLS-PSK 认证机制
	    // ssl_opts.ssl_psk_context = (void *) 42;
	    // ssl_opts.CApath = "DUMMY"; //加载该目录路径下的所有CA证书(trustStore是加载一个具体的CAT证书文件)
	    conn_opts.ssl = &ssl_opts;
    }

    // 每次连接成功后调用该回调函数(包括自动重连成功后也会调用)
	MQTTAsync_setConnected(client, client, onReConnect);

    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start connect, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    // 等待连接和订阅完成
    while (!connected || !subscribed) {
        usleep(100000);  // 100ms
    }

    // 发布消息
    payload = (char *)malloc(PAYLOAD_SIZE);
    if (payload != NULL)
        strncpy(payload, PAYLOAD, PAYLOAD_SIZE);
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    pubmsg.payload = (void*)payload;
    pubmsg.payloadlen = PAYLOAD_SIZE;
    pubmsg.qos = QOS;
    //如果想清除之前服务器保留的消息，则必须retained=1且发布空消息
    //1：表示该消息会被服务器保存 0: 反之
    pubmsg.retained = 0;

    MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;
    pub_opts.onSuccess = onSend;    //发送成功回调
    pub_opts.onFailure = onSendFailure;
    pub_opts.context = client;

    if ((rc = MQTTAsync_sendMessage(client, PUBLISH_TOPIC, &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start sendMessage, return code %d\n", rc);
        return EXIT_FAILURE;
    }

#if 0
    //查询消息发送状态，判断某个消息是否已经完成发送和确认 (上面发送消息时库会给 pub_opts.token 赋值)
    //非阻塞查询消息是否被确认
	rc = MQTTAsync_isComplete(client, pub_opts.token);
    if (MQTTASYNC_SUCCESS != rc)
        fprintf(stderr, "sendMessage failed, token %d\n", pub_opts.token);

   //阻塞等待消息被确认或发送成功, 超时单位毫秒; 适合需要同步确认消息发送结果的场景
	rc = MQTTAsync_waitForCompletion(client, pub_opts.token, 5000L);
    if (MQTTASYNC_SUCCESS != rc)
        fprintf(stderr, "sendMessage failed, token %d\n", pub_opts.token);
#endif

    sleep(1);
    
    if ((rc = MQTTAsync_sendMessage(client, PUBLISH_TOPIC, &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start sendMessage, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    // 等待消息发送或接收完成
    while (!finished) {
    // while (1) {
        usleep(100000);  // 100ms
    }

    free(payload);
    sleep(1); //等待连接完成   

    // 查询连接状态
    int isConnected = MQTTAsync_isConnected(client);    //连接状态 0:未连接 1:已连接
    printf("Connected status: %s\n", isConnected?"on-line":"off-line");

    // 取消订阅
	MQTTAsync_responseOptions unsub_opts = MQTTAsync_responseOptions_initializer;
    unsub_opts.onSuccess = onUnSubscribe;
    unsub_opts.onFailure = onUnSubscribeFailure;
	unsub_opts.context = client;
	rc = MQTTAsync_unsubscribe(client, SUBSCRIBE_TOPIC, &unsub_opts); //取消订阅后就收不到离线消息了
    if (rc != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start subscribe, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    //等待取消订阅完成回调后再销毁客户端，避免资源冲突
    while (!unsubscribed)   
    {
        usleep(100000);  // 100ms
    }

    // 销毁客户端
    MQTTAsync_destroy(&client);

    printf("Finished\n");
    return EXIT_SUCCESS;
}




    // 获取未发送成功的token,并释放
#if 0
	int i = 0, rc = 0, count = 0;
	MQTTAsync_token *tokens;
	do
	{
		rc = MQTTAsync_getPendingTokens(c, &tokens);
		i = 0;
		if (tokens)
		{
			while (tokens[i] != -1)
				++i;
			MQTTAsync_free(tokens);
		}
		if (i > 0)
			MySleep(100);
	}
	while (i > 0 && ++count < 10);
#endif




