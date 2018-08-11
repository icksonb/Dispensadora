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
    void OperationCycle(); //OK
    bool F_Coil(struct Instruction *Inst, unsigned int Index); //OK
    bool F_Timer(struct Instruction *Inst, unsigned int Index_Inst); //OK
    bool F_Stepper(struct Instruction *Inst, unsigned int Index_Inst); //OK
    void SetContact(unsigned int Index, unsigned char Vector, unsigned char VectorIndex, unsigned char isNO); //OK
    void SetCoil(unsigned int Index, unsigned char Vector, unsigned char Type, unsigned char VectorIndex); //OK
    void SetTimer(unsigned int Index, unsigned char Mode, unsigned char Base, unsigned int SetValue,
                  unsigned char Type, unsigned char ResetVector, unsigned char ResetIndex); //OK
    void SetStepper(unsigned int Index, unsigned char Mode, unsigned char Base,
                    unsigned int Speed, unsigned int SetAccValue, unsigned int SetTotalValue,
                    unsigned int SetDecValue, unsigned char Vector, unsigned char Vector_Index); //OK
    void SetInstruction(unsigned int Index, unsigned char Func_Type, unsigned int Func_Index, bool HasOrInstruction,
                        struct Contact Contact_1, struct Contact Contact_2, struct Contact Contact_3); //OK
    void SetNewCoilInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                               unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                               unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                               unsigned char Vector_Coil, unsigned char VectorIndex_Coil, unsigned char Type_Coil,
                               bool HasOrInstruction); //OK

    void SetNewTimerInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                                   unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                                   unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                                   unsigned char Timer_Index, unsigned char Mode, unsigned char Base,
                                   unsigned int SetValue, unsigned char Type, unsigned char ResetVector,
                                   unsigned char ResetIndex, bool HasOrInstruction); //OK

    void SetNewStepperInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                                  unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                                  unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                                  unsigned char Stepper_Index, unsigned char Base, unsigned int Speed,
                                  unsigned int SetAccValue, unsigned int SetTotalValue, unsigned int SetDecValue,
                                  unsigned char Vector, unsigned char Vector_Index, bool HasOrInstruction); //OK
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
    void ProcessWriteSingleRegister(); //OK
    void ProcessWriteSingleRegisterTimer(); //OK
    void sendTx(unsigned char size); //OK
    void CodigoLadder();
    void CodigoTeste();
    void Fail_LoopCycleTimeout(); //OK
    void Flash_wb(uint32_t ui32Address, char byte); //OK
    void clear_Seg(uint32_t segmento); //OK
    //void copy_X2B(uint16_t Segment, unsigned char *Data_ptr1, unsigned char *Data_ptr2, char byte1, char byte2); //OK
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

void OperationCycle()//EXECUTAR ISSO NA INTERRUPÃƒâ€¡ÃƒÆ’O DE TIMER
{
//      IE2 &= ~UCA0RXIE;                          // disable USCI_A0 RX interrupt
    //Verify Cycle Timeout
    if (is5msLoop)
    {
        is5msLoop = false;
        if (!isLoopCycleFinished)
        {
            Fail_LoopCycleTimeout();
            return;
        }
        isLoopCycleFinished = false;
        atualizaentradas(); //Atualizar Entradas     //****************CRIAR FUNÃ‡ÃƒO PARA AS ENTRADAS *******    OK
        //Realizar comunicaÃƒÂ§ÃƒÂ£o

//            P7OUT ^= S6;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;
//            for (aux1 = 0; aux1 < 1000; aux1++)
//                            teste[aux1/10]=aux1;

        if (isFirstLoop)
        {
            while (i_Instruction < aux_Inst)
            {
                switch(v_Instruction[i_Instruction].Func_Type)
                {
                case FUNC_COIL:
                    i_Instruction += F_Coil(&(v_Instruction[i_Instruction]), i_Instruction);
                    break;
                case FUNC_TIMER:
                    i_Instruction += F_Timer(&(v_Instruction[i_Instruction]), i_Instruction);
                    break;
                case FUNC_STEPPER:
                    i_Instruction += F_Stepper(&(v_Instruction[i_Instruction]), i_Instruction);
                    break;
                }
                i_Instruction=i_Instruction+1;
            }
            isFirstLoop = false;
        }
        i_Instruction = 0;

        while (i_Instruction < aux_Inst)
        {
            if (i_Instruction == tmp)
                tmp = i_Instruction;
            switch(v_Instruction[i_Instruction].Func_Type)
            {
            case FUNC_COIL:
                i_Instruction += F_Coil(&(v_Instruction[i_Instruction]), i_Instruction);
                break;
            case FUNC_TIMER:
                i_Instruction = i_Instruction + F_Timer(&(v_Instruction[i_Instruction]), i_Instruction);
                break;
            case FUNC_STEPPER:
                i_Instruction = i_Instruction + F_Stepper(&(v_Instruction[i_Instruction]), i_Instruction);
                break;
            }
            i_Instruction=i_Instruction+1;
        }

        if (isFrameReady == true)
            ProcessFrame();
        if (isFrameReady == false)
            //IE2 |= UCA0RXIE;
            IntEnable(INT_UART0);

        if (aux_GetConfigs != 0 && Counter_ms%50 == 0)
        {
            ProcessGetConfigs();
        }
        //   P8OUT^=BIT0;
        //        for (i = 0; i < NUMERO_DE_INSTRUCOES; i=i+1)
        //        {
        //
        //
        //            switch(v_Instruction[0].Func_Type)
        //            {
        //            case FUNC_COIL:
        ////                P8OUT^=BIT0;
        //                i += F_Coil(&(v_Instruction[0]), 0);
        ////                i += TESTE1(0, 0);
        //                break;
        //            case FUNC_TIMER:
        //                i = i + F_Timer(&v_Instruction[0], 0);
        //                break;
        //            }
        //        }
        //        P8OUT^=BIT1;
        //        F_Coil(v_Instruction[0], 0);                                              //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        isLoopCycleFinished = true;
        if (Counter_ms == 50000)
            Counter_ms = 1;
//            else
//                Counter_ms++;
    }
    else
    {
        aux_Stepper = 0;
        i_Instruction = Stepper_Instructions[aux_Stepper++];
        while (i_Instruction > 0)// >= ???
        {
            F_Stepper(&(v_Instruction[i_Instruction]), i_Instruction);
            i_Instruction = Stepper_Instructions[aux_Stepper++];
        }
//            while (i_Instruction < aux_Inst)
//            {
//                if (v_Instruction[i_Instruction].Func_Type == FUNC_STEPPER)
//                    i_Instruction = i_Instruction + F_Stepper(&(v_Instruction[i_Instruction]), i_Instruction);
//
//                i_Instruction=i_Instruction+1;
//            }
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

bool F_Coil(struct Instruction *Inst, unsigned int Index)
{
    //P8OUT^=BIT0;
    bool aux_state = 0;
    unsigned int index = v_Coil[(*Inst).Func_Index].Index;
    bool Result = GetInstructionResult(Inst);
    unsigned int i = 0;
    if (Index + 1 < aux_Inst)
        while (v_Instruction[Index+i].Has_Or_Instruction)
        {
            i++;
            Result = Result||GetInstructionResult(&v_Instruction[Index+i]);
        }

    switch(v_Coil[(*Inst).Func_Index].Type)
    {
    //Common function
    case COIL_C:
        if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Q)
        {
            Q[index] = Result;
            atualizasaidas(index, Result);
//                if (Result)
//                    P3OUT |= BIT1;            <<<<<<<<<<<<<<<<<<<<<<<************<CRIAR FUNÃ‡ÃƒO PARA RECEBER INDEX************     OK
//                else
//                    P3OUT &= ~BIT1;
        }
        else
            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_X)
            {
                X[index] = Result;
            }
            else
                if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Y)
                {
                    Y[index] = Result;
                }
                else
                    if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_M)
                    {
                        M[index] = Result;
                    }
                    else
                        if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Z)
                        {
                            z[index] = Result;
                        }
        break;

        //Pulsed function
    case COIL_P:
        if (isFirstLoop)
        {
            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Q)
            {
                Qaux[index] = Result;
            }
            else
                if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_X)
                {
                    Xaux[index] = Result;
                }
                else
                    if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Y)
                    {
                        Yaux[index] = Result;
                    }
                    else
                        if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_M)
                        {
                            Maux[index]=Result;
                        }
                        else
                            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Z)
                            {
                                zaux[index]=Result;
                            }
        }
        else
        {
            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Q && Qaux[index] == false && Result)
            {
                Q[index] = !Q[index];
                Qaux[index] = true;
                if (Q[index])

                    atualizasaidas(index, true);
                //<<<<<<<<<<<<<<<<<<<<<<<************APROVEITAR A FUNÃ‡ÃƒO ATIVARPINO ************
                else
                    atualizasaidas(index, false);
            }
            else
                if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_X && Xaux[index] == false && Result)
                {
                    X[index] = !X[index];
                    Xaux[index] = true;
                }
                else
                    if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Y && Yaux[index] == false && Result)
                    {
                        Y[index] = !Y[index];
                        Yaux[index] = true;
                    }
                    else
                        if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_M && Maux[index] == false && Result)
                        {
                            M[index] = !M[index];
                            ProcessGetPowerState();
                            Maux[index] = true;
                        }
                        else
                            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Q && Qaux[index] == true && !(Result))
                            {
                                Qaux[index] = false;
                            }
                            else
                                if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_X && Xaux[index] == true && !(Result))
                                {
                                    Xaux[index] = false;
                                }
                                else
                                    if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Y && Yaux[index] == true && !(Result))
                                    {
                                        Yaux[index] = false;
                                    }
                                    else
                                        if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_M && Maux[index] == true && !(Result))
                                        {
                                            Maux[index] = false;
                                        }
                                        else
                                            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Z && zaux[index] == true && !(Result))
                                            {
                                                zaux[index] = false;
                                            }
        }
        break;
    //Set function
    case COIL_S:
        if (Result)
        {
            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Q)
            {
                Q[index] = true;
                atualizasaidas(index, true);
                if (index == 0x01 && aux_ProductCounter)
                {
                    aux_ProductCounter = 0;
                }
            }
            else
                if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Y)
                {
                    Y[index] = true;
                }
                else
                    if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_M)
                    {
                        M[index] = true;
                    }
                    else
                        if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_X)
                        {
                            X[index] = true;
                        }
                        else
                            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Z)
                            {
                                z[index] = true;
                            }
        }
        break;
    //Reset function
    case COIL_R:
        if (Result)
        {
            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Q)
            {
                Q[index] = false;
                Qaux[index] = false;
                atualizasaidas(index, false);
                if (index == 0x01 && !aux_ProductCounter)
                {
                    ProductCounter++;
                    aux_ProductCounter = 1;
                    R[8] = ProductCounter>>8;
                    R[9] = ProductCounter&0x00FF;
                    /*copy_X2B(SEG_D, F_R+(unsigned int)(8), F_R+(unsigned int)(9),
                             R[8], R[9]);
                    copy_B2X(SEG_D);*/
                    Flash_wb(F_R+(unsigned int)(8), R[8]);
                    Flash_wb(F_R+(unsigned int)(9), R[9]);
                    ProcessGetCounter();
                }
            }
            else
                if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Y)
                {
                    Y[index] = false;
                    Yaux[index] = false;
                }
                else
                    if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_M)
                    {
                        M[index] = false;
                        Maux[index] = false;
                        if (index == 0x0b)
                            ProcessGetPowerState();
                    }
                    else
                        if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_X)
                        {
                            X[index] = false;
                            Xaux[index] = false;
                        }
                        else
                            if (v_Coil[(*Inst).Func_Index].Vector == CONTACT_VECTOR_Z)
                            {
                                z[index] = false;
                            }
        }
        break;

    }

    return i;
}


