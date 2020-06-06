#include "CUnit/CUnit.h"
#include <stdio.h>
#include "core/wiznet.h"

#define SIZE_MOCK_REGS 1200

uint8_t spi_send_buffer[16];
uint8_t spi_recv_buffer[16];
uint8_t spi_selected;
uint8_t mock_wiznet_regs[SIZE_MOCK_REGS];

static int wiznet_init_suite(void) { return 0; }
static int wiznet_clean_suite(void) { return 0; }

void test_write(void)
{
    memset(spi_send_buffer, 0, 4);
    sc_wiznet_write(1, 5);
    CU_ASSERT_EQUAL(spi_selected, 0);
    CU_ASSERT_EQUAL(spi_send_buffer[0], 0xf0); // Write opcode
    CU_ASSERT_EQUAL(spi_send_buffer[1], 0); // address hi
    CU_ASSERT_EQUAL(spi_send_buffer[2], 1); // address low
    CU_ASSERT_EQUAL(spi_send_buffer[3], 5); // data
}

void test_init(void)
{
    memset(mock_wiznet_regs, 0, SIZE_MOCK_REGS);
    sc_wiznet_init();
    CU_ASSERT_EQUAL(mock_wiznet_regs[0], 0x80);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x1a], 0x55);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x1b], 0x55);
}

void test_set_mac(void)
{
    uint8_t mac[] = {1, 2, 3, 4, 5, 254};
    memset(mock_wiznet_regs, 0, SIZE_MOCK_REGS);
    sc_wiznet_set_mac_address(mac);
    CU_ASSERT_EQUAL(mock_wiznet_regs[9], 1);
    CU_ASSERT_EQUAL(mock_wiznet_regs[10], 2);
    CU_ASSERT_EQUAL(mock_wiznet_regs[11], 3);
    CU_ASSERT_EQUAL(mock_wiznet_regs[12], 4);
    CU_ASSERT_EQUAL(mock_wiznet_regs[13], 5);
    CU_ASSERT_EQUAL(mock_wiznet_regs[14], 254);
}

void test_set_ip(void)
{
    uint8_t ip[] = {192, 168, 10, 211};
    uint8_t subnet[] = {255, 255, 255, 224};
    memset(mock_wiznet_regs, 0, SIZE_MOCK_REGS);
    sc_wiznet_set_ip_address(ip, subnet);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x0f], 192);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x10], 168);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x11], 10);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x12], 211);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x5], 255);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x6], 255);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x7], 255);
    CU_ASSERT_EQUAL(mock_wiznet_regs[0x8], 224);
}

void test_disconnect(void)
{
    mock_wiznet_regs[WIZ_S0_CR] = 0;
    sc_wiznet_disconnect(1);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_CR], 0);
    sc_wiznet_disconnect(0);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_CR], 0x8);
}

void test_close(void)
{
    mock_wiznet_regs[WIZ_S0_CR] = 0;
    sc_wiznet_close(1);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_CR], 0);
    sc_wiznet_close(0);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_CR], 0x10);
}

void test_open_socket(void)
{
    memset(mock_wiznet_regs, 0, SIZE_MOCK_REGS);
    mock_wiznet_regs[WIZ_S0_SR] = WIZ_SOCK_INIT;

    const int r = sc_wiznet_open_socket(0, 1, 8080);
    CU_ASSERT_EQUAL(r, 0);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_MR], 1);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_PORT], 31);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_PORT + 1], 144);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_CR], WIZ_CR_OPEN);
    CU_ASSERT_EQUAL(mock_wiznet_regs[WIZ_S0_MR], 1);
}

void test_send_invalid(void)
{
    uint8_t data = 1;
    CU_ASSERT_EQUAL(sc_wiznet_send(1, &data, 1), -1);
    CU_ASSERT_EQUAL(sc_wiznet_send(0, NULL, 1), -1);
    CU_ASSERT_EQUAL(sc_wiznet_send(0, &data, 0), -1);
}

void test_receive_invalid(void)
{
    uint8_t data;
    CU_ASSERT_EQUAL(sc_wiznet_receive(1, &data, 1), -1);
    CU_ASSERT_EQUAL(sc_wiznet_receive(0, NULL, 1), -1);
    CU_ASSERT_EQUAL(sc_wiznet_receive(0, &data, 0), -1);
}

int main_wiznet (void)
{
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("wiznet", wiznet_init_suite, wiznet_clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

#define ADD_TEST(t) \
    if (NULL == CU_add_test(pSuite, #t, t)) \
    {                                       \
        CU_cleanup_registry();              \
        return CU_get_error();              \
    }

    ADD_TEST(test_write)
    ADD_TEST(test_init)
    ADD_TEST(test_set_ip)
    ADD_TEST(test_set_mac)
    ADD_TEST(test_disconnect)
    ADD_TEST(test_close)
    ADD_TEST(test_open_socket)
    ADD_TEST(test_send_invalid)
    ADD_TEST(test_receive_invalid)

    return CU_get_error();
}
