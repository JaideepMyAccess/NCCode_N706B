/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_osi_api.h
 * Author       : hujun
 * Created      : 2017-3-29
 * Description  : OSI API function declarations
 *
 *****************************************************************************
 */

#ifndef _NWY_OSI_API_H_
#define _NWY_OSI_API_H_

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
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */
#define NWY_TIME_ZONE_SECOND (15 * 60)
#define NWY_THRAD_EVENT_LIST_MAX              (20)
#define NWY_PRINTF_BUF_MAX_SIZE       256
#define NWY_QUEUE_MSG_MAX             2048
#define NWY_QUEUE_INDEX_MAX             50
/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */

typedef enum
{
    NWY_OSI_SHUTDOWN_RESET = 0,               ///< normal reset
    NWY_OSI_SHUTDOWN_FORCE_DOWNLOAD = 0x5244, ///< 'RD' reset to force download mode
    NWY_OSI_SHUTDOWN_DOWNLOAD = 0x444e,       ///< 'DN' reset to download mode
    NWY_OSI_SHUTDOWN_BL_DOWNLOAD = 0x4244,    ///< 'BD' reset to bootloader download mode
    NWY_OSI_SHUTDOWN_CALIB_MODE = 0x434c,     ///< 'CL' reset to calibration mode
    NWY_OSI_SHUTDOWN_NB_CALIB_MODE = 0x4e43,  ///< 'NC' reset to NB calibration mode
    NWY_OSI_SHUTDOWN_BBAT_MODE = 0x4241,      ///< 'BA' reset to BBAT mode
    NWY_OSI_SHUTDOWN_UPGRADE = 0x4654,        ///< 'FT' reset to upgrade mode
    NWY_OSI_SHUTDOWN_POWER_OFF = 0x4f46,      ///< 'OF' power off
    NWY_OSI_SHUTDOWN_PSM_SLEEP = 0x5053,      ///< 'PS' power saving mode
} nwy_shutdown_mode_e;
typedef enum nwy_thread_priority_e
{
    NWY_OSI_PRIORITY_IDLE = 1, // reserved
    NWY_OSI_PRIORITY_LOW = 8,
    NWY_OSI_PRIORITY_BELOW_NORMAL = 16,
    NWY_OSI_PRIORITY_NORMAL = 24,
    NWY_OSI_PRIORITY_ABOVE_NORMAL = 32,
    NWY_OSI_PRIORITY_HIGH = 40,
    NWY_OSI_PRIORITY_REALTIME = 48,
    NWY_OSI_PRIORITY_HISR = 56, // reserved
} nwy_thread_priority_e;

typedef enum
{
    NWY_OSA_NO_SUSPEND = 0,
    NWY_OSA_SUSPEND = -1,
}nwy_timeout_type_e;

typedef enum
{
    NWY_TIMER_ONE_TIME = 0,
    NWY_TIMER_PERIODIC = 1,
}nwy_timer_type_e;

typedef enum
{
    NWY_TIMER_UNKNOWN_STATUS = -1,
    NWY_TIMER_INACTIVE_STATUS = 0,
    NWY_TIMER_ACTIVE_STATUS = 1,
}nwy_timer_state_e;




/* Task states returned by eTaskGetState. */
typedef enum
{
    NWY_RUNNING = 0,    /* A task is querying the state of itself, so must be running. */
    NWY_READY,          /* The task being queried is in a read or pending ready list. */
    NWY_BLOCKED,        /* The task being queried is in the Blocked state. */
    NWY_SUSPENDED,      /* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
    NWY_DELETED,        /* The task being queried has been deleted, but its TCB has not yet been freed. */
    NWY_COMPLETED,
    NWY_SLEEP,
    NWY_INVALID         /* Used as an 'invalid state' value. */
} nwy_thread_state_e;



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

typedef void * nwy_osi_timer_t;
typedef void * nwy_osi_thread_t;
typedef void * nwy_osi_msg_queue_t;
typedef void * nwy_osi_mutex_t;
typedef void * nwy_osi_semaphore_t;
typedef void * nwy_timer_cb_para_t;
typedef void (*nwy_osi_cb_func)(void *ctx);
typedef void (*nwy_event_cb_func_t)(unsigned event,void *param);

