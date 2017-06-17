#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import json
import argparse

from yaodao_dict import YaoDaoDict 

def main(args):
  f = open(args.list) if args.list != '-' else sys.stdin
  dict = YaoDaoDict(None, None, 'youdao_cookie')
  for line in f:
    text = line.strip()
    if len(text) > 0 and not text.startswith('-'):
      result = dict.query(args.keyfrom, args.key, text)
      print(text)
      if result and 'basic' in result:
        if 'us-phonetic' in result['basic']:
          print(u'- US: [{}]'.format(result['basic']['us-phonetic']))
        if 'uk-phonetic' in result['basic']:
          print(u'- UK: [{}]'.format(result['basic']['uk-phonetic']))
        if 'explains' in result['basic']:
          for e in result['basic']['explains']:
            print(u'- {}'.format(e))
      print('')

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('list', help='list file')
  parser.add_argument('--keyfrom',
                      default=os.getenv('YD_KEYFROM'),
                      help='Keyfrom')
  parser.add_argument('--key',
                      default=os.getenv('YD_KEY'),
                      help='Key')
  args = parser.parse_args()

  main(args)
