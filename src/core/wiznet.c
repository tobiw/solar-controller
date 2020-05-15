/*
 * Hardware-independent implementation of Wiznet5100.
 */
#include "hw/io.h"
#include "core/wiznet.h"
#include <stddef.h>

#ifdef UNIT_TEST
extern uint8_t mock_wiznet_regs[32];
#define WIZ_WAIT_CR_CLEAR
#else
#define WIZ_WAIT_CR_CLEAR while (sc_wiznet_read(WIZ_S0_CR))
#endif


static
void sc_wiznet_write(uint16_t addr, uint8_t data)
{
    hw_spi_select(1);
    hw_spi_xmit(0xf0); // Wiznet Write OpCode
    hw_spi_xmit((addr & 0xff00) >> 8); // send address high byte
    hw_spi_xmit(addr & 0x00ff); // send address low byte
    hw_spi_xmit(data);
    hw_spi_select(0);

#ifdef UNIT_TEST
    mock_wiznet_regs[addr] = data;
#endif
}

static
void sc_wiznet_write_16bit(uint16_t addr, uint16_t data)
{
    sc_wiznet_write(addr, (data & 0xff00) >> 8);
    sc_wiznet_write(addr + 1, data & 0x00ff);
}

static
uint8_t sc_wiznet_read(uint16_t addr)
{
#ifdef UNIT_TEST
    return mock_wiznet_regs[addr];
#endif

    uint8_t r;
    hw_spi_select(1);
    hw_spi_xmit(0x0f); // Wiznet Read OpCode
    hw_spi_xmit((addr & 0xff00) >> 8); // send address high byte
    hw_spi_xmit(addr & 0x00ff); // send address low byte
    r = hw_spi_xmit(0);
    hw_spi_select(0);
    return r;
}

static
uint16_t sc_wiznet_read_16bit(uint16_t addr)
{
    return ((sc_wiznet_read(addr) << 8) + sc_wiznet_read(addr + 1));
}

void sc_wiznet_init()
{
    sc_wiznet_write(WIZ_MR, 0x80); // soft reset
    sc_wiznet_write(WIZ_RMSR, 0x55); // RX memory size: 2KB
    sc_wiznet_write(WIZ_TMSR, 0x55); // TX memory size: 2KB
}

void sc_wiznet_set_mac_address(uint8_t *mac)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        sc_wiznet_write(WIZ_SAR + i, mac[i]);
    }
}

void sc_wiznet_set_ip_address(uint8_t *ip, uint8_t *subnet)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        sc_wiznet_write(WIZ_SIPR + i, ip[i]);
        sc_wiznet_write(WIZ_SUBR + i, subnet[i]);
    }
}

void sc_wiznet_close(uint8_t sock)
{
    if (sock != 0) return;
    sc_wiznet_write(WIZ_S0_CR, 0x10); //close socket
    WIZ_WAIT_CR_CLEAR;
}

void sc_wiznet_disconnect(uint8_t sock)
{
    if (sock != 0) return;
    sc_wiznet_write(WIZ_S0_CR, 0x8); //disconnect current connection
    WIZ_WAIT_CR_CLEAR;
}

int8_t sc_wiznet_open_socket(uint8_t sock, uint8_t eth_protocol, uint16_t port)
{
    if (sock != 0) return -1;

    if (sc_wiznet_read(WIZ_S0_SR) == WIZ_SOCK_CLOSED) sc_wiznet_close(sock);

    sc_wiznet_write(WIZ_S0_MR, eth_protocol);
    sc_wiznet_write_16bit(WIZ_S0_PORT, port);
    sc_wiznet_write(WIZ_S0_CR, WIZ_CR_OPEN); // send open socket command
    WIZ_WAIT_CR_CLEAR;

    if (sc_wiznet_read(WIZ_S0_SR) == WIZ_SOCK_INIT)
        return 0;
    sc_wiznet_close(sock);

    return -1;
}

int8_t sc_wiznet_listen(uint8_t sock)
{
    if (sock != 0) return -1;

    if (sc_wiznet_read(WIZ_S0_SR) == WIZ_SOCK_INIT)
    {
        sc_wiznet_write(WIZ_S0_CR, WIZ_CR_LISTEN); // send listen command
        WIZ_WAIT_CR_CLEAR;
        if (sc_wiznet_read(WIZ_S0_SR) == WIZ_SOCK_LISTEN)
            return 0;
        sc_wiznet_close(sock);
    }
    return -1;
}

int8_t sc_wiznet_send(uint8_t sock, const uint8_t *data, uint16_t len)
{
    uint16_t offset_addr, real_addr, tx_size, timeout;

    if (len == 0 || sock !=0 || data == NULL) return -1;

    // Read 16-bit TX Free Size Register
    tx_size = sc_wiznet_read_16bit(WIZ_S0_TX_FSR);

    timeout = 0;

    // Check available TX size
    while (tx_size < len)
    {
        hw_mssleep(1);
        tx_size = sc_wiznet_read_16bit(WIZ_S0_TX_FSR);
        if (timeout++ > 1000)
        {
            sc_wiznet_disconnect(sock);
            return -1;
        }
    }

    // Read 16-bit TX Write Pointer
    offset_addr = sc_wiznet_read_16bit(WIZ_S0_TX_WR);

    while (len--)
    {
        // Write data fragment to real address in TX memory
        real_addr = WIZ_TXBUF + (offset_addr & WIZ_TXBUF_MASK);
        sc_wiznet_write(real_addr, *data);

        offset_addr++;
        data++;
    }

    // Update 16-bit TX Write Pointer
    sc_wiznet_write_16bit(WIZ_S0_TX_WR, offset_addr);

    // Send command
    sc_wiznet_write(WIZ_S0_CR, WIZ_CR_SEND);
    WIZ_WAIT_CR_CLEAR;

    return 0;
}

int8_t sc_wiznet_receive(uint8_t sock, uint8_t *data, uint16_t len)
{
    uint16_t offset_addr, real_addr;

    if (len == 0 || sock !=0 || data == NULL) return -1;

    if (len > 512) len = 510;

    offset_addr = sc_wiznet_read_16bit(WIZ_S0_RX_RD);

    while (len--)
    {
        real_addr = WIZ_RXBUF + (offset_addr & WIZ_RXBUF_MASK);
        *data = sc_wiznet_read(real_addr);
        offset_addr++;
        data++;
    }
    *data = '\0';

    // Update 16-bit RX Read Pointer
    sc_wiznet_write_16bit(WIZ_S0_RX_RD, offset_addr);

    // Receive command
    sc_wiznet_write(WIZ_S0_CR, WIZ_CR_RECV);
    hw_ussleep(5);

    return 0;
}