typedef struct nwy_pipe_t
{
    volatile bool running;
    volatile bool eof;
    unsigned size;
    unsigned rd;
    unsigned wr;
    nwy_osi_semaphore_t rd_avail_sema;
    nwy_osi_semaphore_t wr_avail_sema;
    unsigned rd_cb_mask;
    nwy_event_cb_func_t rd_cb;
    void *rd_cb_ctx;
    unsigned wr_cb_mask;
    nwy_event_cb_func_t wr_cb;
    void *wr_cb_ctx;
    char data[];
}nwy_pipe_t;


typedef struct nwy_thread_info_t
{
    nwy_osi_thread_t thread_handle;         /* The handle of the task to which the rest of the information in the structure relates. */
    const char *thread_name;            /* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    unsigned long thread_number;        /* A number unique to the task. */
    nwy_thread_state_e thread_current_state;      /* The state in which the task existed when the structure was populated. */
    unsigned long thread_current_priority;  /* The priority at which the task was running (may be inherited) when the structure was populated. */
    unsigned long thread_base_priority;     /* The priority to which the task will return if the task's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES is defined as 1 in FreeRTOSConfig.h. */
    uint32_t thread_run_time_counter;       /* The total run time allocated to the task so far, as defined by the run time stats clock.  See http://www.freertos.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS is defined as 1 in FreeRTOSConfig.h. */
    uint32_t *thread_stack_base;        /* Points to the lowest address of the task's stack area. */
    uint16_t thread_stack_highwatermark;    /* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack.convert word count to byte count should multiply 4 */
} nwy_thread_info_t;

#ifndef NWY_TIME_STRUCT
#define NWY_TIME_STRUCT
typedef struct nwy_time_t
{
    unsigned short year; //  Year-2000, tm_year: Year-1900
    unsigned char mon;  // [1,12], tm_mon: (0-11)
    unsigned char day;  // [1,31], tm_mday: (1-31)
    unsigned char hour; // [0,23], tm_hour: (0-23)
    unsigned char min;  // [0,59], tm_min: (0-59)
    unsigned char sec;  // [0,59], tm_sec: (0-60)
    unsigned char wday; // [1,7] 1 for Monday, tm_wday: (0-6) 0 for Sunday
#ifdef FEATURE_NWY_AT_QLTS
        unsigned char is_dst;
#endif
} nwy_time_t;
#endif

typedef struct nwy_timeval_t {
  long    tv_sec;         /* seconds */
  long    tv_usec;        /* and microseconds */
}nwy_timeval_t;

typedef struct nwy_event_msg_t
{
    uint32 id;     ///< event identifier
    uint32 param1; ///< 1st parameter
    uint32 param2; ///< 2nd parameter
    uint32 param3; ///< 3rd parameter
    void* param4;  ///< 4rd parameter
} nwy_event_msg_t;

typedef void (*nwy_timer_cb_func)(void * ctx);

/* Special values for timer callback execution mode */
#define NWY_TIMER_IN_ISR ((nwy_osi_thread_t *)NULL)

/**
 * Special value to indicate timer callback will be invoked in
 * timer service thread.
 */
#define NWY_TIMER_IN_SERVICE ((nwy_osi_thread_t *)0xffffffff)

typedef struct nwy_timer_para_t
{
    int expired_time;
    nwy_timer_type_e type;
    nwy_osi_thread_t thread_hdl;    /*NWY_TIMER_IN_ISR:callback in ISR context;NWY_TIMER_IN_SERVICE:callback in timer manager thread;Other:callback in thread_hdl*/
    nwy_timer_cb_func cb;
    nwy_timer_cb_para_t cb_para;
}nwy_timer_para_t;

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


//thread API

/*
*****************************************************************************
* Prototype     : nwy_create_thread
* Description   : Create thread
* Input         : task_name: task name
                 priority: priority level
                 cb: thread execution function
                 argv: thread execution function parameters
                 event_count: maximum event count supported by thread
                 stack_size: thread stack size
                 stack: thread stack address, NULL for system default allocation
* Output        : hdl: created thread handle
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_create(nwy_osi_thread_t *hdl, char *task_name, uint8 priority,
                        nwy_osi_cb_func cb, void *argv, uint32 event_count, uint32 stack_size, void *stack);

/*
*****************************************************************************
* Prototype     : nwy_thread_get_current
* Description   : Get current context thread handle
* Input         : NA
* Output        : hdl: current context thread handle
* Return Value  : Current context thread handle
* Author        : hujun
*****************************************************************************
*/
nwy_osi_thread_t nwy_thread_get_current(void);

