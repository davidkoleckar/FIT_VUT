#!/usr/bin/env python3
'''Kontrola zavorek
   a cesarova sifra.
'''
import itertools 

def balanced_paren(parenstr):
  '''Kontrola zavorek.

  Argumenty:
  parenstr -- vstupni retezec

  Navratova hodnota: boolean
  '''
  stack = []
  paren = {'(': ')', '[': ']', '{': '}', '<' : '>'}

  for char in parenstr: #projdu retezec po znaku
    if char in paren.keys(): #pokud znak je oteviraci zavorka
      stack.append(char) # hodim zavorku na zasobnik
    elif char in paren.values():  #pokud je znak ukoncovaci zavorka
      if paren[stack.pop()] == char:  #pokud jsou zavorky stejenho typu popnu ze zasobniku
        continue
      else:
        return False
  return not stack #pokud neni prazdny zasobnik vratim false

def caesar_list(word, key = [1,2,3]):
  '''Vrati vstupni retezec zakodovany podle klice Caesarovou sifrou.

  Argumenty:
  word -- Vstupni retezec, slozeny z 26 malych pismen anglicke abecedy
  key -- Klic udavajici posun v abecede, ve forme seznamu cisel.

  Navratova hodnota: string
  '''

  sifra = '' #vysledny retezec sifry
  g = itertools.cycle(key) #cyklus pres seznam klicu

  for ch in word:
    if ord(ch) >= ord('a') and ord(ch) <= ord('z'):
      znak = ord(ch) + next(g) #prictu k ascii hodnotu klice
      if znak > ord('z'): 
        znak -= 26
      sifra += chr(znak)
    else:
      raise ValueError('retezec neni z anglickych, malych pismen')

  return sifra


def caesar_varnumkey(word, *key):
  '''Vrati vstupni retezec zakodovany podle klice Caesarovou sifrou.

  Argumenty:
  word -- Vstupni retezec, slozeny z 26 malych pismen anglicke abecedy
  key -- Klic udavajici posun v abecede, ve forme promenneho poctu parametru (cisel).

  Navratova hodnota: string
  '''

  if not key:
    keylist = [1,2,3]

  else:
    keylist = []
    for k in key:
      keylist.append(k)

  sifra = '' #vysledny retezec sifry
  g = itertools.cycle(keylist) #cyklus pres seznam klicu

  for ch in word:
    if ord(ch) >= ord('a') and ord(ch) <= ord('z'):
      znak = ord(ch) + next(g) #prictu k ascii hodnotu klice
      if znak > ord('z'): 
        znak -= 26
      sifra += chr(znak)
    else:
      raise ValueError('retezec neni z anglickych, malych pismen')

  return sifra
