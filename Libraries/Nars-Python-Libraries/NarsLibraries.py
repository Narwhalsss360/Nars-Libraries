from PIL import Image
import requests
from io import BytesIO

def getImageFromUrl(url):
    req = requests.get(url)
    return Image.open(BytesIO(req.content))

def map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
