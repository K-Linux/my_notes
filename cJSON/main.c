
#define NUM 1

#if (NUM == 1)

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

//每个结点和节点都是一个JSON结构体
int main(int argc, char *argv)
{
	//创建头指针
	cJSON *cjson_test = NULL;
	cJSON *cjson_address = NULL;
	cJSON *cjson_skill = NULL;
	char *str = NULL;

	//创建一个JSON结构体，并将头指针指向该结构体的头结点
	cjson_test = cJSON_CreateObject();
	//添加一条字符串类型的JSON结构体(添加一个链表节点)
	cJSON_AddStringToObject(cjson_test, "name", "Linux");
	//添加一条整型JSON结构体(添加一个链表节点)
	cJSON_AddNumberToObject(cjson_test, "age", 21);
	//添加一条浮点型JSON结构体(添加一个链表节点)
	cJSON_AddNumberToObject(cjson_test, "weight", 60.5);

    //另一种方式添加JSON结构体(添加一个链表节点)
	cJSON_AddItemToObject(cjson_test, "height", cJSON_CreateNumber(165));
	cJSON_AddItemToObject(cjson_test, "alias", cJSON_CreateString("小明"));

	//创建一个JSON结构体对象，并将头指针指向该结构体的头结点
	cjson_address = cJSON_CreateObject();
	cJSON_AddStringToObject(cjson_address, "country", "China");
	cJSON_AddNumberToObject(cjson_address, "zip-code", 366301);
	//添加一个嵌套的JSON结构体(将头指针cjson_test的最后一个节点的child指针指向cjson_address，即赋值)
	cJSON_AddItemToObject(cjson_test, "address", cjson_address);

	//创建一个JSON结构体对象，并将头指针指向该结构体的头结点
	cjson_skill = cJSON_CreateArray();
	cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("C"));
	cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("Java"));
	cJSON_AddItemToArray(cjson_skill, cJSON_CreateNumber(100));
	//添加一个数组类型的JSON结构体(将头指针cjson_test的最后一个节点的child指针指向cjson_skill，即赋值)
	cJSON_AddItemToObject(cjson_test, "skill", cjson_skill);

	//添加一个布尔类型的JSON结构体(添加一个链表节点)
	cJSON_AddBoolToObject(cjson_test, "return1", 0);
	cJSON_AddBoolToObject(cjson_test, "return2", 1);
    //添加true和false类型的JSON结构体
    cJSON_AddTrueToObject(cjson_test, "vip1");
    cJSON_AddFalseToObject(cjson_test, "vip2");
    //添加true和null类型的JSON结构体
    cJSON_AddTrueToObject(cjson_test, "vip3");
    cJSON_AddNullToObject(cjson_test, "vip4");

	//将cJSON结构体结构体解析成JSON格式的字符串，在堆中开辟char *的内存并返回
	str = cJSON_Print(cjson_test);
	printf("%s\n", str);

	//注意free释放char *堆内存
	free(str);
    //释放JSON结构体 (释放cjson_test时，cjson_address也会一起释放)
	cJSON_Delete(cjson_test);

	return 0;
}

#elif (NUM == 2)

#include <stdio.h>
#include "cJSON.h"

char *message = 
"{                              				\
    \"name\":\"mculover666\",   				\
    \"age\": 22,                				\
    \"weight\": 55.5,           				\
    \"address\":                				\
        {                       				\
            \"country\": \"China\",				\
            \"zip-code\": 366301				\
        },                      				\
    \"skill\": [\"c\", \"Java\", \"99\"],	\
    \"student\": true          				\
}";

