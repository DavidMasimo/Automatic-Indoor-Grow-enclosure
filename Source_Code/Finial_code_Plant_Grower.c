#include <xc.h>
#include <stdio.h>
#include <htc.h>
#include <math.h>

#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is always Disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP Disabled)

#define _XTAL_FREQ 16000000
#define Dry_Sen 875
#define Wet_Sen 480
#define TEMP_THRESHOLD_L 18
#define TEMP_THRESHOLD_H 25
#define MOISTURE_THRESHOLD_LOW 30
#define MOISTURE_THRESHOLD_HIGH 70
#define humidity_THRESHOLD_LOW 40
#define humidity_THRESHOLD_HIGH 70




// Function Prototypes
void Initialize(void);
void UARTInit(void);
void UARTSend(char data);
char LCDInit(float Set_Temperature, float Set_Moisture, float Set_Humidity);
void controlFans(float Temperature,float Humidity);
void controlPump(float Moisture_percent);
void SelectADCChannel(unsigned char channel);
float ReadADC(void);
float CalculateTemperature(float RawTemperatureADC);
float Moisture_Percent_Conv(float RawMoistureADC);
float calculateHumidity(float RawHumidityADC);
float Calc_SVP(float Temperature);
float Calc_VPD(float Temperature, float Humidity);
void newline();
void UARTSendString(char str[]);
void controlPTC(float Temperature);


void main()
{
    float RawTemperatureADC = 0;
    float RawMoistureADC = 0;
    float RawHumidityADC = 0;
    float Temperature = 0 ;
    float Moisture_percent = 0;
    float Humidity = 0;
    float vpd = 0;
    float Set_Temperature = 30;
    float Set_Moisture = 60;
    float Set_Humidity = 60;
    
    Initialize(); // Initialize PIC for ADC
    UARTInit();   // Initialize UART for communication
   
     __delay_ms(200);
    LCDInit(Set_Temperature,Set_Moisture,Set_Humidity);
   
    while(1)
    {
        // Read raw ADC value
        RawHumidityADC = ReadADC();
        
        Humidity = calculateHumidity(RawHumidityADC);
        Temperature = CalculateTemperature(RawTemperatureADC);
       
        // Calculate humidity
        
        
        Moisture_percent = Moisture_Percent_Conv(RawMoistureADC);
        
        vpd = Calc_VPD(Temperature, Humidity);
        UARTSend(0x80);
        // Send data over UART
        char buffer[150];
        sprintf(buffer, "Moisture: %.0f%%,Temperature: %.2f C,Humidity: %.2f%% RH,VPD: %.3f kPa\n" , Moisture_percent, Temperature, Humidity, vpd);
        UARTSendString(buffer);
        

        // Logic for controlling the pump based on moisture level
        
        controlPTC(Temperature);
        controlFans(Temperature,Humidity);
        controlPump(Moisture_percent);

        __delay_ms(2000); // Delay before the next reading
    }
}

float ReadADC(void) {
    
    float RawADC;
    ADON = 1;              // turn AtoD converter on.
    GO_DONE = 1;    // Start ADC conversion
    while (GO_DONE); // Wait for conversion to finish
    RawADC =(ADRESH*256 ) + ADRESL; 
     __delay_ms(100); //delay to help settle the readings from the soil sensor
    //I think code should go here to switch to the next channel to read
    return RawADC; // Return the Raw moisture ADC conversion to re-purpose in code
}


float Moisture_Percent_Conv(float RawMoistureADC){
    float Moisture_percent;
   
      // Calculate moisture percentage
       Moisture_percent = 100 - ((RawMoistureADC - Wet_Sen) / (Dry_Sen - Wet_Sen)) * 100;
    if (Moisture_percent < 0) Moisture_percent = 0;
    if (Moisture_percent > 100) Moisture_percent = 100;
   
    return Moisture_percent;
}

float CalculateTemperature(float RawTemperatureADC)
{
    float Temperature = 0;
    float V_in = 0;

    // Convert Raw ADC value to input voltage (assuming 3.3V V_REF and 10-bit ADC)
    V_in = (RawTemperatureADC / 1023.0f) * 3.27f;

    // Apply the corrected linear relationship
    Temperature = (72.3f * V_in) - 20.9f;

    return Temperature;
}

