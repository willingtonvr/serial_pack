# -*- coding: utf-8 -*-
"""
Created on Thu May 11 14:45:47 2017

@author: wito
"""

import requests
r = requests.get('http://localhost:8080/cargador')
print r.json()

#
# Ahora vamos a poner datos
#
print "-------------"
r = requests.post('http://localhost:8080/cargador/tipo',data = {'nombre':'CEX3','voltaje':'12'})
print r.text
print
print r.json()




