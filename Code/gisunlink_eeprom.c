/*
* _COPYRIGHT_
*
* File Name: gisunlink_eeprom.c
* System Environment: JOHAN-PC
* Created Time:2018-12-25
* Author: johan
* QQ: 4493953
* E-mail: johaness@qq.com
* Description: 
*
*/

#include "gisunlink_eeprom.h"

#define BASE_CONFIG_ADDR				0x4780
#define SYSTEM_BIT_ADDR					BASE_CONFIG_ADDR
#define RUNTIME_DATA_ADDR				(BASE_CONFIG_ADDR + 1)
#define SEC_BIT							(0) //sec min hour1 hour2 run
#define MIN_BIT							(1) //sec min hour1 hour2 run
#define HOUR_LBIT						(2) //sec min hour1 hour2 run
#define HOUR_HBIT						(3) //sec min hour1 hour2 run

#define SYSTEMINITFLAGS					0x0F
#define MAXTIMELENGHT					4
#define MAXWRITERECORD					31 //( (128 - 1) / MAXTIMELENGHT ) ~= 31 (100000 * 31 = 3100000次)

static uint8 readblockPos = 0;
static uint8 writeblockPos = 0;
volatile uint8 xdata systemTime[MAXTIMELENGHT];

static uint8 Read_APROM_BYTE(UINT16 code *u16_addr) {
	uint8 rdata;
	rdata = *u16_addr >> 8;
	return rdata;
}

static void Erase_Config_APPOM(void) {
	IAPAL = RUNTIME_DATA_ADDR & 0xff;
	IAPAH = (RUNTIME_DATA_ADDR >> 8) & 0xff;
	IAPFD = 0xFF;
	set_IAPEN; 
	set_APUEN;
	IAPCN = 0x22; 		
	set_IAPGO; 	
}

static void Write_SystemTime_To_XRAM(uint8 index,uint8 u8EPData) {
	systemTime[index] = u8EPData;	
}

static void Write_SystemFlags_To_APPOM(void) {
	set_IAPEN; 
	set_APUEN;
	IAPCN = 0x21;
	IAPAL = (SYSTEM_BIT_ADDR & 0xff);
	IAPAH = (SYSTEM_BIT_ADDR >> 8) & 0xff;
	IAPFD = SYSTEMINITFLAGS;
	set_IAPGO;	
	clr_APUEN;
	clr_IAPEN;
}

static void Write_SystemTime_To_APPOM(void) {
	uint8 i = 0;	
	uint16 address = 0;
	set_IAPEN; 
	set_APUEN;
	IAPCN = 0x21;

	for(i = 0; i < MAXTIMELENGHT; i++) {
		address = RUNTIME_DATA_ADDR + (writeblockPos * MAXTIMELENGHT) + i;
		IAPAL = (address & 0xff);
		IAPAH = (address >> 8) & 0xff;
		IAPFD = systemTime[i];
		set_IAPGO;			
	}
	clr_APUEN;
	clr_IAPEN;
}

static uint8 gisunlink_eeprom_calac_cur_block(void) {
	uint8 i = 0; 
	uint8 val = 0;
	uint16 address = 0;
	for(i = 0; i <= MAXWRITERECORD; i++) {
		address = (RUNTIME_DATA_ADDR + (i * MAXTIMELENGHT));
		val = Read_APROM_BYTE((UINT16 code *)address);
		if(val == 0xFF) {
			break;
		}
	}
	writeblockPos = i;
	if(writeblockPos) {
		readblockPos = (writeblockPos - 1);
	}
	return i;
}

static void gisunlink_eeprom_check_first_init(void) {
	Erase_Config_APPOM();
	Write_SystemFlags_To_APPOM();
	Write_SystemTime_To_XRAM(SEC_BIT,0);
	Write_SystemTime_To_XRAM(MIN_BIT,0);
	Write_SystemTime_To_XRAM(HOUR_LBIT,0);
	Write_SystemTime_To_XRAM(HOUR_HBIT,0);	
	Write_SystemTime_To_APPOM();	
}

