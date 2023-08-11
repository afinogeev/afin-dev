import requests
   

class Instrument:

    def __init__(self, host):
        self.__connected = False
        self.__host = 'http://' + host + '.local'
        self.__sess = requests.Session()
        self.__getRequest(url=self.__host)
    
    def isConnected(self):
        return self.__connected
    
    def set(self, params):
        url = self.__host + '/state'
        return self.__getRequest(url=url, params=params)
    
    def get(self):
        url = self.__host + '/state'
        return self.__getRequest(url=url)

    def __checkResponseStatus(self, resp):
        if resp.status_code == 200:
            self.__connected = True
        else:
            self.__connected = False
        
        return self.__connected
    
    def __getRequest(self, url='', params={}):
        try:
            resp = self.__sess.get(url=url, params=params)
            self.__checkResponseStatus(resp)
            return resp.content
        except:
            self.__connected = False
        return ''
    

if __name__ == '__main__':
    pass