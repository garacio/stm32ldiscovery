 /**
 ******************************************************************************
 * @file    discover_functions.c
 * @author  Microcontroller Division
 * @version V1.0.0
 * @date    Apri-2011
 * @brief   Discover demo functions
 ******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */

/* Includes ------------------------------------------------------------------*/

 
#include "stm32l1xx_exti.h"
#include "misc.h"

//#include "stm32_tsl_api.h"
//#include "stm32l15x_tsl_ct_acquisition.h"

#include "discover_board.h"
#include "discover_functions.h"
#include "stm32l_discovery_lcd.h"
#include "icc_measure.h"
  
/*Variables placed in DataFlash */

#if defined __GNUC__
uint8_t Bias_Current __attribute__ ((section (".DataFlash")));
bool Auto_test __attribute__ ((section (".DataFlash")));
#else
/* ADC converter value for Bias current value*/
uint8_t Bias_Current @ ".DataFlash" ;   
/* Flag for autotest placed in Data Flash for return from RESET after STANDBY */
bool Auto_test @ ".DataFlash" ;
#endif

extern float Current_STBY;
extern uint8_t t_bar[2];
extern uint16_t Int_CurrentSTBY;

/* Used for indicate that the automatic test is ON (set in interrupt handler).*/


/* To indicate if user button function is actived*/
bool UserButton ;
/* Used for detect keypressed*/
extern volatile bool KeyPressed;

/**
  * @brief  automatic test for VDD
  * @caller auto_test
  * @param None
  * @retval None
  */
void test_vdd(void)
{
  uint16_t vdd_test;
  uint16_t tab[6];
   
  LCD_GLASS_DisplayString("VDD   ");
  DELAY;
  
  vdd_test = (int)Vref_measure();
  DELAY;
  
  /* Test if value is correct */	
  if ((vdd_test>VCC_MAX) || (vdd_test<VCC_MIN))
  {
    while(1)
    {
      LCD_GLASS_ScrollSentence("VDD ERROR ",1,SCROLL_SPEED); //Press reset for exit
      DELAY;
      convert_into_char (vdd_test, tab);
            
      /* To add unit and decimal point  */
      tab[5] = 'V';
      tab[4] = ' ';
      tab[1] |= DOT; /* To add decimal point for display in volt */
      tab[0] = ' ';
            
      LCD_GLASS_DisplayStrDeci(tab);      
      DELAY;
      DELAY;      
    }
  }
  
  LCD_GLASS_DisplayString("VDD OK");
  DELAY ;
}

/**
  * @brief  Automatic test current in Run Mode
  * @caller auto_test
  * @param None
  * @retval None
  */ 
void test_icc_Run(void)
{
  uint16_t icc_test;
   uint16_t tab[6];
   
  LCD_GLASS_DisplayString("RUN   ");
  DELAY;
  
  icc_test = (int)Icc_RUN();
  DELAY;
  
  if ((icc_test>ICC_RUN_MAX) || (icc_test<ICC_RUN_MIN))
  {
    while (1)
    {
      KeyPressed = FALSE;
      LCD_GLASS_ScrollSentence("RUN ERROR ",1,SCROLL_SPEED); //Press reset for exit
      DELAY;
      convert_into_char((uint32_t)(icc_test), tab);
    
      /* To add unit and decimal point  */
      tab[5] = 'A';
      tab[4] = 'm';
      tab[3] = ' ';
      tab[0] |= DOT; /* To add decimal point for display */
    
      LCD_GLASS_DisplayStrDeci(tab);
      DELAY;
      DELAY;
    }
  }

  LCD_GLASS_DisplayString("RUN OK");
  DELAY;
}


/**
  * @brief  Automatic test bias value
  * @caller auto_test
  * @param None
  * @retval None
  */ 
void test_Bias(void)
{
  float Current;
    
  LCD_GLASS_DisplayString("BIAS   ");
  DELAY;
  Current = Bias_Current * Vdd_appli()/ADC_CONV; 
  Current *= 20L;
  display_MuAmp((uint32_t)Current);
  DELAY;
  
  if ((Bias_Current > ICC_BIAS_MAX) || (Bias_Current == 0 ))
  {
    while (1)
    {
      KeyPressed = FALSE;
      LCD_GLASS_ScrollSentence("BIAS ERROR ",1,SCROLL_SPEED); //Press reset for exit
      Current = Bias_Current * Vdd_appli()/ADC_CONV; 
      Current *= 20L;
      display_MuAmp((uint32_t)Current);
      DELAY;
      DELAY;
    }
  }

  LCD_GLASS_DisplayString("BIASOK");
  DELAY;
}

