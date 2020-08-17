#!/usr/bin/env python3
'''
  Dekorator limit_calls
  Generatorova funkce
  Trida log
'''
import math,os

def limit_calls(max_calls = 2, error_message_tail = 'called too often'):
  '''
  Dekorator pro omezeni poctu volani funkce

  Argumenty:
  max_calls -- maximalni pocet volani funkce
  error_message_tail -- varovna zprava 
  '''
  def decorator(func):
    def wrapper(*args, **kwargs):
      calls = getattr(wrapper, 'calls', 0)
      calls += 1
      if calls >= max_calls:
        raise Exception(error_message_tail)
      setattr(wrapper, 'calls', calls)
      return func(*args, **kwargs)

    return wrapper
  return decorator


def ordered_merge(*arg,selector=[]):
  '''
  Generatorova funkce

  Argumenty:
  arg -- libovolny pocet iterovatelnych objektu
  selector -- udavajici pozici prvku

  Navratova hodnota: list
  '''
  result = []
  i = 0
  l = len(arg)
  for num, s in enumerate(selector):
    result.append(arg[s][i])
    if(num != len(selector)-1):
      if(num%l == l-1 or selector[num+1] == s):
        i += 1

  return(result)

class Log:
  '''
  Trida Log, pro praci se soubory 
  '''
  def __init__(self, par):
    '''
    Inicializace tridy.
    
    Argumenty:
    par -- nazev souboru
    '''
    self.x = par
    self.y = None

  def __enter__(self):
    '''
    Otevreni souboru
    '''
    self.y = open(self.x,"a+")
    return self

  def __exit__(self, exc_type, exc_val, exc_tb):
    '''
    Uzavreni souboru
    '''
    if(exc_type != None or exc_val != None or exc_tb != None):
      self.y.close()
      f1 = open(self.x,'r')
      f2 = open('newfile.txt','a+')
      f2.write('Begin\n')
      f2.write(f1.read())
      f2.write('End\n')
      f2.close()
      f1.close()
      os.remove(self.x)
      os.rename('newfile.txt',self.x)
    else:
      self.y.close()

  def logging(self,value):  
    '''
    Zapis retezce do souboru

    Argumenty:
    value -- retezec pro zapsani
    '''
    self.y.write(value+'\n')





