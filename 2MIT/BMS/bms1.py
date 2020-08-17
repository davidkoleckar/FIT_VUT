#!/usr/bin/env python3

import sys, os, struct
from os import path

#velikost packetu a jeho hlavicky
PACKETSIZE = 188
HEADERSIZE = 4

#funkce pro tisk chybovych hlaseni
def printError(whatHappend):
	print(whatHappend, file=sys.stderr)
	sys.exit(1)

#funkce pro extrakci konkretnich bitu
def bitExtracted(number, k, p): 
	binheader = '{:032b}'.format(number)
	return (int(binheader[k:p],2))

#funkce pro parsovani tabulky PAT 
def parsePAT(data, infoPAT):
	tableID = bitExtracted(struct.unpack('>L',data[0:4])[0],0,8)
	if tableID != 0x00:
		return True
	sectionLength = bitExtracted(struct.unpack('>L',data[0:4])[0],12,24)
	i = 8
	while i < sectionLength:
		programNumber = bitExtracted(struct.unpack('>L',data[i:i+4])[0],0,16)
		pid = bitExtracted(struct.unpack('>L',data[i:i+4])[0],19,32)
		#print( "\nprogramnumber " + str(hex(programNumber)) + "\npid " +str(hex(pid)))
		if programNumber == 0x00:
			nitpid = pid
		#ulozeni informaci do slovniku (programnumber, pid)
		infoPAT[str(programNumber)] = str(pid)
		i+=4
	return False, nitpid

#funkce pro spusteni parsovani tabulky PMT 
def runPMT(actualPid,infoPAT):
	for key in infoPAT:
		if infoPAT[key] == str(actualPid):
			return True
	return False

#funkce pro parsovani tabulky PMT
def parsePMT(data, infoPMT):
	#kontrola id tabulky
	tableID = bitExtracted(struct.unpack('>L',data[0:4])[0],0,8)
	if tableID != 0x02:
		return True

	sectionLength = bitExtracted(struct.unpack('>L',data[0:4])[0],12,24)
	programNumber = bitExtracted(struct.unpack('>L',data[1:5])[0],16,32)
	programInfoLength = bitExtracted(struct.unpack('>L',data[8:12])[0],20,32)

	#pokud neni programnumber ve slovniku vytvorime nove pole
	if str(programNumber) not in infoPMT:
		infoPMT[str(programNumber)] = []

	offset = programInfoLength + 12
	i=0
	while i < sectionLength:
		elementaryPid = bitExtracted(struct.unpack('>L',data[i+offset:i+offset+4])[0],11,24)
		esInfoLength = bitExtracted(struct.unpack('>L',data[i+offset+1:i+offset+5])[0],20,32)
		if elementaryPid != 0x1fff:
			#print("epid "+str(elementaryPid)+ "  " +str(programNumber))
			#vlozeni elementarypid do pole, pokud zde jeste neni
			if str(elementaryPid) not in infoPMT[str(programNumber)]:
				infoPMT[str(programNumber)].append(str(elementaryPid))
		i+=esInfoLength+5

	return False

#funkce pro parsovani descriptoru
def parseDescript(data, length, offset, defTag):
	i = 0
	while i < length:
		#kontrola kvuli pristupu mimo pole
		if i+1 > length:
			break
		size = data[i+offset+1]
		tag = data[i+offset]
		if tag == defTag:
			#vraceni dat z descriptoru
			tmp = data[i+offset:i+offset+2+size]
			return tmp
		i += size+2
	return None 