/**
  * @brief  Automatic test current in STOP Mode
  * @caller auto_test
  * @param None
  * @retval None
  */
void test_icc_STOP(void)
{
  uint16_t icc_test;
  
  LCD_GLASS_DisplayString("STOP  ");
  DELAY;
  
  /* Current value measured in Halt mode*/	
  icc_test = (int)Icc_Stop_NoRTC();
  DELAY;
  
  /* Test if value is correct */
  if ((icc_test>ICC_STOP_MAX) || (icc_test<ICC_STOP_MIN))
  {
    Delay(1);	
    icc_test = (int)Icc_Stop_NoRTC();
    DELAY;		
    
  /* Test if value is correct */
    if ((icc_test>ICC_STOP_MAX) || (icc_test<ICC_STOP_MIN))
    {
      while (1)
      {
        KeyPressed = FALSE;
        LCD_GLASS_ScrollSentence("ICC STOP ERROR ",1,SCROLL_SPEED); //Press reset for exit
        DELAY;
        display_MuAmp((uint32_t)icc_test);
        DELAY;
        DELAY;
      }
    }
  }
  
  LCD_GLASS_DisplayString("STOPOK");
  DELAY;
}


/**
  * @brief  Automatic test current in STBY Mode
  * @caller auto_test
  * @param None
  * @retval None
  */
void test_icc_STBY(void)
{
  
  LCD_GLASS_DisplayString("STBY  ");
  DELAY;
  
  /* Current value measured in Halt mode*/	
  ADC_Icc_Test(MCU_STBY);

  /* No Return go to RESET */
}

/**
  * @brief  Run auto test
  * @caller main 
  * @param None
  * @retval None
  */ 
void auto_test(void)
{
  uint16_t tab[6]={0x20,0x20,0x20,0x20,0x20,0x20};
  
  AUTOTEST(TRUE) ;
	
  /* Switch off leds*/
  GPIO_LOW(LD_PORT,LD_GREEN);	
  GPIO_LOW(LD_PORT,LD_BLUE);		
  BAR0_OFF;
  BAR1_OFF;
  BAR2_OFF;
  BAR3_OFF;
  
  /* To display version */
  LCD_GLASS_DisplayString(" TEST ");
  DELAY;
  STR_VERSION;
  LCD_GLASS_DisplayStrDeci(tab);
  DELAY;
  DELAY;
  
  /* And launch the tests*/
  test_vdd();
  test_icc_Run();
  test_Bias();
  test_icc_STOP();
  test_icc_STBY();
  
  /* Infinite loop: Press reset button at the end of test for exit*/
  while (1)
  {
    LCD_GLASS_ScrollSentence("TEST OK ",1,SCROLL_SPEED);
    KeyPressed = FALSE;
  }
}


/**
  * @brief  Run auto test
  * @caller main after RESET 
  * @param None
  * @retval None
  */ 
void auto_test_part2(void)
{
  float Current_STBY;
  
  /* Substract bias curent*/
  if ( Int_CurrentSTBY > Bias_Current )
    Int_CurrentSTBY -= Bias_Current;
  
  Current_STBY = Int_CurrentSTBY * Vdd_appli()/ADC_CONV;  
  
  Current_STBY *= 20L;
  
  display_MuAmp((uint32_t)Current_STBY);
  DELAY;
  
 /* Test if value is correct */
    if ((Current_STBY > ICC_STBY_MAX) || (Current_STBY < ICC_STBY_MIN))
    {
      while (1)
      {
        KeyPressed = FALSE;
        LCD_GLASS_ScrollSentence("ICC STBY ERROR ",1,SCROLL_SPEED); //Press reset for exit
        DELAY;
        display_MuAmp((uint32_t)Current_STBY);
        DELAY;
        DELAY;
      }
    }
    
  LCD_GLASS_DisplayString("STBYOK");
  DELAY;    
  
   /* Infinite loop: Press reset button at the end of test for exit*/
  while (1)
  {
    LCD_GLASS_ScrollSentence("TEST OK ",1,SCROLL_SPEED);
    KeyPressed = FALSE;
  }
}
/**
  * @brief Measures the BIAS current PJ1 Must be on OFF position
  * @caller main 
  * @param None
  * @retval None
  */  