float calculateHumidity(float RawHumidityADC)
{
    float voltage, humidity;
    
    // Convert RawADC to voltage
    voltage = RawHumidityADC * (3.3f / 1023.0f);
    
    // Reverse the scaling from the voltage divider
    voltage = voltage / 0.80f;
    
    // Calculate relative humidity from voltage (adjust offset and slope as needed)
    humidity = (voltage - 0.826f) / 0.0315f;  
    
    // Limit humidity to 0-100% range
    if (humidity > 100.0) humidity = 100.0;
    if (humidity < 0.0) humidity = 0.0;
    
    return humidity;
}

float Calc_SVP(float Temperature) {
    float svp = 0;
    svp = 610.78f * exp((Temperature / (Temperature + 237.3f)) * 17.2694f);  // Use exp() for exponential
    return svp;  // SVP in Pascals
}

float Calc_VPD(float temperature, float humidity) {
    // Step 1: Calculate Saturation Vapor Pressure (SVP)
    float svp = Calc_SVP(temperature);

    // Step 2: Calculate VPD
    float vpd = svp * (1.0f - (humidity / 100.0f));
    return vpd / 1000.0f;  // Convert from Pascals to kPa
}

// Function to control the pump based on moisture percentage
void controlPump(float moisture_percent) {
    if (moisture_percent < MOISTURE_THRESHOLD_LOW) {
        LATB0 = 1; // Turn ON pump
    } else if (moisture_percent > MOISTURE_THRESHOLD_HIGH) {
        LATB0 = 0; // Turn OFF pump
    }
}

void controlPTC(float Temperature) {
    if (Temperature < TEMP_THRESHOLD_L) {
        LATB2 = 1;   // Turn ON PTC heater (Assuming LATB0 is controlling the heater)
    } else if (Temperature > TEMP_THRESHOLD_H ) {
        LATB2 = 0;  // Turn OFF PTC heater
    }
}

void controlFans(float temperature, float Humidity) {
    // Control fans if temperature or humidity is out of range
    if (temperature < TEMP_THRESHOLD_L || temperature > TEMP_THRESHOLD_H || Humidity < humidity_THRESHOLD_LOW || Humidity > humidity_THRESHOLD_HIGH) {
        LATB4 = 1;  // Turn ON fan
        LATB5 = 1;  // Turn ON fan
    } else {
        LATB4 = 0;  // Turn OFF fan
        LATB5 = 0;  // Turn OFF fan
    }
}
// Function to send a single character via UART
void UARTSend(char data)
{
    while(!TXIF);   // Wait until TXREG is empty
    TXREG = data;   // Send the data
}

char LCDInit(float Set_Temperature, float Set_Moisture, float Set_Humidity)
{
    UARTSend(0x0C);
    char prompt;
    float TempThresh;
    float MoistThresh;
    float HumidThresh;
    prompt = 1;
    TempThresh = Set_Temperature;
    MoistThresh = Set_Moisture;
    HumidThresh = Set_Humidity;

    do {   
        if(prompt == 1) {
            UARTSend(0x80);
            printf("Enter max temp:\r%0.0fC ", TempThresh);
            if(RD1) {
                TempThresh++; 
                if (TempThresh > 35) TempThresh = 35;
                __delay_ms(300);
            } else if(RD2) {
                TempThresh--; 
                if (TempThresh < 10) TempThresh = 10;
                __delay_ms(300);
            } else if (RD0) {
                __delay_ms(500);
                prompt = 2;
                __delay_ms(1000);
            }
        }
        if(prompt == 2) {
            UARTSend(0x80);
            printf("Enter max moist:\r%0.0f%% ", MoistThresh);
            if(RD1) {
                MoistThresh++; 
                if (MoistThresh > 100) MoistThresh = 100;
                __delay_ms(300);
            } else if(RD2) {
                MoistThresh--; 
                if (MoistThresh < 30) MoistThresh = 30;
                __delay_ms(300);
            } else if (RD0) {
                __delay_ms(500);
                prompt = 3;
                __delay_ms(1000);
            }
        }
        if(prompt == 3) {
            UARTSend(0x80);
            printf("Enter max humid:\r%0.0f%% ", HumidThresh);
            if(RD1) {
                HumidThresh++; 
                if (HumidThresh > 90) HumidThresh = 90;
                __delay_ms(300);
            } else if(RD2) {
                HumidThresh--; 
                if (HumidThresh < 40) HumidThresh = 40;
                __delay_ms(300);
            } else if (RD0) {
                __delay_ms(500);
                prompt = 0;
                UARTSend(0x0C);
                printf("Nice, values are:\rTemperature=%0.0f\rMoisture=%0.0f\rHumidity=%0.0f\r", TempThresh, MoistThresh, HumidThresh);
                __delay_ms(1000);
                break; // Exit the loop when prompt == 0
            }
        }
    } while(1);

    return 1; // Return a value if needed, or adjust to your program's requirements
}

