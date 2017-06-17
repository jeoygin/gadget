#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import json
import getpass
import argparse
import hashlib

from yaodao_dict import YaoDaoDict

def login(args):
  if args.login:
    if args.username and len(args.username) > 0:
      username = args.username
      print('Username: {}'.format(username))
    else:
      username = raw_input('Username: ')
  
    if args.password and len(args.password) > 0:
      password = args.password
      print('Password: {}'.format('*' * len(password)))
    else:
      password = getpass.getpass('Password: ')
  
    m2 = hashlib.md5()
    m2.update(password)
    password_md5 = m2.hexdigest()
  
    dict = YaoDaoDict(username, password_md5)
    return dict if dict.login else None
  else:
    return YaoDaoDict(None, None)

def main(args):
  dict = login(args)
  if not dict:
    print('Failed to login!')
    return

  f = open(args.list) if args.list != '-' else sys.stdin
  for line in f:
    text = line.strip()
    if len(text) > 0 and not text.startswith('-'):
      word = text
      result = dict.query(args.keyfrom, args.key, word)
      failed = True

      if result and 'basic' in result:
        phonetic = u''
        if 'us-phonetic' in result['basic']:
          phonetic = result['basic']['us-phonetic']
        elif 'uk-phonetic' in result['basic']:
          phonetic = result['basic']['uk-phonetic']
        else:
          phonetic = result['basic']['phonetic']

        desc = u''
        if 'explains' in result['basic']:
          for e in result['basic']['explains']:
            if len(desc) > 0:
              desc += '\n'
            desc += e 

        if dict.add_word(word = word, desc = desc,
                         phonetic = phonetic, tags = ''):
          failed = False
          print("Added {}".format(word))

      if failed:
        print("Failed to add {}".format(word))    

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('list', help='list file')
  parser.add_argument('--login',
                      default=False,
                      type=bool,
                      help='Login')
  parser.add_argument('--username',
                      default=os.getenv('YD_USERNAME'),
                      help='Username')
  parser.add_argument('--password',
                      default=os.getenv('YD_PASSWORD'),
                      help='Password')
  parser.add_argument('--keyfrom',
                      default=os.getenv('YD_KEYFROM'),
                      help='Keyfrom')
  parser.add_argument('--key',
                      default=os.getenv('YD_KEY'),
                      help='Key')
  args = parser.parse_args()
  main(args)
