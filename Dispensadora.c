//*****************************************************************************
//
// Dispensadora.c -
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_flash.h"
#include "inc/hw_ints.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/sysctl.h"


//CRIAR FUNÇÕES ABAIXO DAQUI
    void Start();
    void OperationCycle();
    bool F_Coil(struct Instruction *Inst, unsigned int Index);
    bool F_Timer(struct Instruction *Inst, unsigned int Index_Inst);
    bool F_Stepper(struct Instruction *Inst, unsigned int Index_Inst);
    void SetContact(unsigned int Index, unsigned char Vector, unsigned char VectorIndex, unsigned char isNO);
    void SetCoil(unsigned int Index, unsigned char Vector, unsigned char Type, unsigned char VectorIndex);
    void SetTimer(unsigned int Index, unsigned char Mode, unsigned char Base, unsigned int SetValue,
                  unsigned char Type, unsigned char ResetVector, unsigned char ResetIndex);
    void SetStepper(unsigned int Index, unsigned char Mode, unsigned char Base,
                    unsigned int Speed, unsigned int SetAccValue, unsigned int SetTotalValue,
                    unsigned int SetDecValue, unsigned char Vector, unsigned char Vector_Index);
    void SetInstruction(unsigned int Index, unsigned char Func_Type, unsigned int Func_Index, bool HasOrInstruction,
                        struct Contact Contact_1, struct Contact Contact_2, struct Contact Contact_3);
    void SetNewCoilInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                               unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                               unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                               unsigned char Vector_Coil, unsigned char VectorIndex_Coil, unsigned char Type_Coil,
                               bool HasOrInstruction);

    void SetNewTimerInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                                unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                                unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                                unsigned char Timer_Index, unsigned char Mode, unsigned char Base,
                                unsigned int SetValue, unsigned char Type, unsigned char ResetVector,
                                unsigned char ResetIndex, bool HasOrInstruction);

    void SetNewStepperInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                                  unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                                  unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                                  unsigned char Stepper_Index, unsigned char Base, unsigned int Speed,
                                  unsigned int SetAccValue, unsigned int SetTotalValue, unsigned int SetDecValue,
                                  unsigned char Vector, unsigned char Vector_Index, bool HasOrInstruction);
    bool GetInstructionResult(struct Instruction *Inst);
    bool GetContactState(struct Contact *Contact_1);
    void ProcessFrame();
    void ProcessGetConfigs();
    void ProcessGetSpeed();
    void ProcessGetLength();
    void ProcessGetCounter();
    void ProcessGetPowerState();
    void ProcessGetSensorState();
    void ProcessGetTemperatureState();
    void ProcessGetStampState();
    void ProcessWriteSingleRegister();
    void ProcessWriteSingleRegisterTimer();
    void sendTx(unsigned char size);
    void CodigoLadder();
    void CodigoTeste();
    void Fail_LoopCycleTimeout();
    void Flash_wb(uint32_t ui32Address, char byte);
    void clear_Seg(uint32_t segmento);
    void copy_X2B(unsigned int Segment, unsigned char *Data_ptr1, unsigned char *Data_ptr2, char byte1, char byte2);
    void copy_B2X(unsigned int Segment);
    unsigned char checksum(unsigned char *frame, unsigned char length);


    void clear_Seg(unsigned int segmento);

void Flash_wb(uint32_t ui32Address, char byte)
{
    //Prepara para gravar um word
    char vectorWrite[4];
    char *point;
    char i, j = 0;

    //Seta o endereço para gravação correto (múltiplo de 4)
    while(ui32Address % 4 != 0)
    {
        ui32Address--;
        j++;
    }
    point = ui32Address;
    for(i=0; i<4; i++)
    {
        if(i == j)
            vectorWrite[i] = byte;
        else
            vectorWrite[i] = point[i];

    }

    FlashProgram(vectorWrite, (uint32_t) ui32Address, (uint32_t) 4);
    //FlashErase((uint32_t) ui32Address);

}


void clear_Seg(uint32_t segmento)
{
    FlashErase((uint32_t) segmento);
}



int main(void)
{

    //Configura para utilizar o Clock Interno
    uint32_t clock;
    clock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                        SYSCTL_OSC_INT |
                        SYSCTL_USE_PLL |
                        SYSCTL_CFG_VCO_480), 120000000);

    //Habilita pinos para serem GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION | SYSCTL_PERIPH_GPIOJ))
    {
    }

    //Configura o PJ0 como entrada e aguarda habilitação
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    while(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == 0)
    {
        SysCtlDelay(100);
    }

    Flash_wb(0x3FFF, 0x09);
    Flash_wb(0x3FFE, 0x01);

    Flash_wb(0x4001, 0x05);
    Flash_wb(0x4000, 0x02);
    Flash_wb(0x4003, 0x03);
    //clear_Seg(0x4001);
    //clear_Seg(0x3FFF);
    //Flash_wb(0x4001, 0x07);




    //Configura PN0 como saída e realiza o blink
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    while(1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
        SysCtlDelay(clock/2/3);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
        SysCtlDelay(clock/2/3);
    }




}
