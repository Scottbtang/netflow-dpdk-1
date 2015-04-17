#ifndef __NETFLOW_INIT_H_
#define __NETFLOW_INIT_H_

#include "probe.h"

#include <rte_log.h>
#include <rte_ethdev.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_hash.h>
#include <rte_hash_crc.h>

#define NB_SOCKETS  2
#define NB_MBUF     8192
#define MBUF_SIZE   (2048 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)

#define MEMPOOL_CACHE_SIZE  256

static const struct rte_eth_conf port_conf = {
    .rxmode = {
        .mq_mode        = ETH_MQ_RX_RSS,
        .split_hdr_size = 0,
        .header_split   = 0, /**< Header Split disabled */
        .hw_ip_checksum = 0, /**< IP checksum offload disabled */
        .hw_vlan_filter = 0, /**< VLAN filtering disabled */
        .jumbo_frame    = 0, /**< Jumbo Frame Support disabled */
        .hw_strip_crc   = 0, /**< CRC stripped by hardware */
    },
    .rx_adv_conf = {
        .rss_conf = {
            .rss_key = NULL,
            .rss_hf = ETH_RSS_IPV4 | ETH_RSS_IPV6,              /* 1/10G RSS offload features */
            //.rss_hf   = ETH_RSS_NONF_IPV4_UDP |
            //               ETH_RSS_NONF_IPV4_TCP,              /* 40G RSS offload features */
        },
    },
    .txmode = {
        .mq_mode = ETH_MQ_TX_NONE,
    },
};

#define RX_PTHRESH 8 /**< Default values of RX prefetch threshold reg. */
#define RX_HTHRESH 8 /**< Default values of RX host threshold reg. */
#define RX_WTHRESH 4 /**< Default values of RX write-back threshold reg. */

static const struct rte_eth_rxconf rx_conf = {
    .rx_thresh = {
        .pthresh = RX_PTHRESH,
        .hthresh = RX_HTHRESH,
        .wthresh = RX_WTHRESH,
    },
};

#define TX_PTHRESH 36
#define TX_HTHRESH 0  /**< Default values of TX host threshold reg. */
#define TX_WTHRESH 0  /**< Default values of TX write-back threshold reg. */

static const struct rte_eth_txconf tx_conf = {
    .tx_thresh = {
        .pthresh = TX_PTHRESH,
        .hthresh = TX_HTHRESH,
        .wthresh = TX_WTHRESH,
    },
};

static struct rte_mempool *pktmbuf_pool[NB_SOCKETS];
static int numa_on = 1;

typedef struct rte_hash lookup_stuct_t;
static lookup_stuct_t *netflow_V5_lookup_struct[NB_SOCKETS];

int netflow_init(probe_t *);


#endif