// Initialize the PIC for ADC
void Initialize(void)
{
    OSCCON = 0x7F;   // Set up internal oscillator for 16 MHz.
    INTCON = 0x00;   // Disable interrupts

    //Humidity sensor
    TRISE0 = 1;      // Select analog input pin (RA0)
      ANS5 = 1;      // Set RA0 to analog

    //Temperature sensor
    TRISE1 = 1;      // Select analog input pin (RA0)
      ANS6 = 1;      // Set RA0 to analog

    //Moister sensor
    TRISE2 = 1;      // Select analog input pin (RA0)
      ANS7 = 1;      // Set RA0 to analog

    ADCON0 = 0x14;  // Analog input 0, ADC off, and GO_DONE = 0
    //00010100 - 0x14
    //00011000 - 0x18
    //00011100 - 1C
    ADCON1 = 0x00;   // Reference Voltage set to Vdd/Vss
    ADCON2 = 0x92;   // Right Justified, Acquisition time of 8 TAD (8uS) and F(osc)/16
    
   
    
     //Relay connection points and triggers 
    TRISB0 = 0;      // Set RB0 as output (Intake Fan control)
     LATB0 = 0;      // Initialize Fan Latch to OFF
    
    TRISB1 = 0;      // Set RB1 as output (Intake Fan control)
     LATB1 = 0;      // Initialize Fan Latch to OFF

    TRISB2 = 0;      // Set RB2 as output (Heat Fan control)
     LATB2 = 0;      // Initialize Fan Latch to OFF
  
    TRISB3 = 0;      // Set RB3 as output (Water Pump control)
     LATB3 = 0;      // Initialize WaterPump Latch to OFF

    TRISD0 = 1;
    TRISD1 = 1;
    TRISD2 = 1;
}

// UART Initialization
void UARTInit(void)
{
    OSCTUNE = 0;    // Used to tune the frequency of the internal oscillator
    BRGH = 1;       // Choose high-speed Baud rate
    SYNC = 0;       // Set to asynchronous mode
    BRG16 = 0;      // Use 8-Bit Baud generator
    SPBRG = 103;    // Baud selection for 9600 (for 16 MHz internal clock)
    
    RX9 = 0;        // Set 8-Bit Reception
    TX9 = 0;        // Set 8-Bit Transmission
    SPEN = 1;       // Enable Serial Port
    CREN = 1;       // Enable continuous receive
    TXEN = 1;       // Enable USART Transmitter
    UARTSend(0x0c);
    UARTSend(0x11);
    __delay_ms(10);
}

void UARTSendString(char str[])
{
    int i = 0; // Use an index to avoid pointers
    while(str[i] != '\0') { // Loop until the null terminator
        UARTSend(str[i]); // Send each character of the string
        i++; // Increment the index
    }
    newline(); // Send a newline after the string
}
void newline()
{
    while(!TXIF);   // Wait until TXREG is empty
    TXREG = '\n';
    __delay_ms(10);
    while(!TXIF);   // Wait until TXREG is empty
    TXREG = '\r';
    __delay_ms(10);
}
