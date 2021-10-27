from datetime import datetime
from PIL import Image
import requests
from io import BytesIO

class result():

    def __init__(self):
        self.data = None
        self.resultMessage = None
        pass

    pass

class Logger():

    def __init__(self):
        pass

    pass

def getImageFromUrl(url):
    functionResult = result()
    req = requests.get(url)
    functionResult.data = Image.open(BytesIO(req.content))
    return functionResult