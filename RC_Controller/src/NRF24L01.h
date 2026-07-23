#ifndef NRF24L01_H
#define NRF24L01_H
#include <cstdint>

namespace nrf24
{

// -----------------------------------------------------------------------------
// Register Addresses
// -----------------------------------------------------------------------------

enum class Register : uint8_t
{
    CONFIG          = 0x00,
    EN_AA           = 0x01,
    EN_RXADDR       = 0x02,
    SETUP_AW        = 0x03,
    SETUP_RETR      = 0x04,
    RF_CH           = 0x05,
    RF_SETUP        = 0x06,
    STATUS          = 0x07,
    OBSERVE_TX      = 0x08,
    RPD             = 0x09,

    RX_ADDR_P0      = 0x0A,
    RX_ADDR_P1      = 0x0B,
    RX_ADDR_P2      = 0x0C,
    RX_ADDR_P3      = 0x0D,
    RX_ADDR_P4      = 0x0E,
    RX_ADDR_P5      = 0x0F,

    TX_ADDR         = 0x10,

    RX_PW_P0        = 0x11,
    RX_PW_P1        = 0x12,
    RX_PW_P2        = 0x13,
    RX_PW_P3        = 0x14,
    RX_PW_P4        = 0x15,
    RX_PW_P5        = 0x16,

    FIFO_STATUS     = 0x17,

    DYNPD           = 0x1C,
    FEATURE         = 0x1D
};

// -----------------------------------------------------------------------------
// CONFIG Register Bits
// -----------------------------------------------------------------------------

enum ConfigBits : uint8_t
{
    CONFIG_PRIM_RX      = (1u << 0),
    CONFIG_PWR_UP       = (1u << 1),
    CONFIG_CRCO         = (1u << 2),
    CONFIG_EN_CRC       = (1u << 3),
    CONFIG_MASK_MAX_RT  = (1u << 4),
    CONFIG_MASK_TX_DS   = (1u << 5),
    CONFIG_MASK_RX_DR   = (1u << 6)
};

// -----------------------------------------------------------------------------
// EN_AA Register Bits
// -----------------------------------------------------------------------------

enum EnAaBits : uint8_t
{
    ENAA_P0 = (1u << 0),
    ENAA_P1 = (1u << 1),
    ENAA_P2 = (1u << 2),
    ENAA_P3 = (1u << 3),
    ENAA_P4 = (1u << 4),
    ENAA_P5 = (1u << 5)
};

// -----------------------------------------------------------------------------
// EN_RXADDR Register Bits
// -----------------------------------------------------------------------------

enum EnRxAddrBits : uint8_t
{
    ERX_P0 = (1u << 0),
    ERX_P1 = (1u << 1),
    ERX_P2 = (1u << 2),
    ERX_P3 = (1u << 3),
    ERX_P4 = (1u << 4),
    ERX_P5 = (1u << 5)
};

// -----------------------------------------------------------------------------
// SETUP_AW
// -----------------------------------------------------------------------------

enum class AddressWidth : uint8_t
{
    AW_3_BYTES = 0x01,
    AW_4_BYTES = 0x02,
    AW_5_BYTES = 0x03
};

// -----------------------------------------------------------------------------
// SETUP_RETR
// -----------------------------------------------------------------------------

constexpr uint8_t SETUP_RETR_ARC_MASK = 0x0F;
constexpr uint8_t SETUP_RETR_ARD_MASK = 0xF0;

// -----------------------------------------------------------------------------
// RF_SETUP Register Bits
// -----------------------------------------------------------------------------

enum RfSetupBits : uint8_t
{
    RF_PWR_LOW      = (1u << 1),
    RF_PWR_HIGH     = (1u << 2),

    RF_DR_HIGH      = (1u << 3),
    PLL_LOCK        = (1u << 4),
    RF_DR_LOW       = (1u << 5),

    CONT_WAVE       = (1u << 7)
};

enum class DataRate : uint8_t
{
    RATE_1MBPS      = 0,
    RATE_2MBPS      = 1,
    RATE_250KBPS    = 2
};

enum class TxPower : uint8_t
{
    NEG18_DBM = 0,
    NEG12_DBM = 1,
    NEG6_DBM  = 2,
    ZERO_DBM  = 3
};

// -----------------------------------------------------------------------------
// STATUS Register Bits
// -----------------------------------------------------------------------------

enum StatusBits : uint8_t
{
    STATUS_TX_FULL  = (1u << 0),

    STATUS_RX_DR    = (1u << 6),
    STATUS_TX_DS    = (1u << 5),
    STATUS_MAX_RT   = (1u << 4)
};

constexpr uint8_t STATUS_RX_P_NO_MASK = 0x0E;

// -----------------------------------------------------------------------------
// OBSERVE_TX
// -----------------------------------------------------------------------------

constexpr uint8_t OBSERVE_TX_ARC_CNT_MASK  = 0x0F;
constexpr uint8_t OBSERVE_TX_PLOS_CNT_MASK = 0xF0;

// -----------------------------------------------------------------------------
// FIFO_STATUS Register Bits
// -----------------------------------------------------------------------------

enum FifoStatusBits : uint8_t
{
    FIFO_RX_EMPTY   = (1u << 0),
    FIFO_RX_FULL    = (1u << 1),

    FIFO_TX_EMPTY   = (1u << 4),
    FIFO_TX_FULL    = (1u << 5),
    FIFO_TX_REUSE   = (1u << 6)
};

// -----------------------------------------------------------------------------
// DYNPD Register Bits
// -----------------------------------------------------------------------------

enum DynpdBits : uint8_t
{
    DPL_P0 = (1u << 0),
    DPL_P1 = (1u << 1),
    DPL_P2 = (1u << 2),
    DPL_P3 = (1u << 3),
    DPL_P4 = (1u << 4),
    DPL_P5 = (1u << 5)
};

// -----------------------------------------------------------------------------
// FEATURE Register Bits
// -----------------------------------------------------------------------------

enum FeatureBits : uint8_t
{
    EN_DYN_ACK = (1u << 0),
    EN_ACK_PAY = (1u << 1),
    EN_DPL     = (1u << 2)
};

} // namespace nrf24

#endif // NRF24L01_H