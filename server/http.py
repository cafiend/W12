from twisted.web import server, resource, static
from twisted.internet import reactor
from twisted.python import log

from vxcontroller import vx
import re

_mime_types = {'css':'text/css','js':'application/x-javascript','jpeg':'image/jpeg','jpg':'image/jpeg', 'gif':'image/gif','png':'image/png'}

class VxHTTPResource(resource.Resource):
	isLeaf = True
	
	def __init__(self):
		resource.Resource.__init__(self)
	
	# Render / - Index
	def render_Root(self, request):
		request.write("<html><body>")
		request.write("<p>Using the default template.</p>")
		request.write("<ul>")
		
		apps = vx.getConnectedApplications()
		
		if len(apps) == 0:
			request.write("No application are connected")
		else:
			# Write each application
			for app in apps:
				request.write("<li>")
				href = '<a href="%(id)s">Application-%(id)s</a>' % {'id':app}
				request.write(href)
				request.write("</li>")

		
		request.write("</ul>")
		request.write("<p>Using the test template.</p>")
		request.write("<ul>")
		
		apps = vx.getConnectedApplications()
		
		if len(apps) == 0:
			request.write("No application are connected")
		else:
			# Write each application
			for app in apps:
				request.write("<li>")
				href = '<a href="/test/%(id)s">Application-%(id)s</a>' % {'id':app}
				request.write(href)
				request.write("</li>")

		
		request.write("</ul>")
		request.write("<p><a href='/html_test.html'>HTML5 Test Page</a></p>")
		request.write("</body></html>")
		request.finish()
		
		# This is the correct way to do this
		return server.NOT_DONE_YET
	
	# Render a connected application
	def render_AvailableApplication(self, request, appid, template='index.template'):
		
		# Get WebSocket Handler for this application
		handler = vx.getWebSocketHandlerPath(appid)
		
		# Get html that will be served
		templateFile = open(template)
		template = templateFile.read()
		templateFile.close()
		
		# Inject correct handler into html
		html = template % {'id':handler}
		
		# Write html
		request.write(html)
		
		request.finish()
		
		return server.NOT_DONE_YET
		
	# Render an application that is in use
	def render_UnavailableApplication(self, request, clientID):
		return"<html><body>Application is already in use</body></html>"
	
	# Render application that has not connected
	def render_UnknownApplication(self, request, clientID):
			return "<html><body>Unknown Application</body></html>"
	
	# Appropriately render an application
	def render_Application(self, request, template='index.template'):
		appid = request.path.split('/')[-1]
		if appid in vx.getConnectedApplications():
			
			if vx.applicationIsAvailable(appid):
				return self.render_AvailableApplication(request, appid, template)
			else:
				return self.render_UnavailableApplication(request, appid)
				
		return self.render_UnknownApplication(request, appid)

	# Serve js files
#	def render_JavaScript(self, request, f):
#		# Get JS that will be served
#		jsFile = open(f)
#		js = jsFile.read()
#		jsFile.close()
#		request.setHeader("content-type", "text/javascript");
#		return js

	# Serve css files
#	def render_css(self, request, f):
#		# Get CSS that will be served
#		cssFile = open(f)
#		css = cssFile.read()
#		cssFile.close()
#		request.setHeader("content-type", "text/css");
#		return css

	# Render the html template test page
	def render_templateTestPage(self, request):
		
		# Get html that will be served
		templateFile = open('test.template')
		template = templateFile.read()
		templateFile.close()
		
		# Write html
		request.write(template)
		
		request.finish()
		
		return server.NOT_DONE_YET

	#Render all static content
	def render_Static(self, request, filepath):
		f = None
		try:
			f = open(filepath)
		except IOError:
			request.setResponseCode(404)
			return "<html><body>File not found</body></html>"
		content = f.read()
		f.close()
		ext = filepath.split('.')[-1] # get everything after the last period
		request.setHeader("content-type", _mime_types[ext])
		return content
		
	# Hander GET request
	def render_GET(self, request):

		if request.path == '/html_test.html':
			return self.render_templateTestPage(request)
		elif re.match("/test/\d+", request.path):
			return self.render_Application(request, "test.template")
		elif re.match("/\d+", request.path):
			return self.render_Application(request)
		elif re.match(".+\.(css|js|gif|png|jpeg|jpg)$", request.path):
			f = request.path.split('/', 1)[-1]
			return self.render_Static(request, f)
#		elif re.match("/jqueryui/js/.+\.js", request.path) or re.match("/((draw(_pr)?)|(processing))\.js", request.path):
#			f = request.path.split('/', 1)[-1]
#			return self.render_JavaScript(request, f)
#		elif re.match("/jqueryui/css/.+\.css", request.path):
#			f = request.path.split('/', 1)[-1]
#			return self.render_css(request, f)

		return self.render_Root(request)

