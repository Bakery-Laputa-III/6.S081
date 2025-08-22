#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*
 * ls.c - 列出目录内容程序
 * 该程序实现类似Unix的ls命令功能，可以列出指定目录下的文件和子目录信息
 */

/*
 * fmtname - Format Name (格式化名称)
 * 从文件路径中提取文件名并进行格式化，使其长度固定为DIRSIZ
 * 参数: path - 文件路径字符串
 * 返回值: 格式化后的文件名（固定长度，不足部分用空格填充）
 */
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // 查找最后一个斜杠后的第一个字符
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // 返回空格填充的名称
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

/*
 * ls - List (列表)
 * 列出指定路径下的文件和目录信息
 * 参数: path - 要列出的目录或文件路径
 * 功能: 打印文件/目录的类型、inode号、大小等信息
 */
void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // 打开文件或目录
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  // 获取文件状态信息
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // 根据文件类型处理
  switch(st.type){
  case T_FILE:
    // 处理普通文件：打印文件名、类型、inode号和大小
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    // 处理目录：遍历目录中的所有条目
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

/*
 * main - Main Function (主函数)
 * 程序入口点，解析命令行参数并调用ls函数
 * 参数: argc - 参数个数，argv - 参数数组
 * 功能: 如果没有参数则列出当前目录，否则列出所有指定路径
 */
int
main(int argc, char *argv[])
{
  int i;

  // 如果没有参数，列出当前目录
  if(argc < 2){
    ls(".");
    exit(0);
  }
  // 遍历所有参数，列出每个指定路径
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
