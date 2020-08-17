/**
 * Projekt: PDS - DHCP attacks
 *
 * @file network.cpp
 *
 * @author David Koleckar - xkolec07
 *
 * @brief Funkce pro praci se siti
 */

#include "network.h"

//IP
struct iphdr *createIP(unsigned short len, unsigned int src, unsigned int dst){

	static struct iphdr ip;

	ip.ihl = 5;
	ip.version = 4;
	ip.tos = 0;
	ip.tot_len = htons(sizeof(struct iphdr) + len);
	ip.frag_off = htons(IP_DF);
	ip.ttl = 64;
	ip.protocol = IPPROTO_UDP;
	ip.check = 0;
	ip.id = htons(rand());
	ip.saddr = src;
	ip.daddr = dst;
	ip.check = checksum(&ip, sizeof(struct iphdr));

	return &ip;
}
//Prejato z:
//http://www.microhowto.info/howto/calculate_an_internet_protocol_checksum_in_c.html
unsigned int checksum(void* vdata,size_t length) {
    // Cast the data pointer to one that can be indexed.
    char* data=(char*)vdata;

    // Initialise the accumulator.
    uint64_t acc=0xffff;

    // Handle any partial block at the start of the data.
    unsigned int offset=((uintptr_t)data)&3;
    if (offset) {
        size_t count=4-offset;
        if (count>length) count=length;
        uint32_t word=0;
        memcpy(offset+(char*)&word,data,count);
        acc+=ntohl(word);
        data+=count;
        length-=count;
    }

    // Handle any complete 32-bit blocks.
    char* data_end=data+(length&~3);
    while (data!=data_end) {
        uint32_t word;
        memcpy(&word,data,4);
        acc+=ntohl(word);
        data+=4;
    }
    length&=3;

    // Handle any partial block at the end of the data.
    if (length) {
        uint32_t word=0;
        memcpy(&word,data,length);
        acc+=ntohl(word);
    }

    // Handle deferred carries.
    acc=(acc&0xffffffff)+(acc>>32);
    while (acc>>16) {
        acc=(acc&0xffff)+(acc>>16);
    }

    // If the data began at an odd byte address
    // then reverse the byte order to compensate.
    if (offset&1) {
        acc=((acc&0xff00)>>8)|((acc&0x00ff)<<8);
    }

    // Return the checksum in network byte order.
    return htons(~acc);
}

//UDP
struct udphdr *createUDP(unsigned short len, unsigned short src, unsigned short dst){

	static struct udphdr udp;
	udp.source = htons(src);
	udp.dest = htons(dst);
	udp.len = htons(sizeof(struct udphdr) + len);
	udp.check = 0;

	return &udp;
}


//ETHERNET
struct ethhdr *createEther(unsigned char *src, unsigned char *dst){

	static struct ethhdr eth;
	
	memcpy(eth.h_source, src, ETH_ALEN);
	memcpy(eth.h_dest, dst, ETH_ALEN);
	eth.h_proto = htons(ETH_P_IP);

    return &eth;
}

//DHCP (BOOTP)
struct dhcphdr *createDHCP(unsigned char  *chaddr, unsigned int xid, unsigned short type, unsigned int sadr, unsigned int yadr){

	static struct dhcphdr dhcp;

	memset(&dhcp, 0x0, sizeof(struct dhcphdr));
	dhcp.type = type;
	dhcp.htype = 1;
	dhcp.hlen = ETH_ALEN;
	dhcp.hops = 0;
	dhcp.xid = htonl(xid);
	dhcp.secs = htons(0);
	dhcp.flags = htons(0);
	dhcp.ciaddr = 0;
	dhcp.yaddr = yadr;
	dhcp.siaddr = sadr;
	dhcp.giaddr = 0;
	memcpy(dhcp.chaddr, chaddr, 6);
	/*dhcp.sname;
	dhcp.file;*/
	dhcp.dhcp_cookie = htonl(0x63825363);  
	
	return &dhcp;  
}

unsigned char *createDiscover(){
	unsigned char *data = (unsigned char *)malloc(4);

	data[0] = 0x35;
	data[1] = 0x01;
	data[2] = 0x01;
	data[3] = 0xff;
	
	return data;
}

unsigned char *createNak(unsigned int serverAddr){
	unsigned char *data = (unsigned char *)malloc(10);

	data[0] = 0x35;
	data[1] = 0x01;
	data[2] = 0x06;
	data[3] = 0x36;
	data[4] = 0x04;
	memcpy(&data[5],&serverAddr, 4);
	data[9] = 0xff;
	
	return data;
}

unsigned char *createRequest(unsigned int serverAddr, unsigned int reqAddr){
	unsigned char *data = (unsigned char *)malloc(16);

	data[0] = 0x35;
	data[1] = 0x01;
	data[2] = 0x03;
	data[3] = 0x36;
	data[4] = 0x04;
	memcpy(&data[5],&serverAddr, 4);
	data[9] = 0x32;
	data[10] = 0x04;
	memcpy(&data[11],&reqAddr, 4);
	data[15] = 0xff;
	
	return data;
}