bool F_Timer(struct Instruction *Inst, unsigned int Index_Inst)
{

//        unsigned int Index;
    unsigned char Mode;
    unsigned char Base;
    unsigned long long int SetValue;
    unsigned long long int ReadValue;
    unsigned char Type;
    unsigned char State;
    unsigned char Contact_Reset;
    unsigned char Contact_Reset_Index;
    unsigned long long int Timer_Count;
    unsigned long long int Timer_Target;


    unsigned char index = v_Timer[(*Inst).Func_Index].Index;
//        if (index == tmp_index)
//            index == index;
    bool Result = GetInstructionResult(Inst);
    unsigned char i = 0;
    while (v_Instruction[Index_Inst+i].Has_Or_Instruction)
    {
        i++;
        Result |= GetInstructionResult(&v_Instruction[Index_Inst+i]);
    }


     switch(v_Timer[(*Inst).Func_Index].Mode)
     {

     case TIMER_MODE_T0:

         break;
     //Delay on Energization
     case TIMER_MODE_T1:
         if (Result)
         {
             v_Timer[index].Enabled = true;
             if (v_Timer[index].Timer_Target == 0)
             {
                 v_Timer[index].Enabled = true;
                 v_Timer[index].Timer_Target = Counter_ms + v_Timer[index].Timer_Count;
                 if (v_Timer[index].Timer_Target > 50000)
                     v_Timer[index].Timer_Target -= 50000;
                 v_Timer[index].ReadValue = 0;

             }
             else
//                     if (v_Timer[index].Timer_Target > Counter_ms)
                 if (v_Timer[index].Timer_Target > (v_Timer[index].Timer_Count > v_Timer[index].Timer_Target ? (Counter_ms > 10000 ? 0 : Counter_ms) : Counter_ms))
                 {


                     v_Timer[index].ReadValue++;
                 }
                 else
                 {
                     v_Timer[index].State = true;
                     T[index] = true;
                 }
         }
         else
         {

             v_Timer[index].Enabled = false;
             v_Timer[index].State = false;
             T[index] = false;
             v_Timer[index].Timer_Target = 0;
         }

         break;

     case TIMER_MODE_T2:

         break;

     case TIMER_MODE_T3:
         if (v_Timer[index].State == false && v_Timer[index].Timer_Target == 0 && Result)
         {
             if (index == 0x0D)
                 index == index;
             T[index] = true;
             v_Timer[index].State = true;
         }
         else
             if (v_Timer[index].State == true && v_Timer[index].Timer_Target == 0 && !(Result))
             {
                 v_Timer[index].Enabled = true;
                 v_Timer[index].Timer_Target = Counter_ms + v_Timer[index].Timer_Count;
                 if (v_Timer[index].Timer_Target > 50000)
                     v_Timer[index].Timer_Target -= 50000;
                 v_Timer[index].State = true;
                 T[index] = true;

             }
             else
                 if ((v_Timer[index].Timer_Target > (v_Timer[index].Timer_Count > v_Timer[index].Timer_Target ? (Counter_ms > 10000 ? 0 : Counter_ms) : Counter_ms)) && !(Result) && v_Timer[index].State == true)
                 {

                     v_Timer[index].ReadValue++;
                 }
                 else
                     if (v_Timer[index].Timer_Target <= Counter_ms && !(Result) && v_Timer[index].State == true)
                     {
                         T[index] = false;
                         v_Timer[index].State = false;
                         v_Timer[index].Timer_Target = 0;
                         v_Timer[index].ReadValue = 0;
                     }
         break;
     //Delay on De-energization
     case TIMER_MODE_T4:



         if (v_Timer[index].Enabled == false && v_Timer[index].Timer_Target == 0 && Result)
         {

             v_Timer[index].Enabled = true;
         }
         else
             if (v_Timer[index].Enabled == true && v_Timer[index].Timer_Target == 0 && !(Result))
             {
                 v_Timer[index].Enabled = false;
                 v_Timer[index].Timer_Target = Counter_ms + v_Timer[index].Timer_Count;
                 if (v_Timer[index].Timer_Target > 50000)
                     v_Timer[index].Timer_Target -= 50000;
                 v_Timer[index].State = true;
                 T[index] = true;

             }
             else
                 if ((v_Timer[index].Timer_Target > (v_Timer[index].Timer_Count > v_Timer[index].Timer_Target ? (Counter_ms > 10000 ? 0 : Counter_ms) : Counter_ms)) && !(Result))
                 {

                     v_Timer[index].ReadValue++;
                 }
                 else
                 {

                     v_Timer[index].State = false;
                     T[index] = false;
                     v_Timer[index].Timer_Target = 0;
                     v_Timer[index].ReadValue = 0;
                 }
         break;

     case TIMER_MODE_T5:

         break;

     case TIMER_MODE_T6:
         if (Result)
         {
             v_Timer[index].Enabled = true;
             if (v_Timer[index].Timer_Target == 0)
             {
                 v_Timer[index].Enabled = true;
                 v_Timer[index].Timer_Target = Counter_ms + v_Timer[index].Timer_Count;
                 if (v_Timer[index].Timer_Target > 50000)
                     v_Timer[index].Timer_Target -= 50000;
                 v_Timer[index].ReadValue = 0;

                 v_Timer[index].State = true;
                 T[index] = true;

             }
             else
                 if (v_Timer[index].Timer_Target > (v_Timer[index].Timer_Count > v_Timer[index].Timer_Target ? (Counter_ms > 10000 ? 0 : Counter_ms) : Counter_ms))
                 {
                     v_Timer[index].ReadValue++;
                 }
                 else
                 {
                     v_Timer[index].Timer_Target = Counter_ms + v_Timer[index].Timer_Count;
                     if (v_Timer[index].Timer_Target > 50000)
                         v_Timer[index].Timer_Target -= 50000;
                     v_Timer[index].ReadValue = 0;

                     v_Timer[index].State = (v_Timer[index].State ? false : true);
                     T[index] = v_Timer[index].State;
                 }
         }
         else
         {

             v_Timer[index].Enabled = false;
             v_Timer[index].State = false;
             T[index] = false;
             v_Timer[index].Timer_Target = 0;
         }
         break;

     }
     return i;
}

