#include <Arduino.h>

/*
STATE - UINT16
0: Unavailable
1: Available
2: Occupied
3: Preparing
4: Charging
5: Finishing
6: Suspended EV
7: Suspended EVSE
8: Not ready
9: Faulted
*/
const uint16_t REG_STATE = 1000;
uint16_t STATE, lSTATE = 0;

/*
Charge State - UINT16
0: Charging process not started (no vehicle connected) 
1: Connected, waiting for release (by RFID or local) 
2: Charging process starts 
3: Charging 
4: Suspended (loading paused) 
5: Charging process successfully com-pleted (vehicle still plugged in) 
6: Charging process completed by user (vehicle still plugged in) 
7: Charging ended with error (vehicle still connected)
*/
const uint16_t REG_CHG_STATE= 1001;
uint16_t CHG_STATE, lCHG_STATE = 0;

/*
Actual Output Voltage (V) - UINT32
1 = 0.1 V
*/
const uint16_t REG_OUT_VOLT= 1003;
float OUT_VOLT, lOUT_VOLT;
uint16_t OUTVOLT[2];

/*
Actual Charging Power (unsigned) (W) - UINT32
*/
const uint16_t REG_CHG_POW = 1005;
uint16_t CHG_POW, lCHG_POW;
uint16_t CHGPOW[2];


/*
Actual Output Current (A) - UINT32
1 = 0.01 A
*/
const uint16_t REG_I_OUT = 1007;
float I_OUT, lI_OUT;
uint16_t IOUT[2];

/*
Actual Output Power (W) - UINT32
*/
const uint16_t REG_OUT_POW = 1009;
uint32_t OUT_POW;

/*
Maximum Charging Power (of EV) determined by the EVSE - UINT32
*/
const uint16_t REG_MAX_CHG_POW = 1013;
uint32_t MAX_CHG_POW;

/*
Minimum Charging Power (of EV) determined by the EVSE - UINT32
*/
const uint16_t REG_MIN_CHG_POW = 1015;
uint32_t MIN_CHG_POW;

/*
Charging Time in Seconds for the cur-rent charging session (s) - UINT32
*/
const uint16_t REG_CHG_TIME = 1017;
uint32_t CHG_TIME;

/*
Energy transferred to EV (at output) for the current charging session (WH) - UINT32
*/
const uint16_t REG_ACT_CHG_ENR = 1019;
uint16_t ACT_CHG_ENR, lACT_CHG_ENR = 0;
uint16_t ACTCHGENR[2];

/*
Current Total Power Consumption of complete charger (grid connection) (W) - UINT32
*/
const uint16_t REG_POW_CONS_TOTAL = 1047;
uint32_t POW_CONS_TOTAL;

/*
Current Total Power Consumption of complete charger L1 (grid connection) (W) - UINT32
*/
const uint16_t REG_POW_CONS_TOTAL_L1 = 1049;
uint32_t POW_CONS_TOTAL_L1;

/*
Current Total Power Consumption of complete charger L2 (grid connection) (W) - UINT32
*/
const uint16_t REG_POW_CONS_TOTAL_L2 = 1051;
uint32_t POW_CONS_TOTAL_L2;

/*
Current Total Power Consumption of complete charger L3 (grid connection) (W) - UINT32
*/
const uint16_t REG_POW_CONS_TOTAL_L3 = 1053;
uint32_t POW_CONS_TOTAL_L3;

/*
Error Code - UINT64
*/
const uint16_t REG_ERR_CODE = 1055; 
uint64_t ERR_CODE;

/*
Line 1 current consumed by the EVSE (A) - UINT32
1 = 0.01 A
*/
const uint16_t REG_I_L1 = 1067;
uint32_t I_L1;

/*
Line 2 current consumed by the EVSE (A) - UINT32
1 = 0.01 A
*/
const uint16_t REG_I_L2 = 1069;
uint32_t I_L2;

/*
Line 3 current consumed by the EVSE (A) - UINT32
1 = 0.01 A
*/
const uint16_t REG_I_L3 = 1071;
uint32_t I_L3;

/*
Line 1 voltage at the EVSE (A) - UINT32
1 = 0.1 V
*/
const uint16_t REG_V_L1 = 1073;
float V_L1, lV_L1;
uint16_t VL1[2];

/*
Line 2 voltage at the EVSE (A) - UINT32
1 = 0.1 V
*/
const uint16_t REG_V_L2 = 1075;
uint16_t VL2[2];
float V_L2, lV_L2;

/*
Line 3 voltage at the EVSE (A) - UINT32
1 = 0.1 V
*/
const uint16_t REG_V_L3 = 1077;
float V_L3, lV_L3;
uint16_t VL3[2];