#include "config.h"

#define SERIAL_RX USART_RX_vect
#define SERIAL_UDRE USART_UDRE_vect

typedef struct
{
  uint8_t *data;
  uint8_t head;
  volatile uint8_t tail;
} queue;

queue serial_rx_buffer = {.head = 0, .tail = 0};
queue serial_tx_buffer = {.head = 0, .tail = 0};

uint8_t serial_get_rx_buffer_count()
{
  uint8_t rtail = serial_rx_buffer.tail; // Copy to limit multiple calls to volatile
  if (serial_rx_buffer.head >= rtail)
  {
    return (serial_rx_buffer.head - rtail);
  }
  return (RX_BUFFER_SIZE - (rtail - serial_rx_buffer.head));
}

uint8_t serial_get_tx_buffer_count()
{
  uint8_t ttail = serial_tx_buffer.tail;
  if (serial_tx_buffer.head >= ttail)
  {
    return (serial_tx_buffer.head - ttail);
  }
  return (TX_BUFFER_SIZE - (ttail - serial_tx_buffer.head));
}

void serial_init()
{
  // buffer init
  serial_rx_buffer.data = malloc(RX_BUFFER_SIZE * sizeof(uint8_t));
  serial_tx_buffer.data = malloc(TX_BUFFER_SIZE * sizeof(uint8_t));

  // memset(&serial_rx_buffer.data, 0, RX_BUFFER_SIZE * sizeof(uint8_t));
  // memset(&serial_tx_buffer.data, 0, TX_BUFFER_SIZE * sizeof(uint8_t));
// Set baud rate
#if BAUD_RATE < 57600
  uint16_t UBRR0_value = ((F_CPU / (8L * DEFAULT_BAUD_RATE)) - 1) / 2;
  UCSR0A &= ~(1 << U2X0); // baud doubler off  - Only needed on Uno XXX
#else
  uint16_t UBRR0_value = ((F_CPU / (4L * BAUD_RATE)) - 1) / 2;
  UCSR0A |= (1 << U2X0); // baud doubler on for high baud rates, i.e. 115200
#endif
  UBRR0H = UBRR0_value >> 8;
  UBRR0L = UBRR0_value;

  // deaktivace pinu 0, 1 jako digital out/in, ale nastaveni jako UART
  UCSR0B |= 1 << RXEN0;
  UCSR0B |= 1 << TXEN0;

  // povoleni interuptu, abych mohl potvrdit, že je cely jeden byte poslan
  UCSR0B |= 1 << RXCIE0;
}

void serial_cleanup()
{ // clean all dynamic memory
  if (serial_rx_buffer.data != NULL)
  {
    free(serial_rx_buffer.data);
    serial_rx_buffer.data = NULL;
  }

  if (serial_tx_buffer.data != NULL)
  {
    free(serial_tx_buffer.data);
    serial_tx_buffer.data = NULL;
  }
}

void serial_write(uint8_t data)
{
  // další hlavay queue
  uint8_t next_head = serial_tx_buffer.head + 1;
  if (next_head == TX_BUFFER_SIZE)
  {
    next_head = 0;
  }

  // čeká na místo v queue
  while (next_head == serial_tx_buffer.tail)
  { // > 64
    // loop cekani
  }

  // uložení dat
  serial_tx_buffer.data[serial_tx_buffer.head] = data;
  serial_tx_buffer.head = next_head;

  // Povolte přerušení prázdného registru dat, abyste se ujistili, že běží tx-streaming
  UCSR0B |= (1 << UDRIE0);
}

ISR(SERIAL_UDRE)
{
  uint8_t tail = serial_tx_buffer.tail;
  // pop 1 bytu z queue
  UDR0 = serial_tx_buffer.data[tail];

  // update tailu
  tail++;
  if (tail == TX_BUFFER_SIZE)
  {
    tail = 0;
  }

  serial_tx_buffer.tail = tail;

  // vypněte Data Register Empty Interrupt pro zastavení TX-streamingu, pokud se tím přenos ukončí
  if (tail == serial_tx_buffer.head)
  {
    UCSR0B &= ~(1 << UDRIE0);
  }
}

uint8_t serial_read()
{
  uint8_t tail = serial_rx_buffer.tail; // dočasný tail bufferu
  if (serial_rx_buffer.head == tail)
  {
    return SERIAL_NO_DATA;
  }
  else
  {
    uint8_t data = serial_rx_buffer.data[tail];

    tail++;
    if (tail == RX_BUFFER_SIZE)
    {
      tail = 0;
    }
    serial_rx_buffer.tail = tail;

    return data;
  }
}

ISR(SERIAL_RX)
{
  uint8_t data = UDR0;
  uint8_t next_head;

  // push characteru
  next_head = serial_rx_buffer.head + 1;
  if (next_head == RX_BUFFER_SIZE)
  {
    next_head = 0;
  }

  // zapisování dat do bufferu dokud není full
  if (next_head != serial_rx_buffer.tail)
  {
    serial_rx_buffer.data[serial_rx_buffer.head] = data;
    serial_rx_buffer.head = next_head;
  }
  // tady nemám alarm na přetečení
}

void serial_reset_read_buffer()
{
  serial_rx_buffer.tail = serial_rx_buffer.head;
}
