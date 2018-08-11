//*****************************************************************************
//
// Dispensadora.c -
// CONFIGURAÇÃO DE TESTE (J,K,L -> ENTRADA; M, N, P -> SAÍDAS)
// STATUS E ERRO (PORTA Q)
//*****************************************************************************

#include "clp.h"

//SAIDAS DO MICRO, DIREÇÃO DA PORTA 5, 7 E 8, 2.
#define S1 BIT7
#define S2 BIT1
#define S3 BIT2
#define S4 BIT3
#define S5 BIT4
#define S6 BIT5
#define S7 BIT6
#define S8 BIT7
#define S9 BIT0
#define S10 BIT1
#define ERRO BIT1
#define STATUS BIT2

//ENTRADAS DO MICRO, DIREÇÃO DA PORTA 1 E 6.
#define E1 BIT0
#define E2 BIT1
#define E3 BIT2
#define E4 BIT3
#define E5 BIT4
#define E6 BIT5
#define E7 BIT6
#define E8 BIT0
#define E9 BIT1
#define E10 BIT2

//COPIAR ACIMA DO MAIN A PARTIR DAQUI
unsigned long long int Counter_ms=0, i=0, j=0, tmp = 46;
uint16_t i_Instruction = 0;
char I[33];
char X[33];
char Y[33];
char z[33];
char Q[33];
//Rising pulse indicator: default is false
char Maux[33];
char Qaux[33];
char Xaux[33];
char Yaux[33];
char zaux[33];
//
char M[33];
char T[33];
char R[33];
struct Timer v_Timer[33];
struct Stepper v_Stepper[8];
struct Instruction v_Instruction[120];
struct Coil v_Coil[120];
struct Contact v_Contact[360];
uint16_t Stepper_Instructions[10];
uint16_t aux_Stepper_Inst = 0, aux_Stepper = 0;
uint16_t aux1, aux2, aux3, aux4;
unsigned char *F_I, *F_X, *F_Y, *F_z, *F_Q, *F_M, *F_R, *F_T, *F_TVALUE;

char FrameTx[20];
char FrameRx[20];
char contFrame = 0;
bool isLoopCycleFinished;
bool isFrameReady = false;
bool isFirstLoop = true;
bool is5msLoop = true;
bool RoutineStarted = false;
bool estado = false;
uint32_t count_us = 0;
uint32_t last_count_us = 0;
uint32_t aux_count_us = 0;
uint32_t count_ms = 0;
uint32_t last_count_ms = 0;
uint32_t ProductCounter = 0, aux_ProductCounter = 0;
uint16_t aux_value = 0, aux_value2 = 0, aux_value3 = 0;

unsigned char aux_GetConfigs = 0;
uint16_t tmp_index = 29;

uint16_t aux_Contact = 0;
uint16_t aux_Coil = 0;
uint16_t aux_Inst = 0;

uint16_t tempos[100], index_tempos = 0, aux_tempo = 0;

//COPIAR ACIMA DO MAIN ATÃƒâ€° AQUI

uint16_t teste[100];

//Novas funções para o TM4C1923
    void Flash_rw(uint32_t segmento, char byte); //OK
    void configureUart(uint32_t clock, uint32_t baud); //OK
