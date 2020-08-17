#!/usr/bin/env python3
'''
Knihovna pro praci s polynomy.
Neni implementovano umocnovani polynomu.
'''
import string
from itertools import zip_longest

class Polynomial:
  '''
  Trida Polynomial, ktera pracuje s polynomy 
    -vypis polynomu
    -porovnavani polynomu
    -scitani polynomu
    -hodnota polynomu pro zadane x
    -derivace polynomu
  '''
  def __init__(self, *par, **dpar):
    '''Inicializace tridy.
    Instance lze vytvaret tremi zpusoby:
    1.Polynomial([1,-3,0,2])
    2.Polynomial(1,-3,0,2)
    3.Polynomial(x0=1,x3=2,x1=-3)

    Argumenty:
    par -- vstupni polynom
    dpar -- vstupni polynom ve formatu x0=1 atd
    '''
    if dpar: #pokud je zadan ve formatu x0=1 atd.
      sort = sorted(dpar.items()) #seradim prvky
      d = []
      i=0
      last = sort[-1] #ulozim si posledni prvek
      for k,v in sort: #prochazim jednotlive prvky polynomu
        key = 'x'+str(i)#x0,x1,x2 atd
        i = i + 1
        if k != key: #pokud nektery prvek chybi doplnim ho 
          while True: 
            d.append(0) 
            key = 'x'+str(i)
            i = i + 1
            if key == k:
              break
          d.append(v) 
        else:
          d.append(v) 
        if k == last[0]: #pokud jsem na konci polynomu koncim
          break  
      
      self.x = tuple(d)
    elif type(par[0]) is list: #pokud je zadan ve formatu listu
      self.x = tuple(par[0])
    elif type(par) is tuple: #pokud je zadan jako tuple
      self.x = par
      
  def __str__(self):
    '''
    Tisk polynomu ve forme retezce.

    Argumenty:
    self -- vstupni polynom

    Navratova hodnota: polynom ve forme retezce
    '''
    result = ''

    for i, coef in reversed(list(enumerate(self.x))): #prochazim polynom 
      power =  i #mocnina
      if coef: #pokud neni koeficient nula
        if coef < 0: #pokud je zaporny
          coef = -coef
          if result: #nastavim vhodne znamenko, podle toho zda je to zacatek retezce nebo ne
            sign = ' - '
          else:
            sign = '- '
        elif coef > 0: #pokud je kladny
          if result:
            sign = ' + '
          else:
            sign = ''

        if coef == 1 and power != 0: #pokud je koeficient 1 nepisu nic pred x
          str_coef = ''
        else: 
          str_coef = str(coef) 

        if power == 0:
            str_power = ''
        elif power == 1:
            str_power = 'x'
        else: #pokud je to mocnina vetsi jak 1
            str_power = 'x^' + str(power)

        result += sign + str_coef + str_power 
    if result == '': #pokud neni zadan polynom vratim 0
      return str(0)
    return result

  def __eq__(self, other):
    '''
    Porovnavani dvou polynomu.

    Argumenty:
    self -- vstupni polynom1
    other -- vstupni polynom2

    Navratova hodnota: boolean (true pokud jsou shodne)
    '''
    bol = True
    for x,y in zip_longest(self.x,other.x, fillvalue=0):
      if x != y: #pokud se prvky lisi vracim false
        bol = False
    return bol

  def __add__(self, other):
    '''
    Scitani dvou polynomu.

    Argumenty:
    self -- vstupni polynom1
    other -- vstupni polynom2

    Navratova hodnota: secteny polynom
    '''
    return Polynomial([x+y for x,y in zip_longest(self.x,other.x, fillvalue=0)])

  def __pow__(self,value):
    '''
    Umocneni polynomu - neni implementovano

    Argumenty:
    self -- vstupni polynom1

    '''
    return 0

  def at_value(self,value,value2 = 0):
    '''
    Hodnota polynomu pro zadane X

    Argumenty:
    self -- vstupni polynom
    value -- hodnota X podle ktere se ma spocitat vysledna hodnota polynomu
    value2 -- hodnota Y pokud je zadan je vypocitan rozdil mezi hodnotou prvniho a druheho parametru

    Navratova hodnota: hodnota polynomu pro zadane X
    '''
    total = 0 #celkovy vysledek
    if value2 == 0: #pokud neni zadan druhy parametr 
      for power, coeff in enumerate(self.x):
          total += (value**power) * coeff #vypocitam hodnotu polynomu pro zadane value
    else: #pokud je zadan value2
      for power, coeff in enumerate(self.x): #vypocitam hodnotu polynomu pro value2
        total += (value2**power) * coeff
      for power, coeff in enumerate(self.x): #vypocitam hodnotu polynomu pro value kterou odectu
        total -= (value**power) * coeff
    return total

  def derivative(self):
    '''
    Derivace polynomu.

    Argumenty:
    self -- vstupni polynom

    Navratova hodnota: derivace polynomu
    '''
    return Polynomial([self.x[i] * i for i in range(1, len(self.x))])


def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()