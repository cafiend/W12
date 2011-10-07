from twisted.python import log
from websocketServer import VxWebSocketHandler

class VxController:	
	def __init__(self):
		# Connected Application clients
		self.apps = {}
		self.appID = 1   # Counter for application ids
		
		# WebSocket handlers
		self.websocketHandlers = {}
		
		# References to pass messages along to other servers
		self.httpRoot = None    # Root of http resource tree
		self.socketSite = None  # Websocket site
	
	# Setters for other server
	def setHttpRoot(self, root):
		self.httpRoot = root

	def setWebSocketSite(self, site):
		self.site = site
	
	### Application Management ###
	
	# Get the next application id
	def nextApplicationID(self):
		appid = self.appID
		self.appID = self.appID + 1
		return str(appid)
		
	# Register a connected client application
	def registerApplication(self, address, appHandler):
		appid = self.nextApplicationID()
		
		app = {}
		# Set application information
		app['address'] = address
		app['appHandler'] = appHandler
		
		
		# Add WebSocket handler	
		app['wsHandler'] = None
		# Get path of websocket handler
		handlerPath = '/' + appid + '_Handler'
		app['wsHandlerPath'] = handlerPath
		
		# Associate this appid with handlerPath
		self.websocketHandlers[handlerPath] = appid
		# Create handler for handlerLocation
		self.site.addHandler(handlerPath, VxWebSocketHandler)
		
		# Add the client
		self.apps[appid] = app
		
		return appid
		
	# Unregister a client application
	def unregisterApplication(self, appid):
		log.msg("Unregistering application %s" % appid)
		
		# Close websocket connection
		wsHandler = self.apps[appid]['wsHandler']
		if wsHandler != None:
			wsHandler.closeConnection()
		
		del self.apps[appid]
		
	# Return a list of applications
	def getConnectedApplications(self):
		return self.apps.keys()
	
	# Check if appid is a valid connected application
	def isValidApplication(self, appid):
		return appid in self.apps
	
	# Check if application is available to users
	def applicationIsAvailable(self, appid):
		# Application is available if it has no websocket handler
		return self.apps[appid]['wsHandler'] == None
		
	### WebSocket Management ###
	
	# Register a websocket handler and associate it with an appid
	def registerWebSocketHandler(self, handlerPath, handler):
		appid = self.websocketHandlers[handlerPath]
		self.apps[appid]['wsHandler'] = handler

	# Unregister an applications websocket handler
	def unregisterWebSocketHandler(self, handlerPath):
		log.msg("unregistering websocket handler %s" % handlerPath)
		appid = self.websocketHandlers[handlerPath]
		if appid in self.apps:
			self.apps[appid]['wsHandler'] = None
	
	# Get the path to an applications websocket handler
	def getWebSocketHandlerPath(self, appid):
		if not self.isValidApplication(appid):
			return None
		
		return self.apps[appid]['wsHandlerPath']
	
	# Check if a given handler path has an associated handler instance
	def webSocketLocationHasHandler(self, handlerPath):
		appid = self.websocketHandlers[handlerPath]
		return self.apps[appid]['wsHandler'] != None
	
	### Server communication ###
	
	# Send an event to the web socket server
	def pushWebSocketEvent(self, appid, event):
		wsockHandler = self.apps[appid]['wsHandler']
		wsockHandler.sendEvent(event)

	# Send event to vx server
	def pushVxEvent(self, handlerPath, event):
		appid = self.websocketHandlers[handlerPath]
		
		handler = self.apps[appid]['appHandler']
		handler.sendEvent(event)

vx = VxController()