static bool gisunlink_eeprom_compare_runtime(uint8 sec, uint8 min, uint16 hour) {
	uint16 index_address =  0;
	gisunlink_sys_time time = {0};
	bool retVal = false;
	index_address = (RUNTIME_DATA_ADDR + (readblockPos * MAXTIMELENGHT));
	time.sec = Read_APROM_BYTE((UINT16 code *)(index_address+SEC_BIT));
	time.min = Read_APROM_BYTE((UINT16 code *)(index_address+MIN_BIT));
	time.hour = (Read_APROM_BYTE((UINT16 code *)(index_address+HOUR_HBIT)) << 8) | (Read_APROM_BYTE((UINT16 code *)(index_address+HOUR_LBIT)));		
	if(sec == time.sec && min == time.min && hour == time.hour) {
		retVal = true;
	}
	return retVal;
}

void gisunlink_eeprom_read_runtime(gisunlink_sys_time *time) {
	uint16 index_address =  0;
	index_address = (RUNTIME_DATA_ADDR + (readblockPos * MAXTIMELENGHT));
	time->sec = Read_APROM_BYTE((UINT16 code *)(index_address+SEC_BIT));
	time->min = Read_APROM_BYTE((UINT16 code *)(index_address+MIN_BIT));
	time->hour = (Read_APROM_BYTE((UINT16 code *)(index_address+HOUR_HBIT)) << 8) | (Read_APROM_BYTE((UINT16 code *)(index_address+HOUR_LBIT)));
}

void gisunlink_eeprom_write_runtime(uint8 sec, uint8 min, uint16 hour) {
	if(gisunlink_eeprom_compare_runtime(sec,min,hour) == false)	{
		gisunlink_eeprom_calac_cur_block();
		Write_SystemTime_To_XRAM(SEC_BIT,sec);
		Write_SystemTime_To_XRAM(MIN_BIT,min);
		Write_SystemTime_To_XRAM(HOUR_LBIT,hour & 0xFF);
		Write_SystemTime_To_XRAM(HOUR_HBIT,(hour >> 8) & 0xFF);	
		Write_SystemTime_To_APPOM();	
	}
}

bool gisunlink_eeprom_max_write_runtime_record_chk(void) {
	bool isMaxWriteRecord = false;
	gisunlink_sys_time time = {0};
	if(writeblockPos >= MAXWRITERECORD) {
		gisunlink_eeprom_read_runtime(&time);	//读完当前时间后重置读写坐标位
		readblockPos = 0;	
		writeblockPos = 0;	//必须置写位为0
		Erase_Config_APPOM();
		Write_SystemFlags_To_APPOM();	//重新写系统标志位
		Write_SystemTime_To_XRAM(SEC_BIT,time.sec);
		Write_SystemTime_To_XRAM(MIN_BIT,time.min);
		Write_SystemTime_To_XRAM(HOUR_LBIT,time.hour & 0xFF);
		Write_SystemTime_To_XRAM(HOUR_HBIT,(time.hour >> 8) & 0xFF);	
		Write_SystemTime_To_APPOM();	
		isMaxWriteRecord = true;
	}
	return isMaxWriteRecord;
}

bool gisunlink_eeprom_check_first_run(void) {
	uint8 flags = 0;
	flags = Read_APROM_BYTE(SYSTEM_BIT_ADDR);
	gisunlink_eeprom_calac_cur_block();
	//如果是第一次开机先在第一记录点写0000 并置 初始化位为0x0F
	if(flags != SYSTEMINITFLAGS) {
		gisunlink_eeprom_check_first_init();
		return true;
	} else {
		//检测到当前已是最大写记录 则 擦除页数据重新写入第一坐标点
		gisunlink_eeprom_max_write_runtime_record_chk();
	}
	return false;
}
