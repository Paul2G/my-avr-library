#define F_CPU 16000000UL
#define BAUD 115200
#define UBRRx ((F_CPU/BAUD/8)-1)

//Formato de texto
#define REGULAR	0
#define BOLD	1	
#define UNDERL	4

// Secuencias de escape
#define BACK_SPACE 8
#define ENTER 13
#define SPACE 32
#define STRL 20

//Colores
enum color {
	BLACK = 0,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
};

void UART_init( void );

void UART_putchar( char data );
char UART_getch( void );
void UART_puts(char *str);
void UART_gets(char *str);

//Secuencias ASCII para formatear en linea de comando
void UART_setText(uint8_t tipo, uint8_t color);
void UART_gotoxy(uint8_t x, uint8_t y);
void UART_clrscr( void );

//Convertidores de texto
void itoa(uint32_t number, char* str, uint8_t base, uint8_t size);
uint32_t atoi(char *str);
