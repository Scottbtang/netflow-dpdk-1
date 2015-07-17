# Changelog

# DPDK Basic
Before using DPDK, you need to load DPDK kernel modules (uio, igb_uio).
Commands:
 modprobe uio
 insmod ${DPDK}/x86_64-native-linuxapp-gcc/kmod/igb_uio.ko

After load uio, igb_uio, unbind network device driver from ixgbe to igb_uio.
commands(if you want to unbind interface card p2p1):
python ${DPDK}/tools/dpdk_nic_bind.py --bind=igb_uio p2p1

# Build
~~~~
export RTE_SDK=<DPDK SDK path>
export RTE_TARGET=x86_64-native-linuxapp-gcc

make
~~~~

To use multiqueues, configure different rth_eth_conf.
File: netflow-init.h

In 1/10G Ethernet(ixgbe), rss_hf = ETH_RSS_IPV4,
In 40G Ethernet(i40e), rss_hf = ETH_RSS_NONF_IPV4_UDP | ETH_RSS_NONF_IPV4_TCP, 

code:
~~~
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

~~~~

# Run

To run, command line interpreter, run 2 threads
ex) 2 lcore
~~~~
./build/dprobe -c 0x3 -n 2 -- -p 0x1 -H 127.0.0.1 -P 2055 -q 2
~~~~

-H : Netflow Collector IP
-P : Netflow Collector Port Number
-q : Number of Receive queues

~~~~
./build/dprobe -c 0x15 -n 4 -- -m "2:0.0,4:0.1" -q 2
~~~~
desc: lcore(2) processes port(0) queue(0)
      lcore(4) processes port(0) queue(1)

# Reference
* DPDK build : http://www.analytaps.net/Netflow_DPDK

