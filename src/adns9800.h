#define REG_Motion_Burst                         0x50
#define REG_Configuration_IV                     0x39
#define REG_SROM_Enable                          0x13
#define REG_SROM_Load_Burst                      0x62
#define REG_Power_Up_Reset                       0x3a


// f0-clk f1-mo f4-nsc f5-mot d5-mi
#define ADNS_NSC_PORT PORTF
#define ADNS_NSC_BIT 4
#define ADNS_CLK_PORT PORTF
#define ADNS_CLK_BIT 0
#define ADNS_MO_PORT PORTF
#define ADNS_MO_BIT 1
#define ADNS_MI_PIN PIND
#define ADNS_MI_BIT 5

#define adns_com_begin() ADNS_NSC_PORT &= ~(1 << ADNS_NSC_BIT)
#define adns_com_end() ADNS_NSC_PORT |= 1 << ADNS_NSC_BIT

#define firmware_length 3070

extern prog_uchar firmware_data[] PROGMEM;


prog_uchar firmware_data[] PROGMEM = {


void adns_send(byte d) {
	byte i = 8;
	while (--i) {
		ADNS_CLK_PORT &= ~(1 << ADNS_CLK_BIT);
		if (d & 0x80)
			ADNS_MO_PORT |= 1 << ADNS_MO_BIT;
		else
			ADNS_MO_PORT &= ~(1 << ADNS_MO_BIT);
		d <<= 1;
		ADNS_CLK_PORT |= 1 << ADNS_CLK_BIT;
	}
}
byte adns_recv(void) {
	ADNS_CLK_PORT &= ~(1 << ADNS_CLK_BIT);
	byte d = 0;
	byte i = 8;
	for (;;) {
		ADNS_CLK_PORT |= 1 << ADNS_CLK_BIT;
		d = d << 1 | ((ADNS_MI_PIN >> ADNS_MI_BIT) & 1);
		if (--i == 0)
			return d;
		ADNS_CLK_PORT &= ~(1 << ADNS_CLK_BIT);
	}
}

void adns_write(byte addr, byte data) {
	adns_send(addr | 0x80);
	adns_send(data);
}
byte adns_read(byte addr) {
	adns_send(addr);
	return adns_recv();
}

void adns_burst_motion(sword* dx, sword* dy) {
  adns_com_begin();
  adns_send(REG_Motion_Burst);
  adns_recv();
  adns_recv();
  dx = adns_recv();
  dx |= adns_recv() << 8;
  dy = adns_recv();
  dy |= adns_recv() << 8;
  adns_com_end();
}

void adns_init(void) {
  sword temp1;
  sword temp2;
  adns_com_begin(); // ensure that the serial port is reset
  adns_com_end(); // ensure that the serial port is reset
  adns_com_begin(); // ensure that the serial port is reset
  adns_write(REG_Power_Up_Reset, 0x5a); // force reset
  delay(50); // wait for it to reboot
  adns_burst_motion(&temp1, &temp2);
  {
	  word count = firmware_length;
	  byte* ptr = firmware_data;
	  adns_com_begin();
	  adns_write(REG_Configuration_IV, 0x02); // bit 1 = 1 for 3k mode, other bits are reserved 
	  adns_write(REG_SROM_Enable, 0x1d);
	  delay(10); // assume that the frame rate is as low as 100fps... even if it should never be that low
	  adns_write(REG_SROM_Enable, 0x18);   // start SROM download
	  adns_send(REG_SROM_Load_Burst | 0x80); // write burst destination adress
	  delayMicroseconds(15);
	  while (--count){ 
		  adns_send(pgm_read_byte(ptr++));
	  adns_com_end();
  }
  delay(10);
  adns_com_begin();

  //enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
  // reading the actual value of the register is important because the real
  // default value is different from what is said in the datasheet, and if you
  // change the reserved bytes (like by writing 0x00...) it would not work.
  {
  byte laser_ctrl0 = adns_read(REG_LASER_CTRL0);
  adns_write(REG_LASER_CTRL0, laser_ctrl0 & 0xf0);
  adns_com_end();
}

init() {
	// optical sensor
  pinMode (ncs, OUTPUT);
  pinMode(mot, INPUT);
  attachInterrupt(mot, pointer_moved, FALLING);
  
  // 48MHz / 24 = 2MHz = fSCLK
  spi4teensy3::init(5,1,1);

  perform_startup();
  delay(100);
  configure();
}