/*
*****************************************************************************
* Prototype     : nwy_thread_set_priority
* Description   : Set thread priority
* Input         : hdl: task handle
                  new_pri: new priority level
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_set_priority(nwy_osi_thread_t hdl, uint8 new_pri);

/*
*****************************************************************************
* Prototype     : nwy_thread_get_priority
* Description   : Get thread priority
* Input         : hdl: task handle
* Output        : priority: priority level of task handle
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_get_priority(nwy_osi_thread_t hdl, uint8 *priority);

/*
*****************************************************************************
* Prototype     : nwy_thread_suspend
* Description   : Suspend thread
* Input         : hdl: task handle
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_suspend(nwy_osi_thread_t hdl);

/*
*****************************************************************************
* Prototype     : nwy_thread_resume
* Description   : Resume thread
* Input         : hdl: task handle
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_resume(nwy_osi_thread_t hdl);

/*
*****************************************************************************
* Prototype     : nwy_thread_exit
* Description   : Exit thread
* Input         : hdl: task handle, if hdl is NULL, exit current task
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_exit(nwy_osi_thread_t hdl);

/*
*****************************************************************************
* Prototype     : nwy_thread_sleep
* Description   : Thread sleep
* Input         : ms: sleep time in milliseconds
* Output        : NA
* Return Value  : void
* Author        : hujun
*****************************************************************************
*/
void nwy_thread_sleep(uint32 ms);

/*
*****************************************************************************
* Prototype     : nwy_thread_usleep
* Description   : Thread sleep
* Input         : ms: sleep time in microseconds
* Output        : NA
* Return Value  : void
* Author        : hujun
*****************************************************************************
*/
void nwy_thread_usleep(uint32 us);