//CRIAR FUNÇÕES ABAIXO DAQUI
    void Start(); //OK
    void OperationCycle();
    bool F_Coil(struct Instruction *Inst, uint16_t Index);
    bool F_Timer(struct Instruction *Inst, uint16_t Index_Inst);
    bool F_Stepper(struct Instruction *Inst, uint16_t Index_Inst);
    void SetContact(uint16_t Index, unsigned char Vector, unsigned char VectorIndex, unsigned char isNO);
    void SetCoil(uint16_t Index, unsigned char Vector, unsigned char Type, unsigned char VectorIndex);
    void SetTimer(uint16_t Index, unsigned char Mode, unsigned char Base, uint16_t SetValue,
                  unsigned char Type, unsigned char ResetVector, unsigned char ResetIndex);
    void SetStepper(uint16_t Index, unsigned char Mode, unsigned char Base,
                    uint16_t Speed, uint16_t SetAccValue, uint16_t SetTotalValue,
                    uint16_t SetDecValue, unsigned char Vector, unsigned char Vector_Index);
    void SetInstruction(uint16_t Index, unsigned char Func_Type, uint16_t Func_Index, bool HasOrInstruction,
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
                                uint16_t SetValue, unsigned char Type, unsigned char ResetVector,
                                unsigned char ResetIndex, bool HasOrInstruction);

    void SetNewStepperInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                                  unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                                  unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                                  unsigned char Stepper_Index, unsigned char Base, uint16_t Speed,
                                  uint16_t SetAccValue, uint16_t SetTotalValue, uint16_t SetDecValue,
                                  unsigned char Vector, unsigned char Vector_Index, bool HasOrInstruction);
    bool GetInstructionResult(struct Instruction *Inst); //OK
    bool GetContactState(struct Contact *Contact_1); //OK
    void ProcessFrame(); //OK
    void ProcessGetConfigs(); //OK
    void ProcessGetSpeed(); //OK
    void ProcessGetLength(); //OK
    void ProcessGetCounter(); //OK
    void ProcessGetPowerState(); //OK
    void ProcessGetSensorState(); //OK
    void ProcessGetTemperatureState(); //OK
    void ProcessGetStampState(); //OK
    void ProcessWriteSingleRegister();
    void ProcessWriteSingleRegisterTimer(); //OK
    void sendTx(unsigned char size); //OK
    void CodigoLadder();
    void CodigoTeste();
    void Fail_LoopCycleTimeout(); //OK
    void Flash_wb(uint32_t ui32Address, char byte); //OK
    void clear_Seg(uint32_t segmento); //OK
    void copy_X2B(uint16_t Segment, unsigned char *Data_ptr1, unsigned char *Data_ptr2, char byte1, char byte2); //OK
    //void copy_B2X(uint16_t Segment);
    unsigned char checksum(unsigned char *frame, unsigned char length); //OK

void atualizaentradas(){
     //   P8OUT^=BIT0;
      if(PIN(PJ, E1))
          I[1]=false;
      else
          I[1]=true;

      if(PIN(PJ, E2))
          I[2]=false;
      else
          I[2]=true;
      
      if(PIN(PJ, E3))
          I[3]=false;
      else
          I[3]=true;
      
      if(PIN(PJ, E4))
          I[4]=false;
      else
          I[4]=true;
      
      if(PIN(PJ, E5))
          I[5]=false;
      else
          I[5]=true;
      
      if(PIN(PJ, E6))
          I[6]=false;
      else
          I[6]=true;
      
      if(PIN(PJ, E7))
          I[7]=false;
      else
          I[7]=true;
      
      if(PIN(PK, E8))
          I[8]=false;
      else
          I[8]=true;
      
      if(PIN(PK, E9))
          I[9]=false;
      else
          I[9]=true;
      
      if(PIN(PK, E10))
          I[10]=false;
      else
          I[10]=true;
    //  P8OUT^=BIT1;
}

void atualizasaidas(int ind, bool res){
    switch(ind){
    case 1:
        if (res)
            POUT(PM, S1, 1);
        else
            POUT(PM, S1, 0);
        break;
    case 2:
        if (res)
            POUT(PN, S2, 1);
        else
            POUT(PN, S2, 0);
        break;
    case 3:
        if (res)
            POUT(PN, S3, 1);
        else
            POUT(PN, S3, 0);
        break;
    case 4:
        if (res)
            POUT(PN, S4, 1);
        else
            POUT(PN, S4, 0);
        break;
    case 5:
        if (res)
            POUT(PN, S5, 1);
        else
            POUT(PN, S5, 0);
        break;
    case 6:
        if (res)
            POUT(PN, S6, 1);
        else
            POUT(PN, S6, 0);
        break;
    case 7:
        if (res)
            POUT(PN, S7, 1);
        else
            POUT(PN, S7, 0);
        break;
    case 8:
        if (res)
            POUT(PN, S8, 1);
        else
            POUT(PN, S8, 0);
        break;
    case 9:
        if (res)
            POUT(PP, S9, 1);
        else
            POUT(PP, S9, 0);
        break;
    case 10:
        if (res)
            POUT(PP, S10, 1);
        else
            POUT(PP, S10, 0);
        break;
    }
}

