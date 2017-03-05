#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "print.h"

#define REG_PRODUCT_ID                           0x00
#define REG_Motion_Burst                         0x50
#define REG_Configuration_IV                     0x39
#define REG_SROM_Enable                          0x13
#define REG_SROM_Load_Burst                      0x62
#define REG_Power_Up_Reset                       0x3a
#define REG_LASER_CTRL0                          0x20
#define REG_CONF_1                               0x0f


// f0-clk f1-mo f4-nsc f5-mot d5-mi
#define ADNS_NSC_PORT PORTF
#define ADNS_NSC_BIT 4
#define ADNS_CLK_PORT PORTF
#define ADNS_CLK_BIT 0
#define ADNS_MO_PORT PORTF
#define ADNS_MO_BIT 1
#define ADNS_MI_PIN PIND
#define ADNS_MI_BIT 4
#define ADNS_MOT_PIN PINF
#define ADNS_MOT_BIT 5

#define adns_com_begin() ADNS_NSC_PORT &= ~(1 << ADNS_NSC_BIT)
#define adns_com_end() ADNS_NSC_PORT |= 1 << ADNS_NSC_BIT

#define firmware_length 3070

extern prog_uchar firmware_data[] PROGMEM;

typedef uint8_t  byte;
typedef int16_t  sword;
typedef uint16_t word;


void adns_send(byte d) {
    byte i = 8;
    do {
        ADNS_CLK_PORT &= ~(1 << ADNS_CLK_BIT);
        if (d & 0x80)
            ADNS_MO_PORT |= 1 << ADNS_MO_BIT;
        else
            ADNS_MO_PORT &= ~(1 << ADNS_MO_BIT);
        d <<= 1;
        ADNS_CLK_PORT |= 1 << ADNS_CLK_BIT;
    } while (--i);
}
byte adns_recv(void) {
    ADNS_CLK_PORT &= ~(1 << ADNS_CLK_BIT);
    byte d = 0;
    byte i = 8;
    for (;;) {
        ADNS_CLK_PORT |= 1 << ADNS_CLK_BIT;
        d <<= 1;
		d |= (ADNS_MI_PIN >> ADNS_MI_BIT) & 1;
        if (--i == 0)
            return d;
        ADNS_CLK_PORT &= ~(1 << ADNS_CLK_BIT);
    }
}

void adns_write(byte addr, byte data) {
	_delay_us(120);
    adns_send(addr | 0x80);
    adns_send(data);
}
byte adns_read(byte addr) {
	_delay_us(120);
    adns_send(addr);
	_delay_us(100);
    return adns_recv();
}

void adns_motion(sword* dx, sword* dy) {
	/*if (ADNS_MOT_PIN & (1 << ADNS_MOT_BIT)) {
		*dx = *dy = 0;
		return;
	}*/
	adns_com_begin();
	adns_send(REG_Motion_Burst);
	_delay_us(100);
	adns_recv();
	adns_recv();
	byte lo = adns_recv();
	*dx = adns_recv() << 8 | lo;
	lo = adns_recv();
	*dy = adns_recv() << 8 | lo;
	adns_com_end();
	_delay_us(500);
 }

void adns_init(void) {
  sword temp1;
  sword temp2;
  
  adns_com_begin(); // ensure that the serial port is reset
  adns_com_end();
  adns_com_begin();
  adns_write(REG_Power_Up_Reset, 0x5a); // force reset
  adns_com_end();
  _delay_ms(50); // wait for it to reboot
  adns_motion(&temp1, &temp2);
  {
      word count = firmware_length + 1;
      byte* ptr = firmware_data - 1;
      adns_com_begin();
      adns_write(REG_Configuration_IV, 0x02); // bit 1 = 1 for 3k mode, other bits are reserved 
      adns_write(REG_SROM_Enable, 0x1d);
      _delay_ms(10); // assume that the frame rate is as low as 100fps... even if it should never be that low
      adns_write(REG_SROM_Enable, 0x18);   // start SROM download
      adns_send(REG_SROM_Load_Burst | 0x80); // write burst destination address
      while (--count) {
		  _delay_us(15);
          adns_send(pgm_read_byte(++ptr));
	  }
      _delay_us(160);
      adns_com_end();
  }
  _delay_ms(10);

  // enable laser(bit 0 = 0), in normal mode (bits 3,2,1 = 000)
  // reading the actual value of the register is important because the real
  // default value is different from what is said in the datasheet, and if you
  // change the reserved bytes (like by writing 0x00...) it would not work.
  adns_com_begin();
  adns_write(REG_LASER_CTRL0, adns_read(REG_LASER_CTRL0) & 0xf0);
  //adns_com_end();
  
  //adns_com_begin();
	print("\nADNS id="); phex(adns_read(REG_PRODUCT_ID));
	print("\nADNS id2="); phex(adns_read(1));
	adns_write(REG_CONF_1, 10);
  adns_com_end();
}
