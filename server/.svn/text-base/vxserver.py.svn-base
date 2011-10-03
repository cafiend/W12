from twisted.application import service
from twisted.internet.protocol import ServerFactory, Protocol
from twisted.protocols.basic import LineReceiver
from twisted.python import log
from vxcontroller import vx

import command

class VxProtocol(LineReceiver):

	delimiter = "\n"

	def __init__(self):
		self.setLineMode()
		self.id = None

	def connectionMade(self):
		client = self.transport.getHost()
		log.msg('application at %s connected' % client)
		self.id = vx.registerApplication(client, self)
	
	def connectionLost(self, reason):
		log.msg('application (%s) disconnected' % self.id)
		vx.unregisterApplication(self.id)
		
	def lineReceived(self, data):
		cmd = command.process(data)
		self.processCommand(cmd)
		
	def processCommand(self, cmd):
		if cmd['name'] == "TXT":
			vx.pushWebSocketEvent(self.id, cmd)
			return
			
		if cmd['name'] == "RECT":
			vx.pushWebSocketEvent(self.id, cmd)
			return
			
		if cmd['name'] == "CLEAR":
			vx.pushWebSocketEvent(self.id, cmd)
			self.sendEvent("EVENT EXPOSE\n")
			return
	
	def sendEvent(self, event):
		self.transport.write(event)


class VxFactory(ServerFactory):
	protocol = VxProtocol

	def __init__(self):
		pass