#funkce pro parsovani tabulky NIT
def parseNIT(data):
	#inicializace promenych 
	networkName = ""
	bandwidth = constellation = guard = coderate = 0
	#kontrola id tabulky
	tableID = bitExtracted(struct.unpack('>L',data[0:4])[0],0,8)
	if tableID != 0x40:
		return True

	networkID = bitExtracted(struct.unpack('>L',data[1:5])[0],16,32)
	networkDescLength = bitExtracted(struct.unpack('>L',data[6:10])[0],20,32)
	#parsovani descriptoru, ktery vraci pole dat nebo None
	#0x40 = tag network name
	tmp = parseDescript(data, networkDescLength, 10, 0x40)
	if tmp != None:
		size = tmp[1]
		#vytvoreni network jmena 
		networkName = "".join(map(chr, tmp[2:size+2]))
	
	loopoffset = 10+networkDescLength+2
	transportStreamLength = bitExtracted(struct.unpack('>L',data[loopoffset-4:loopoffset])[0],20,32)
	i = 0
	while i < transportStreamLength:
		offsetTerrestrial = loopoffset + 6 + i
		tsDescLength = bitExtracted(struct.unpack('>L',data[loopoffset+2:loopoffset+6])[0],20,32)
		#parsovani descriptoru
		terrestrial = parseDescript(data, tsDescLength, offsetTerrestrial, 0x5a)
		if terrestrial != None:
			#bandwidth 000 = 8 mhz | 001 = 7 mhz | 010 = 6 mhz | 011 = 5 mhz
			bandwidth = bitExtracted(struct.unpack('>L',terrestrial[6:10])[0],0,3)
			#constellation 00 = QPSK | 01 = 16-QAM | 10 =64-QAM 
			constellation = bitExtracted(struct.unpack('>L',terrestrial[6:10])[0],8,10)
			#guard 00 = 1/32 | 01 = 1/16 | 10 =1/8 | 11 = 1/4
			guard = bitExtracted(struct.unpack('>L',terrestrial[6:10])[0],19,21)
			#coderateHP 000 = 1/2 | 001 = 2/3 | 010 = 3/4 | 011 = 5/6 | 100 = 7/8
			coderate = bitExtracted(struct.unpack('>L',terrestrial[6:10])[0],13,16)

		i += 6 + tsDescLength

	return False, bandwidth, constellation, guard, coderate, networkName, str(networkID)

#funkce pro parsovani tabulky SDT
def parseSDT(data, pusi, sdtData, infoSDT, newSDT, sdt):
	#pokud je nastaven pusi bit packetu, 
	#ukladej data do sdtData dokud nedorazi dalsi pusi bit
	if pusi:
		if not newSDT:
			sdt = not sdt
		newSDT = not newSDT
	#pokud nemam vsechna data ukladej je do pole
	if sdt:
		sdtData +=bytearray(data)
	#jinak zacni parsovat tabulku SDT
	if not sdt:
		#inicializace promenych
		providerName = serviceName = ""
		sectionLength = bitExtracted(struct.unpack('>L',data[0:4])[0],12,24)
		#kontrola id tabulky
		tableID = bitExtracted(struct.unpack('>L',data[0:4])[0],0,8)
		if tableID != 0x42:
			return sdt, newSDT
		i = 11 # offset
		while i < sectionLength-1:
			descLength = bitExtracted(struct.unpack('>L',sdtData[i+1:i+5])[0],20,32)
			serviceID = bitExtracted(struct.unpack('>L',sdtData[i:i+4])[0],0,16)
			#parsovani descriptoru
			desc = parseDescript(sdtData, descLength, i+5, 0x48)
			if desc != None:
				providerNameLength = desc[3]
				serviceNameLength = desc[providerNameLength+4]
				providerName = "".join(map(chr, desc[4:providerNameLength+4]))
				serviceName = "".join(map(chr, desc[providerNameLength+5:serviceNameLength+providerNameLength+5]))
				#ulozeni provider name, service name do slovniku
				infoSDT[str(serviceID)] = [serviceName, providerName]
			i += descLength + 5
		return sdt, newSDT

	else:
		return sdt, newSDT

