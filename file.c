#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "string.h"

/*******************
*    Function:       g_MoveFile
*    Description:    移动文件/目录（也可以重命名）
*    Calls:
*    Called by:
*    Input:          char *oldFileName      原来的文件名
                     char *newFIleName      移动后的文件名
*
*    Output:
*    Return:         0     成功
*                    非0   失败
*
*
*    Others:
********************/

int g_MoveFile(const char *oldFileName, const char *newFileName)
{
    if (oldFileName == NULL || newFileName == NULL)
    {
        return -1;
    }
    const char *pOld = oldFileName;
    const char *pNew = newFileName;
    int result = 0;
    result = rename(pOld, pNew);
    if (result)
    {
        return 0;

    }
    else
    {
        return -1;
    }

}

/*******************
*    Function:       g_FilePrintByLine
*    Description:    根据行数打印文件的内容
*    Calls:
*    Called by:
*    Input:          const char *FilePath      文件名
                     int LineCount      打印的行数
*
*    Output:
*    Return:         *res     返回二进制1的个数
*
*
*    Others:
********************/
int g_FilePrintByLine(const char *filePath, int lineCount)
{
    if (filePath == NULL)
    {
        return -1;
    }
    const char *pFileName = filePath;
    FILE *pFile = fopen(pFileName, "r");
    if (pFile == NULL)
    {
        return -1;
    }
    char ch;
    int i = 0;
    do
    {
         ch = fgetc(pFile);

         //当换行符等于lineCount，
         if (ch == '\n')
         {
             i++;
         }
         if (i == lineCount)
         {
             break;
         }
         putchar(ch);

    }while(ch != EOF);
    fclose(pFile);
    return 0;
}

/*******************
*    Function:       g_GetLineCount
*    Description:    获取文件行数
*    Calls:
*    Called by:
*    Input:          const char *filePath      文件名
                     int lineCount      文件的行数
*
*    Output:
*    Return:         *lineCount     返回文件行数
*
*
*    Others:
********************/
int g_GetLineCount(const char *filePath/*in*/, int *lineCount/*out*/)
{
    if (filePath == NULL || lineCount == NULL)
    {
        return -1;
    }
    const char *pFile = filePath;
    FILE *p = fopen(pFile, "r");
    if (p == NULL)
    {
        return -1;
    }
    char ch = 0;
    *lineCount = 0;
    /*do
    {
        ch = fgetc(p);
        while (ch == '\n')
        {
            (*lineCount)++;
        }
        printf("%c", ch);
    }while(ch != EOF);  错误*/

    for (ch = 0; ch != EOF;)
    {
        ch = fgetc(p);
        if ( ch == '\n')
        {
            (*lineCount)++;
        }

    }
    fclose(p);
    return *lineCount;
}

/*******************
*    Function:       g_LineReadByCount
*    Description:    按每行规定字数读取
*    Calls:
*    Called by:
*    Input:          const char *filePath      文件名
                     int lineCount      文件的行数
*
*    Output:
*    Return:         *lineCount     返回影响的行数
*
*
*    Others:
********************/
int g_LineReadByCount(const char *filePath/*in*/, int offsetStart/*in*/, int count/*in*/)
{
    int flag = 0;
    char ch;
    int lineCount            = 0;              //总行数
    int lineStart            = 0;              //开始的行号
    int lineEnd              = 0;              //结束的行号
    int currentLineCount     = 0;              //当前行号

    if (filePath == NULL)
    {
        flag = -1;
        return flag;
    }
    const char *pFile = filePath;
    FILE *pFopen = fopen(pFile, "r");
    if (pFopen == NULL)
    {
        flag = -2;
        return flag;
    }
    //获取总行数
     for (ch = 0; ch != EOF;)
    {
        ch = fgetc(pFopen);
        if ( ch == '\n')
        {
            (lineCount)++;
        }

    }
    //获取开始与结束的行号
     if (offsetStart >= 0)
        {
            lineStart = offsetStart - 1;
        }
        else
        {
            lineStart = lineCount-(-offsetStart);
        }
        lineEnd = lineStart + count - 1;
    //文件指针回到开头
    rewind(pFopen);
    //遍历文件
    for (ch = 0; ch != EOF;)
    {
        ch = fgetc(pFopen);
        if (ch == '\n')
        {
            currentLineCount++;
        }
        if(currentLineCount >= lineStart && currentLineCount <= lineEnd)
        {
            putchar(ch);
        }
    }


    /*
    char ch = 0;
    int lineCount = 0;
    int printLine = 0;
    for (ch = 0; ch != EOF;)
    {
        ch = fgetc(pFopen);
        if ( ch == '\n')
        {
            lineCount++;

        }
        if (offsetStart >= 0)
        {
            printLine = offsetStart + count;
            if (lineCount < offsetStart)
            {
                continue;
            }
            if (lineCount == count)
            {
                break;
            }
        }
        else
        {

        }

        putchar(ch);
    }
*/
    return flag;
}

