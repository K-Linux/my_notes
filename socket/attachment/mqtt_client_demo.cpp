
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#include <unistd.h>
#include "mqtt/MQTTAsync.h"

// #define ADDRESS         "tcp://broker.hivemq.com:1883"
// #define ADDRESS         "tcp://localhost:1883"
// #define ADDRESS         "tcp://127.0.0.1:1883"
#define ADDRESS         "127.0.0.1:1883"
#define CLIENTID        "LinuxARM_C_Async_client"
#define TOPIC           "test/topic"
#define PAYLOAD         "test/topic"
#define PAYLOAD_SIZE    1024
#define QOS             1       //服务器质量 0 1 2
#define TIMEOUT         10000L

// #define RECONNECT       1
// #define DEBUG           1
// #define CREATE_OPTIONS_CLIENT 1
// #define URIS            1

volatile int connected = 0;
volatile int subscribed = 0;
volatile int unsubscribed = 0;
volatile int finished = 0;
volatile int ssl_flag = 0;
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
typedef struct
{
    /* 当发布消息时，库会分配一个 token 给这条消息，当消息发布成功(失败)时会执行 onSend(onFailure) 回调
     * 回调中会带上这个 token，告知哪条消息发送成功或失败了 */
	MQTTAsync_token token;
	union
	{
		int qos;        //订阅时服务器授予的 Qos
		int* qosList;   //多主题订阅时的 Qos 列表（MQTT5.0 版本才有）
		struct
		{
			MQTTAsync_message message;  //发布的消息
			char* destinationName;      //主题名
		} pub;
		struct
		{
			char* serverURI;            //连接的服务器地址
			int MQTTVersion;            //MQTT 协议版本
			int sessionPresent;         //会话是否存在
		} connect;
	} alt;
} MQTTAsync_successData;

//发送端需要配置 MQTTAsync_message, 接收端即能看到这些配置
typedef struct
{
	char struct_id[4];          //结构标识，必须是"MQTT"
	int struct_version;         //结构版本，通常是 0 或 1
	int payloadlen;             //负载长度（字节数）
	void* payload;              //指向负载数据的指针
	int qos;                    //Qos等级（0,1,2）
	int retained;               //保留标志，0=非保留 1=保留
	int dup;                    //是否为重复消息，接收时有效
	int msgid;                  //接收消息的ID，用于唯一标识一条消息，用于判断该消息是否重发；当消息重发时 dup 会设置为 1
	MQTTProperties properties;  //MQTT5.0 属性（可选）
} MQTTAsync_message;

typedef struct
{
    MQTTAsync_token token;      //失败请求的 token
    int code;                   //错误码，使用 MQTTAsync_strerror(code) 获取错误描述字符串
    const char *message;        //可选的错误消息字符串，可能为 NULL
} MQTTAsync_failureData;

#endif


// Trace回调函数，用于接收库内部日志
void traceCallback(enum MQTTASYNC_TRACE_LEVELS level, char* message)
{
    const char* levelStr = NULL;
    switch(level)
    {
        case MQTTASYNC_TRACE_MAXIMUM: levelStr = "MAXIMUM"; break;
        case MQTTASYNC_TRACE_MEDIUM: levelStr = "MEDIUM"; break;
        case MQTTASYNC_TRACE_MINIMUM: levelStr = "MINIMUM"; break;
        case MQTTASYNC_TRACE_PROTOCOL: levelStr = "PROTOCOL"; break;
        case MQTTASYNC_TRACE_ERROR: levelStr = "ERROR"; break;
        default: levelStr = "UNKNOWN"; break;
    }
    printf("[MQTT TRACE][%s] %s\n", levelStr, message);
}

