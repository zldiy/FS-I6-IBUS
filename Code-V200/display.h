


enum DIS_MODE
{
	NOMAL0,
	STANDBY_MODE,
	RUN_MODE,
	SET_MODE,
	VALUE_ADJ_MODE,
	FAULT_MODE
};

// 1 2 4 8 16 32 64 128 

#define led1 1
#define led2 2
#define led3 4
#define led4 8
#define led5 16
#define led6 32
#define led7 64
#define led8 128

#define RUN_LED       led1 

#define	VOLTAGE_KV    led2
#define	CURRENT_MA    led3
#define	FREQUENCY_HZ  led4

#define	TEMP_LED      led5

#define CURRENT_A     led6
#define VOLTAGE_V     led7

#define	LOCAL_LED     led8
   



//extern unsigned int code dis_item[];  //用于存储需要读取的数值
extern uint8_t g_key_cmd;  //按键的全局变量

/*
#define SEC_POWER_P dis_item[0]
#define FRI_POWER_D dis_item[1]
#define SEC_POWER_D dis_item[2]
#define SEC_VOLT_P	dis_item[3]
#define SEC_CURRENT_P dis_item[4]
#define FRI_VOLT    dis_item[5]
#define FRI_CURRENT dis_item[6]
#define SEC_VOLT    dis_item[7]
#define SEC_CURRENT dis_item[8]
#define DC_BUS      dis_item[9]
#define RESERVE1    dis_item[10]
#define TRAN_TEMP   dis_item[11]
#define IGBT_TEMP   dis_item[12]
#define RESERVE2    dis_item[13]
#define CONTROL_STATE dis_item[14]
#define RUN_STATE   dis_item[15]
#define FLASH_TIME  dis_item[16]
#define BEFOR_FLASH  dis_item[17]
#define FAULT_NUM    dis_item[18]
#define STOP_FAULT   dis_item[19]
#define WARNING_FAULT dis_item[20]
#define FREQUENCY    dis_item[21]
#define ELE_VALUE    dis_item[22]
#define LOW_PROTECT  dis_item[23]
#define OUTPUT_STATE  dis_item[24]
#define SHORT_TIME    dis_item[25]
#define READ_REG_ADDR  dis_item[16]
*/

//extern uint16_t SEC_POWER_P ;
//extern uint16_t FRI_POWER_D ;
//extern uint16_t SEC_POWER_D ;
//extern uint16_t SEC_VOLT_P	;
//extern uint16_t SEC_CURRENT_P ;
//extern uint16_t FRI_VOLT    ;
//extern uint16_t FRI_CURRENT ;
//extern uint16_t SEC_VOLT    ;
//extern uint16_t SEC_CURRENT ;
//extern uint16_t DC_BUS      ;
////extern uint16_t RESERVE1    ;
//extern uint16_t TRAN_TEMP   ;
//extern uint16_t IGBT_TEMP   ;
////extern uint16_t RESERVE2    ;
////extern uint16_t CONTROL_STATE ;
////extern uint16_t RUN_STATE   ;
////extern uint16_t FLASH_TIME  ;
////extern uint16_t BEFOR_FLASH  ;
extern uint16_t FAULT_NUM   ; 
////extern uint16_t STOP_FAULT  ;
////extern uint16_t WARNING_FAULT ;
//extern uint16_t FREQUENCY    ;
////extern uint16_t ELE_VALUE   ; 
////extern uint16_t LOW_PROTECT ;
////extern uint16_t OUTPUT_STATE ; 
////extern uint16_t SHORT_TIME ;   
extern uint16_t READ_REG_ADDR ;

void display_process(void);
void init_gpio();