bool GetInstructionResult(struct Instruction *Inst)
{
    return (GetContactState(&(*Inst).Inst_Contact_1) && GetContactState(&(*Inst).Inst_Contact_2) && GetContactState(&(*Inst).Inst_Contact_3));
 //   return 1;
}

bool GetContactState(struct Contact *Contact_1)
{

    switch((*Contact_1).Vector)
    {
    case CONTACT_VECTOR_I:

        if ((*Contact_1).isNO == true)
            return I[(*Contact_1).Index];
        else
            return !I[(*Contact_1).Index];
        break;
    case CONTACT_VECTOR_X:

        if ((*Contact_1).isNO == true)
            return X[(*Contact_1).Index];
        else
            return !X[(*Contact_1).Index];
        break;
    case CONTACT_VECTOR_Y:

        if ((*Contact_1).isNO == true)
            return Y[(*Contact_1).Index];
        else
            return !Y[(*Contact_1).Index];
        break;
    case CONTACT_VECTOR_Z:

        if ((*Contact_1).isNO == true)
            return z[(*Contact_1).Index];
        else
            return !z[(*Contact_1).Index];
        break;
    case CONTACT_VECTOR_Q:

        if ((*Contact_1).isNO == true)
            return Q[(*Contact_1).Index];
        else
            return !Q[(*Contact_1).Index];
        break;
    case CONTACT_VECTOR_M:

        if ((*Contact_1).isNO == true)
            return M[(*Contact_1).Index];
        else
            return !M[(*Contact_1).Index];
        break;
    case CONTACT_VECTOR_T:

        if ((*Contact_1).isNO == true)
//            return TO[Contact_1.Index];
//              return v_Timer[(*Contact_1).Index].State;
        return T[(*Contact_1).Index];
        else
//            return !TO[Contact_1.Index];
//                return !v_Timer[(*Contact_1).Index].State;
            return !T[(*Contact_1).Index];

        break;
    case CONTACT_VECTOR_ZERO:

            return true;
        break;
    }
}