void onSend(void* context, MQTTAsync_successData* response)
{
    printf("token: %d\n", response->token);
    printf("Published topic: %s\n", response->alt.pub.destinationName);
    printf("Payload length: %d\n", response->alt.pub.message.payloadlen);
    printf("Message sent successfully\n");
}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{
    int code = response->code;
    printf("Token: %d\n", response->token);
    printf("Error code: %d\n", response->code);
    printf("Error code: %d, %s\n", code, MQTTAsync_strerror(code));
    if (response->message)
        printf("Error message: %s\n", response->message);

    printf("Failed to send message\n");
}
//发送端需要配置 MQTTAsync_message, 接收端即能看到这些配置
int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message)
{
    printf("topic:[%s], message:[%.*s]\n", topicName, message->payloadlen, (char*)message->payload);
    printf("Qos:%d, Retained:%d dup:%d, msgid:%d\n", message->qos, message->retained, message->dup, message->msgid);
    printf("accept succeeded\n");
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    finished = 1;   //标记消息处理完成
    return 1;
}
void onSubscribe(void* context, MQTTAsync_successData* response)
{
    printf("Granted Qos: %d\n", response->alt.qos);
    printf("Subscribe succeeded\n");
    subscribed = 1;
}
void onUnSubscribe(void* context, MQTTAsync_successData* response)
{
    printf("Unsubscribe succeeded\n");
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
    printf("Disconnect success\n");
    finished = 1;
}
void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
    printf("Token: %d\n", response->token);
    printf("Error code: %d\n", response->code);
    if (response->message)
        printf("Error message: %s\n", response->message);

    printf("subscribe failed\n");
    finished = 1;
}
void onUnSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
    printf("Unsubscribe failed\n");
}
void onReConnect(void* context, char* cause)
{
    printf("Successfully onReConnect\n");
}
void onConnect(void* context, MQTTAsync_successData* response)
{
    printf("Server URI: %s\n", response->alt.connect.serverURI);
    printf("MQTT Version: %d\n", response->alt.connect.MQTTVersion);
    printf("Session Present: %d\n", response->alt.connect.sessionPresent);
    printf("Successfully connected\n");
    connected = 1;

    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_responseOptions sub_opts = MQTTAsync_responseOptions_initializer;
    sub_opts.onSuccess = onSubscribe;
    sub_opts.onFailure = onSubscribeFailure;
    sub_opts.context = client;

    //一次性批量订阅主题
    //int qoss[2] = {2, 2};
    //char* topics[2] = {willTopic, test_topic};
    //rc = MQTTAsync_subscribeMany(client, 2, topics, qoss, &sub_opts);//一次性批量订阅主题
    //rc = MQTTAsync_unsubscribeMany(client, count, topics, qoss, response);//一次性批量取消订阅主题

    int rc = MQTTAsync_subscribe(client, TOPIC, QOS, &sub_opts);
    if (rc != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start subscribe, return code %d\n", rc);
        finished = 1;
    }
}
void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    int code = response->code;
    printf("Token: %d\n", response->token);
    printf("Error code: %d, %s\n", code, MQTTAsync_strerror(code));
    if (response->message)
        printf("Error message: %s\n", response->message);
    printf("Connect failed\n");
    finished = 1;

}
void onConnectionLost(void* context, char* cause)
{
    if (cause)
        printf("cause:%s\n", cause);
    connected = 0;

    printf("Connection lost, Trying to reconnect\n");
    // 若 automaticReconnect = true, 则库会自动处理重连，一般不需要手动调用 MQTTAsync_reconnect() 函数
#ifdef RECONNECT
    MQTTAsync client = (MQTTAsync)context;
    int rc = MQTTAsync_reconnect(client);//异常断线后手动重连
    if (rc != MQTTASYNC_SUCCESS)
        printf("Failed to start reconnect, return code %d\n", rc);
#endif
}

