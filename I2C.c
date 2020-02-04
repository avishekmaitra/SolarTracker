#include "msp.h"
#include <stdint.h>
#define ACCEL_ADDRESS 0x1C

uint8_t TransmitFlag = 0;

void InitI2C(uint8_t DeviceAddress)
{
    // Set I2C pins of eUSCI_B0
    P1->SEL0 |= BIT6 | BIT7;

    // Enable eUSCIB0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);
    __enable_interrupt();

    // Configure USCI_B0 for I2C mode
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST;   // Software reset enabled

    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST |   // Remain eUSCI in reset mode
            EUSCI_B_CTLW0_MODE_3 |            // I2C mode
            EUSCI_B_CTLW0_MST |               // Master mode
            EUSCI_B_CTLW0_SYNC |              // Sync mode
            EUSCI_B_CTLW0_SSEL__SMCLK;        // SMCLK
            EUSCI_B0->BRW = 30;               // baudrate = SMCLK / 30 = 100kHz

    EUSCI_B0->I2CSA = DeviceAddress;          // Slave address
    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;  // Release eUSCI from reset

    EUSCI_B0->IE |= EUSCI_A_IE_RXIE |         // Enable receive interrupt
    EUSCI_A_IE_TXIE;

}

//Function that writes a single byte to I2C
void WriteI2C(uint8_t MemAddress, uint8_t MemByte)
{
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;          // Set transmit mode (write)
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;       // I2C start condition

    while (!TransmitFlag);            // Wait for I2C address to transmit
    TransmitFlag = 0;
    EUSCI_B0 -> TXBUF = MemAddress;   // Send the memory address

    while (!TransmitFlag);            // Wait for the transmit to complete
    TransmitFlag = 0;
    EUSCI_B0 -> TXBUF = MemByte;      // Send the byte to store in EEPROM

    while (!TransmitFlag);            // Wait for the transmit to complete
    TransmitFlag = 0;
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TXSTP;   // I2C stop condition
}

//Function that reads a single byte from the I2C
uint8_t ReadI2C(uint8_t MemAddress)
{
    uint8_t ReceiveByte;

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;      // Set transmit mode (write)
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;   // I2C start condition

    while (!TransmitFlag);                    // Wait for I2C address to transmit
    TransmitFlag = 0;

    EUSCI_B0 -> TXBUF = MemAddress;           // Send the high byte of the memory address

    while (!TransmitFlag);                    // Wait for the transmit to complete
    TransmitFlag = 0;

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TR;     // Set receive mode (read)
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;   // I2C start condition (restart)

    // Wait for start to be transmitted
    while ((EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT));

    // set stop bit to trigger after first byte
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

    while (!TransmitFlag);            // Wait to receive a byte
    TransmitFlag = 0;

    ReceiveByte = EUSCI_B0->RXBUF;

    return ReceiveByte;
}

//I2C ISR
void EUSCIB0_IRQHandler(void)
{
    // Transmit
    if(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0)
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG0; //CLEAR
        TransmitFlag = 1;                     //Set global Flag
    }

    // Receive
    if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0) // CHECK IF COMPLETE
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG0;
        TransmitFlag = 1;
    }
}

