from datetime import datetime
from PIL import Image
import requests
from io import BytesIO

class Logger():

    def __init__(self):
        pass

    pass

def getImageFromUrl(url):
    req = requests.get(url)
    return Image.open(BytesIO(req.content))