bool F_Stepper(struct Instruction *Inst, unsigned int Index_Inst)
{
    unsigned int index = v_Stepper[(*Inst).Func_Index].Index;
    if (index == tmp_index)
        index == index;
    bool Result = GetInstructionResult(Inst);
    unsigned int i = 0;
    while (v_Instruction[Index_Inst+i].Has_Or_Instruction)
    {
        i++;
        Result |= GetInstructionResult(&v_Instruction[Index_Inst+i]);
    }


    switch(v_Stepper[(*Inst).Func_Index].Stage)//(v_Stepper[index].Stage) ???
    {

    case STEPPER_OFF:
        if (Result)
        {
            v_Stepper[(*Inst).Func_Index].Stage = STEPPER_ACC;
            v_Stepper[(*Inst).Func_Index].ms_wait = 40;
            v_Stepper[(*Inst).Func_Index].aux_ms_wait = 0;
            v_Stepper[(*Inst).Func_Index].aux_inc = 0;
            v_Stepper[(*Inst).Func_Index].Timer_Acc_Target = v_Stepper[(*Inst).Func_Index].Timer_Acc_Count + Counter_ms;
            v_Stepper[(*Inst).Func_Index].Timer_Total_Target = v_Stepper[(*Inst).Func_Index].Timer_Total_Count + Counter_ms;
            v_Stepper[(*Inst).Func_Index].inc = v_Stepper[(*Inst).Func_Index].Timer_Acc_Count/(40-2400/v_Stepper[(*Inst).Func_Index].Speed);
        }
        break;
    case STEPPER_ACC:
        if (Result)
        {
            if (v_Stepper[(*Inst).Func_Index].Timer_Acc_Target > Counter_ms)
            {
                if (++(v_Stepper[(*Inst).Func_Index].aux_ms_wait) >= v_Stepper[(*Inst).Func_Index].ms_wait)
                {

                    Q[v_Stepper[(*Inst).Func_Index].Vector_Index] = (Q[v_Stepper[(*Inst).Func_Index].Vector_Index]== true ? false : true);
                    atualizasaidas(v_Stepper[(*Inst).Func_Index].Vector_Index, Q[v_Stepper[(*Inst).Func_Index].Vector_Index]);

                    if (++(v_Stepper[(*Inst).Func_Index].aux_inc) <= v_Stepper[(*Inst).Func_Index].inc)
                    {
                        v_Stepper[(*Inst).Func_Index].ms_wait -= (v_Stepper[(*Inst).Func_Index].ms_wait >= 4 ? 1 : 0);
                        v_Stepper[(*Inst).Func_Index].aux_inc = 0;
                    }
                    v_Stepper[(*Inst).Func_Index].aux_ms_wait = 0;
                }
            }
            else
            {
                v_Stepper[(*Inst).Func_Index].Stage = STEPPER_CONST;
                v_Stepper[(*Inst).Func_Index].ms_wait = 2400/v_Stepper[(*Inst).Func_Index].Speed;
            }
        }
        else
        {
            v_Stepper[(*Inst).Func_Index].Stage = STEPPER_OFF;
        }
        break;
    case STEPPER_CONST:
        if (Result)
        {
//                if (v_Stepper[(*Inst).Func_Index].Timer_Total_Target == 0)
//                {
//                    if (++(v_Stepper[(*Inst).Func_Index].aux_ms_wait) >= v_Stepper[(*Inst).Func_Index].ms_wait)
//                    {
//                        Q[v_Stepper[(*Inst).Func_Index].Vector_Index] = (Q[v_Stepper[(*Inst).Func_Index].Vector_Index]== true ? false : true);
//                        atualizasaidas(v_Stepper[(*Inst).Func_Index].Vector_Index, Q[v_Stepper[(*Inst).Func_Index].Vector_Index]);
//
//                        v_Stepper[(*Inst).Func_Index].aux_ms_wait = 0;
//                    }
//                }
//                else
            if (v_Stepper[(*Inst).Func_Index].Timer_Total_Target > Counter_ms)
            {
                if (++(v_Stepper[(*Inst).Func_Index].aux_ms_wait) >= v_Stepper[(*Inst).Func_Index].ms_wait)
                {
                    Q[v_Stepper[(*Inst).Func_Index].Vector_Index] = (Q[v_Stepper[(*Inst).Func_Index].Vector_Index]== true ? false : true);
                    atualizasaidas(v_Stepper[(*Inst).Func_Index].Vector_Index, Q[v_Stepper[(*Inst).Func_Index].Vector_Index]);

                    v_Stepper[(*Inst).Func_Index].aux_ms_wait = 0;
                }
            }
        }
        else
        {
            v_Stepper[(*Inst).Func_Index].Stage = STEPPER_DEC;
        }
        break;
    case STEPPER_DEC:
        Q[v_Stepper[(*Inst).Func_Index].Vector_Index] = 0;
        atualizasaidas(v_Stepper[(*Inst).Func_Index].Vector_Index, false);
        v_Stepper[(*Inst).Func_Index].Stage = STEPPER_OFF;
        break;
    default:
        break;

    }
    return i;
}