/*******************
*    Function:       g_KeyValueInput
*    Description:    获取到的key-value写入文件
*    Calls:
*    Called by:
*    Input:          const char *Path      文件名
                     char *key              key
                     char *value            value
*
*    Output:
*    Return:         0     成功
*                    -1     文件不存在
*                    -2     内存不足
*
*
*    Others:
********************/
int g_KeyValueInput(char *key/*in*/, char *value/*in*/, const char *path/*in*/)
{
    int flag = 0;
    //初始化key-value
    char *Key    =      strcat(key, " = ");
    char *Value  =      value;
    //将key-value拼接撑字符串
    char *kyString = strcat(Key, Value);
    //初始化文件路径
    if (path == NULL)
    {
        flag = -1;
        return flag;
    }
    FILE *pF = NULL;
    pF = fopen(path, "a+");
    if (pF == NULL)
    {
        flag = -2;
        return flag;
    }

    //写入文件
    flag = fputs(kyString, pF);
    fputc('\n', pF);
    free(pF);
    return flag;
}

/*******************
*    Function:       g_KeyValueOutput
*    Description:    从文件中读取key-value
*    Calls:
*    Called by:
*    Input:          const char *filePath      文件名
*                     char *key / out /                 key
*                     char *value  / out /              value
*
*    Output:
*    Return:         0     成功
*                    -1     文件不存在
*                    1      文件不可读
*
*    Others:
********************/
int g_KeyValueOutput(char **key/*out*/, char **value/*out*/, const char *filePath/*out*/)
{
    int flag = 0;
    if (filePath == NULL)
    {
        flag = -1;
        return flag;
    }
    //初始化文件路径
    //以行形式读取文件
    FILE *fp = NULL;
    fp = fopen(filePath, "r");
    if (fp == NULL)
    {
        flag = 1;
        return flag;
    }
    //将字符串分割成key-value
    char p[100] = "";
    fgets(p, 100, fp);
    printf("%s\n", p);
    char *str = strtok(p, "=");
    if (str)
    {
        *key = str;
        //printf("%s\n", *key);
    }

    str = strtok(NULL, "=");
    if (str)
    {
        *value = str;
        //printf("%s\n", *value);
    }

    return flag;
}

/*******************
*    Function:       g_TrimStr
*    Description:    从文件中读取key-value
*    Calls:
*    Called by:
*    Input:          const char *str        字符串
*
*    Output:
*    Return:         0     成功
*                    -1     文件不存在
*                    1      文件不可读
*
*    Others:
********************/
char * g_TrimStr(const char *str)
{
    int flag = 0;
    if (str == NULL)
    {
        flag = -1;
        return;
    }
    //fscanf(stdin, "%s", str);
    fprintf(stdin, "str = %s\n", str);
    int flag1 = fseek(stdin, 1, SEEK_SET);
    //fprintf(stdout, "str=%s\n", str);
    char *ch = NULL;
    fgets(ch, 100, stdin);
    printf("%s, %d\n", ch, flag1);
}

/*******************
*    Function:       KeyValueOutput
*    Description:    从文件中读取key-value
*    Calls:
*    Called by:
*    Input:          const char *filePath      文件名
                     int lineCount      文件的行数
*
*    Output:
*    Return:         *lineCount     返回文件行数
*
*
*    Others:
********************/
void g_GetMem_Err(char *p)
{
	char *tmp = NULL;
	tmp = (char *)malloc(100);
	p = tmp;			//不可以修改p对应的值的内存地址
}


/*******************
*    Function:       GetLineCount
*    Description:    获取文件行数
*    Calls:
*    Called by:
*    Input:          const char *filePath      文件名
                     int lineCount      文件的行数
*
*    Output:
*    Return:         *lineCount     返回文件行数
*
*
*    Others:
*    Version:         2.0
********************/
int g_GetLineCount2_0(const char *filePath/*in*/, int *lineCount/*out*/)
{

    if (filePath == NULL || lineCount == NULL)
    {
        return -1;
    }
    FILE * pF = NULL;
    const char *path = filePath;
    pF = fopen(path, "r");
    char arr[MAX_LINE_CONTENT_COUNT];
    int a = MAX_LINE_CONTENT_COUNT;
    while (!feof(pF))
    {
        fgets(arr, a, pF);
        puts(arr);
        (*lineCount)++;
    }
    printf("Line: %d\n", *lineCount);
    return *lineCount;
}
