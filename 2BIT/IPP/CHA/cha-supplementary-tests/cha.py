#!/usr/bin/env python3

#CHA:xkolec07
#import knihoven
import sys, argparse, re, os

#----------------------------------------------------------------
#TISK CHYBOVYCH HLASENI -----------------------------------------
#----------------------------------------------------------------
def printError(msg, code):
  print(msg, file=sys.stderr)
  exit(code)

     
#----------------------------------------------------------------
#ZPRACOVANI ARGUMENTU--------------------------------------------
#----------------------------------------------------------------
def argument():
  parser = argparse.ArgumentParser(description='Projekt do IPP, CHA', add_help=False)
  parser.add_argument('--help', dest="help", action='store_true', default=False, help='vypise napovedu')
  parser.add_argument('--input', dest='input', action='store',default=False, help='vstupni soubor')
  parser.add_argument('--output', dest='output', action='store', default=False, help='vystupni soubor')
  parser.add_argument('--pretty-xml', dest='prettyxml', action='store', default=False, const='4', nargs='?', help='odsazeni o K mezer')
  parser.add_argument('--no-inline', dest='noinline', action='store', default=False,nargs='?',const='1', help='preskoci funkce se specifikatorem inline')
  parser.add_argument('--max-par', dest='maxpar', action='store', default=False,help='skript bude brat v uvahu pouze funkce ktere maji n ci mene parametru')
  parser.add_argument('--no-duplicates', dest='duplicates', action='store', nargs='?',const='1', default=False, help='pri vyskytu vice funkci se stejnym jmenem se ulozi pouze prvni z nich')
  parser.add_argument('--remove-whitespace', dest='whitespace', action='store',nargs='?',const='1', default=False, help='odstrani bile znaky')

  try:
    args=parser.parse_args()

  except:
    printError('spatny argument',1)
  i = len(sys.argv)
  #kontrola duplicitnich argumentu
  c= 0 
  c1= 0 
  c2= 0 
  c3= 0 
  c4= 0 
  c5= 0 
  c6= 0 
  c7= 0
  for p in range(i):
    if sys.argv[p] == '--help':
      c = c+1
    if sys.argv[p].find('--input=') != -1:
      c1 = c1+1
    if sys.argv[p].find('--output=') != -1:
      c2 = c2+1
    if sys.argv[p].find('--pretty-xml') != -1:
      c3 = c3+1
    if sys.argv[p].find('--no-inline') != -1:
      c4 = c4+1
    if sys.argv[p].find('--max-par=') != -1:
      if len(sys.argv[p]) == 10:#kontrola zda je zadane n u max-par
        printError('neni zadane n u max-par',1)
      c5 = c5+1
    if sys.argv[p].find('--no-duplicates') != -1:
      c6 = c6+1
    if sys.argv[p].find('--remove-whitespace') != -1:
      c7 = c7+1

  if c>1 or c1>1 or c2>1 or c3>1 or c4>1 or c5>1 or c6>1 or c7>1:
    printError('nejaky argument byl zadan vicekrat', 1)

  #max-par cislo
  if args.maxpar:
    if args.maxpar.isdigit() == False:
      printError("n neni cislo u --max-par",1)

  #pretty-xml cislo
  if args.prettyxml:
    if args.prettyxml.isdigit() == False:
      printError("n neni cislo u --pretty-xml",1)

  #help
  if args.help:
    # help muze byt jen sam
    if len(sys.argv) == 2:
      parser.print_help()
      sys.exit(0)
    else:
      printError('help musi byt sam',1)

  return args 

#----------------------------------------------------------------
#PROCHAZENI ADRESAROVE STRUKTURY---------------------------------
#----------------------------------------------------------------
def godir(adresa,adresar,nazev):
  #pocet souboru
  pocet = len(os.listdir(adresa))
  #pokud neni zadny 
  if pocet == 0:
    return
  #jinak prochazim a ukladam si .h soubory do poli(nazev,adresar)
  for namefile in os.listdir(adresa):
    current = os.path.join(adresa, namefile)
    #pokud je to slozka
    if os.path.isdir(current): 
      godir(current,adresar,nazev)
    #pokud je to soubor
    elif os.path.isfile(current):
      #musi to byt .h soubor
      if re.search(".*[.]h", namefile) != None:
        adresar.append(adresa)
        nazev.append(namefile)


#----------------------------------------------------------------
#PRETTY XML------------------------------------------------------
#----------------------------------------------------------------
def printline():
  global par
  global vystup
  #pokud je zadan parametr --pretty-xml
  if par.prettyxml:
    #pridam na vystup novy radek
    vystup += "\n"   