#funkce pro prevod  bandwidth z tabulky NIT na retezec 
#(vypis do souboru) nebo hodnotu (vypocet bitrate)
def bandwidthConvert(value, what):
	if value == 0 and what == "str": return "8"
	elif value == 1 and what == "str": return "7"
	elif value == 2 and what == "str": return "6"
	elif value == 3 and what == "str": return "5"
	elif value == 0 and what == "int": return 8
	elif value == 1 and what == "int": return 7
	elif value == 2 and what == "int": return 6
	elif value == 3 and what == "int": return 5

#funkce pro prevod  coderate z tabulky NIT na retezec 
#(vypis do souboru) nebo hodnotu (vypocet bitrate)
def coderateConvert(value, what):
	if value == 0 and what == "str": return "1/2"
	elif value == 1 and what == "str": return "2/3"
	elif value == 2 and what == "str": return "3/4"
	elif value == 3 and what == "str": return "5/6"
	elif value == 4 and what == "str": return "7/8"
	elif value == 0 and what == "int": return 1/2
	elif value == 1 and what == "int": return 2/3
	elif value == 2 and what == "int": return 3/4
	elif value == 3 and what == "int": return 5/6
	elif value == 4 and what == "int": return 7/8

#funkce pro prevod  guard z tabulky NIT na retezec 
#(vypis do souboru) nebo hodnotu (vypocet bitrate)
def guardConvert(value, what):
	if value == 0 and what == "str": return "1/32"
	elif value == 1 and what == "str": return "1/16"
	elif value == 2 and what == "str": return "1/8"
	elif value == 3 and what == "str": return "1/4"
	elif value == 0 and what == "int": return 32/33
	elif value == 1 and what == "int": return 16/17
	elif value == 2 and what == "int": return 8/9
	elif value == 3 and what == "int": return 4/5

#funkce pro prevod constellation z tabulky NIT na retezec 
#(vypis do souboru) nebo hodnotu (vypocet bitrate)
def constellationConvert(value, what):
	if value == 0 and what == "str": return "QPSK"
	elif value == 1 and what == "str": return "16-QAM"
	elif value == 2 and what == "str": return "64-QAM"
	elif value == 0 and what == "int": return 1/4
	elif value == 1 and what == "int": return 1/2
	elif value == 2 and what == "int": return 3/4

#funkce pro vypocet bitrate
#https://dveo.com/broadcast/bit-rate-calculation-for-COFDM-modulation.shtml
def bitrate(bandwidth, constellation, coderate, guard):
	A = round(54000000*(188/204),2)
	B = A * (bandwidthConvert(bandwidth,"int")/8)
	C = B * constellationConvert(constellation,"int")
	D = C * coderateConvert(coderate, "int")
	result = D * guardConvert(guard,"int")

	return round(result,2)