/*
*****************************************************************************
* Prototype     : nwy_thread_event_send
* Description   : Send event to specified thread
* Input         : hdl: destination thread
                  event: event information
                  timeout: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND), in milliseconds
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_event_send(nwy_osi_thread_t hdl, nwy_event_msg_t *event, int timeout);

/*
*****************************************************************************
* Prototype     : nwy_thread_event_send
* Description   : Send event to the front of specified thread
* Input         : hdl: destination thread
                  event: event information
                  timeout: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND), in milliseconds
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_event_front_send(nwy_osi_thread_t hdl, nwy_event_msg_t *event, int timeout);

/*
*****************************************************************************
* Prototype     : nwy_thread_event_wait
* Description   : Thread receive event
* Input         : hdl: receiving thread
                  event: event information
                  timeout: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND), in milliseconds
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_event_wait(nwy_osi_thread_t hdl, nwy_event_msg_t *event, int timeout);

/*
*****************************************************************************
* Prototype     : nwy_thread_pendingevt_num_get
* Description   : Get number of pending events for thread
* Input         : hdl: thread
* Output        : pnum: number of events currently existing for thread
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_pendingevt_num_get(nwy_osi_thread_t hdl, uint32 *pnum);

/*
*****************************************************************************
* Prototype     : nwy_thread_spaceevt_cnt_get
* Description   : Get number of free events that can be sent to thread
* Input         : hdl: thread
* Output        : pnum: number of free events for thread
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_spaceevt_cnt_get(nwy_osi_thread_t hdl, uint32 *pnum);

/*
*****************************************************************************
* Prototype     : nwy_thread_info_get
* Description   : Get thread information
* Input         : hdl: thread
* Output        : thread_info: thread information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_info_get(nwy_osi_thread_t hdl, nwy_thread_info_t *thread_info);

/*
*****************************************************************************
* Prototype     : nwy_thread_list_get
* Description   : Get thread list
* Input         : NA
* Output        : thread_list: thread list
* Return Value  : void
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_list_get(char *thread_list);

/*
*****************************************************************************
* Prototype     : nwy_thread_runtime_stats_get
* Description   : Get current system task statistics
* Input         : NA
* Output        : stats_buf: statistics information print
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_thread_runtime_stats_get(char *stats_buf);

/*
*****************************************************************************
* Prototype     : nwy_msg_queue_create
* Description   : Create message queue
* Input         : msg_size: message queue message size
                  msg_num: maximum number of messages
* Output        : msg_q: message queue
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_msg_queue_create (nwy_osi_msg_queue_t *msg_q, uint32 msg_size, uint32 msg_num);

/*
*****************************************************************************
* Prototype     : nwy_msg_queue_delete
* Description   : Delete message queue
* Input         : msg_q: message queue to delete
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_msg_queue_delete (nwy_osi_msg_queue_t msg_q);

/*
*****************************************************************************
* Prototype     : nwy_msg_queue_send
* Description   : Send message to message queue
* Input         : msg_q: message queue
                  size: size of message to send
                  msg_ptr: message to send
                  timeout: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND), in milliseconds
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_msg_queue_send(nwy_osi_msg_queue_t msg_q, uint32 size, void* msg_ptr, int timeout);

/*
*****************************************************************************
* Prototype     : nwy_msg_queue_send
* Description   : Send message to the front of message queue
* Input         : msg_q: message queue
                  size: size of message to send
                  msg_ptr: message to send
                  timeout: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND), in milliseconds
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_msg_queue_front_send(nwy_osi_msg_queue_t msg_q,uint32 size, void *msg_ptr, int timeout);

/*
*****************************************************************************
* Prototype     : nwy_msg_queue_recv
* Description   : Receive message from message queue
* Input         : msg_q: message queue
                  size: size of message to receive
                  timeout: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND) for no wait, in milliseconds
* Output        : msg_ptr: store received message
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_msg_queue_recv(nwy_osi_msg_queue_t msg_q, uint32 size, void* msg_ptr, int  timeout);

/*
*****************************************************************************
* Prototype     : nwy_msg_queue_pending_num_get
* Description   : Get number of pending messages in message queue
* Input         : msg_q: message queue
* Output        : pnum: number of pending messages in message queue
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_msg_queue_pending_num_get(nwy_osi_msg_queue_t msg_q, uint32 *pnum);

/*
*****************************************************************************
* Prototype     : nwy_msg_queue_get_space_num
* Description   : Get number of free messages in message queue
* Input         : msg_q: message queue
* Output        : pnum: number of free messages in message queue
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_msg_queue_space_num_get(nwy_osi_msg_queue_t msg_q, uint32 *pnum);

/*
*****************************************************************************
* Prototype     : nwy_sdk_mutex_create
* Description   : Create mutex
* Input         :
* Output        : mutex: mutex
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_mutex_create(nwy_osi_mutex_t *mutex);

/*
*****************************************************************************
* Prototype     : nwy_sdk_mutex_lock
* Description   : Acquire mutex
* Input         : mutex: mutex
                  time_out: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND) for no wait, in milliseconds
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_mutex_lock(nwy_osi_mutex_t mutex, int time_out);

/*
*****************************************************************************
* Prototype     : nwy_sdk_mutex_unlock
* Description   : Release mutex
* Input         : mutex: mutex
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_mutex_unlock(nwy_osi_mutex_t mutex);

/*
*****************************************************************************
* Prototype     : nwy_sdk_mutex_delete
* Description   : Delete mutex
* Input         : mutex: mutex
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_mutex_delete(nwy_osi_mutex_t mutex);

/*
*****************************************************************************
* Prototype     : nwy_semaphore_create
* Description   : Create semaphore
* Input         : sem_count: maximum number of semaphores to create
* Output        : sem: created semaphore
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_semaphore_create(nwy_osi_semaphore_t *sem,uint32 sem_count);

/*
*****************************************************************************
* Prototype     : nwy_semaphore_acquire
* Description   : Acquire semaphore
* Input         : sem: semaphore
                  time_out: timeout period, 0xffffffff(NWY_OSA_SUSPEND) for wait indefinitely, 0(NWY_OSA_NO_SUSPEND) for no wait, in milliseconds
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_semaphore_acquire(nwy_osi_semaphore_t sem, int time_out);

/*
*****************************************************************************
* Prototype     : nwy_semahpore_release
* Description   : Release semaphore
* Input         : sem: semaphore
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_semahpore_release(nwy_osi_semaphore_t sem);

/*
*****************************************************************************
* Prototype     : nwy_semahpore_release
* Description   : Delete semaphore
* Input         : sem: semaphore
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_semahpore_delete(nwy_osi_semaphore_t sem);

/*
*****************************************************************************
* Prototype     : nwy_semahpore_count_get
* Description   : Get semaphore count
* Input         : sem: semaphore
* Output        : count: obtained count
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_semahpore_count_get(nwy_osi_semaphore_t sem, uint32 *count);

/*
*****************************************************************************
* Prototype     : nwy_sdk_timer_create
* Description   : Create timer
* Input         : para: timer parameters
* Output        : timer: created timer
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_timer_create(nwy_osi_timer_t *timer, nwy_timer_para_t *para);

/*
*****************************************************************************
* Prototype     : nwy_sdk_timer_destory
* Description   : Destroy timer
* Input         : timer: timer handle
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_timer_destory(nwy_osi_timer_t timer);

/*
*****************************************************************************
* Prototype     : nwy_sdk_timer_start
* Description   : Start timer
* Input         : timer: timer handle
                  para: timer parameters
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_timer_start(nwy_osi_timer_t timer, nwy_timer_para_t *para);

/*
*****************************************************************************
* Prototype     : nwy_sdk_timer_status_get
* Description   : Get timer status
* Input         : timer: timer handle
* Output        : status: timer status
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e  nwy_sdk_timer_status_get(nwy_osi_timer_t timer, nwy_timer_state_e *status);

/*
*****************************************************************************
* Prototype     : nwy_sdk_timer_stop
* Description   : Stop timer
* Input         : timer: timer handle
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sdk_timer_stop(nwy_osi_timer_t timer);

/*
*****************************************************************************
* Prototype     : nwy_uptime_get
* Description   : System uptime
* Input         : NA
* Output        : NA
* Return Value  : Uptime in milliseconds
* Author        : hujun
*****************************************************************************
*/
int64 nwy_uptime_get(void);