def printspace():
  global par
  global vystup
  #pokud je zadan parametr --pretty-xml
  if par.prettyxml:
    #pridam na vystup mezery 
    vystup += " "*(int(par.prettyxml))

#----------------------------------------------------------------
#DETECTION ------------------------------------------------------
#----------------------------------------------------------------
def detection(adresar,celacesta,nazev):
  #globalni promenne
  global vystup
  global par
  #otevru soubor
  try:
    f = open(celacesta, encoding='utf-8')
  except:
    printError("open error", 2)
  #nactu obsah souboru a zavru soubor
  soubor = f.read()
  f.close() 
  #odtsrani #makra
  soubor=re.sub('\#.*', '', soubor,  re.M) 
  #odstani komentare //
  soubor=re.sub('[\/]{2}.*', '', soubor)
  #odstani enum a struct
  soubor = re.sub('typedef\s+enum\s+\s*{[^{}]*}\s*([a-zA-Z0-9_]+)\s*;','',soubor,re.M)
  soubor = re.sub('struct\s+\s*{[^{}]*}\s*([a-zA-Z0-9_]+)\s*;','',soubor,re.M)
  soubor = re.sub('typedef\s+struct\s+\s*{[^{}]*}\s*([a-zA-Z0-9_]+)\s*;','',soubor,re.M)
  soubor = re.sub('struct\s+\s*([a-zA-Z0-9_]+)\s*;','',soubor)

  count = 0 #pocet zanoreni
  state = 0 #STAVY: 0=start 1={} 2=/**/
  code ="" #vysledny kod
  #prochazim soubor po znacich
  for c in range(len(soubor)):
    char = soubor[c]
    #start
    if state == 0: 
      if char == '{':
        state = 1
        count = count + 1
        continue
      elif char == '/':
        c = c + 1
        char = soubor[c]
        if char == '*': 
          state = 2
          count = count + 1
          continue
      else:
        code += soubor[c]
    # {}    
    elif state == 1:
      if char == '{':
        count = count + 1
      elif char == '}':
        count = count - 1
        if count == 0:
          code += ';'
          state = 0

    # /**/
    elif state == 2: 
      if char == '/':
        c = c + 1
        char = soubor[c]
        if char == '*':
          count = count + 1
      elif char == '*':
        c = c + 1
        char = soubor[c]
        if char == '/':
          count = count - 1
          if count == 0:
            state = 0
  #vsechny /n nahradim za mezeru
  code = code.replace('\n',' ')
  #ulozim si vsechny funkce do pole, 
  #funkce jsou oddelene strednikem
  polefunkci = code.split(';')
  seznamnazvu = []
  #prochazim jednotlive funkce
  for funkce in polefunkci:
    #odstranim mezery na zacatku a konci
    funkce = funkce.strip()
    #prazdne funkce preskocit
    if funkce == '':
      continue
    #najdu pozici zavorek()
    lzav = funkce.find('(', 0)
    pzav = funkce.rfind(')', 0)
    #ulozim si typ a nazev funkce
    typnazev = funkce[0:lzav]
    #obsah zavorky tedy parametry
    zavorka = funkce[lzav+1:pzav]
    #ulozim si nazev funkce
    nazevf = typnazev.rsplit(None,1)[-1]
    #zjistim pozici typu
    l = typnazev.find(nazevf, 0)
    #ulozim si typ funkce a odstranim mezery
    typ = typnazev[0:l]
    typ = typ.strip()

    #parametr --inline, preskocim tyto funkce
    if par.noinline: 
      if typnazev.find('inline') != -1:
        continue

    #promenlivy pocet parametru
    vararg = 'no'
    if re.search("\.\.\.",zavorka) != None:
      vararg = 'yes'

    #parametr --max-par
    if par.maxpar:
      #zjistim pocet parametru, jsou oddeleny carkou
      pocetparam = len(zavorka.split(','))
      if vararg == 'yes':
        pocetparam = pocetparam - 1
      #preskocim funkce s vetsim poctem parametru
      if pocetparam > int(par.maxpar):
        continue

    #parametr duplicate
    dup = 0
    if par.duplicates:
      for a in seznamnazvu:
        #pokud existuje jiz funkce se stejnym nazvem 
        if a == nazevf:
          dup = 1
      #tak ji preskocim
      if dup == 1:
        continue
      seznamnazvu.append(nazevf)

    #parametr whitespace
    if par.whitespace:
      #nahrazeni whitespace jednou mezerou
      typ = ' '.join(typ.split())
      if typ.find('*') != -1:
        o = typ.find('*')
        if typ[o:].find(' ') != -1:
          if typ[o+1] == ' ':
            typ = typ[0:o+1]+typ[o+2:]
        if typ.find(' ') != -1:
          if typ[o-1] == ' ':
            typ = typ[0:o-1]+typ[o:]

    #urceni atributu file=''
    global vstup
    if vstup == '':
      filename=nazev
    elif vstup == './':
      filename=adresar[2:]+'/'+nazev
    else:
      k=adresar
      if not adresar.endswith('/'):
        k = adresar+'/'
      filename = k.strip(vstup[0:len(vstup)-1])+nazev

    #zapsani funkce na vystup
    printspace()#kvuli pretty-xml
    vystup += '<function file="'+filename+'" name="'+nazevf+'" varargs="'+vararg+'" rettype="'+typ+'">'
    printline()#kvuli pretty-xml

    #ZPRACOVANI PARAMETRU JEDNOTLIVYCH FUNKCI
    pocetparam = 0
    #rozdelim parametry podle carky
    parametry = zavorka.split(',')
    #prochazim jednotlive parametry funkce
    for param in parametry:
      #odstranim mezery na zacatku a konci
      param = param.strip()
      #... preskocim a taky ()
      if re.search("\.\.\.",param) != None or param=='':
        continue
      #void preskocim
      if zavorka.find('void') != -1:
        continue

      #rozsireni PAR
      nazevparam = param.rsplit(None,1)[-1]
      if nazevparam == '*':
        typ = param
      elif nazevparam == 'const':
        typ = param
      elif nazevparam == '**':
        typ = param
      elif nazevparam == 'float':
        typ = param
      elif nazevparam == 'int':
        typ = param
      elif nazevparam == 'char':
        typ = param
      elif nazevparam.find('char[') != -1:
        typ = param
      else:
        typ = re.sub('[\w\[\]]*?$','', param)
        
      typ = typ.strip()
      #--remove-whitespace
      if par.whitespace:
        #nahrazeni whitespace jednou mezerou
        typ = ' '.join(typ.split())
        if typ.find('*') != -1:
          o = typ.find('*')
          if typ[o:].find(' ') != -1:
            if typ[o+1] == ' ':
              typ = typ[0:o+1]+typ[o+2:]
          if typ.find(' ') != -1:    
            if typ[o-1] == ' ':
              typ = typ[0:o-1]+typ[o:]

      pocetparam = pocetparam + 1 #pocitadlo parametru funkce
      #vytisknu element param 
      printspace()#2x kvuli odsazeni --pretty-xml
      printspace()
      vystup += '<param number="'+str(pocetparam)+'" type="'+typ+'" />'
      printline()#kvuli --pretty-xml

    #uzaviraci tag </function>
    printspace()#kvuli --pretty-xml odsazeni
    vystup += "</function>" #ukoncovaci element
    printline()#kvuli --pretty-xml


