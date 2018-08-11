/*
 * clp.h
 *
 *  Created on: 19 de jun de 2017
 *      Author: usuario
 */

#ifndef CLP_H_
#define CLP_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/flash.h"
#include "utils/uartstdio.h"

//define de bits
#define BIT0 GPIO_PIN_0
#define BIT1 GPIO_PIN_1
#define BIT2 GPIO_PIN_2
#define BIT3 GPIO_PIN_3
#define BIT4 GPIO_PIN_4
#define BIT5 GPIO_PIN_5
#define BIT6 GPIO_PIN_6
#define BIT7 GPIO_PIN_7

//define das portas
#define PJ GPIO_PORTJ_BASE
#define PK GPIO_PORTK_BASE
#define PL GPIO_PORTL_BASE
#define PM GPIO_PORTM_BASE
#define PN GPIO_PORTN_BASE
#define PP GPIO_PORTP_BASE
#define PQ GPIO_PORTQ_BASE
#define PR GPIO_PORTR_BASE
#define PS GPIO_PORTS_BASE
#define PT GPIO_PORTT_BASE

//Define quais portas utilizarão GPIO



//TAMANHO DE BYTES DA FLASH PARA TRANSFERÊNCIA (MULT. 4)
#define FLASH_TAM 1024
#define PDIR(a, b) GPIOPinTypeGPIOOutput(a, b)
#define POUT(a, b, c) GPIOPinWrite(a, b, c)
#define PIN(a, b) GPIOPinRead(a, b)

#define FRAME_START 0x00
#define FRAME_TRANSACTION_H 0x01
#define FRAME_TRANSACTION_L 0x02
#define FRAME_PROTOCOL_H 0x03
#define FRAME_PROTOCOL_L 0x04
#define FRAME_LENGTH_H 0x05
#define FRAME_LENGTH_L 0x06
#define FRAME_UNIT_ID 0x07
#define FRAME_CMD_2 0x08

#define CMD_CONFIGURACAO    0x01
#define CMD_LIGADESLIGA     0x02
#define CMD_PERGUNTAESTADO  0x03
#define CMD_STEP            0x04
#define CMD_PING            0x05
#define CMD_CONTADOR        0x06
#define CMD_STEP_DESLIGADO  0x07
#define CMD_SENHA           0x08

#define MODBUS_WRITE_SINGLE_COIL 0x05
#define MODBUS_WRITE_SINGLE_REGISTER 0x06
#define MODBUS_WRITE_SINGLE_REGISTER_SIZE 0x07
#define MODBUS_READ_SPEED_REGISTER 0x19
#define MODBUS_READ_LENGTH_REGISTER 0x1A
#define MODBUS_REQUEST_CONFIG 0x40

#define REQ_WRITE_SINGLE_REGISTER_AH 0x01
#define REQ_WRITE_SINGLE_REGISTER_AL 0x02
#define REQ_WRITE_SINGLE_REGISTER_VH 0x03
#define REQ_WRITE_SINGLE_REGISTER_VL 0x04

#define REGISTER_VECTOR 0xF0 //0xFF00 0000
#define REGISTER_VECTOR_ATRIBUTE 0x0F //0x0F00
#define REGISTER_VECTOR_INDEX 0xFF //0x00FF

#define REGISTER_BASE_TIMER 0x01
#define REGISTER_VALUE_TIMER 0x02


#define VECTOR_I        0x10
#define VECTOR_X        0x20
#define VECTOR_Y        0x30
#define VECTOR_Z        0x40
#define VECTOR_Q        0x50
#define VECTOR_M        0x60
#define VECTOR_T        0x70
#define VECTOR_TIMER    0x80
#define VECTOR_A        0x90
#define VECTOR_R		0xA0

#define VECTOR_TIMER_SET_VALUE 0x0100

