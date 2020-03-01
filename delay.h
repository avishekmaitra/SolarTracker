
#ifndef DELAY_H_
#define DELAY_H_
#define MS 300
#define CLK FREQ_48_MHZ
#define FREQ_1p5_MHZ 1500000
#define FREQ_3_MHZ   3000000
#define FREQ_6_MHZ   6000000
#define FREQ_12_MHZ  12000000
#define FREQ_24_MHZ  24000000
#define FREQ_48_MHZ  48000000
#define LINEAR_US 100
void delay_ms(int count, int freq);
void delay_us(int count, int freq);
void delay_us_48_MHZ(int count);
void set_DCO(int freq);

#endif /* DELAY_H_ */