#----------------------------------------------------------------
#MAIN------------------------------------------------------------
#----------------------------------------------------------------

#zavolani funkce argument() pro zpracovani argumentu
par = argument()

#na vystup pridam hlavicku dokumentu
vystup ='<?xml version="1.0" encoding="utf-8"?>'
printline()#kvuli prettyxml

#seznam souboru
adresar = []
nazev = []

#VSTUP
#neni --input, prohledavame slozku se skriptem
if par.input == False:
  vstup="./"
  vystup += '<functions dir="'+vstup+'">'
  printline()#kvuli prettyxml
  godir(vstup,adresar,nazev)

#je zadán --input
elif par.input:
#je zadana slozka
  if os.path.isdir(par.input):
    vstup = par.input
    #pokud vstup nekonci / pridam ho
    if not par.input.endswith('/'):
      vstup = par.input+'/'
    vystup += '<functions dir="'+vstup+'">'
    printline()#kvuli prettyxml
    godir(vstup,adresar,nazev)

#je zadany soubor koncovku nekontroluju
  elif os.path.isfile(par.input):
    vstup=""
    vystup += '<functions dir="'+vstup+'">'
    printline()#kvuli prettyxml
    nazev.append(par.input)
    adresar.append("")
  else:
    printError("zadany soubor neexistuje",2)

for a, n in zip(adresar,nazev):
  #cela cesta k souboru
  cela = os.path.join(a, n)
  #zavolani funkce pro detekci funkci
  detection(a,cela,n)

#uzavru tag functions
vystup += "</functions>\n"

#vystupni soubor, pokud neni zadan tak na STDOUT
if par.output == False:
  f = sys.stdout
#pokud je zadan, zapisuji do souboru
else:
  try:
    f = open(par.output,'w+',encoding="utf-8")
  except IOError:
    printError('problem s output',3)

#vypisu vystup
f.write(vystup)

#zavru output
f.close()

sys.exit(0)