int main(int argc, char* argv[])
{
#ifdef DEBUG
    //设置日志级别为 VERBOSE，输出所有调试信息（必须在连接之前设置）
    //MQTTAsync_setTraceLevel(MQTTASYNC_TRACE_ERROR);
    //MQTTAsync_setTraceCallback(traceCallback);
#endif
    int rc;
    MQTTAsync client;

    //获取版本信息
    MQTTAsync_nameValue* ver_info = MQTTAsync_getVersionInfo();
    while (ver_info->name)
    {
        fprintf(stderr, "mqtt info:%s, %s\n", ver_info->name, ver_info->value);
        ver_info++;
    }

#ifdef CREATE_OPTIONS_CLIENT
    MQTTAsync_createOptions createOptions = MQTTAsync_createOptions_initializer;
    createOptions.sendWhileDisconnected = 1;    //1: 连接断开后，允许发送消息存入缓存，等待重新连接后发送。MQTTAsync_sendMessage() 函数依旧返回 true (cleansession 控制接收离线消息，sendWhileDisconnected 控制发送离线消息)
	createOptions.maxBufferedMessages = 50;     //最大缓存消息数，超过后会拒绝新消息或删除旧消息
	createOptions.struct_version = 2;           //用于标识结构体的版本，不同版本支持不同的字段。 0:最基础版本，不支持 MQTTVersion 等字段 1:支持 sendWhileDisconnected maxBufferedMessages 2:支持 persistQoS0 字段。3:支持更多高级选项
	createOptions.MQTTVersion = MQTTVERSION_DEFAULT;
	createOptions.allowDisconnectedSendAtAnyTime = 0;   //1:即使客户端从未连接成功，调用 MQTTAsync_sendMessage() 也不会失败，消息会被缓存等待连接建立后发送  0:反之
	createOptions.deleteOldestMessages = 1;     //1：缓存满时删除最旧消息   0：缓存满时拒绝新消息
	createOptions.restoreMessages = 1;          //1：断线重连时恢复未发送或未确认的消息。即断线时未完成的 Qos 1 或 Qos 2 消息会被保存  0：反之
	createOptions.persistQoS0 = 0;              //不持久化 Qos 0 的消息（一般设置为 0）
#endif

    // MQTTCLIENT_PERSISTENCE_NONE 表示持久化机制，使用内存来保存数据，不持久化到文件
#ifdef CREATE_OPTIONS_CLIENT
    rc = MQTTAsync_createWithOptions(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL, &MQTTAsync_createOptions);
#else
    rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
#endif
    if (rc != MQTTASYNC_SUCCESS)
    {
        MQTTAsync_destroy(&client);
        return -1;
    }

    // 参数2:用户自定义的上下文指针context，会传递给回调函数
    // 参数3:连接断开时调用
    // 参数4:收到订阅的消息调用
    // 参数5:当QoS 1 或 QoS 2 的消息成功发送时调用
    MQTTAsync_setCallbacks(client, client, onConnectionLost, messageArrived, NULL);

    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;
    MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
#ifdef URIS
    char* uris[3] = {"ssl://localhost:1883", "mqtts://localhost:1884", "tcp://127.0.0.1:1885"};
#endif
    conn_opts.connectTimeout = 5;           //5秒未连接成功则超时
	conn_opts.keepAliveInterval = 20;       //客户端会每隔20秒想服务器发送心跳包
    //cleansession 设置为 0，表示服务器会保存会话状态和订阅的主题，客户端断开后重新连接还能自动订阅之前的主题(离线消息也会收到)
    //cleansession 设置为 1，表示客户端每次连接时，服务器不会保留之前的会话状态和订阅信息，断开重连后，离线消息也不会收到
    // 0:客户端断开后，服务器会保存该客户端的订阅信息和未发送的消息，客户端重新连接时可以继续接收离线时的消息。 1：反之
	conn_opts.cleansession = 0;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.username = "admin";
	conn_opts.password = "admin";
	conn_opts.binarypwd.data = "admin&%^_"; //用于存储特殊符号的密码
	conn_opts.binarypwd.len = (int)strlen("admin&%^_");
	conn_opts.MQTTVersion = MQTTVERSION_DEFAULT;

    //遗嘱设置。客户端异常断开连接时，服务器会发布遗嘱消息
	conn_opts.will = &will_opts;
	conn_opts.will->message = "will message"; //遗嘱消息
	conn_opts.will->qos = 1;
	conn_opts.will->retained = 0;             //0:表示遗嘱消息不会被服务器保留，针对本身
	conn_opts.will->topicName = "will topic"; //遗嘱主题
	conn_opts.context = client;
#ifdef URIS
    conn_opts.serverURIs = uris;    //优先连接这些服务器地址。若这些都连接失败则连接创建客户端时默认配置的地址
    conn_opts.serverURIcount = 3;   //表示服务器 URI 数量。会根据这个数量遍历 serverURIs 中的地址
#endif
    //异常断开自动重连机制
    conn_opts.automaticReconnect = true;    //true:开启断线后的自动重连功能     false:不启用自动重连功能
    conn_opts.minRetryInterval = 3;         //自动重连时的最小重连间隔(秒)。每次重连失败后，重连间隔会以指数方式增长
    conn_opts.maxRetryInterval = 6;         //自动重连时的最大重连间隔(秒)。重连间隔时不会超过这个值

    if (ssl_flag)
    {
        char server_key_file[256] = "/etc/mosquitto/certs/ca.crt";  //服务器公钥证书
        char client_key_file[256] = "/etc/mosquitto/certs/client.crt";  //客户端公钥证书
        char server_key_pass[256] = "1234"; //客户端私钥密码
        ssl_opts.sslVersion = MQTT_SSL_VERSION_TLS_1_2; //指定SSL/TLS协议版本为 TLS1.2
        ssl_opts.privateKey = "/mnt/mtd/code/config_file_bak/user_serverkey.pem";//客户端用于SSL认证的私钥文件。私钥文件通常用于客户端身份验证，确保客户端的身份合法
        ssl_opts.enableServerCertAuth = 0;//0：关闭服务器证书认证  1：开启 (控制是否验证服务器证书)(enableServerCertAuth 应该与 verify 同步)
        ssl_opts.verify = 0;              //1：客户端会验证服务器证书的合法性(客户端和服务器都有对方的证书时使用)  0：只建立加密连接但不验证服务器身份(服务器没有客户端的证书时使用)
        ssl_opts.trustStore = server_key_file;//客户端信任的CA证书文件路径，用于验证服务器证书是否合法(若客户端没有服务器证书则不配置)
        ssl_opts.keyStore = client_key_file;//客服端用于向服务器出示的证书文件路径(客户端自己的证书)
        ssl_opts.privateKeyPassword = client_key_pass;//客户端证书的私钥密码
        ssl_opts.enabledCipherSuites = "DEFAULT";//启用的加密套件，使用默认套件
        // ssl_opts.ssl_psk_cb = onPSKAuth; //TLS-PSK 认证机制
        // ssl_opts.ssl_psk_context = (void *) 42;
        // ssl_opts.enabledCipherSuites = "PSK-AES128-CBC-SHA";
        // ssl_opts.ssl->CApath = "DUMMY";  //加载该目录路径下的所有CA证书(trustStore是加载一个具体的CAT证书文件)
        conn_opts.ssl = &ssl_opts;
        // 客户端没有服务器证书时，仅配置 keyStore, privateKeyPassword 即可，其他参数不用配置
        // 服务器没有客户端证书时，仅配置 trustStore, keyStore, privateKeyPassword 即可，其他参数不用配置
        // 客户端和服务器有双方证书时，仅配置 trustStore, keyStore, privateKeyPassword, verify=1 即可，其他参数不用配置
    }

    // 每次连接成功后调用回到函数（包括自动重连成功后也会调用）
    MQTTAsync_setConnected(client, client, onReConnect);

    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start connect, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    //等待连接和订阅完成
    while (!connected || !subscribed) {
        usleep(100000); //100ms
    }

    //发布消息
    payload = (char*)malloc(PAYLOAD_SIZE);
    if (payload != NULL)
        strncpy(payload, PAYLOAD, PAYLOAD_SIZE);
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    pubmsg.payload = (void *)payload;
    pubmsg.payloadlen = PAYLOAD_SIZE;
    pubmsg.qos = QOS;
    //如果想清除之前服务器保留的消息，则必须retained=1且发布空消息
    //1:表示该消息会被服务器保存 0:反之
    pubmsg.retained = 0;

    MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;
    pub_opts.onSuccess = onSend;    //发送成功回调
    pub_opts.onFailure = onSendFailure;
    pub_opts.context = client;

    if ((rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start sendMessage, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    //查询消息发送状态，判断某个消息是否已经完成发送和确认（上面发送消息时库会给 pub_opts.token 赋值）
    //非阻塞查询消息是否被确认
    rc = MQTTAsync_isComplete(client, pub_opts.token);
    if (MQTTASYNC_SUCCESS != rc)
        fprintf(stderr, "sendMessage failed, token %d\n", pub_opts.token);

    //阻塞等待消息被确认或发送成功，超时单位毫秒; 适合需要同步确认消息发送结果的场景
    rc = MQTTAsync_waitForCompletion(client, pub_opts.token, 5000L);
    if (MQTTASYNC_SUCCESS != rc)
        fprintf(stderr, "sendMessage failed, token %d\n", pub_opts.token);

    sleep(1);

    if ((rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start sendMessage, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    //等待消息发送或接收完成
    while (!finished) {
    // while (1) {
        usleep(100000);
    }

    free(payload);
    sleep(1);    //等待连接完成

    //查询连接状态
    int isConnected = MQTTAsync_isConnected(client);    //连接状态 0:未连接 1:已连接
    printf("Connected status: %s\n", isConnected?"on-line":"off-line");

    //取消订阅
    MQTTAsync_responseOptions unsub_opts = MQTTAsync_responseOptions_initializer;
    unsub_opts.onSuccess = onUnSubscribe;
    unsub_opts.onFailure = onUnSubscribeFailure;
    unsub_opts.context = client;
    rc = MQTTAsync_unsubscribe(client, TOPIC, &unsub_opts); //取消订阅后就收不到离线消息了
    if (rc != MQTTASYNC_SUCCESS) {
        fprintf(stderr, "Failed to start subscribe, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    //等待取消订阅完成回调后再销毁客户端，避免资源冲突
    while (!unsubscribed)
    {
        usleep(100000); //100ms
    }

    //销毁客户端
    MQTTAsync_destroy(&client);

    printf("Finished\n");
    return EXIT_SUCCESS;

    return 0;
}


#if 0
    //获取未发送成功的token，并释放
    int i = 0, rc = 0, count = 0;
    MQTTAsync_token *tokens;
    do 
    {
        rc = MQTTAsync_getPendingTokens(client, &tokens);
        i = 0;
        if (token)
        {
            while (tokens[i] != -1)
                ++i;
            MQTTAsync_free(tokens);
        }
        if (i > 0)
            sleep(100);

    }while (i > 0 && ++count < 10);
#endif































