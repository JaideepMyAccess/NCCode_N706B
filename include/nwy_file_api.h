/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_file_api.h
 * Author       : hujun
 * Created      : 2023-5-22
 * Description  : nwy_file_api API function declarations
 *
 *****************************************************************************
 */

#ifndef __NWY_FILE_H__
#define __NWY_FILE_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "nwy_common.h"
#include "nwy_adapt_platform.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*
 ********************************************   *********************************
 * 2 Macro Definition
 ****************************************************************************
 */
#define NWY_FILE_NAME_MAX      (255)

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */

typedef enum{
    NWY_SEEK_SET   = 0,
    NWY_SEEK_CUR   = 1,
    NWY_SEEK_END   = 2
}nwy_fseek_offset_e;

typedef enum
{
	NWY_DT_UNKNOWN = 0,         //Unknown type
	NWY_DT_FIFO = 1,            //Pipe
	NWY_DT_CHR = 2,             //Character device
	NWY_DT_DIR = 3,             //Directory
	NWY_DT_BLK = 4,             //Block device
	NWY_DT_REG = 5,             //Regular file
	NWY_DT_LNK = 6,            //Symbolic link
	NWY_DT_SOCK = 7,           //Socket
	NWY_DT_WHT = 8             //Link
}nwy_dev_type_e;



/*
 *****************************************************************************
 * 4 Global Variable Declaring
 *****************************************************************************
 */




/*
 *****************************************************************************
 * 5 STRUCT Type Definition
 *****************************************************************************
 */
typedef struct{
    int     st_dev;
    int     st_ino;
    int    st_mode;
    int   st_nlink;
    int     st_uid;
    int     st_gid;
    int     st_rdev;
    long     st_size;
    long    st_atime1;
    long      st_spare1;
    long    st_mtime1;
    long      st_spare2;
    long    st_ctime1;
    long      st_spare3;
    int st_blksize;
    int  st_blocks;
    long  st_spare4[2];
} nwy_file_stat;

typedef struct
{
	long tell;
	int fd;
	int buf_pos;
	int buf_end;
	volatile int lock[1];
	/* Any changes to this struct must preserve the property:
	 * offsetof(struct __dirent, buf) % sizeof(off_t) == 0 */
	char buf[2048];
    int16_t reserved; ///< reserved
} nwy_dir_info_t;

typedef struct
{
	unsigned int d_ino;
	long d_off;
	unsigned short d_reclen;
	unsigned char d_type;  //nwy_dir_type_e
	char d_name[256];
}nwy_dirent_t;

typedef void nwy_block_device_t;

/*
 *****************************************************************************
 * 6 UNION Type Definition
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 7 OTHERS Definition
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */

/*
*****************************************************************************
* Prototype     : nwy_file_open
* Description   : Open file
* Input         : path: Absolute path name of the file
                  mode: File open mode, see nwy_file_open_mode_e
* Output        : NA
* Return Value  : Return file handle
* Author        : hujun
*****************************************************************************
*/
int nwy_file_open(const char *path,  nwy_file_open_mode_e mode);
/*
*****************************************************************************
* Prototype     : nwy_file_close
* Description   : Close file
* Input         : fd: File handle
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_file_close(int fd);
/*
*****************************************************************************
* Prototype     : nwy_file_read
* Description   : Read file content
* Input         : fd: File handle
                  size: Maximum length of buffer for file content
* Output        : dst: Stored file content
* Return Value  : Actual length read
* Author        : hujun
*****************************************************************************
*/
int nwy_file_read(int fd, void *dst, int size);

/*
*****************************************************************************
* Prototype     : nwy_file_write
* Description   : Write content to file
* Input         : fd: File handle
                  data: Write content buffer
                  size: Length of content to write
* Output        : NA
* Return Value  : Actual length written
* Author        : hujun
*****************************************************************************
*/
int nwy_file_write(int fd, const void *data, int size);

/*
*****************************************************************************
* Prototype     : nwy_file_seek
* Description   : File tell
* Input         : fd: File handle
* Output        : NA
* Return Value  : Return current file pointer position
* Author        : hujun
*****************************************************************************
*/
long nwy_file_tell(int fd);

/*
*****************************************************************************
* Description   : File seek
* Input         : fd: File handle
                  offset: Offset size
                  mode: Offset mode
* Output        : NA
* Return Value  : Return file handle
* Author        : hujun
*****************************************************************************
*/
long nwy_file_seek(int fd, int offset, nwy_fseek_offset_e mode);

/*
*****************************************************************************
* Prototype     : nwy_file_path_size
* Description   : Get file size by filename
* Input         : path: Absolute path name of the file
* Output        : NA
* Return Value  : File size
* Author        : hujun
*****************************************************************************
*/
long nwy_file_path_size(const char *path);

/*
*****************************************************************************
* Prototype     : nwy_file_fd_size
* Description   : Get file size
* Input         : fd: File handle
* Output        : NA
* Return Value  : File size
* Author        : hujun
*****************************************************************************
*/
long nwy_file_fd_size(int fd);

/*
*****************************************************************************
* Prototype     : nwy_file_exist
* Description   : Check if file exists
* Input         : path: Absolute path name of the file
* Output        : NA
* Return Value  : true if exists, false if not
* Author        : hujun
*****************************************************************************
*/
bool nwy_file_exist(const char *path);