void SetContact(unsigned int Index, unsigned char Vector, unsigned char VectorIndex, unsigned char isNO)
{
    v_Contact[Index].Vector = Vector;//CONTACT_VECTOR_I
    v_Contact[Index].Index = VectorIndex;
    v_Contact[Index].isNO = isNO;
    v_Contact[Index].State = false;
}

void SetCoil(unsigned int Index, unsigned char Vector, unsigned char Type, unsigned char VectorIndex)
{
    v_Coil[Index].Vector = Vector;//CONTACT_VECTOR_M
    v_Coil[Index].Type = Type;//COIL_P
    v_Coil[Index].Index = VectorIndex;
}

void SetTimer(unsigned int Index, unsigned char Mode, unsigned char Base, unsigned int SetValue, unsigned char Type, unsigned char ResetVector, unsigned char ResetIndex)
{
    v_Timer[Index].Index = Index;
    v_Timer[Index].Mode = Mode;//TIMER_MODE_T4
    v_Timer[Index].Base = Base;//TIMER_BASE::MS1
    v_Timer[Index].SetValue = SetValue;
    v_Timer[Index].ReadValue = 0;
    v_Timer[Index].Type = Type;
    v_Timer[Index].State = false;
    v_Timer[Index].Contact_Reset = ResetVector;//CONTACT_VECTOR_ZERO
    v_Timer[Index].Contact_Reset_Index = ResetIndex;
    v_Timer[Index].Timer_Count = (Base == TIMER_BASE_MS1 ? SetValue : Base == TIMER_BASE_MS10 ? 10*SetValue : Base == TIMER_BASE_MS100 ? 100*SetValue : Base == TIMER_BASE_SEC ? 1000*SetValue : Base == TIMER_BASE_MIN ? 600*SetValue : 0);
    v_Timer[Index].Timer_Count /= 5;
    v_Timer[Index].Timer_Target = 0;
    v_Timer[Index].Enabled = false;
}

void SetStepper(unsigned int Index, unsigned char Mode, unsigned char Base,
                    unsigned int Speed, unsigned int SetAccValue, unsigned int SetTotalValue,
                    unsigned int SetDecValue, unsigned char Vector, unsigned char Vector_Index)
{
    v_Stepper[Index].Index = Index;
    v_Stepper[Index].Mode = Mode;//TIMER_MODE_T4
    v_Stepper[Index].Base = Base;//TIMER_BASE::MS1
    v_Stepper[Index].Speed = Speed;
    v_Stepper[Index].ms_wait = 2400/Speed;
    v_Stepper[Index].SetAccValue = SetAccValue;
    v_Stepper[Index].SetTotalValue = SetTotalValue;
    v_Stepper[Index].SetDecValue = SetDecValue;
    v_Stepper[Index].ReadValue = 0;
    v_Stepper[Index].Stage = STEPPER_OFF;
    v_Stepper[Index].State = false;
    v_Stepper[Index].Timer_Acc_Count = (Base == TIMER_BASE_MS1 ? SetAccValue : Base == TIMER_BASE_MS10 ? 10*SetAccValue : Base == TIMER_BASE_MS100 ? 100*SetAccValue : Base == TIMER_BASE_SEC ? 1000*SetAccValue : Base == TIMER_BASE_MIN ? 600*SetAccValue : 0);
    v_Stepper[Index].Timer_Acc_Count /= 5;
    v_Stepper[Index].Timer_Acc_Target = 0;
    v_Stepper[Index].Timer_Total_Count = (Base == TIMER_BASE_MS1 ? SetTotalValue : Base == TIMER_BASE_MS10 ? 10*SetTotalValue : Base == TIMER_BASE_MS100 ? 100*SetTotalValue : Base == TIMER_BASE_SEC ? 1000*SetTotalValue : Base == TIMER_BASE_MIN ? 600*SetTotalValue : 0);
    v_Stepper[Index].Timer_Total_Count /= 5;
    v_Stepper[Index].Timer_Total_Target = 0;
    v_Stepper[Index].Timer_Dec_Count = (Base == TIMER_BASE_MS1 ? SetDecValue : Base == TIMER_BASE_MS10 ? 10*SetDecValue : Base == TIMER_BASE_MS100 ? 100*SetDecValue : Base == TIMER_BASE_SEC ? 1000*SetDecValue : Base == TIMER_BASE_MIN ? 600*SetDecValue : 0);
    v_Stepper[Index].Timer_Dec_Count /= 5;
    v_Stepper[Index].Timer_Dec_Target = 0;
    v_Stepper[Index].Vector = Vector;
    v_Stepper[Index].Vector_Index = Vector_Index;

}

void SetInstruction(unsigned int Index, unsigned char Func_Type, unsigned int Func_Index, bool HasOrInstruction, struct Contact Contact_1, struct Contact Contact_2, struct Contact Contact_3)
{
    v_Instruction[Index].Func_Type = Func_Type;
    v_Instruction[Index].Func_Index = Func_Index;
    v_Instruction[Index].Inst_Contact_1 = Contact_1;
    v_Instruction[Index].Inst_Contact_2 = Contact_2;
    v_Instruction[Index].Inst_Contact_3 = Contact_3;
    v_Instruction[Index].Has_Or_Instruction = HasOrInstruction;

}

void SetNewCoilInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                           unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                           unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                           unsigned char Vector_Coil, unsigned char VectorIndex_Coil, unsigned char Type_Coil,
                           bool HasOrInstruction)
{
    SetContact(aux_Contact++, Vector_1, VectorIndex_1, isNO_1);
    SetContact(aux_Contact++, Vector_2, VectorIndex_2, isNO_2);
    SetContact(aux_Contact++, Vector_3, VectorIndex_3, isNO_3);
    if (!(aux_Inst == 0 ? 0 : v_Instruction[aux_Inst-1].Has_Or_Instruction))
        SetCoil(aux_Coil++, Vector_Coil, Type_Coil, VectorIndex_Coil);

    SetInstruction(aux_Inst++, FUNC_COIL, aux_Coil-1, HasOrInstruction, v_Contact[aux_Contact-3], v_Contact[aux_Contact-2], v_Contact[aux_Contact-1]);
}

void SetNewTimerInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                                   unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                                   unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                                   unsigned char Timer_Index, unsigned char Mode, unsigned char Base,
                                   unsigned int SetValue, unsigned char Type, unsigned char ResetVector,
                                   unsigned char ResetIndex, bool HasOrInstruction)
{
    SetContact(aux_Contact++, Vector_1, VectorIndex_1, isNO_1);
    SetContact(aux_Contact++, Vector_2, VectorIndex_2, isNO_2);
    SetContact(aux_Contact++, Vector_3, VectorIndex_3, isNO_3);
    if (!(aux_Inst == 0 ? 0 : v_Instruction[aux_Inst-1].Has_Or_Instruction))
        SetTimer(Timer_Index, Mode, Base, SetValue, Type, ResetVector, ResetIndex);

    SetInstruction(aux_Inst++, FUNC_TIMER, Timer_Index, HasOrInstruction, v_Contact[aux_Contact-3], v_Contact[aux_Contact-2], v_Contact[aux_Contact-1]);

}

