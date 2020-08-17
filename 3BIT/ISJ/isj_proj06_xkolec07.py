#!/usr/bin/env python3
'''
  Neopakujici se znak ze vstupniho retezce
  a matematicke puzzle.
'''
import itertools
def first_nonrepeating(str):
  '''
  Neopakujici se prvni znak ze vstupniho retezce

  Argumenty:
  str -- vstupni retezec

  Navratova hodnota: neopakujici se znak
  '''
  count = {} # zde se uklada pocet vyskytu urciteho znaku
  for c in str:
      if c not in count: #pokud jeste neni ulozen v count
          count[c] = 0
      count[c] += 1
  for c in str:
      if count[c] == 1: #pokud je count urciteho znaku roven 1
          return c #vratim ho 

def combine4(num, res):
  '''
  Matematicke puzzle dle implementace (https://prlbr.de/2016/6652-17/)

  Argumenty:
  num -- list se ctverici kladnych celych cisel
  res -- hodnota vysledku

  Navratova hodnota: seznam unikatnich resenich
  '''
  permutat = list(itertools.permutations(num)) #permutace ctverice vstupnich cisel

  l = ['%d%s%d%s%d%s%d', 
      '(%d%s%d)%s%d%s%d',
      '%d%s(%d%s%d)%s%d',
      '%d%s%d%s(%d%s%d)',
      '(%d%s%d)%s(%d%s%d)',
      '(%d%s%d%s%d)%s%d',
      '%d%s(%d%s%d%s%d)',
      '((%d%s%d)%s%d)%s%d',
      '(%d%s(%d%s%d))%s%d',
      '%d%s((%d%s%d)%s%d)',
      '%d%s(%d%s(%d%s%d))'
  ]

  op = list(itertools.product(['+', '-', '*', '/'],repeat=3)) #kartzesky soucin operatoru
  
  finish = [] #vysledny list moznych reseni

  for a,b,c,d in permutat:
    for x,y,z in op:
      for inner in l:
        try:
          s = inner % (a, x, b,y,c,z,d)
          if(eval(s) == res): #vyhodnotim vyraz a porovnam ho se zadanym vysledkem
            finish.append(s) #v pripade ze se shoduji ulozim vyraz do listu
        except ZeroDivisionError: #muze nastat situace deleni 0
          continue
  return finish