//COPIAR FUNÃƒâ€¡Ãƒâ€¢ES A PARTIR DAQUIS
void Start()//DEVE ESTAR NO INÃƒÂ�CIO DO MAIN
{
    Counter_ms = 0;
    isLoopCycleFinished = true;

    for (i=0; i < 32; i++)
    {
        I[i]=0;
        X[i]=0;
        Y[i]=0;
        z[i]=0;
        M[i]=0;
        R[i]=0;
        Q[i]=0;
//            T[i]=0;
        Maux[i]=false;
        Qaux[i]=false;
        Xaux[i]=false;
        Yaux[i]=false;
        zaux[i]=false;

    }
    for (i=0; i < 10; i++)
       Stepper_Instructions[i] = 0;
//        X[0]=1;
    X[11]=1;
//        X[2]=1;
//        X[3]=1;
//        X[4]=1;
//        X[5]=1;
//        X[6]=1;
//        X[7]=1;
//        X[8]=1;
//        X[9]=1;

    F_I = FLASH_VECTOR_I;
    F_X = FLASH_VECTOR_X;
    F_Y = FLASH_VECTOR_Y;
    F_z = FLASH_VECTOR_Z;
    F_Q = FLASH_VECTOR_Q;
    F_M = FLASH_VECTOR_M;
    F_R = FLASH_VECTOR_R;
    F_T = FLASH_VECTOR_T;
    F_TVALUE = FLASH_VECTOR_TIMER;

//        Clear_SegX(SEG_C);
//        Clear_SegX(SEG_D);
//        copy_X2B(SEG_C, (uint16_t)(0x1040+0x05*2), (uint16_t)(0x1040+0x05*2+0x01), 0x05, 0x5b);
//        copy_B2X(SEG_C);
//        copy_X2B(SEG_C, (uint16_t)(0x1040+0x06*2), (uint16_t)(0x1040+0x06*2+0x01), 0x00, 0x32);
//        copy_B2X(SEG_C);
//        copy_X2B(SEG_C, (uint16_t)(0x1040+0x07*2), (uint16_t)(0x1040+0x07*2+0x01), 0x07, 0xcb);
//        copy_B2X(SEG_C);
//        copy_X2B(SEG_C, (uint16_t)(0x1040+0x08*2), (uint16_t)(0x1040+0x08*2+0x01), 0x00, 0x32);
//        copy_B2X(SEG_C);

    CodigoLadder();//Tem que terminar de inicializar as variÃƒÂ¡veis
//        CodigoTeste();
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (i < 4)
            {
//                    I[i*8+j] = (*(F_I+i)&(0x01<<j) ? true : false);
//                    X[i*8+j] = (*(F_X+i)&(0x01<<j) ? true : false);
                Y[i*8+j] = (*(F_Y+i)&(0x01<<j) ? true : false);
//                    z[i*8+j] = (*(F_z+i)&(0x01<<j) ? true : false);
//                    Q[i*8+j] = (*(F_Q+i)&(0x01<<j) ? true : false);
//                    M[i*8+j] = (*(F_M+i)&(0x01<<j) ? true : false);
//                    A[i*8+j] = *(F_A+i)&(0x01<<j ? true : false);
//                    T[i*8+j] = (*(F_T+i)&(0x01<<j) ? true : false);
            }
            aux1 = F_TVALUE+2*(8*i+j);
            aux2 = *(F_TVALUE+2*(8*i+j));
            aux3 = F_TVALUE+2*(8*i+j)+1;
            aux4 = *(F_TVALUE+2*(8*i+j)+1);
            aux_value = (*(F_TVALUE+2*(8*i+j))<<8)|*(F_TVALUE+2*(8*i+j)+1);
//                if (i*8+j < 5)
//                {
//                    v_Timer[i*8+j].SetValue = aux_value;
//                    v_Timer[i*8+j].Timer_Count = aux_value/5;
//                }
            R[8*i+j] = *(F_R+(8*i+j));

        }
    }
//        v_Timer[7].SetValue = (R[5]-20)*100;
//        if (v_Timer[7].SetValue > 0)
//            v_Timer[7].SetValue -= 5;
//        v_Timer[7].Timer_Count = v_Timer[7].SetValue/5;
//        v_Timer[2].SetValue = (4200-105*R[7])*(float)(20)/(float)R[5];
//        v_Timer[2].Timer_Count = v_Timer[2].SetValue/5;
    ProductCounter = (R[8]<<8)|R[9];
    RoutineStarted = true;
}


void Fail_LoopCycleTimeout() //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
{
    POUT(PQ, ERRO, 1); //Gerar erro: piscar led de erro para informar o timeout               <<<<<<<<<<<ACENDER INDICANDO ESTOURO DE TIMMEOUT
}

void ProcessFrame()
{
    switch (FrameRx[FRAME_CMD_2])
    {
    case MODBUS_WRITE_SINGLE_COIL:
        break;
    case MODBUS_WRITE_SINGLE_REGISTER:
        ProcessWriteSingleRegister();
        break;
    case MODBUS_READ_SPEED_REGISTER:
        ProcessGetSpeed();
        break;
    case MODBUS_READ_LENGTH_REGISTER:
        ProcessGetLength();
        break;
    case MODBUS_WRITE_SINGLE_REGISTER_SIZE:
        break;
    case MODBUS_REQUEST_CONFIG:
        aux_GetConfigs = 1;
//        ProcessGetSpeed();
//        __delay_cycles(50000);
//        ProcessGetLength();
//        __delay_cycles(50000);
//        ProcessGetCounter();

    }

    for (contFrame = 0; contFrame < 20; contFrame++)
        FrameRx[contFrame] = 0;
    isFrameReady = false;
}