void SetNewStepperInstruction(unsigned char Vector_1, unsigned char VectorIndex_1, unsigned char isNO_1,
                                  unsigned char Vector_2, unsigned char VectorIndex_2, unsigned char isNO_2,
                                  unsigned char Vector_3, unsigned char VectorIndex_3, unsigned char isNO_3,
                                  unsigned char Stepper_Index, unsigned char Base, unsigned int Speed,
                                  unsigned int SetAccValue, unsigned int SetTotalValue, unsigned int SetDecValue,
                                  unsigned char Vector, unsigned char Vector_Index, bool HasOrInstruction)
{
    SetContact(aux_Contact++, Vector_1, VectorIndex_1, isNO_1);
    SetContact(aux_Contact++, Vector_2, VectorIndex_2, isNO_2);
    SetContact(aux_Contact++, Vector_3, VectorIndex_3, isNO_3);
    if (!(aux_Inst == 0 ? 0 : v_Instruction[aux_Inst-1].Has_Or_Instruction))
        SetStepper(Stepper_Index, 0, Base, Speed, SetAccValue, SetTotalValue, SetDecValue, Vector, Vector_Index);

    SetInstruction(aux_Inst++, FUNC_STEPPER, Stepper_Index, HasOrInstruction, v_Contact[aux_Contact-3], v_Contact[aux_Contact-2], v_Contact[aux_Contact-1]);
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


void ProcessWriteSingleRegister()
{
    aux_value = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL];
    switch (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AH]&0xF0)
    {
    case VECTOR_I:
        /*copy_X2B(SEG_D, F_I+(unsigned int)((aux_value&0x18)>>3), 0x00,
                 (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_I+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_I+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))), 0x00);
        copy_B2X(SEG_D);*/
        Flash_rw(F_I+(unsigned int)((aux_value&0x18)>>3), (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_I+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_I+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))));
        I[aux_value] = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        break;
    case VECTOR_X:
        /*copy_X2B(SEG_D, F_X+(unsigned int)((aux_value&0x18)>>3), 0x00,
                 (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_X+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_X+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))), 0x00);
        copy_B2X(SEG_D);*/
        Flash_rw(F_X+(unsigned int)((aux_value&0x18)>>3), (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_X+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_X+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))));
        X[aux_value] = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        break;
    case VECTOR_Y:
        /*copy_X2B(SEG_D, F_Y+(unsigned int)((aux_value&0x18)>>3), 0x00,
                 (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_Y+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_Y+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))), 0x00);
        copy_B2X(SEG_D);*/
        Flash_rw(F_Y+(unsigned int)((aux_value&0x18)>>3), (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_Y+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_Y+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))));
        Y[aux_value] = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        if (aux_value == 0x10)
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
            FrameTx[12] = (Y[aux_value] ? 100 : 101);
            FrameTx[13] = checksum(FrameTx, 14);

            sendTx(14);
        }

        if (aux_value == 0x12)
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
            FrameTx[12] = (Y[aux_value] ? 100 : 101);
            FrameTx[13] = checksum(FrameTx, 14);

            sendTx(14);
        }
        break;
    case VECTOR_Z:
        /*copy_X2B(SEG_D, F_z+(unsigned int)((aux_value&0x18)>>3), 0x00,
                 (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_z+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_z+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))), 0x00);
        copy_B2X(SEG_D);*/
        Flash_rw(F_z+(unsigned int)((aux_value&0x18)>>3), (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_z+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_z+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))));
        z[aux_value] = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        break;
    case VECTOR_Q:
        /*copy_X2B(SEG_D, F_Q+(unsigned int)((aux_value&0x18)>>3), 0x00,
                 (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_Q+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_Q+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))), 0x00);
        copy_B2X(SEG_D);*/
        Flash_rw(F_Q+(unsigned int)((aux_value&0x18)>>3), (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_Q+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_Q+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))));
        Q[aux_value] = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        atualizasaidas(aux_value, FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL]);
        break;
    case VECTOR_M:
        /*copy_X2B(SEG_D, F_M+(unsigned int)((aux_value&0x18)>>3), 0x00,
                 (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_M+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_M+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))), 0x00);
        copy_B2X(SEG_D);*/
        Flash_rw(F_M+(unsigned int)((aux_value&0x18)>>3), (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_M+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_M+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))));
        M[aux_value] = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        break;
    case VECTOR_T:
        /*copy_X2B(SEG_D, F_T+(unsigned int)((aux_value&0x18)>>3), 0x00,
                 (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_T+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_T+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))), 0x00);
        copy_B2X(SEG_D);*/
        Flash_rw(F_T+(unsigned int)((aux_value&0x18)>>3), (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL] ?
                         *(F_T+(unsigned int)((aux_value&0x18)>>3))|(0x01<<(aux_value&0x07)) :
                         *(F_T+(unsigned int)((aux_value&0x18)>>3))&~(0x01<<(aux_value&0x07))));
        T[aux_value] = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        break;
    case VECTOR_R:
        if (aux_value == 0x00)
        {
            X[0x12] = 1;
            aux_value2 = (R[4]<<8)|(R[5]);
            aux_value2++;
            R[4] = aux_value2>>8;
            R[5] = aux_value2&0x00ff;
            ProcessGetSpeed();

            v_Timer[2].SetValue = (4200-105*R[7])*(float)(20)/(float)aux_value2;
            v_Timer[2].Timer_Count = v_Timer[2].SetValue/5;
            aux_value3 = v_Timer[2].SetValue;
            /*copy_X2B(SEG_C, F_TVALUE+(unsigned int)((2-1)*2), F_TVALUE+(unsigned int)((2-1)*2+1),
                             (aux_value3>>8), (aux_value3&0x00FF));
            copy_B2X(SEG_C);*/
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2), (aux_value3>>8));
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2+1), (aux_value3&0x00FF));
        }
        if (aux_value == 0x01)
        {
            X[0x13] = 1;
            aux_value2 = ((R[4]<<8)|R[5])-1;
            R[4] = aux_value2>>8;
            R[5] = aux_value2&0x00ff;
            ProcessGetSpeed();

            v_Timer[2].SetValue = (4200-105*R[7])*(float)(20)/(float)aux_value2;
            v_Timer[2].Timer_Count = v_Timer[2].SetValue/5;
            /*copy_X2B(SEG_C, F_TVALUE+(unsigned int)((2-1)*2), F_TVALUE+(unsigned int)((2-1)*2+1),
                     (aux_value3>>8), (aux_value3&0x00FF));
            copy_B2X(SEG_C);*/
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2), (aux_value3>>8));
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2+1), (aux_value3&0x00FF));
        }
        if (aux_value == 0x02)
        {
            aux_value2 = ((R[6]<<8)|R[7])+1;
            v_Timer[2].SetValue = (4200-105*aux_value2)*((float)20/(float)R[5]);
            v_Timer[2].Timer_Count = v_Timer[2].SetValue/5;

            /*copy_X2B(SEG_C, F_TVALUE+(unsigned int)((2-1)*2), F_TVALUE+(unsigned int)((2-1)*2+1),
                     (aux_value3>>8), (aux_value3&0x00FF));
            copy_B2X(SEG_C);*/
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2), (aux_value3>>8));
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2+1), (aux_value3&0x00FF));

            R[6] = aux_value2>>8;
            R[7] = aux_value2&0x00ff;
            ProcessGetLength();
        }
        if (aux_value == 0x03)
        {
            aux_value2 = ((R[6]<<8)|R[7])-1;
            v_Timer[2].SetValue = (4200-105*aux_value2)*((float)20/(float)R[5]);
            v_Timer[2].Timer_Count = v_Timer[2].SetValue/5;

            /*copy_X2B(SEG_C, F_TVALUE+(unsigned int)((2-1)*2), F_TVALUE+(unsigned int)((2-1)*2+1),
                     (aux_value3>>8), (aux_value3&0x00FF));
            copy_B2X(SEG_C);*/
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2), (aux_value3>>8));
            Flash_rw(F_TVALUE+(unsigned int)((2-1)*2+1), (aux_value3&0x00FF));

            R[6] = aux_value2>>8;
            R[7] = aux_value2&0x00ff;
            ProcessGetLength();
        }
        if (aux_value == 0x08)
        {
            /*copy_X2B(SEG_D, F_R+(unsigned int)(8),F_R+(unsigned int)(9),
                                 0x00, 0x00);*/
            Flash_rw(F_R+(unsigned int)(8), 0x00);
            Flash_rw(F_R+(unsigned int)(9), 0x00);
            R[8]=0;
            R[9]=0;
            ProductCounter = 0;
            /*copy_X2B(SEG_D, F_R+(unsigned int)(8), F_R+(unsigned int)(9),
                     R[8], R[9]);
            copy_B2X(SEG_D);*/
            Flash_rw(F_R+(unsigned int)(8), R[8]);
            Flash_rw(F_R+(unsigned int)(9), R[9]);
            ProcessGetCounter();
        }
        if (aux_value == 0x00 || aux_value == 0x01)
        {
            /*copy_X2B(SEG_D, F_R+(unsigned int)(4),F_R+(unsigned int)(5),
                     R[4], R[5]);*/
            Flash_rw(F_R+(unsigned int)(4), R[4]);
            Flash_rw(F_R+(unsigned int)(5), R[5]);
        }
        if (aux_value == 0x02 || aux_value == 0x03)
        {
            /*copy_X2B(SEG_D, F_R+(unsigned int)(6),F_R+(unsigned int)(7),
                     R[6], R[7]);*/
            Flash_rw(F_R+(unsigned int)(6), R[6]);
            Flash_rw(F_R+(unsigned int)(7), R[7]);
        }

        //copy_B2X(SEG_D);
