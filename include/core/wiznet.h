#include <stdint.h>

#ifdef UNIT_TEST
#define static
#endif

#define WIZ_MR   0x0000 // Mode Register
#define WIZ_GAR  0x0001 // Gateway Address: 0x0001 to 0x0004
#define WIZ_SUBR 0x0005 // Subnet mask Address: 0x0005 to 0x0008
#define WIZ_SAR  0x0009 // Source Hardware Address (MAC): 0x0009 to 0x000E
#define WIZ_SIPR 0x000F // Source IP Address: 0x000F to 0x0012
#define WIZ_RMSR 0x001A // RX Memory Size Register
#define WIZ_TMSR 0x001B // TX Memory Size Register

#define WIZ_S0_MR      0x0400 // Socket 0: Mode Register Address
#define WIZ_S0_CR      0x0401 // Socket 0: Command Register Address
#define WIZ_S0_IR      0x0402 // Socket 0: Interrupt Register Address
#define WIZ_S0_SR      0x0403 // Socket 0: Status Register Address
#define WIZ_S0_PORT    0x0404 // Socket 0: Source Port: 0x0404 to 0x0405
#define WIZ_S0_TX_FSR  0x0420 // Socket 0: Tx Free Size Register: 0x0420 to 0x0421
#define WIZ_S0_TX_RD   0x0422 // Socket 0: Tx Read Pointer Register: 0x0422 to 0x0423
#define WIZ_S0_TX_WR   0x0424 // Socket 0: Tx Write Pointer Register: 0x0424 to 0x0425
#define WIZ_S0_RX_RSR  0x0426 // Socket 0: Rx Received Size Pointer Register: 0x0425 to 0x0427
#define WIZ_S0_RX_RD   0x0428 // Socket 0: Rx Read Pointer: 0x0428 to 0x0429

#define WIZ_TXBUF      0x4000 // W5100 Send Buffer Base Address
#define WIZ_RXBUF      0x6000 // W5100 Read Buffer Base Address
#define WIZ_TXBUF_MASK 0x07FF // Tx 2K Buffer Mask
#define WIZ_RXBUF_MASK 0x07FF // Rx 2K Buffer Mask

// S0_MR values
#define WIZ_MR_CLOSE	  0x00    // Unused socket
#define WIZ_MR_TCP		  0x01    // TCP
#define WIZ_MR_UDP		  0x02    // UDP
#define WIZ_MR_IPRAW	  0x03	  // IP LAYER RAW SOCK
#define WIZ_MR_MACRAW	  0x04	  // MAC LAYER RAW SOCK
#define WIZ_MR_PPPOE	  0x05	  // PPPoE
#define WIZ_MR_ND		  0x20	  // No Delayed Ack(TCP) flag
#define WIZ_MR_MULTI	  0x80	  // support multicating

// S0_CR values
#define WIZ_CR_OPEN          0x01	  // Initialize or open socket
#define WIZ_CR_LISTEN        0x02	  // Wait connection request in tcp mode(Server mode)
#define WIZ_CR_CONNECT       0x04	  // Send connection request in tcp mode(Client mode)
#define WIZ_CR_DISCON        0x08	  // Send closing reqeuset in tcp mode
#define WIZ_CR_CLOSE         0x10	  // Close socket
#define WIZ_CR_SEND          0x20	  // Update Tx memory pointer and send data
#define WIZ_CR_SEND_MAC      0x21	  // Send data with MAC address, so without ARP process
#define WIZ_CR_SEND_KEEP     0x22	  // Send keep alive message
#define WIZ_CR_RECV          0x40	  // Update Rx memory buffer pointer and receive data

// S0_SR values
#define WIZ_SOCK_CLOSED      0x00     // Closed
#define WIZ_SOCK_INIT        0x13	  // Init state
#define WIZ_SOCK_LISTEN      0x14	  // Listen state
#define WIZ_SOCK_SYNSENT     0x15	  // Connection state
#define WIZ_SOCK_SYNRECV     0x16	  // Connection state
#define WIZ_SOCK_ESTABLISHED 0x17	  // Success to connect
#define WIZ_SOCK_FIN_WAIT    0x18	  // Closing state
#define WIZ_SOCK_CLOSING     0x1A	  // Closing state
#define WIZ_SOCK_TIME_WAIT	 0x1B	  // Closing state
#define WIZ_SOCK_CLOSE_WAIT  0x1C	  // Closing state
#define WIZ_SOCK_LAST_ACK    0x1D	  // Closing state
#define WIZ_SOCK_UDP         0x22	  // UDP socket
#define WIZ_SOCK_IPRAW       0x32	  // IP raw mode socket
#define WIZ_SOCK_MACRAW      0x42	  // MAC raw mode socket
#define WIZ_SOCK_PPPOE       0x5F	  // PPPOE socket

static void sc_wiznet_write(uint16_t addr, uint8_t data);
static uint8_t sc_wiznet_read(uint16_t addr);
void sc_wiznet_init();
void sc_wiznet_set_mac_address(uint8_t *mac);
void sc_wiznet_set_ip_address(uint8_t *ip, uint8_t *subnet);
void sc_wiznet_close(uint8_t sock);
void sc_wiznet_disconnect(uint8_t sock);
int8_t sc_wiznet_open_socket(uint8_t sock, uint8_t eth_protocol, uint16_t port);
int8_t sc_wiznet_listen(uint8_t sock);
int8_t sc_wiznet_send(uint8_t sock, const uint8_t *data, uint16_t len);
int8_t sc_wiznet_receive(uint8_t sock, uint8_t *data, uint16_t len);
