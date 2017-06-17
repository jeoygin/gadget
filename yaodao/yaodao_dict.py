#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import json
import cookielib, urllib2, urllib
import getpass
import argparse
import hashlib
import requests

class SmartRedirectHandler(urllib2.HTTPRedirectHandler):
    def http_error_302(self, req, fp, code, msg, headers):
        result = urllib2.HTTPRedirectHandler.http_error_302(self, req, fp, code, msg, headers)
        result.status = code
        result.headers = headers
        return result

fake_header = [
  ('User-Agent', 'Mozilla/5.0 (Macintosh Intel Mac OS X 10_10_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36'),
  ('Content-Type', 'application/x-www-form-urlencoded'),
  ('Cache-Control', 'no-cache'),
  ('Accept', '*/*'),
  ('Connection', 'Keep-Alive'),
]

class YaoDaoDict(object):
  def __init__(self, username, password, cookie_filename='youdao_cookie', local_store='store'):
    self.username = username
    self.password = password
    self.cookie_filename = cookie_filename
    self.local_store = local_store
    self.cj = cookielib.LWPCookieJar(cookie_filename)
    if os.access(cookie_filename, os.F_OK):
      self.cj.load(cookie_filename, ignore_discard=True, ignore_expires=True)
    self.opener = urllib2.build_opener(
      SmartRedirectHandler(),
      urllib2.HTTPHandler(debuglevel=0),
      urllib2.HTTPSHandler(debuglevel=0),
      urllib2.HTTPCookieProcessor(self.cj)
    )
    self.opener.addheaders = fake_header

  def login(self):
    self.cj.clear()
    self.opener.open('http://account.youdao.com/login?back_url=http://dict.youdao.com&service=dict')
    login_data = urllib.urlencode({
      'app'  : 'web',
      'tp'  : 'urstoken',
      'cf'  : '7',
      'fr'  : '1',
      'ru'  : 'http://dict.youdao.com',
      'product'  : 'DICT',
      'type'  : '1',
      'um'  : 'true',
      'username'  : self.username,
      'password'  : self.password,
      'savelogin' : '1',
    })
    response = self.opener.open('https://logindict.youdao.com/login/acc/login', login_data)
    if response.headers.get('Set-Cookie').find(self.username) > -1:
      self.cj.save(self.cookie_filename, ignore_discard=True, ignore_expires=True)
      return True
    else:
      return False

  def query(self, keyfrom, key, word):
    local_file = '{}/{}.json'.format(self.local_store, word.lower())
    if os.access(local_file, os.F_OK):
      try:
        with open(local_file, 'r') as f:
          data = json.load(f.read())
          if 'basic' in data:
            return data
      except Exception:
        pass

    url = 'http://fanyi.youdao.com/openapi.do?keyfrom={}&key={}&type=data&doctype=json&version=1.1&q={}'.format(keyfrom, key, word)
    r = requests.get(url)
    if r.status_code == 200:
      with open(local_file, 'w') as f:
        f.write(json.dumps(r.json()))
      return r.json()
    else:
      return None

  def add_word(self, word, desc, phonetic, tags):
    post_data = urllib.urlencode({
      'word' : word,
      'phonetic' : unicode(phonetic).encode('utf-8'),
      'desc': unicode(desc).encode('utf-8'),
      'tags' : unicode(tags).encode('utf-8'),
    })
    self.opener.addheaders = fake_header + [
      ('Referer', 'http://dict.youdao.com/wordbook/wordlist'),
    ]
    response = self.opener.open('http://dict.youdao.com/wordbook/wordlist?action=add', post_data)
    return response.headers.get('Location') == 'http://dict.youdao.com/wordbook/wordlist'