//        if (aux_value <= 0x01)
//            ProcessGetSpeed();
        break;
    case VECTOR_TIMER:

        aux1 = F_TVALUE+(unsigned int)((aux_value-1)*2);
        aux2 = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VH];
        aux3 = F_TVALUE+(unsigned int)((aux_value-1)*2+1);
        aux4 = FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];
        /*copy_X2B(SEG_C, F_TVALUE+(unsigned int)((aux_value-1)*2), F_TVALUE+(unsigned int)((aux_value-1)*2+1),
                 FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VH], FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL]);
        copy_B2X(SEG_C);*/
        Flash_rw(F_TVALUE+(unsigned int)((aux_value-1)*2), FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VH]);
        Flash_rw(F_TVALUE+(unsigned int)((aux_value-1)*2+1), FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL]);

//        aux_value = (FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VH]<<8)|FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_VL];

        aux_value = (*(F_TVALUE+(unsigned int)((aux_value-1)*2))<<8)|*(F_TVALUE+(unsigned int)((aux_value-1)*2+1));
        v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].SetValue = aux_value;
        v_Timer[FrameRx[FRAME_CMD_2+REQ_WRITE_SINGLE_REGISTER_AL]].Timer_Count = aux_value/5;
//      ProcessWriteSingleRegisterTimer();
        break;
    }
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

/*void copy_X2B(uint16_t Segment, unsigned char *Data_ptr1, unsigned char *Data_ptr2, char byte1, char byte2)
{
    uint32_t end1 = (uint32_t) Data_ptr1;
    uint32_t end2 = (uint32_t) Data_ptr2;
    Flash_rw(end1, byte1);
    SysCtlDelay(5);
    Flash_rw(end2, byte2);
}*/

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
    
        
    //CONFIGURAÇÃO DE TESTE (J,K,L -> ENTRADA; M, N, P -> SAÍDAS)
    PDIR(PM, S1);
    PDIR(PN, (S2|S3|S4|S5|S6|S7|S8));
    PDIR(PP, (S9|S10));
    POUT(PM, S1, 1);
    POUT(PN, (S2|S3|S4|S5|S6|S7|S8), 1);
    POUT(PP, (S9|S10), 1);


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

    if (RoutineStarted == true)
        while(1)
        {
            if (last_count_us < count_us)
            {
                if (is5msLoop){
                    last_count_ms = count_ms;
//                    OperationCycle();
                }
                last_count_us = count_us;
                OperationCycle();
                if (index_tempos < 100 && M[1] == 1)
                    tempos[index_tempos++] = count_us;
            }
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

void CodigoTeste()
{
//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0F, false,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_Q, 0x09, COIL_C, 0);
//    SetNewTimerInstruction(CONTACT_VECTOR_ZERO, 0x00, true,
//                           CONTACT_VECTOR_ZERO, 0x00, true,
//                           CONTACT_VECTOR_ZERO, 0x00, true,
//                           0x0B, TIMER_MODE_T6, TIMER_BASE_MS1, 50, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
//    //Linha 51-30
//    SetNewTimerInstruction(CONTACT_VECTOR_I, 0x01, true,
//                           CONTACT_VECTOR_ZERO, 0x00, true,//CONTACT_VECTOR_Q, 0x05, true,
//                           CONTACT_VECTOR_ZERO, 0x00, true,
//                           0x01, TIMER_MODE_T1, TIMER_BASE_MS1, 100, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
//
//    SetNewStepperInstruction(CONTACT_VECTOR_T, 0x01, true,
//                             CONTACT_VECTOR_ZERO, 0x00, true,
//                             CONTACT_VECTOR_ZERO, 0x00, true,
//                             0x01, TIMER_BASE_MS1, 800,
//                             50, 1000, 10,
//                             CONTACT_VECTOR_Q, 0x06, 0);//adicionar proteção emergência aqui???
////    SetNewCoilInstruction(CONTACT_VECTOR_T, 0x0B, true,
////                          CONTACT_VECTOR_ZERO, 0x00, true,
////                          CONTACT_VECTOR_ZERO, 0x00, true,
////                          CONTACT_VECTOR_Q, 0x06, COIL_C, 0);
//
//    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, false,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_Q, 0x05, COIL_C, 0);

    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x01, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x01, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x02, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x03, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x04, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x05, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x06, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x06, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x07, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x07, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x08, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x08, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x09, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x09, COIL_C, 0);
    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x0A, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_ZERO, 0x00, true,
                          CONTACT_VECTOR_Q, 0x0A, COIL_C, 0);

}

void CodigoLadder()
{

//Habilita operação da máquina
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, true,//se a emergencia não estiver ativa
//                          CONTACT_VECTOR_M, 0x0A, false,//o habilita geral estiver ativo (invertido)
                      CONTACT_VECTOR_I, 0x06, true,//o contator estiver fechado
                      CONTACT_VECTOR_X, 0x05, false,//e não tiver parado por falha de sensor
                      CONTACT_VECTOR_M, 0x0B, COIL_C, 0);//cria a flag M0B para habilitar a operação geral da máquina

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0B, true,//se todas as condições iniciais para operação estiverem ok
                      CONTACT_VECTOR_Y, 0x10, true,//o habilita sensor de garrafão estiver ativo ??? era true
                      CONTACT_VECTOR_I, 0x01, false,//e o sensor do garrafão identificar um objeto
                      CONTACT_VECTOR_M, 0x0D, COIL_C, 0);//cria uma flag auxiliar M0D para espelhar a operação da entrada I01 no caso válido

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0D, false,//se o sensor do garrafão no caso válido estiver sem objetos
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x06, COIL_S, 0);//habilita a leitura de 1 objeto para o acionamento do lacre

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0D, false,//se o sensor do garrafão no caso válido estiver sem objetos
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x07, COIL_S, 0);//habilita a leitura de 1 objeto para o acionamento do selo

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0D, false,//se o sensor do garrafão no caso válido estiver sem objetos
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x08, COIL_S, 0);//habilita a leitura de 1 objeto para o acionamento da faca



SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0D, true,//caso o sensor do garrafão no caso válido indentificar algum objeto
                      CONTACT_VECTOR_M, 0x06, true,//estiver habilitada a leitura de 1 objeto para acionamento do lacre
                      CONTACT_VECTOR_M, 0x1C, false,//e a operação da faca estiver inativa
                      CONTACT_VECTOR_M, 0x1A, COIL_S, 0);//cria flag para indicar a permissão para acionamento de 1 ciclo do lacre


SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0D, true,//caso o sensor do garrafão no caso válido indentificar algum objeto
                      CONTACT_VECTOR_M, 0x07, true,//e estiver habilitada a leitura de 1 objeto para acionamento do selo
                      CONTACT_VECTOR_M, 0x1F, false,//e a operação da faca estiver inativa
                      CONTACT_VECTOR_M, 0x1B, COIL_S, 0);//cria flag para indicar a permissão para acionamento de 1 ciclo do selo

SetNewCoilInstruction(CONTACT_VECTOR_Y, 0x12, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1B, COIL_R, 0);//cria flag para indicar a permissão para acionamento de 1 ciclo do selo

//
//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x09, true,
//                          CONTACT_VECTOR_M, 0x1F, false,//não lembro o que é ???
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_M, 0x1B, COIL_S, 0);//cria flag para indicar a permissão para acionamento de 1 ciclo do selo

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0A, false,//se a emergencia não estiver ativa
                      CONTACT_VECTOR_I, 0x06, true,//o contator estiver fechado
                      CONTACT_VECTOR_ZERO, 0x00, true,//e não tiver parado por falha de sensor
                      CONTACT_VECTOR_M, 0x03, COIL_S, 0);//cria a flag M0B para habilitar a operação geral da máquina

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0A, false,//se a emergencia não estiver ativa
                      CONTACT_VECTOR_I, 0x06, true,//o contator estiver fechado
                      CONTACT_VECTOR_ZERO, 0x00, true,//e não tiver parado por falha de sensor
                      CONTACT_VECTOR_M, 0x0A, COIL_S, 0);//cria a flag M0B para habilitar a operação geral da máquina




//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0D, true,//caso o sensor do garrafão no caso válido indentificar algum objeto
//                          CONTACT_VECTOR_M, 0x08, true,//estiver habilitada a leitura de 1 objeto para acionamento da faca
//                          CONTACT_VECTOR_M, 0x1A, false,//e a operação do lacre estiver inativo
//                          CONTACT_VECTOR_M, 0x1C, COIL_S, 0);//cria flag para indicar a permissão para acionamento de 1 ciclo da faca


//
//    //Habilita início de operação do lacre, se a faca estiver parada
//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0E, true,//se existir permissão para acionamento de 1 ciclo
//                          CONTACT_VECTOR_M, 0x1C, false,//a operação da faca estiver inativa
//                          CONTACT_VECTOR_M, 0x1A, false,//e a operação do lacre estiver inativo
//                          CONTACT_VECTOR_M, 0x1A, COIL_P, 0);//habilita a operação do lacre

//    //Habilita início de operação da faca, se o lacre estiver parado
//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0E, true,//se existir permissão para acionamento de 1 ciclo
//                          CONTACT_VECTOR_M, 0x1A, false,//a operação do lacre estiver inativo
//                          CONTACT_VECTOR_M, 0x1C, false,//e a operação da faca estiver inativa
//                          CONTACT_VECTOR_M, 0x1C, COIL_P, 0)//habilita a operação da faca



SetNewCoilInstruction(CONTACT_VECTOR_M, 0x1A, true,
                      CONTACT_VECTOR_M, 0x01, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,//habilitador de lacre //!!! modifiquei de false para true !!! Modifiquei de  Y01 para ZERO;
                      CONTACT_VECTOR_M, 0x01, COIL_P, 0);

SetNewTimerInstruction(CONTACT_VECTOR_M, 0x01, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,//CONTACT_VECTOR_Q, 0x05, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x01, TIMER_MODE_T1, TIMER_BASE_MS1, 100, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x01, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x01, COIL_R, 0);//adicionar proteção emergência aqui???
Stepper_Instructions[aux_Stepper_Inst++] = aux_Inst;
SetNewStepperInstruction(CONTACT_VECTOR_T, 0x01, true,
                         CONTACT_VECTOR_ZERO, 0x00, true,
                         CONTACT_VECTOR_ZERO, 0x00, true,
                         0x01, TIMER_BASE_MS1, 252,
                         50, 1100, 10,
                         CONTACT_VECTOR_Q, 0x02, 0);//adicionar proteção emergência aqui???
SetNewTimerInstruction(CONTACT_VECTOR_T, 0x01, true,
                       CONTACT_VECTOR_Q, 0x01, false,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x04, TIMER_MODE_T1, TIMER_BASE_MS1, 1100, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewTimerInstruction(CONTACT_VECTOR_T, 0x01, true,//Tempo mínimo de atuação
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x07, TIMER_MODE_T1, TIMER_BASE_MS1, 200, 0, CONTACT_VECTOR_ZERO, 0x00, 0);

SetNewCoilInstruction(CONTACT_VECTOR_T, 0x04, true,//
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x01, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x04, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x01, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x04, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x11, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x04, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1A, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x04, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x06, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x04, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x01, COIL_S, 0);////////////////

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,//
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x01, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x01, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x11, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1A, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x06, COIL_R, 0);

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, false,//
                      CONTACT_VECTOR_T, 0x07, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x11, COIL_S, 0);

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,
                      CONTACT_VECTOR_M, 0x11, true,
                      CONTACT_VECTOR_T, 0x07, true,
                      CONTACT_VECTOR_Q, 0x01, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,
                      CONTACT_VECTOR_M, 0x11, true,
                      CONTACT_VECTOR_T, 0x07, true,
                      CONTACT_VECTOR_M, 0x01, COIL_R, 0);
//    //Habilita início de operação da faca, se o lacre estiver parado
//    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,//nao é isso
//                          CONTACT_VECTOR_M, 0x1A, false,//voltar aqui
//                          CONTACT_VECTOR_M, 0x1C, false,
//                          CONTACT_VECTOR_M, 0x1C, COIL_P, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,
                      CONTACT_VECTOR_M, 0x11, true,
                      CONTACT_VECTOR_M, 0x08, true,//estiver habilitada a leitura de 1 objeto para acionamento da faca
                      CONTACT_VECTOR_M, 0x1C, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,
                      CONTACT_VECTOR_M, 0x11, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1A, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,
                      CONTACT_VECTOR_M, 0x11, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x06, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x02, true,
                      CONTACT_VECTOR_M, 0x11, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x11, COIL_R, 0);

//
//Habilita início de operação do selo, se a faca estiver parada
//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0E, true,
//                          CONTACT_VECTOR_M, 0x1C, false,//voltar aqui
//                          CONTACT_VECTOR_M, 0x1F, false,
//                          CONTACT_VECTOR_M, 0x1B, COIL_P, 0);