unsigned char *createOffer(unsigned int serverAddr, unsigned int gateway, int lease, const char *domain, unsigned int dns){
	char *name = (char*)domain;
	int len = strlen(name);
	unsigned char *data = (unsigned char *)malloc(42+len);
	int leaseTime = htonl(lease); 
	unsigned int mask = inet_addr("255.255.255.0");
	unsigned int broad = inet_addr("255.255.255.255");

	data[0] = 0x35;
	data[1] = 0x01;
	data[2] = 0x02;
	data[3] = 0x36;
	data[4] = 0x04;
	memcpy(&data[5],&serverAddr, 4);
	data[9] = 0x33;
	data[10] = 0x04;
	memcpy(&data[11],&leaseTime, 4);
	data[15] = 0x01;
	data[16] = 0x04;
	memcpy(&data[17],&mask, 4);
	data[21] = 0x1c;
	data[22] = 0x04;
	memcpy(&data[23],&broad, 4);
	data[27] = 0x03;
	data[28] = 0x04;
	memcpy(&data[29],&gateway, 4);
	data[33] = 0x06;
	data[34] = 0x04;
	memcpy(&data[35],&dns, 4);
	data[39] = 0x0f;
	data[40] = len;
	memcpy(&data[41],name, len);
	data[41+len] = 0xff;
	
	return data;
}

unsigned char *createAck(unsigned int serverAddr, unsigned int gateway, int lease, const char *domain, unsigned int dns){
	char *name = (char*)domain;
	int len = strlen(name);
	unsigned char *data = (unsigned char *)malloc(42+len);
	int leaseTime = htonl(lease); 
	unsigned int mask = inet_addr("255.255.255.0");
	unsigned int broad = inet_addr("255.255.255.255");

	data[0] = 0x35;
	data[1] = 0x01;
	data[2] = 0x05;
	data[3] = 0x36;
	data[4] = 0x04;
	memcpy(&data[5],&serverAddr, 4);
	data[9] = 0x33;
	data[10] = 0x04;
	memcpy(&data[11],&leaseTime, 4);
	data[15] = 0x01;
	data[16] = 0x04;
	memcpy(&data[17],&mask, 4);
	data[21] = 0x1c;
	data[22] = 0x04;
	memcpy(&data[23],&broad, 4);
	data[27] = 0x03;
	data[28] = 0x04;
	memcpy(&data[29],&gateway, 4);
	data[33] = 0x06;
	data[34] = 0x04;
	memcpy(&data[35],&dns, 4);
	data[39] = 0x0f;
	data[40] = len;
	memcpy(&data[41],name, len);
	data[41+len] = 0xff;
	
	return data;
}

void randomMac(unsigned char mac[]){
	for (int i = 0; i < 6; i++) {
		mac[i] = rand();
	}
}

int createSocket(){
	int rawsock;
	if((rawsock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
		printEcode(ESOCKET);
		exit(EXIT_FAILURE);
	}
	return rawsock;
}

void bindSocket(int rawsock, const char *iface){
	struct sockaddr_ll sll;
	sll.sll_family = AF_PACKET;
	sll.sll_protocol = htons(ETH_P_IP);
	sll.sll_halen = ETH_ALEN;
	sll.sll_ifindex = getInterfaceIndex(rawsock,iface);
	unsigned char *mac = getInterfaceHwAdrr(rawsock,iface);
	memcpy(sll.sll_addr, mac, ETH_ALEN);

	if(bind(rawsock, (struct sockaddr *)&sll, sizeof(struct sockaddr_ll)) < 0){
		printEcode(EBIND);
		exit(EXIT_FAILURE);
	}

	free(mac);
}

unsigned char *getInterfaceHwAdrr(int rawsock, const char *iface){
	struct ifreq interface;
	strncpy((char *)interface.ifr_name, iface, strlen(iface) + 1);

	if(ioctl(rawsock, SIOCGIFHWADDR, &interface) < 0){
		printEcode(EIOCTL);
		exit(EXIT_FAILURE);
    }

    unsigned char *value = (unsigned char *) malloc (ETH_ALEN);
    memcpy(value, interface.ifr_hwaddr.sa_data, ETH_ALEN);
    return value;
}

int getInterfaceIndex(int rawsock,const char *iface){
	struct ifreq interface;
	strncpy((char *)interface.ifr_name, iface, strlen(iface) + 1);

	if(ioctl(rawsock, SIOCGIFINDEX, &interface) < 0){
		printEcode(EIOCTL);
		exit(EXIT_FAILURE);
    }

    return interface.ifr_ifindex;
}

unsigned char *getInterfaceIP(int rawsock,const char *iface){
	struct ifreq interface;
	strncpy((char *)interface.ifr_name, iface, IFNAMSIZ);

	if(ioctl(rawsock, SIOCGIFADDR, &interface) < 0){
		printEcode(EIOCTL);
		exit(EXIT_FAILURE);
    }

    unsigned char *value = (unsigned char *) malloc(4);
    memcpy(value, interface.ifr_addr.sa_data+2, 4);
    return value;	
}