void ProcessGetConfigs()
{
    switch (aux_GetConfigs++)
    {
    case 1:
//        ProcessGetSpeed();
        break;
    case 2:
//        ProcessGetLength();
        break;
    case 3:
        ProcessGetCounter();
        break;
    case 4:
        ProcessGetPowerState();
        break;
    case 5:
        ProcessGetSensorState();
        break;
    case 6:
//        ProcessGetTemperatureState();
        break;
    case 7:
//        ProcessGetStampState();
        aux_GetConfigs = 0;
        break;
    }
}

void ProcessGetSpeed()
{
    FrameTx[0] = 0x7e;
    FrameTx[1] = 0x00;
    FrameTx[2] = 0x00;
    FrameTx[3] = 0x00;
    FrameTx[4] = 0x00;
    FrameTx[5] = 0x0E;
    FrameTx[6] = 0x06;
    FrameTx[7] = 0x00;
    FrameTx[8] = MODBUS_WRITE_SINGLE_REGISTER;
    FrameTx[9] = 0x13;
    FrameTx[10] = 0x00;
    FrameTx[11] = R[4];
    FrameTx[12] = R[5];
    FrameTx[13] = checksum(FrameTx, 14);

    sendTx(14);
}


void ProcessGetLength()
{
    FrameTx[0] = 0x7e;
    FrameTx[1] = 0x00;
    FrameTx[2] = 0x00;
    FrameTx[3] = 0x00;
    FrameTx[4] = 0x00;
    FrameTx[5] = 0x0E;
    FrameTx[6] = 0x06;
    FrameTx[7] = 0x00;
    FrameTx[8] = MODBUS_WRITE_SINGLE_REGISTER;
    FrameTx[9] = 0x13;
    FrameTx[10] = 0x01;
    FrameTx[11] = R[6];
    FrameTx[12] = R[7];
    FrameTx[13] = checksum(FrameTx, 14);

    sendTx(14);
}

void ProcessGetCounter()
{
    FrameTx[0] = 0x7e;
    FrameTx[1] = 0x00;
    FrameTx[2] = 0x00;
    FrameTx[3] = 0x00;
    FrameTx[4] = 0x00;
    FrameTx[5] = 0x0E;
    FrameTx[6] = 0x06;
    FrameTx[7] = 0x00;
    FrameTx[8] = MODBUS_WRITE_SINGLE_REGISTER;
    FrameTx[9] = 0x12;
    FrameTx[10] = 0x00;
    FrameTx[11] = R[8];
    FrameTx[12] = R[9];
    FrameTx[13] = checksum(FrameTx, 14);

    sendTx(14);
}

void ProcessGetPowerState()
{
    FrameTx[0] = 0x7e;
    FrameTx[1] = 0x00;
    FrameTx[2] = 0x00;
    FrameTx[3] = 0x00;
    FrameTx[4] = 0x00;
    FrameTx[5] = 0x0E;
    FrameTx[6] = 0x06;
    FrameTx[7] = 0x00;
    FrameTx[8] = MODBUS_WRITE_SINGLE_REGISTER;
    FrameTx[9] = 0x10;
    FrameTx[10] = 0x04;
    FrameTx[11] = 0x00;
    FrameTx[12] = (Y[0x10] ? 100 : 101);
    FrameTx[13] = checksum(FrameTx, 14);

    sendTx(14);
}
void ProcessGetSensorState()
{
        FrameTx[0] = 0x7e;
        FrameTx[1] = 0x00;
        FrameTx[2] = 0x00;
        FrameTx[3] = 0x00;
        FrameTx[4] = 0x00;
        FrameTx[5] = 0x0E;
        FrameTx[6] = 0x06;
        FrameTx[7] = 0x00;
        FrameTx[8] = MODBUS_WRITE_SINGLE_REGISTER;
        FrameTx[9] = 0x10;
        FrameTx[10] = 0x06;
        FrameTx[11] = 0x00;
        FrameTx[12] = (Y[0x12] ? 100 : 101);
        FrameTx[13] = checksum(FrameTx, 14);

        sendTx(14);
}
void ProcessGetTemperatureState()
{
        FrameTx[0] = 0x7e;
        FrameTx[1] = 0x00;
        FrameTx[2] = 0x00;
        FrameTx[3] = 0x00;
        FrameTx[4] = 0x00;
        FrameTx[5] = 0x0E;
        FrameTx[6] = 0x06;
        FrameTx[7] = 0x00;
        FrameTx[8] = MODBUS_WRITE_SINGLE_REGISTER;
        FrameTx[9] = 0x10;
        FrameTx[10] = 0x07;
        FrameTx[11] = 0x00;
        FrameTx[12] = (Y[0x0C] ? 100 : 101);
        FrameTx[13] = checksum(FrameTx, 14);

        sendTx(14);
}
void ProcessGetStampState()
{
        FrameTx[0] = 0x7e;
        FrameTx[1] = 0x00;
        FrameTx[2] = 0x00;
        FrameTx[3] = 0x00;
        FrameTx[4] = 0x00;
        FrameTx[5] = 0x0E;
        FrameTx[6] = 0x06;
        FrameTx[7] = 0x00;
        FrameTx[8] = MODBUS_WRITE_SINGLE_REGISTER;
        FrameTx[9] = 0x10;
        FrameTx[10] = 0x08;
        FrameTx[11] = 0x00;
        FrameTx[12] = (Y[0x0D] ? 100 : 101);
        FrameTx[13] = checksum(FrameTx, 14);

        sendTx(14);
}



