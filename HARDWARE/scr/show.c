#include "show.h"
char show_num;//屏幕号
char show_num_sta = 0; //屏幕号状态
char next_data;//显示数据的第二屏幕
unsigned char choose_show;//选择显示的屏幕
unsigned char choose_admin;//选择修改的数据的屏幕
/*********************************************************************************************************
* 函 数 名 : void	Cut_screen()
* 功能说明 : 屏幕切屏
* 形    参 : 无
* 返 回 值 : 无
* 备    注 : 2022-11-28
*********************************************************************************************************/
void	Cut_screen(void)
{
    OLED_Refresh();//更新显存--当显示的数据发生改变时需更新
    if(show_num != show_num_sta) ////当值发生改变--用于检测值是否发生改变
    {
        show_num_sta = show_num; //保存当前值
        OLED_Clear();//清屏
    }
    if(auto_flag)
        OLED_show_str(120, 48, "A");
    else  OLED_show_str(120, 48, "W");
    switch (show_num)
    {
    case 0:
        main_sreen();//主屏幕
        set_show();//选择屏幕
        break;
    case 1:
        data_show();//显示设备状态屏幕
        break;
    case 2:
        show_admin();//显示限值修改屏幕
        break;
    case 3:
        show_Learn_instruction();//显示学习指令屏幕
        break;
    case 4:
        show_beep();//显示报警屏幕
        break;
    }
}
void  main_sreen(void)//主屏幕
{
    OLED_show_str(20, 0, "智能家居");
    OLED_show_str(16, 16, "数据显示");
    OLED_show_str(16, 32, "限值修改");
    OLED_show_str(16, 48, "学习指令");
}
u8 next_data_sta;
void data_show(void)
{
    if(next_data != next_data_sta) ////当值发生改变--用于检测值是否发生改变
    {
        next_data_sta = next_data; //保存当前值
        OLED_Clear();//清屏
    }
    if(next_data == 0)
    {
        OLED_show_str(0, 0, "温度");
        OLED_ShowNum(16 * 2 + 8, 0, tem, 2, 16);

        OLED_show_str(0, 16, "湿度");
        OLED_ShowNum(16 * 2 + 8, 16, hum, 2, 16);

        OLED_show_str(0, 32, "光照");
        OLED_ShowNum(16 * 2 + 8, 32, light, 2, 16);
        OLED_show_str(0, 48, "C  O");
        OLED_ShowNum(16 * 2 + 8, 48, co, 2, 16);
    }
    else
    {
        OLED_show_str(0, 0, "烟雾");
        OLED_ShowNum(16 * 2 + 8, 0, ppm, 2, 16);
        OLED_show_str(0, 16, "人数");
        OLED_ShowNum(16 * 2 + 8, 16, people, 2, 16);
        OLED_show_str(0, 32, "窗帘");
        if(MotorRun_flag) OLED_show_str(16 + 16 + 16, 32, "on");
        else 							OLED_show_str(16 + 16 + 16, 32, "of");
        OLED_show_str(0, 48, "1:");
        if(IO_A7_IN_STA == 0)				OLED_show_str(16, 48, "on");
        else												OLED_show_str(16, 48, "of");
        OLED_show_str(16 + 16, 48, "2:");
        if(IO_A8_IN_STA == 0)				OLED_show_str(16 + 16 + 16, 48, "on");
        else												OLED_show_str(16 + 16 + 16, 48, "of");
        OLED_show_str(16 + 16 + 16 + 16, 48, "3:");
        if(IO_A12_IN_STA == 0)				OLED_show_str(16 + 16 + 16 + 16 + 16, 48, "on");
        else													OLED_show_str(16 + 16 + 16 + 16 + 16, 48, "of");
        OLED_show_str(16 + 16 + 16 + 16 + 16 + 16, 48, "4:");
        if(IO_A11_IN_STA == 0)				OLED_show_str(16 + 16 + 16 + 16 + 16 + 16 + 16, 48, "on");
        else													OLED_show_str(16 + 16 + 16 + 16 + 16 + 16 + 16, 48, "of");
    }
}
void show_beep(void)
{
    if(ppm_beep)
    {
        OLED_show_str(0, 0, "烟雾");
        OLED_ShowNum(16 * 2 + 8, 0, ppm, 2, 16);
        OLED_show_str(16, 32, "ppm_h!!!");
        OLED_ShowNum(16 * 4 + 16, 48, PPM_MAX, 2, 16);
    }
    else if(co_beep)
    {
        OLED_show_str(0, 0, "C O");
        OLED_ShowNum(16 * 2 + 8, 0, co, 2, 16);
        OLED_show_str(16, 32, "co_h!!!");
        OLED_ShowNum(16 * 4 + 16, 48, co_MAX, 2, 16);
    }
}
void show_admin(void)//限值显示函数
{
    OLED_show_str(16, 32, "烟雾上限");
    OLED_ShowNum(16 * 4 + 16, 32, PPM_MAX, 2, 16);
    OLED_show_str(16, 16, "C O 上限");
    OLED_ShowNum(16 * 4 + 16, 16, co_MAX, 2, 16);
}

