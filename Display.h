#define A	0x21
#define C	0x27
#define F	0x30
#define K	0x107
#define L	0x10A

#define HOLA 0
#define ERROR 1

void Display_init(uint16_t ptodisp);

void Display_number(uint16_t value, uint8_t base);
void Display_char(uint8_t place, uint8_t value);
void Display_pair(uint8_t left, uint8_t right, uint8_t baseL, uint8_t baseR);
void Display_decimal(uint16_t integer, uint8_t decimal);

void Display_battery(uint8_t level);
void Display_message(uint8_t message, uint8_t numErr);