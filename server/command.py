class Command(object):
	def __init__(self, cmd):
		pass

def process(cmd):
	
	params = cmd.split(' ')
	name = params.pop(0)
	
	handler = lookupHandler(name)
	command = handler(params)
	
	command['raw'] = cmd
	command['name'] = name
	return command
	
def TXT_Handler(params):
	text = ' '.join(params[2:])
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	command['text'] = text
	
	return command
def ARC_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	command['width'] = params[2]
	command['height'] = params[3]
	command['start'] = params[4]
	command['stop'] = params[5]

	return command

def ELIP_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	command['width'] = params[2]
	command['height'] = params[3]
	
	return command

def LINE2D_Handler(params):
	command = {}
	
	command['x0'] = params[0]
	command['y0'] = params[1]
	command['x1'] = params[2]
	command['y1'] = params[3]
	
	return command
	
def POINT2D_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	
	return command

def QUAD_Handler(params):
	command = {}
	
	command['x0'] = params[0]
	command['y0'] = params[1]
	command['x1'] = params[2]
	command['y1'] = params[3]
	command['x2'] = params[4]
	command['y2'] = params[5]
	command['x3'] = params[6]
	command['y3'] = params[7]
	
	return command
				
def RECT_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	command['width'] = params[2]
	command['height'] = params[3]

	return command

def TRI_Handler(params):
	command = {}
	
	command['x0'] = params[0]
	command['y0'] = params[1]
	command['x1'] = params[2]
	command['y1'] = params[3]
	command['x2'] = params[4]
	command['y2'] = params[5]
	
	return command
	
def CLEAR_Handler(params):
	command = {}
	
	return command

def lookupHandler(name):
	"""
	This function kinda looks like ass. Surely there must be a way to
	clean it up...
	"""
	if (name == "TXT"):
		return TXT_Handler
	if (name == "ARC"):
		return ARC_Handler
	if (name == "ELIP"):
		return ELIP_Handler
	if (name == "LI2D"):
		return LINE2D_Handler
	if (name == "PO2D"):
		return POINT2D_Handler
	if (name == "QUAD"):
		return QUAD_Handler
	if (name == "RECT"):
		return RECT_Handler		
	if (name == "TRI"):
		return RECT_Handler	
	if (name == "CLEAR"):
		return CLEAR_Handler
		
