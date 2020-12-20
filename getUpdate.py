import re
from urllib import request
import os
import configparser
import tkinter.messagebox
import tkinter
import webbrowser

def isOldVersion(a, b):
  for i in range(3):
    if a[i] != b[i]:
      return a[i] < b[i]
  return False

header = {
  "Host": "github.com",
  "Refer": "https://github.com/Hell-Tractor/KingofCoding/blob/master/settings.ini",
  "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
  "Accept-Encoding": "gzip, deflate, br",
  "Accept-Language": "zh-CN,zh;q=0.9",
  "Connection": "keep-alive",
  "Sec-Fetch-Dest": "document",
  "Sec-Fetch-Mode": "navigate",
  "Sec-Fetch-Site": "same-origin",
  "Sec-Fetch-User": "?1",
  "Upgrade-Insecure-Requests": "1",
  "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36"
}

url = 'https://github.com/Hell-Tractor/KingofCoding/blob/master/settings.ini'

request.urlretrieve(url, './settings.html')

with open('./settings.html', 'r', encoding='utf-8') as f:
  html = f.read()
  tag = re.findall('tag</span>=[a-zA-Z]+', html)[0].split('</span>=')[1]
  version = [int(i) for i in re.findall('version</span>=[0-9]+\.[0-9]+\.[0-9]+', html)[0].split('</span>=')[1].split('.')]

os.remove('./settings.html')

config = configparser.ConfigParser()
path = './settings.ini'
config.read(path)

cur_tag = config['version']['tag'].split(';')[0]
cur_version = [int(i) for i in config['version']['version'].split('.')]

if cur_tag < tag or (cur_tag == tag and isOldVersion(cur_version, version)):
  top = tkinter.Tk()
  top.withdraw()
  top.update()
  res = tkinter.messagebox.askokcancel(title="New version release", message="A new version has been released\nclick ok to download")
  top.destroy()
  if res:
    ver = 'v%d.%d.%d-%s' % (version[0], version[1], version[2], tag.lower())
    webbrowser.open('https://github.com/Hell-Tractor/KingofCoding/releases/download/%s/KingofCoding-%s-x64.zip' % (ver, ver), new=2)