void show_Learn_instruction(void)//学习指令显示函数
{
    if(next_data != next_data_sta) ////当值发生改变--用于检测值是否发生改变
    {
        next_data_sta = next_data; //保存当前值
        OLED_Clear();//清屏
    }
    if(next_data == 0)
    {
        OLED_show_str(0, 0, "开空调1");
        if(Learn_on_ok1) OLED_show_str(16 * 4, 0, "已学习");
        else OLED_show_str(16 * 4, 0, "未学习");
        OLED_show_str(0, 16, "关空调1");
        if(Learn_off_ok1) OLED_show_str(16 * 4, 16, "已学习");
        else OLED_show_str(16 * 4, 16, "未学习");

        OLED_show_str(0, 32, "开电视");
        if(Learn_on_ok2) OLED_show_str(16 * 4, 32, "已学习");
        else OLED_show_str(16 * 4, 32, "未学习");
        OLED_show_str(0, 48, "关电视");
        if(Learn_off_ok2) OLED_show_str(16 * 4, 48, "已学习");
        else OLED_show_str(16 * 4, 48, "未学习");
    }
    else
    {
        OLED_show_str(0, 0, "开空调2");
        if(Learn_on_ok3) OLED_show_str(16 * 4, 0, "已学习");
        else OLED_show_str(16 * 4, 0, "未学习");
        OLED_show_str(0, 16, "关空调2");
        if(Learn_off_ok3) OLED_show_str(16 * 4, 16, "已学习");
        else OLED_show_str(16 * 4, 16, "未学习");

        OLED_show_str(0, 32, "开电视2");
        if(Learn_on_ok4) OLED_show_str(16 * 4, 32, "已学习");
        else OLED_show_str(16 * 4, 32, "未学习");
        OLED_show_str(0, 48, "关电视2");
        if(Learn_off_ok4) OLED_show_str(16 * 4, 48, "已学习");
        else OLED_show_str(16 * 4, 48, "未学习");
    }

}
void set_show(void) //选择屏幕
{
    switch (choose_show)
    {
    case 0:
        OLED_ShowString (0, 16, "->", 16);
        OLED_ShowString (0, 32, "  ", 16);
        OLED_ShowString (0, 48, "  ", 16);
        break;
    case 1:
        OLED_ShowString (0, 16, "  ", 16);
        OLED_ShowString (0, 32, "->", 16);
        OLED_ShowString (0, 48, "  ", 16);
        break;
    case 2:
        if(show_num == 0)
        {
            OLED_ShowString (0, 16, "  ", 16);
            OLED_ShowString (0, 32, "  ", 16);
            OLED_ShowString (0, 48, "->", 16);
        }
        if(show_num == 2)
        {
            OLED_ShowString (0, 16, "  ", 16);
            OLED_ShowString (0, 32, "  ", 16);
            OLED_ShowString (0, 48, "  ", 16);
        }
        break;
    }
}