void ProcessWriteSingleRegisterTimer()
{
    switch (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AH]&0x0F)
        {
        case REGISTER_BASE_TIMER:
            v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Base = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
            break;
        case REGISTER_VALUE_TIMER:
            v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].SetValue = (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VH]<<8)+FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
            v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Timer_Count = (v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Base == TIMER_BASE_MS1 ? v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].SetValue : v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Base == TIMER_BASE_MS10 ? 10*v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].SetValue : v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Base == TIMER_BASE_MS100 ? 100*v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].SetValue : v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Base == TIMER_BASE_SEC ? 1000*v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].SetValue : v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Base == TIMER_BASE_MIN ? 600*v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].SetValue : 0);
            v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Timer_Count /= 5;
            break;
        }
}


void configureUart(uint32_t clock, uint32_t baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, clock, baud,
                                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                        UART_CONFIG_PAR_NONE));
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

void Flash_wb(uint32_t ui32Address, char byte)
{
    IntMasterDisable();
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
            vectorWrite[i] = (char) point[i];

    }
    FlashProgram(vectorWrite, (uint32_t) ui32Address, (uint32_t) 4);
    IntMasterEnable();
}


void Flash_rw(uint32_t segmento, char byte)
{
    IntMasterDisable();
    //Verifica o múltiplo de 4
    if(FLASH_TAM %4 != 0) goto retorna;

    uint32_t i, j, aux = 0;
    char vetor[256];
    char *point, *point2;

    //Adequa o endereço para 16kB
    while(segmento % 16384 != 0)
    {
        segmento = segmento - 1;
        j++;
    }

    //Verifica se o offset de memória
    if(j > FLASH_TAM) goto retorna;

    //Realiza troca para espaço de transferência
    uint32_t enderecoTroca = 0x8000;
    point = segmento;
    FlashErase((uint32_t) enderecoTroca);    
    FlashProgram(point, (uint32_t) enderecoTroca, (uint32_t) FLASH_TAM);  
    
    //Apaga e regrava
    FlashErase((uint32_t) segmento);
    point = enderecoTroca;
    i = 0;
    for(i=0; i<FLASH_TAM; i=i+256)
    {
        point = enderecoTroca + i;
        for(aux=0; aux<256; aux++)
        {
            if((aux+i) != j)
                vetor[aux] = point[aux];    
            else
                vetor[aux] = byte;
        }
        FlashProgram(vetor, (uint32_t) segmento+i, (uint32_t) 256);      
    }

retorna:
    IntMasterEnable();
    return 0;

}

void clear_Seg(uint32_t segmento)
{
    Flash_rw(segmento, 0xFF);
}

void Clear_SegX(uint32_t Segment)
{
    uint16_t i;
    for (i = 0; i < 64; i++)
        clear_Seg(Segment+i);
}

