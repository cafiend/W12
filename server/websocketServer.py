from datetime import datetime

from twisted.internet import task
from websocket import WebSocketHandler, WebSocketSite
from twisted.python import log

import vxcontroller

class VxWebSocketHandler(WebSocketHandler):
	def __init__(self, transport):
		WebSocketHandler.__init__(self, transport)

	def __del__(self):
		log.msg('Deleting websocket handler')

	def frameReceived(self, frame):
		path = self.transport._request.path
		vxcontroller.vx.pushVxEvent(path, frame)

	def connectionMade(self):
		log.msg('Connected to client.')
		path = self.transport._request.path
		vxcontroller.vx.registerWebSocketHandler(path, self)

	def connectionLost(self, reason):
		log.msg('Lost websocket connection.')
		path = self.transport._request.path
		vxcontroller.vx.unregisterWebSocketHandler(path)
	
	def closeConnection(self):
		log.msg('Closing websocket connection')
		self.transport.loseConnection()
	
	def sendEvent(self, event):
		log.msg("Sending Event %s to Browser" % event['name'])
		self.transport.write(event['raw'])
		
