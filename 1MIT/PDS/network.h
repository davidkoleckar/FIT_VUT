/**
 * Projekt: PDS - DHCP attacks
 *
 * @file network.h
 *
 * @author David Koleckar - xkolec07
 *
 * @brief Hlavickovy soubor pro network.cpp
 */
#ifndef NETWORK_H
#define NETWORK_H

//vlozeni hlavicek
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include "error.h"

//struktura DHCP (BOOTP) hlavicky
struct dhcphdr{
	unsigned char type;
	unsigned char htype;
	unsigned char hlen;
	unsigned char hops;
	unsigned int xid;
	unsigned short secs;
	unsigned short flags;
	unsigned int ciaddr;
	unsigned int yaddr;
	unsigned int siaddr;
	unsigned int giaddr;
	unsigned char chaddr[16];
	unsigned char sname[64];
	unsigned char file[128];
	unsigned int dhcp_cookie;    
};

//velikosti hlavicek, paketu 
#define DIS_HLEN 4
#define REQ_HLEN 16
#define DHCP_HLEN 240
#define UDP_HLEN 8
#define IP_HLEN 20
#define OFF_HLEN 42
#define ACK_HLEN 42
#define NAK_HLEN 10

#define PACKET_HLEN 512

/**
 * Funkce pro vytvoreni IP hlavicky
 *
 * @param len Delka hlavicky.
 * @param src Zdrojova adresa.
 * @param dst Cilova adresa 
 * @return Struktura obsahujici IP hlavicku.
 */
struct iphdr *createIP(unsigned short len, unsigned int src, unsigned int dst);

/**
 * Funkce pro vypocet checksum.
 * Funkce byla prejata ze stranky
 * http://www.microhowto.info/howto/calculate_an_internet_protocol_checksum_in_c.html
 *
 * @param vdata Data.
 * @param length Delka hlavicky.
 * @return Velikost checksum.
 */
unsigned int checksum(void* vdata,size_t length);

/**
 * Funkce pro vytvoreni UDP hlavicky
 *
 * @param len Delka hlavicky.
 * @param src Zdrojova adresa - port.
 * @param dst Cilova adresa - port 
 * @return Struktura obsahujici UDP hlavicku.
 */
struct udphdr *createUDP(unsigned short len, unsigned short src, unsigned short dst);

/**
 * Funkce pro vytvoreni Ethernet hlavicky
 *
 * @param src Zdrojova adresa.
 * @param dst Cilova adresa.
 * @return Struktura obsahujici Ethernet hlavicku.
 */
struct ethhdr *createEther(unsigned char *src, unsigned char *dst);

/**
 * Funkce pro vytvoreni DHCP (BOOTP) hlavicky
 *
 * @param chaddr MAC adresa zarizeni.
 * @param xid ID z DHCP hlavicky.
 * @param type Typ zpravy.
 * @param sadr Adresa serveru
 * @param yadr Klientova adresa.
 * @return Struktura obsahujici DHCP hlavicku.
 */
struct dhcphdr *createDHCP(unsigned char  *chaddr, unsigned int xid, unsigned short type, unsigned int sadr, unsigned int yadr);

/**
 * Funkce pro vytvoreni DHCP options typu DISCOVER 
 *
 * @return Options DHCP DISCOVER.
 */
unsigned char *createDiscover();

/**
 * Funkce pro vytvoreni DHCP options typu REQUEST 
 *
 * @param serverAddr Adresa serveru.
 * @param reqAddr Pozadovana adresa.
 * @return Options DHCP REQUEST.
 */
unsigned char *createRequest(unsigned int serverAddr, unsigned int reqAddr);

/**
 * Funkce pro vytvoreni DHCP options typu OFFER
 *
 * @param serverAddr Adresa serveru.
 * @param gateway Adresa IPv4 gateway.
 * @param lease Pozadovana doba vypujcky.
 * @param domain Nazev domain.
 * @param dns Adresa IPv4 dns.
 * @return Options DHCP OFFER.
 */
unsigned char *createOffer(unsigned int serverAddr, unsigned int gateway, int lease, const char *domain, unsigned int dns);

/**
 * Funkce pro vytvoreni DHCP options typu ACK
 *
 * @param serverAddr Adresa serveru.
 * @param gateway Adresa IPv4 gateway.
 * @param lease Pozadovana doba vypujcky.
 * @param domain Nazev domain.
 * @param dns Adresa IPv4 dns.
 * @return Options DHCP ACK.
 */
unsigned char *createAck(unsigned int serverAddr, unsigned int gateway, int lease, const char *domain, unsigned int dns);

/**
 * Funkce pro vytvoreni DHCP options typu NAK 
 *
 * @param serverAddr Adresa serveru.
 * @return Options DHCP NAK.
 */
unsigned char *createNak(unsigned int serverAddr);

/**
 * Funkce pro generovani nahodne MAC adresy
 *
 * @param mac MAC adresa.
 */
void randomMac(unsigned char mac[]);

/**
 * Funkce pro vytvoreni socketu 
 *
 * @return Socket.
 */
int createSocket();

/**
 * Funkce pro svazani jmena do socketu
 *
 * @param rawsock Socket.
 * @param iface Nazev rozhrani.
 */
void bindSocket(int rawsock, const char *iface);

/**
 * Funkce pro ziskani MAC adresy rozhrani
 *
 * @param rawsock Socket.
 * @param iface Nazev rozhrani.
 * @return MAC adresa rozhrani.
 */
unsigned char *getInterfaceHwAdrr(int rawsock, const char *iface);

/**
 * Funkce pro ziskani indexu rozhrani
 *
 * @param rawsock Socket.
 * @param iface Nazev rozhrani.
 * @return Index rozhrani.
 */
int getInterfaceIndex(int rawsock,const char *iface);

/**
 * Funkce pro ziskani IP adresy rozhrani
 *
 * @param rawsock Socket.
 * @param iface Nazev rozhrani.
 * @return IP adresa rozhrani.
 */
unsigned char *getInterfaceIP(int rawsock,const char *iface);


#endif
