from StringIO import StringIO
from zipfile import ZipFile
from urllib import urlopen

resp = urlopen("https://onedrive.live.com/download?cid=C1835F35C0EF9E9D&resid=C1835F35C0EF9E9D%2114549&authkey=AAik1ummNmp-wb4")
zipfile = ZipFile(StringIO(resp.read()))
zipfile.extractall("")