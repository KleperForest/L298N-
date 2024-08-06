#define F_CPU 16000000UL // Definir frecuencia del CPU para delays
#include <avr/io.h>
#include <util/delay.h>

// Definición de pines
#define IN1_PIN PD2
#define IN2_PIN PD3
#define EN_PIN  PB1

// Inicialización de pines
void setup_pins() {
	// Configuración de pines de control como salidas
	DDRD |= (1 << IN1_PIN) | (1 << IN2_PIN);
	// Configuración del pin EN como salida
	DDRB |= (1 << EN_PIN);

	// Inicializar pines en estado bajo
	PORTD &= ~(1 << IN1_PIN);
	PORTD &= ~(1 << IN2_PIN);
	PORTB &= ~(1 << EN_PIN);
}

// Inicialización de PWM en el pin EN
void setup_pwm() {
	// Configurar Timer1 para PWM en PB1 (OC1A)
	TCCR1A |= (1 << WGM10) | (1 << COM1A1); // Fast PWM, 8 bits
	TCCR1B |= (1 << WGM12) | (1 << CS11);   // Prescaler 8

	// Inicializar PWM en 0
	OCR1A = 0;
}

// Función para controlar el motor
void control_motor(uint8_t speed, uint8_t direction) {
	// Configurar la velocidad del motor (0-255)
	OCR1A = speed;

	// Configurar la dirección del motor
	if (direction == 0) { // Hacia adelante
		PORTD |= (1 << IN1_PIN);
		PORTD &= ~(1 << IN2_PIN);
		} else { // Hacia atrás
		PORTD &= ~(1 << IN1_PIN);
		PORTD |= (1 << IN2_PIN);
	}
}

int main(void) {
	setup_pins();
	setup_pwm();

	while (1) {
		// Mover motor hacia adelante con velocidad media
		control_motor(128, 0);
		_delay_ms(2000);

		// Detener motor
		control_motor(0, 0);
		_delay_ms(1000);

		// Mover motor hacia atrás con velocidad completa
		control_motor(255, 1);
		_delay_ms(2000);

		// Detener motor
		control_motor(0, 1);
		_delay_ms(1000);
	}
}