void copy_X2B(uint16_t Segment, unsigned char *Data_ptr1, unsigned char *Data_ptr2, char byte1, char byte2)
{
    uint32_t end1 = (uint32_t) Data_ptr1;
    uint32_t end2 = (uint32_t) Data_ptr2;
    Flash_rw(end1, byte1);
    SysCtlDelay(5);
    Flash_rw(end2, byte2);
}

void sendTx(unsigned char size)
{
    contFrame = 0;
    while (contFrame <= size)
    {
        UARTCharPutNonBlocking(UART0_BASE, FrameTx[contFrame]);
        contFrame = contFrame + 1;
    }
}

unsigned char checksum(unsigned char *frame, unsigned char length)
{
    unsigned char sum = 0;
    int aux_i = 0;

    for (aux_i = 0; aux_i < length-1; aux_i++)
    {
        sum += frame[aux_i];
    }

    return (0xff - sum);
}

int main(void)
{
    //Configura para utilizar o Clock Interno
    uint32_t clock;
    clock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                        SYSCTL_OSC_INT |
                        SYSCTL_USE_PLL |
                        SYSCTL_CFG_VCO_480), 120000000);

    //Habilita os periféricos GPIOs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    
/*        
    //CONFIGURAÇÃO DE TESTE (J,K,L -> ENTRADA; M, N, P -> SAÍDAS)
    PDIR(PM, S1);
    PDIR(PN, (S2|S3|S4|S5|S6|S7|S8));
    PDIR(PP, (S9|S10));
    POUT(PM, S1, 1);
    POUT(PN, (S2|S3|S4|S5|S6|S7|S8), 1);
    POUT(PP, (S9|S10), 1);
*/

    configureUart(clock, 115200);

    SysCtlDelay(100);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    uint32_t tempoTimer = (clock/1000000)*61; //Tempo para 61us (1/(32768/2))
    TimerLoadSet(TIMER0_BASE, TIMER_A, tempoTimer);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
    IntMasterEnable();


    F_I = FLASH_VECTOR_I;
    //Flash_rw(0x4011, 0xFC);
    Flash_rw(F_I+(unsigned int)(2), 0x05);
    Flash_rw(F_I+(unsigned int)(1), 0x09);
    Flash_rw((F_I+(unsigned int)(1)), *(F_I+(unsigned int)(1))+2);
    //copy_X2B(SEG_D, F_I+(unsigned int)(2), F_I+(unsigned int)(1), 0x05, 0x06);
    //copy_X2B(SEG_D, F_I[4], 0x00, F_I[0], 0x00);
    
    PDIR(PN, BIT0);

    while(1){
        POUT(PN, BIT0, 1);
    }


}

//Interrupção de UART RX
void UARTIntHandler(void)
{
    uint32_t ui32Status;
    //
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART0_BASE, false);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //

        if (contFrame == 0)
            FrameRx[contFrame] = UARTCharGetNonBlocking(UART0_BASE);
        if (contFrame >= 1 && contFrame <= 4)
            FrameRx[contFrame] = UARTCharGetNonBlocking(UART0_BASE);
        if (contFrame == 5)
            FrameRx[contFrame] = UARTCharGetNonBlocking(UART0_BASE);
        if (contFrame > 5 && contFrame < FrameRx[5])
            FrameRx[contFrame] = UARTCharGetNonBlocking(UART0_BASE);
        
        contFrame++;
        if (contFrame == FrameRx[5])
        {
            isFrameReady = true;
            contFrame = 0;
        }
        if (FrameRx[0] != 0x7e)
            contFrame = 0;
    }
}

void Timer0IntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //P2OUT ^= STATUS;                            // Toggle P1.0
    if(estado) estado = false;
    else estado = true;

    POUT(PQ, STATUS, estado);
    //CCR0 += 2;//165/83                           // Add Offset to CCR0
    if (++aux_count_us == 83)//42
    {
        aux_count_us = 0;
        count_ms += 5;
        is5msLoop = true;
        Counter_ms++;
    }
    count_us++;

    //
    // Update the interrupt status.
    //
    IntMasterDisable();
    IntMasterEnable();
}
