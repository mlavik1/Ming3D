from StringIO import StringIO
from zipfile import ZipFile
from urllib import urlopen

resp = urlopen("https://docs.google.com/uc?export=download&id=1p4GA1lcoKZZcPsGSPOZ6ZWb85yyvmm2b")
zipfile = ZipFile(StringIO(resp.read()))
zipfile.extractall("")