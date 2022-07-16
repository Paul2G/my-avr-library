/* Funcion para inicializar el Timer0 y generar */
/* la temporizacion de 1 Sec. */
void Clock_init ( uint8_t timer );
void Clock_set (uint8_t hour, uint8_t min, uint8_t sec);
void Clock_puts();

/* Funcion para verificar bandera del segundo */
uint8_t Clock_secFlag ( void );

uint64_t millis();
uint8_t seconds();
uint8_t minutes();
uint8_t hours();