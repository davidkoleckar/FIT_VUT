Projekt do predmetu PDS 2017/18 - DHCP utoky
Autor: David Koleckar - xkolec07

Pouziti:
1.DHCP Starvation
	sudo ./pds-dhcpstarve -i interface
	-i interface (řetězec) jméno rozhraní dle OS
	pro zobrazeni napovedy: ./pds-dhcpstarve -h
	(aplikaci je nutne spustit s root pravy [sudo], kvuli tvorbe raw socketu)

2.Rogue DHCP Server
	sudo ./pds-dhcprogue -i interface -p pool -g gateway -n dns-server -d domain -l lease-time
	-i interface (řetězec) jméno rozhraní dle OS
	-p pool (řětezec) pool adres reprezentovaný ve formátu <první_IPv4_adresa>-<poslední_IPv4_adresa>
	-g IPv4 adresa výchozí brány pro segment, ve kterém se nachází oběť
	-n IPv4 adresa DNS serveru 
	-d (řetězec) jméno domény, ve které se zařízení nachazí
	-l (číslo) počet sekund reprezentující dobu DHCP výpůjčky
	pro zobrazeni napovedy: ./pds-dhcprogue -h
	(aplikaci je nutne spustit s root pravy [sudo], kvuli tvorbe raw socketu)

Ukonceni aplikaci zaslanim signalu SIGINT (CTRL+C).

Odevzdane soubory:
	dhcprogue.cpp dhcprogue.h
	dhcpstarve.cpp dhcpstarve.h
	error.cpp error.h
	network.cpp network.h
	Makefile
	dokumentace.pdf
	readme