SetNewCoilInstruction(CONTACT_VECTOR_M, 0x1B, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1F, COIL_S, 0);


SetNewCoilInstruction(CONTACT_VECTOR_M, 0x1B, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x02, COIL_P, 0);
SetNewTimerInstruction(CONTACT_VECTOR_M, 0x02, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x02, TIMER_MODE_T1, TIMER_BASE_MS1, 250, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x02, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x03, COIL_R, 0);
Stepper_Instructions[aux_Stepper_Inst++] = aux_Inst;
SetNewStepperInstruction(CONTACT_VECTOR_T, 0x02, true,
                         CONTACT_VECTOR_ZERO, 0x00, true,
                         CONTACT_VECTOR_ZERO, 0x00, true,
                         0x02, TIMER_BASE_MS1, 276,
                         50, 940, 10,
                         CONTACT_VECTOR_Q, 0x04, 0);
SetNewTimerInstruction(CONTACT_VECTOR_T, 0x02, true,
                       CONTACT_VECTOR_Q, 0x03, false,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x05, TIMER_MODE_T1, TIMER_BASE_MS1, 1000, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewTimerInstruction(CONTACT_VECTOR_T, 0x02, true,//Tempo mínimo de atuação
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x08, TIMER_MODE_T1, TIMER_BASE_MS1, 200, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x05, true,//
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x03, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x05, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x02, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x05, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x12, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x05, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1B, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x05, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x07, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x05, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x09, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x05, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x02, COIL_S, 0);/////////////////////

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,//
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x03, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x02, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x12, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1B, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x07, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x09, COIL_R, 0);

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, false,//
                      CONTACT_VECTOR_T, 0x08, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x12, COIL_S, 0);

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
                      CONTACT_VECTOR_M, 0x12, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x03, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
                      CONTACT_VECTOR_M, 0x12, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x02, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
                      CONTACT_VECTOR_M, 0x12, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1B, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
                      CONTACT_VECTOR_M, 0x12, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x07, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
                      CONTACT_VECTOR_M, 0x12, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x09, COIL_R, 0);
//    SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
//                          CONTACT_VECTOR_M, 0x1F, false,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_M, 0x1B, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1F, COIL_R, 0);

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x03, true,
                      CONTACT_VECTOR_M, 0x12, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x12, COIL_R, 0);



SetNewCoilInstruction(CONTACT_VECTOR_M, 0x1C, true,
                      CONTACT_VECTOR_Y, 0x10, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x03, COIL_P, 0);

SetNewTimerInstruction(CONTACT_VECTOR_M, 0x03, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x03, TIMER_MODE_T1, TIMER_BASE_MS1, 10, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x03, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x05, COIL_R, 0);
Stepper_Instructions[aux_Stepper_Inst++] = aux_Inst;
SetNewStepperInstruction(CONTACT_VECTOR_T, 0x03, true,
                         CONTACT_VECTOR_ZERO, 0x00, true,
                         CONTACT_VECTOR_ZERO, 0x00, true,
                         0x03, TIMER_BASE_MS1, 800,
                         50, 800, 10,
                         CONTACT_VECTOR_Q, 0x06, 0);
SetNewTimerInstruction(CONTACT_VECTOR_T, 0x03, true,
                       CONTACT_VECTOR_Q, 0x05, false,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x06, TIMER_MODE_T1, TIMER_BASE_MS1, 860, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewTimerInstruction(CONTACT_VECTOR_T, 0x03, true,//Tempo mínimo de atuação
                       CONTACT_VECTOR_Q, 0x05, false,
                       CONTACT_VECTOR_ZERO, 0x00, true,
                       0x09, TIMER_MODE_T1, TIMER_BASE_MS1, 100, 0, CONTACT_VECTOR_ZERO, 0x00, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x06, true,//
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x05, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x06, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x03, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x06, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x13, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x06, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1C, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x06, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x08, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_T, 0x06, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x03, COIL_S, 0);//////////////////////
//



SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,//
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x05, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x03, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x13, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1C, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x05, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x08, COIL_R, 0);



SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, true,//
                      CONTACT_VECTOR_T, 0x09, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x13, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, false,
                      CONTACT_VECTOR_M, 0x13, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_Q, 0x05, COIL_S, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, false,
                      CONTACT_VECTOR_M, 0x13, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x03, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, false,
                      CONTACT_VECTOR_M, 0x13, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x1C, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, false,
                      CONTACT_VECTOR_M, 0x13, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x08, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_I, 0x04, false,
                      CONTACT_VECTOR_M, 0x13, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x13, COIL_R, 0);

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x1D, true,
                      CONTACT_VECTOR_I, 0x05, true,
                      CONTACT_VECTOR_Y, 0x10, false,
                      CONTACT_VECTOR_Q, 0x07, COIL_C, 0);

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x1E, true,
                      CONTACT_VECTOR_I, 0x05, true,
                      CONTACT_VECTOR_Y, 0x10, false,
                      CONTACT_VECTOR_Q, 0x08, COIL_C, 0);

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0B, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,//!!!modifiquei de false para true.!!!Desfiz y11 para zero
                      CONTACT_VECTOR_I, 0x05, true,
                      CONTACT_VECTOR_Q, 0x09, COIL_C, 0);

SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0B, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,//!!!modifiquei de false para true.!!!Desfiz y11 para zero
                      CONTACT_VECTOR_I, 0x05, true,
                      CONTACT_VECTOR_Q, 0x0A, COIL_C, 0);

//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0F, false,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_M, 0x1C, COIL_S, 0);
//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0F, false,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_M, 0x03, COIL_S, 0);
//    SetNewCoilInstruction(CONTACT_VECTOR_M, 0x0F, false,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_ZERO, 0x00, true,
//                          CONTACT_VECTOR_M, 0x0F, COIL_S, 0);

SetNewCoilInstruction(CONTACT_VECTOR_X, 0x01, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x05, COIL_S, 0);

SetNewCoilInstruction(CONTACT_VECTOR_X, 0x02, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x05, COIL_S, 0);

SetNewCoilInstruction(CONTACT_VECTOR_X, 0x03, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x05, COIL_S, 0);

SetNewCoilInstruction(CONTACT_VECTOR_X, 0x01, true,
                      CONTACT_VECTOR_I, 0x06, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x01, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_X, 0x02, true,
                      CONTACT_VECTOR_I, 0x06, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x02, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_X, 0x03, true,
                      CONTACT_VECTOR_I, 0x06, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x03, COIL_R, 0);
SetNewCoilInstruction(CONTACT_VECTOR_X, 0x05, true,
                      CONTACT_VECTOR_I, 0x06, false,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_X, 0x05, COIL_R, 0);

SetNewCoilInstruction(CONTACT_VECTOR_I, 0x06, false,
                      CONTACT_VECTOR_M, 0x0A, true,
                      CONTACT_VECTOR_ZERO, 0x00, true,
                      CONTACT_VECTOR_M, 0x0A, COIL_R, 0);

//    Stepper_Instructions[aux_Stepper_Inst++] = aux_Inst;
//    SetNewStepperInstruction(CONTACT_VECTOR_ZERO, 0x00, true,
//                             CONTACT_VECTOR_ZERO, 0x00, true,
//                             CONTACT_VECTOR_ZERO, 0x00, true,
//                             0x03, TIMER_BASE_MS1, 1200,
//                             50, 0, 10,
//                             CONTACT_VECTOR_Q, 0x06, 0);
}