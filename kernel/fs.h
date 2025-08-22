/*
 * fs.h - File System Header (文件系统头文件)
 * 该头文件定义了xv6文件系统的磁盘格式和数据结构
 * 内核和用户程序都会使用这个头文件来理解文件系统的布局
 */

#include "types.h"

// On-disk file system format.
// Both the kernel and user programs use this header file.


/* 根目录的inode编号，固定为1 */
#define ROOTINO  1   // root i-number

/* 文件系统块大小，固定为1024字节 */
#define BSIZE 1024  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:

/* 超级块结构体 - 描述文件系统的整体布局和参数 */
struct superblock {
  uint magic;        /* 文件系统魔数，必须是FSMAGIC用于验证文件系统有效性 */
  uint size;         /* 文件系统镜像的总大小（以块为单位） */
  uint nblocks;      /* 数据块的数量 */
  uint ninodes;      /* inode节点的数量 */
  uint nlog;         /* 日志块的数量 */
  uint logstart;     /* 第一个日志块的块号 */
  uint inodestart;   /* 第一个inode块的块号 */
  uint bmapstart;    /* 第一个空闲位图块的块号 */
};

/* 文件系统魔数，用于标识有效的xv6文件系统 */
#define FSMAGIC 0x10203040

/* 直接索引块数量，文件可以直接访问12个数据块 */
#define NDIRECT 12

/* 间接索引块数量，一个块可以存储的uint指针数量 */
#define NINDIRECT (BSIZE / sizeof(uint))

/* 文件最大块数 = 直接索引块数 + 间接索引块数 */
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure

/* 磁盘inode结构体 - 存储文件的元数据信息 */
struct dinode {
  short type;           /* 文件类型（普通文件、目录、设备文件等） */
  short major;          /* 主设备号（仅用于T_DEVICE类型文件） */
  short minor;          /* 次设备号（仅用于T_DEVICE类型文件） */
  short nlink;          /* 文件系统中指向该inode的硬链接数量 */
  uint size;            /* 文件大小（以字节为单位） */
  uint addrs[NDIRECT+1];   /* 数据块地址数组，前12个为直接索引，最后1个为间接索引 */
};

// Inodes per block.

/* 每个块可以容纳的inode数量 */
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i

/* 计算包含指定inode的块号 */
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block

/* 每个块的位数（用于空闲位图） */
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b

/* 计算包含指定块号的空闲位图块号 */
#define BBLOCK(b, sb) ((b)/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.

/* 目录项中文件名的最大长度 */
#define DIRSIZ 14

/* 目录项结构体 - 目录中的每个条目 */
struct dirent {
  ushort inum;      /* inode编号，指向文件的元数据 */
  char name[DIRSIZ]; /* 文件名，最大长度为DIRSIZ */
};

