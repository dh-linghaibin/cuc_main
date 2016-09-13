
#ifndef __MENU_H
#define __MENU_H

#include "Type.h"


typedef struct DataNode
{
    u8 mode;//当前模式 0：自动模式 1：手动模式 2：差分模式  20
    u8 push_book;  //21
    u16 ste_arrive;//推书位置 22 23
    u8 dibide_num;
    u16 divide_setp;
} tDataNode;
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单参数初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
void MenuInit(void);
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuModeSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 更新相应模式进入相应功能  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
void MenuModeSet(u8 cmd);
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuServiceTime(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单时间服务函数，主要用于后拨的自动换档  
* 作    者: by lhb_steven
* 日    期: 2016/3/18
************************************************************************************************************/ 
void MenuServiceTime(void);
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuGetMode(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 模式状态  
* 作    者: by lhb_steven
* 日    期: 2016/3/26
************************************************************************************************************/ 
u8 MenuGetMode(void);
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuSetBeind(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 设置前拨档位  
* 作    者: by lhb_steven
* 日    期: 2016/6/2
************************************************************************************************************/ 
void MenuSetBeind(u8 cmd);
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuGetPushBook(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
u8 MenuGetPushBook(void);
/**********************************************函数定义***************************************************** 
* 函数名称: u16 MenuGetArrive(void) 
* 输入参数: void 
* 返回参数: u16  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
u16 MenuGetArrive(void);
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuAsk(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/19
************************************************************************************************************/ 
void MenuAsk(void);

#endif


