_zero 	= frozenset(["CLEAR","NOST","NOFI","PUSH_STYLE","POP_STYLE"]);
_one 	= frozenset(["STW", "ELIP_MODE", "RECT_MODE", "ST_CAP", "ST_JOIN", "BEGIN_SHAPE", "END_SHAPE"]);
_two 	= frozenset(["PO2D", "SIZE", "VERTEX"]);
_three	= frozenset(["BG", "ST_D", "ST_F", "FI_D", "FI_F"]);
_four	= frozenset(["RECT", "LI2D", "ELIP", "CM_D", "CM_F"]);
_six 	= frozenset(["ARC", "TRI"]);
_eight	= frozenset(["QUAD"]);

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
	
def ZERO_Handler(params):
	command = {}
	
	return command
def ONE_Handler(params):
	command = {}
	
	command['w'] = params[0]
	
	return command
def TWO_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	
	return command
def THREE_Handler(params):
	command = {}
	
	command['r'] = params[0]
	command['g'] = params[1]
	command['b'] = params[2]
	
	return command
				
def FOUR_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	command['width'] = params[2]
	command['height'] = params[3]

	return command
def SIX_Handler(params):
	command = {}
	
	command['x'] = params[0]
	command['y'] = params[1]
	command['width'] = params[2]
	command['height'] = params[3]
	command['start'] = params[4]
	command['stop'] = params[5]

	return command
def EIGHT_Handler(params):
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

def lookupHandler(name):
	"""
	This function kinda looks like ass. Surely there must be a way to
	clean it up...
	Maybe change this to a static dict with name:function_handle data
	"""
	if (name == "TXT"):
		return TXT_Handler
	if (name in _zero):
		return ZERO_Handler
	if (name in _one):
		return ONE_Handler
	if (name in _two):
		return TWO_Handler
	if (name in _three):
		return THREE_Handler
	if (name in _four ):
		return FOUR_Handler
	if (name in _six):
		return SIX_Handler
	if (name in _eight):
		return EIGHT_Handler
	
	
		
