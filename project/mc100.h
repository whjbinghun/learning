#ifndef _MC100_H
#define _MC100_H
extern "C"{

// 2013-3-27

#ifndef WINAPI
#define WINAPI				__stdcall
#endif

#define MC100_PORTA			0
#define MC100_PORTB			1
#define MC100_PORTC			2

_declspec(dllimport) int WINAPI mc100_scan_device(void);
_declspec(dllimport) int WINAPI mc100_open(int id);
_declspec(dllimport) int WINAPI mc100_close(int id);
_declspec(dllimport) int WINAPI mc100_set_pin(int id, int pin);
_declspec(dllimport) int WINAPI mc100_clear_pin(int id, int pin);
_declspec(dllimport) int WINAPI mc100_check_pin(int id, int pin);
_declspec(dllimport) int WINAPI mc100_set_push_pull(int id, int port, int value);
_declspec(dllimport) int WINAPI mc100_set_pull_up(int id, int port, int value);
_declspec(dllimport) int WINAPI mc100_read_port(int id, int port);
_declspec(dllimport) int WINAPI mc100_write_port(int id, int port, int value);
_declspec(dllimport) int WINAPI mc100_spi_send(int id, unsigned char * buffer, int length);
_declspec(dllimport) int WINAPI mc100_spi_transmit(int id, unsigned char * buffer, int length);

}
#endif