int main(void)
{
    cJSON* cjson_test = NULL;
    cJSON* cjson_name = NULL;
    cJSON* cjson_age = NULL;
    cJSON* cjson_weight = NULL;
    cJSON* cjson_address = NULL;
    cJSON* cjson_address_country = NULL;
    cJSON* cjson_address_zipcode = NULL;
    cJSON* cjson_skill = NULL;
    cJSON* cjson_student = NULL;
    int    skill_array_size = 0, i = 0;
    cJSON* cjson_skill_item = NULL;

    /* 解析整段JSON字符串数据(一行或回车都行)，存储在cJSON结构体内，在堆中开辟一块内存并返回cJSON *指针 */
    cjson_test = cJSON_Parse(message);
    if (cjson_test == NULL) {
        printf("parse fail.\n");
        return -1;
    }

    /* 依次根据名称提取JSON数据（键值对） */
    cjson_name = cJSON_GetObjectItem(cjson_test, "name");
    cjson_age = cJSON_GetObjectItem(cjson_test, "age");
    cjson_weight = cJSON_GetObjectItem(cjson_test, "weight");

    printf("name: %s\n", cjson_name->valuestring);
    printf("age:%d\n", cjson_age->valueint);
    printf("weight:%.1f\n", cjson_weight->valuedouble);

    /* 解析嵌套json数据 */
    cjson_address = cJSON_GetObjectItem(cjson_test, "address");
    cjson_address_country = cJSON_GetObjectItem(cjson_address, "country");
    cjson_address_zipcode = cJSON_GetObjectItem(cjson_address, "zip-code");
    // 使用type判断字符串或整型
    if (cjson_address_country->type == cJSON_String)
        printf("address-country:%s\n", cjson_address_country->valuestring);
    if (cjson_address_zipcode->type == cJSON_Number)
        printf("address-zipcode:%d\n", cjson_address_zipcode->valueint);

    /* 解析数组 */
    cjson_skill = cJSON_GetObjectItem(cjson_test, "skill");
    skill_array_size = cJSON_GetArraySize(cjson_skill);     //获取数组长度
    printf("skill:[");
    for(i = 0; i < skill_array_size; i++)
    {
        cjson_skill_item = cJSON_GetArrayItem(cjson_skill, i);
        printf("%s,", cjson_skill_item->valuestring);
    }
    printf("\b]\n");    // \b表示backspace（与\n要有间隔）
    //可以使用 cJSON_ArrayForEach 遍历JSON数组

    /* 解析布尔型数据 */
    cjson_student = cJSON_GetObjectItem(cjson_test, "student");
    if(cjson_student->valueint == 0) {
        printf("student: false\n");
    } else {
        printf("student:true\n");
    }

	//释放堆中cJSON结构体内存
	cJSON_Delete(cjson_test);
    
    return 0;
}

#elif (NUM == 3)
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

int main(void)
{
	//手动创建一个JSON格式的字符串
	char *char_json = "{\"habit\":\"runing\"}";
	//将字符串char_json转换为cJSON结构体
	cJSON *json = cJSON_Parse(char_json);
	//将cJSON结构体结构体解析成JSON格式的字符串，在堆中开辟char *的内存并返回
	char *buf1 = cJSON_Print(json);
	//将cJSON结构体转换为无格式的一行字符串，在堆中开辟char *的内存并返回
	char *buf2 = cJSON_PrintUnformatted(json);	
    printf("%s\n%s\n", buf1, buf2);
	//手动释放char *堆内存
	free(buf1);
	free(buf2);
	cJSON_Delete(json);
}
#elif (NUM = 4)
#elif (NUM = 5)
#elif (NUM = 6)
#elif (NUM = 7)
#elif (NUM = 8)
#elif (NUM = 9)


#endif














#if 0
	ON_AddNullToObject(cJSON * const object, const char * const name);
	
	cJSON_AddTrueToObject(cJSON * const object, const char * const name);
	
	cJSON_AddFalseToObject(cJSON * const object, const char * const name);
	
	cJSON_AddBoolToObject(cJSON * const object, const char * const name, const cJSON_bool boolean);
	
	cJSON_AddNumberToObject(cJSON * const object, const char * const name, const double number);
	
	cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string);
	
	cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw);
	
	cJSON_AddObjectToObject(cJSON * const object, const char * const name);
	
	cJSON_AddArrayToObject(cJSON * const object, const char * const name);
#endif