void Bias_measurement(void)
{
  float Current;
  uint16_t MeasurINT;
  
  LCD_GLASS_ScrollSentence("      ** BIAS CURRENT ** JP1 OFF **",1,SCROLL_SPEED);	
  
  MeasurINT = ADC_Icc_Test(MCU_STOP_NoRTC);
  Current = MeasurINT * Vdd_appli()/ADC_CONV; 
  Current *= 20L;
  
  display_MuAmp((uint32_t)Current);

  /* To unlock Flash data */
  DATA_EEPROM_Unlock();
  
  /* To store the value in E²Prom */
  /* Sorry for the cast !!! */
  DATA_EEPROM_FastProgramByte((uint32_t)&Bias_Current, MeasurINT) ;
  
  /* To lock Flash data */
  DATA_EEPROM_Lock();	
  
  while (1)  /* Infinite loop for force to restart application */
  { 
    MeasurINT = ADC_Icc_Test(MCU_STOP_NoRTC);
    Current = MeasurINT * Vdd_appli()/ADC_CONV; 
    Current *= 20L;
    
    display_MuAmp((uint32_t)Current);
    Delay(800) ;
  }

}

/**
  * @brief converts a number into char
  * @caller several callers for display values
  * @param Number digit to displays
  *  p_tab values in array in ASCII   
  * @retval None
  */ 
void convert_into_char(uint32_t number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0, hundreds=0, thousands=0, misc=0;
  
  units = (((number%10000)%1000)%100)%10;
  tens = ((((number-units)/10)%1000)%100)%10;
  hundreds = (((number-tens-units)/100))%100%10;
  thousands = ((number-hundreds-tens-units)/1000)%10;
  misc = ((number-thousands-hundreds-tens-units)/10000);
  
  *(p_tab+4) = units + 0x30;
  *(p_tab+3) = tens + 0x30;
  *(p_tab+2) = hundreds + 0x30;
  *(p_tab+1) = thousands + 0x30;
  *(p_tab) = misc + 0x30;

}

/**
  * @brief Function to return the VDD measurement
  * @caller All measurements: VDD display or Current
  *
  * Method for VDD measurement:
  * The VREFINT is not stored in memory.
  *   In this case:
  *   Vdd_appli = (Theorical_Vref/Vref mesure) * ADC_Converter
  *   Theorical_Vref = 1.224V
  *   ADC_Converter 4096
  *   ---> LSBIdeal = VREF/4096 or VDA/4096
  * @param None   
  * @retval VDD measurements
  */
float Vdd_appli(void)
{
  uint16_t MeasurINT ;

  float f_Vdd_appli ;
  
  /*Read the BandGap value on ADC converter*/
  MeasurINT = ADC_Supply();	
  
  /* We use the theorical value */
  f_Vdd_appli = (VREF/MeasurINT) * ADC_CONV;

  /* Vdd_appli in mV */  
  f_Vdd_appli *= 1000L;
	
  return f_Vdd_appli;
}

/**
  * @brief Function to measure VDD
  * @caller main
  * @param None   
  * @retval Vdd value in mV
  */
uint16_t Vref_measure(void)
{
  uint16_t tab[6];	
  uint16_t Vdd_mV ;
  
  Vdd_mV = (uint16_t)Vdd_appli();

  convert_into_char (Vdd_mV, tab);
	
  /* To add unit and decimal point  */
  tab[5] = 'V';
  tab[4] = ' ';
  tab[1] |= DOT; /* To add decimal point for display in volt */
  tab[0] = ' ';
	
  LCD_GLASS_DisplayStrDeci(tab);

  return Vdd_mV;
}

/**
  * @brief funtion to display the current in µA
  * @caller several funcions
  * @param Current value.
  * @retval none
  */ 
void display_MuAmp (uint32_t Current)
{
  uint16_t tab[6];
          
  convert_into_char(Current, tab);
  tab[5] = 'A';
  tab[4] = 'µ';
		
/* Test the significant digit for displays 3 or 4 digits*/
  if ( tab[0] != '0')
  {
    tab[1] |= DOT; /* To add decimal point */
  }  else  {
    /* To shift for suppress '0' before decimal */
    tab[0] = tab[1] ;	
    tab[0] |= DOT ;
    tab[1] = tab[2] ;
    tab[2] = tab[3] ;		
    tab[3] = ' ';
  }
	
  LCD_GLASS_DisplayStrDeci(tab);
}

/**
  * @brief funtion Current measurement in RUN mode
  * @caller main and test_icc_RUN
  * @param none
  * @retval Current (mA)
  */ 
