#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_CONTENT_COUNT 100
//移动文件/目录（也可以重命名）
int g_MoveFile(const char *oldFileName, const char *newFileName);
//根据行数打印文件的内容
int g_FilePrintByLine(const char *filePath, int lineCount);
//获取文件行数
int g_GetLineCount(const char *filePath/*in*/, int *lineCount/*out*/);
//按每行规定字数读取
int g_LineReadByCount(const char *filePath/*in*/, int offsetStart/*in*/, int count/*in*/);
//获取到的key-value写入文件
int g_KeyValueInput(char *key/*in*/, char *value/*in*/, const char *path/*in*/);
//从文件中读取key-value
int g_KeyValueOutput(char **key/*out*/, char **value/*out*/, const char *filePath/*out*/);
//
char *g_TrimStr(const char *str);
//
void g_GetMem_Err(char *p);
//
int g_GetLineCount2_0(const char *filePath/*in*/, int *lineCount/*out*/);

#endif // FILE_H_INCLUDED