/*
*****************************************************************************
* Prototype     : nwy_date_set
* Description   : Set date
* Input         : p_time: date information
                  timezone: time zone, The unit of time zone is fifteen minutes, which is equivalent to a quarter hour of modern time, or 15 minutes
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_date_set(nwy_time_t *p_time, int  timezone);

/*
*****************************************************************************
* Prototype     : nwy_date_get
* Description   : Get date
* Input         :
* Output        : p_time: date information
                  timezone: time zone, The unit of time zone is fifteen minutes, which is equivalent to a quarter hour of modern time, or 15 minutes
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_date_get(nwy_time_t *p_time, int* timezone);

/*
*****************************************************************************
* Prototype     : nwy_sys_timestamp_set
* Description   : Set system time
* Input         :*time:Timestamp, The time is seconds from 1970-01-01 UTC.
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sys_timestamp_set(nwy_timeval_t *time);
/*
*****************************************************************************
* Prototype     : nwy_sys_timestamp_get
* Description   : get system time
* Input         :*time:Timestamp, Used to save parameter information
* Output        : *time:Timestamp, The time is seconds from 1970-01-01 UTC.
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sys_timestamp_get(nwy_timeval_t *time);

/*
*****************************************************************************
* Prototype     : nwy_sys_timezone_set
* Description   : set system timezone
* Input         :tz:timezone,The unit of time zone is fifteen minutes, which is equivalent to a quarter hour of modern time, or 15 minutes
* Output        :
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_sys_timezone_set(int tz);

/*
*****************************************************************************
* Prototype     : nwy_timestamp_to_date
* Description   : Convert timestamps into dates and times.
* Input         :timestamp:Pointer to the nwy_timeval_t structure, containing the timestamp to be converted.
* Output        :tm       :Pointer to the nwy_time_t structure, used to store the converted date and time.
* Return Value  : nwy_error_e
* Author        :
*****************************************************************************
 */
nwy_error_e nwy_timestamp_to_date(nwy_timeval_t *timestamp, nwy_time_t *tm);

/*
*****************************************************************************
* Prototype     : nwy_timestamp_to_date
* Description   : Convert date and time to timestamp.
* Input         :tm        :Pointer to the nwy_time_t struct, containing the date and time to be converted.
* Output        :timestamp :Pointer to the nwy_timeval_t structure, used to store the converted timestamp.
* Return Value  : nwy_error_e
* Author        :
*****************************************************************************
 */
nwy_error_e nwy_date_to_timestamp(nwy_time_t *tm, nwy_timeval_t *timestamp);



#ifdef __cplusplus
   }
#endif


#endif