#define SEG_C                 0x4040
#define SEG_D                 0x4000
#define FLASH_VECTOR_I        0x4010
#define FLASH_VECTOR_X        0x4014
#define FLASH_VECTOR_Y        0x4018
#define FLASH_VECTOR_Z        0x401C
#define FLASH_VECTOR_Q        0x4020
#define FLASH_VECTOR_M        0x4024
#define FLASH_VECTOR_A        0x4028
#define FLASH_VECTOR_R		  0x402C //até 3B
#define FLASH_VECTOR_T        0x403C
#define FLASH_VECTOR_TIMER    0x4040 //até 4F

// Criando a enumeraÃ§Ã£o:

/*enum boolean {

    true = 1, false = 0

};*/

// Permitindo a sua declaraÃ§Ã£o como um tipo qualquer:

//typedef  enum boolean  bool;

enum CONTACT_VECTOR {CONTACT_VECTOR_ZERO=0, CONTACT_VECTOR_I, CONTACT_VECTOR_X, CONTACT_VECTOR_Z, CONTACT_VECTOR_Q, CONTACT_VECTOR_Y, CONTACT_VECTOR_M, CONTACT_VECTOR_T, CONTACT_VECTOR_C, CONTACT_VECTOR_R, CONTACT_VECTOR_G, CONTACT_VECTOR_N, CONTACT_VECTOR_D};

enum FUNC {FUNC_ZERO=0, FUNC_COIL, FUNC_TIMER, FUNC_STEPPER, FUNC_END};

enum COIL {COIL_ZERO=0, COIL_C, COIL_P, COIL_S, COIL_R};

enum STEPPER {STEPPER_ZERO=0, STEPPER_ACC, STEPPER_CONST, STEPPER_DEC, STEPPER_OFF};

enum TIMER_MODE {TIMER_MODE_ZERO=0, TIMER_MODE_T0, TIMER_MODE_T1, TIMER_MODE_T2, TIMER_MODE_T3, TIMER_MODE_T4, TIMER_MODE_T5, TIMER_MODE_T6};

enum TIMER_BASE  {TIMER_BASE_ZERO=0, TIMER_BASE_MS1, TIMER_BASE_MS10, TIMER_BASE_MS100, TIMER_BASE_SEC, TIMER_BASE_MIN};



struct Contact

{

    unsigned char Vector;

    uint16_t Index;

    unsigned char isNO;

    unsigned char State;

};



struct Timer

{

    uint16_t Index;

    unsigned char Mode;

    unsigned char Base;

    uint16_t SetValue;

    uint16_t ReadValue;

    unsigned char Type;

    unsigned char State;

    unsigned char Enabled;

    unsigned char Contact_Reset;

    unsigned char Contact_Reset_Index;

    uint16_t Timer_Count;

    uint16_t Timer_Target;

};

struct Stepper

{

    uint16_t Index;

    unsigned char Mode;

    unsigned char Base;

    uint16_t Speed;

    uint16_t ms_wait;

    uint16_t aux_ms_wait;

    uint16_t inc;

    uint16_t aux_inc;

    uint16_t SetAccValue;

    uint16_t SetTotalValue;

    uint16_t SetDecValue;

    uint16_t ReadValue;

    unsigned char Stage;

    unsigned char State;

    uint16_t Timer_Acc_Count;

    uint16_t Timer_Acc_Target;

    uint16_t Timer_Total_Count;

    uint16_t Timer_Total_Target;

    uint16_t Timer_Dec_Count;

    uint16_t Timer_Dec_Target;

    unsigned char Vector;

    unsigned char Vector_Index;


};



struct Coil

{

    unsigned char Vector;

    unsigned char Type;

    uint16_t Index;

};



struct Instruction

{

    unsigned char Func_Type;

    unsigned char Func_Index;

    bool Has_Or_Instruction;

    struct Contact Inst_Contact_1;

    struct Contact Inst_Contact_2;

    struct Contact Inst_Contact_3;

};







#endif /* CLP_H_ */