float Icc_RUN(void)
{
  float Current;
  uint16_t MeasurINT;
  uint16_t tab[6];	
	
  MeasurINT = ADC_Icc_Test(MCU_RUN);
  
  Current = MeasurINT * Vdd_appli()/ADC_CONV;
  Current *= 100L; 
  
  convert_into_char((uint32_t)(Current), tab);

  /* To add unit and decimal point  */
  tab[5] = 'A';
  tab[4] = 'm';
  tab[3] = ' ';
  tab[0] |= DOT; /* To add decimal point for display */

  LCD_GLASS_DisplayStrDeci(tab);
	
  return (Current);
}

/**
  * @brief funtion Current measurement in SLEEP mode
  * @caller main
  * @param none
  * @retval Current (µA)
  */ 
float Icc_SLEEP(void)
{
  float Current;
  uint16_t MeasurINT;
  uint16_t tab[6];	
  
  /* To init the mode and measurement*/
  MeasurINT = ADC_Icc_Test(MCU_SLEEP);

  
  /* Substract bias curent*/
  Current = MeasurINT * Vdd_appli()/ADC_CONV;  

  Current *= 100L;
  convert_into_char((uint32_t)(Current), tab);

  /* To add unit and decimal point  */
  tab[5] = 'A';
  tab[4] = 'm';
  tab[3] = ' ';
  tab[0] |= DOT; /* To add decimal point for display */

  LCD_GLASS_DisplayStrDeci(tab);
  
  return(Current);
}

/**
  * @brief funtion Current measurement in Low power
  * @caller main
  * @param none
  * @retval Current (mA)
  */ 
float Icc_LPRUN(void)
{
  float Current;
  uint16_t MeasurINT;

  /* To init the mode and measurement*/
  MeasurINT = ADC_Icc_Test(MCU_LP_RUN);
  
  /* Substract bias curent*/
  if ( MeasurINT > Bias_Current )
    MeasurINT -= Bias_Current;
  
  Current = MeasurINT * Vdd_appli()/ADC_CONV;  

  Current *= 20L;
  
  display_MuAmp((uint32_t)Current);

  return(Current);
}

/**
  * @brief funtion Current measurement in Low power
  * @caller main
  * @param none
  * @retval Current (µA)
  */ 
float Icc_LPSLEEP(void)
{
  float Current;
  uint16_t MeasurINT;
  
  /* To init the mode and measurement*/
  MeasurINT = ADC_Icc_Test(MCU_LP_SLEEP);
  
  /* Substract bias curent*/
  if ( MeasurINT > Bias_Current )
    MeasurINT -= Bias_Current;
  
  Current = MeasurINT * Vdd_appli()/ADC_CONV;  

  Current *= 20L;
  
  if ((int) Current<MAX_CURRENT)
  {	
    display_MuAmp((uint32_t)Current);
  } else{
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString("Error");
  }
  
  return(Current);
}

/**
  * @brief funtion Current measurement in Stop mode with LCD ON
  * @caller main and test_icc_LCD
  * @param none
  * @retval Current (µA)
  */
float Icc_STOP(void)
{
  float Current;
  uint16_t MeasurINT;
  
  /* To init the mode and measurement*/	
  MeasurINT = ADC_Icc_Test(MCU_STOP_RTC);
  
  /* Substract bias curent*/
  if ( MeasurINT > Bias_Current )
      MeasurINT -= Bias_Current;
  
  Current = MeasurINT * Vdd_appli()/ADC_CONV; 
  Current *= 20L;
  
  /* To test if value is "normal"   */
  if ((int) Current<MAX_CURRENT)
  {	
    display_MuAmp((uint32_t)Current);
  }
  else
  {
    if (!Auto_test)
    {
      LCD_GLASS_Clear();
      LCD_GLASS_DisplayString("Error");
    }
  }

  return (Current);
}

/**
  * @brief funtion Current measurement in Stop mode with LCD OFF
  * @caller main
  * @param none
  * @retval none
  */
float Icc_Stop_NoRTC(void)
{
  float Current;
  uint16_t MeasurINT;
	
  /* To init the mode and measurement*/		
  MeasurINT = ADC_Icc_Test(MCU_STOP_NoRTC);
  
  /* Substract bias curent*/
  if ( MeasurINT > Bias_Current )
    MeasurINT -=	Bias_Current;

  Current = MeasurINT * Vdd_appli()/ADC_CONV; 
  Current *= 20L;
  
  display_MuAmp((uint32_t)Current);
  return (Current);
}	

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
