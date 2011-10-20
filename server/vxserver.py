
# The MIT License (MIT)
# Copyright (c) 2011 Derek Ingrouville, Julien Lord, Muthucumaru Maheswaran

# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
"RECT_MODE", "ST_CAP", "ST_JOIN", "BEGIN_SHAPE", "END_SHAPE", "VERTEX",
"CR_FONT", "TXT_FONT", "LOAD_FONT", "PUSH_MAT", "POP_MAT", "TRANSL_2i", 
"TRANSL_2f","ROTATE","REG_CB"]
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
			# print "Removed call to EXPOSE\n" #self.sendEvent("EVENT EXPOSE\n")
			return
	
	def sendEvent(self, event):
		log.msg("In VxProtocol.sendEvent:: %s" % event) 
		self.transport.write(event)


class VxFactory(ServerFactory):
	protocol = VxProtocol

	def __init__(self):
		pass