#hlavni beh programu
def main(argv):
	#kontrola argumentu
	if len(argv) > 2 or len(argv) < 2:
		printError("Chybny pocet argumentu programu")
	#kontrola existence souboru
	if not path.exists(argv[1]):
		printError("Soubor neexistuje")

	#inicializace promenych a datovych poli
	n = p = nitpid = bandwidth = constellation = guard = coderate = 0
	pat = nit = sdt = pmt = newSDT = True
	networkName = networkID = ""
	sdtData = bytearray()
	infoPAT = {}
	packets = {}
	infoSDT = {}
	infoPMT = {}

	#otevreni souboru
	binaryfile = open(argv[1],'rb')
	while True:
		#nastaveni pozice pro cteni
		binaryfile.seek(n, 0)
		#nacteni jednoho packetu
		packetarray = binaryfile.read(PACKETSIZE)
		#pokud neni prazdny -> parse
		if packetarray:
			############## PARSE PACKET #######################
			adaptionField = []
			payload = []
			#extrakce hlavicky paketu
			header = struct.unpack('>L',packetarray[0:4])[0]

			#extrakce jednotlivych poli z hlavicky
			syncByte = bitExtracted(header,0,8)
			transportError = bitExtracted(header,8,9)
			pusi = bitExtracted(header,9,10)
			pid = bitExtracted(header,11,24)
			adaptionFieldControl = bitExtracted(header,26,28)
			afLen = 0

			#pokud je packet chybny, nic nedelej
			if syncByte != 0x47 or pid == 0x1fff or transportError:
				pass
			#jinak ho zpracuj
			else:
				if (adaptionFieldControl == 0x2) or (adaptionFieldControl == 0x3):
					afLen = int(packetarray[4])
					adaptionField = packetarray[HEADERSIZE:HEADERSIZE+afLen]
				if (adaptionFieldControl == 0x1) or (adaptionFieldControl == 0x3):
					if pusi:
						afLen+=1
					payload = packetarray[HEADERSIZE+afLen:]

				#print("sync= " + str(hex(syncByte)) + "\n" + "transportError= " + str(hex(transportError)) + "\n" 
				#	 + "payloadStart= " + str(hex(pusi)) + "\n"  
				#	 + "pid= " + str(hex(pid)) + "\n"  
				#	 + "adaption_field_control= " + str(hex(adaptionFieldControl)) )
			############## END PARSE PACKET #######################

				#PAT
				if pid == 0x00 and pat:
					pat, nitpid = parsePAT(payload, infoPAT)
				#NIT
				elif pid == nitpid and nit:
					nit, bandwidth, constellation, guard, coderate, networkName, networkID = parseNIT(payload)
				#SDT
				elif pid == 0x11 and sdt:
					sdt, newSDT = parseSDT(payload, pusi, sdtData, infoSDT, newSDT, sdt)
				#PMT
				elif not pat and runPMT(pid, infoPAT):
					pmt = parsePMT(payload, infoPMT)

			#pocitani packetu pro jednotlive PID
			if str(pid) not in packets:
				packets[str(pid)] = 1
			else:
				packets[str(pid)] = packets[str(pid)] + 1
			#celkovy pocet packetu
			p += 1
			#posunuti zarazky pro cteni souboru
			n += PACKETSIZE
		else:
			break
	
	#zavreni vstupniho souboru
	binaryfile.close()
	#vypocet bitrate
	totalBitrate = bitrate(bandwidth, constellation, coderate, guard)
	#incializace slovniku, kde je ulozena dvojice (pid, retezec obsahujici service name, provider name, bitrate)
	result = {}
	#prochazim jednotlive programnumber
	for key in infoSDT:
		#ziskam jeho pid
		piid = infoPAT[key]
		#prochazim jednotlive elementarypid v PMT tabulce 
		for epid in infoPMT[key]:
			if epid in packets:
				#pokud je pid platne prictu pocet PMT packetu kvuli vypoctu bitrate 
				val = packets.get(piid) + packets.get(epid)
				packets[piid] = val
		#vypocet bitrate a zaokrouhleni na 2 des. mista
		computeBitrate = round(((packets[piid] / p) * totalBitrate) / 1000000,2)
		result[int(piid)] = "0x"+'{:04x}'.format(int(piid)) + "-" + infoSDT[key][1] + "-" + infoSDT[key][0] + ": " + str(computeBitrate) + " Mbps\n"
		

	#otevreni vystupniho souboru a zapis
	output = argv[1].rsplit('.', 1)[0]
	outFile = open(output+".txt", "w")
	outFile.write("Network name: " + networkName +"\n" +
			"Network ID: " + networkID +"\n" +
			"Bandwidth: " + bandwidthConvert(bandwidth,"str") +" MHz\n" +
			"Constellation: " + constellationConvert(constellation,"str") + "\n" +
			"Guard interval: " + guardConvert(guard, "str") +"\n" +
			"Code rate: " + coderateConvert(coderate,"str") + "\n\n")
	#serazeni podle pid a vypis retezcu
	for key in sorted(result):
		outFile.write(result[key])
	outFile.close()

	sys.exit(0)

if __name__ == "__main__":
	main(sys.argv)