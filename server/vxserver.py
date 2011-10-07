from twisted.application import service
from twisted.internet.protocol import ServerFactory, Protocol
from twisted.protocols.basic import LineReceiver
from twisted.python import log
from vxcontroller import vx

import command

_available_commands = frozenset(
["TXT", "ARC", "ELIP", "LI2D", "PO2D", "QUAD", "RECT", "TRI", 
"BG", "CM_D","CM_F", "ST_D","ST_F", "STW", "NOST", "NOFI", 
"FI_D", "FI_F","PUSH_STYLE","POP_STYLE", "SIZE", "ELIP_MODE", 
"RECT_MODE", "ST_CAP", "ST_JOIN", "BEGIN_SHAPE", "END_SHAPE", "VERTEX"]
);

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
	
		if cmd['name'] in _available_commands:
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