/*
*****************************************************************************
* Prototype     : nwy_file_remove
* Description   : Delete file
* Input         : path: Absolute path name of the file
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_file_remove(const char* path);

/*
*****************************************************************************
* Prototype     : nwy_file_fd_size
* Description   : Rename file
* Input         : oldpath: Original filename
                  newpath: New filename
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_file_rename(const char *oldpath, const char *newpath);
/*
*****************************************************************************
* Prototype     : nwy_file_sync
* Description   : File sync
* Input         : fd: File handle
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_file_sync(int fd);

/*
*****************************************************************************
* Prototype     : nwy_file_fd_trunc
* Description   : Truncate file by file handle
* Input         : fd: File handle
* Output        : len: Truncate length
* Return Value  : Return truncate length
* Author        : hujun
*****************************************************************************
*/
int nwy_file_fd_trunc(int fd, long len);

/*
*****************************************************************************
* Prototype     : nwy_file_path_trunc
* Description   : Truncate file by filename
* Input         : path: Filename
* Output        : NA
* Return Value  : Return truncate length
* Author        : hujun
*****************************************************************************
*/
int nwy_file_path_trunc(const char *path, long len);

/*
*****************************************************************************
* Prototype     : nwy_file_path_stat_get
* Description   : Get file stat information by filename
* Input         : path: Filename
* Output        : st: File information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_file_path_stat_get(const char *path, nwy_file_stat *st);

/*
*****************************************************************************
* Prototype     : nwy_file_path_stat_get
* Description   : Get file stat information by file handle
* Input         : path: Filename
* Output        : st: File information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_file_fd_stat_get(int fd, nwy_file_stat *st);

/*
*****************************************************************************
* Prototype     : nwy_dir_mk
* Description   : Create directory
* Input         : path: Directory name
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_mk(const char *path);

/*
*****************************************************************************
* Prototype     : nwy_dir_rm
* Description   : Delete directory
* Input         : path: Directory name
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_rm(const char *path);
/*
*****************************************************************************
* Prototype     : nwy_dir_recursive_rm
* Description   : Delete directory, force delete
* Input         : path: Directory name
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_recursive_rm(const char *path);

/*
*****************************************************************************
* Prototype     : nwy_dir_open
* Description   : Open directory
* Input         : path: Directory name
* Output        : info: Directory handle
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_open(const char *name, nwy_dir_info_t *info);

/*
*****************************************************************************
* Prototype     : nwy_dir_close
* Description   : Close directory
* Input         : pdir: Directory pointer
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_close(nwy_dir_info_t *pdir);

/*
*****************************************************************************
* Prototype     : nwy_dir_read
* Description   : Read directory
* Input         : pdir: Directory pointer
* Output        : NA
* Return Value  : Directory pointer read
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_read(nwy_dir_info_t *pdir, nwy_dirent_t *d_info);
/*
*****************************************************************************
* Prototype     : nwy_dir_tell
* Description   : Current file offset relative to directory header
* Input         : pdir: Directory pointer
* Output        : NA
* Return Value  : Offset size
* Author        : hujun
*****************************************************************************
*/
long nwy_dir_tell(nwy_dir_info_t *pdir);

/*
*****************************************************************************
* Prototype     : nwy_dir_seek
* Description   : Set directory stream reading position
* Input         : path: Directory name
                  loc: Offset from the beginning of the directory file
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_seek(nwy_dir_info_t *pdir, long loc);

/*
*****************************************************************************
* Prototype     : nwy_dir_rewind
* Description   : Reset directory reading position to the beginning
* Input         : path: Directory name
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dir_rewind(nwy_dir_info_t *pdir);

/*
*****************************************************************************
* Prototype     : nwy_vfs_free_size_get
* Description   : Get remaining size under / directory
* Input         : NA
* Output        : path: Partition path
                  avail_space: Obtained size
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_vfs_free_size_get(const char* path,unsigned long long *avail_space);
/*
*****************************************************************************
* Prototype     : nwy_vfs_total_size_get
* Description   : get fs total size
* Input         : path :path
* Output        : total_space : total size
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_vfs_total_size_get(const char* path,unsigned long long *total_space);


/*
*****************************************************************************
* Prototype     : nwy_vfs_logical_block_device_create
* Description   : create logical block device
* Input         : name :physical dev name
* Return Value  : NULL for fail
* Author        : zhaoyong
*****************************************************************************
*/
nwy_block_device_t *nwy_vfs_logical_block_device_create(char *name, uint32_t offset, uint32_t size);

/*
*****************************************************************************
* Prototype     : nwy_vfs_mount
* Description   : mount block device to fs
* Input         : base_path : fs mount point
* Input         : nwy_bdev  : block device
* Return Value  : nwy_error_e
* Author        : zhaoyong
*****************************************************************************
*/
int nwy_vfs_mount(const char *base_path, nwy_block_device_t *nwy_bdev);

/*
*****************************************************************************
* Prototype     : nwy_vfs_unmount
* Description   : unmount fs partition
* Input         : base_path : fs mount point
* Return Value  : nwy_error_e
* Author        : zhaoyong
*****************************************************************************
*/
int nwy_vfs_unmount(const char *base_path);

/*
*****************************************************************************
* Prototype     : nwy_vfs_mkfs
* Description   : format block device to fs
* Input         : nwy_bdev  : block device
* Return Value  : nwy_error_e
* Author        : zhaoyong
*****************************************************************************
*/
int nwy_vfs_mkfs(nwy_block_device_t *nwy_bdev);

#ifdef __cplusplus
   }
#endif

#endif
