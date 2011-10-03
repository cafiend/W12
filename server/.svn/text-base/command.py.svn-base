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
	
def RECT_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	command['width'] = params[2]
	command['height'] = params[3]

	return command
	
def CLEAR_Handler(params):
	command = {}
	
	return command

def lookupHandler(name):
	if (name == "TXT"):
		return TXT_Handler
		
	if (name == "RECT"):
		return RECT_Handler
		
	if (name == "CLEAR"):
		return CLEAR_Handler
		
