/*

The MIT License (MIT)
Copyright (c) 2011 Derek Ingrouville, Julien Lord, Muthucumaru Maheswaran

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/

//Standard stuff. Set websocket.
$(document).ready(function() {
	
	var _loaded_fonts = new Array;
	var _pmx = -1;
	var _pmy = -1;
	/* Initialise callback checks to false */
	var _cb = new Array();
	_cb['CLICK'] = false;
	_cb['MMOVE'] = false;
	_cb['MDRAG'] = false;
	_cb['MDOWN'] = false;
	
	var ws = new WebSocket(ws_server);
	var root_canvas = document.getElementById("root");
	var context = root_canvas.getContext("2d");
	//root_canvas.addEventListener("click", OnClick, false);

	//This will need to be more flexible.
	var canvas = document.getElementById("root");
	// attaching the Processing engine to the canvas
	var p = new Processing(canvas);
	p.noLoop();
		
	ws.onmessage = function(evt) {
		handleEvent(evt.data, p);
	}
	ws.onopen = function(evt) {
		$('#conn_status').html('<b>Connected</b>');
		sendExpose(ws);
	}
	ws.onerror = function(evt) {
		$('#conn_status').html('<b>Error</b>');
	}
	ws.onclose = function(evt) {
		clearCanvas(p);
		$('#conn_status').html('<b>Closed</b>');
	}
	$(function() {
		$( ".draggable" ).draggable({cancel: "canvas"});
		$( "div.draggable, canvas" ).disableSelection();
	});
	function clearCanvas(p) {
		p.background(255);
	}
	
	function handleEvent(cmd, p) {
		$('#output').text(cmd);
		
		/* Processing doesn't always deal well with incorrect datatypes. 
		 * Casting numbers explicitly helps smooth things over
		 * 
		 * Also... this is messy. I dislike it. Needs to be refactored eventually.
		 */
		items = cmd.split(' ')
		var cmd_name = items.shift();
		if (cmd_name == "TXT") {
			var x = parseInt(items.shift());
			var y = parseInt(items.shift());
			var text = items.join(" ");
			p.text(text, x, y);
		} else if (cmd_name == "ARC") {
			p.arc(parseInt(items[0]),parseInt(items[1]),parseInt(items[2]),parseInt(items[3]),parseFloat(items[4]),parseFloat(items[5]));
		} else if (cmd_name == "ELIP") {
			p.ellipse(parseInt(items[0]),parseInt(items[1]),parseInt(items[2]),parseInt(items[3]));
		} else if (cmd_name == "LI2D") {
			p.line(parseInt(items[0]), parseInt(items[1]), parseInt(items[2]), parseInt(items[3]));
		} else if (cmd_name == "PO2D") {
			p.point(parseInt(items[0]), parseInt(items[1]));
		} else if (cmd_name == "QUAD") {
			p.quad(parseInt(items[0]), parseInt(items[1]), parseInt(items[2]), parseInt(items[3]), parseInt(items[4]), parseInt(items[5]), parseInt(items[6]), parseInt(items[7]));
		} else if (cmd_name == "RECT") {
			p.rect(parseInt(items[0]),parseInt(items[1]),parseInt(items[2]),parseInt(items[3]));
		} else if (cmd_name == "TRI") {
			p.triangle(parseInt(items[0]), parseInt(items[1]), parseInt(items[2]), parseInt(items[3]), parseInt(items[4]), parseInt(items[5]));
		} else if (cmd_name == "BG") {
			p.background(parseInt(items[0]),parseInt(items[1]),parseInt(items[2]));
		} else if (cmd_name == "CM_D") {
			p.colorMode(parseInt(items[0]), parseInt(items[1]), parseInt(items[2]), parseInt(items[3]));
		} else if (cmd_name == "CM_F") {
			p.colorMode(parseInt(items[0]), parseFloat(items[1]), parseFloat(items[2]), parseFloat(items[3]));
		} else if (cmd_name == "ST_D") {
			p.stroke(parseInt(items[0]), parseInt(items[1]),parseInt(items[2]));
		} else if (cmd_name == "ST_F") {
			p.stroke(parseFloat(items[0]), parseFloat(items[1]),parseFloat(items[2]));
		} else if (cmd_name == "FI_D") {
			p.fill(parseInt(items[0]), parseInt(items[1]),parseInt(items[2]));
		} else if (cmd_name == "FI_F") {
			p.fill(parseFloat(items[0]), parseFloat(items[1]),parseFloat(items[2]));
		} else if (cmd_name == "STW") {
			p.strokeWeight(parseInt(items[0]));
		} else if (cmd_name == "NOST") {
			p.noStroke();
		} else if (cmd_name == "NOFI") {
			p.noFill();
		} else if (cmd_name == "PUSH_STYLE") {
			p.pushStyle();
		} else if (cmd_name == "POP_STYLE") {
			p.popStyle();
		} else if (cmd_name == "SIZE") {
			var x = parseInt(items[0]);
			var y = parseInt(items[1]);
			p.size(x, y);
			$(canvas).parent().width(x);
			$(canvas).parent().height(y);
		} else if (cmd_name == "ELIP_MODE") {
			p.ellipseMode(parseInt(items[0]));
		} else if (cmd_name == "RECT_MODE") {
			p.rectMode(parseInt(items[0]));
		} else if (cmd_name == "ST_CAP") {
			p.strokeCap(items[0]);
		} else if (cmd_name == "ST_JOIN") {
			p.strokeJoin(items[0]);
		} else if (cmd_name == "BEGIN_SHAPE") {
			p.beginShape(items[0]);
		} else if (cmd_name == "END_SHAPE") {
			p.endShape(items[0]);
		} else if (cmd_name == "VERTEX") {
			p.vertex(parseInt(items[0]), parseInt(items[1]));
		} else if (cmd_name == "LOAD_FONT") {
			var size = parseInt(items.shift());
			var str = items.join(" ");
			var f = p.loadFont(str, size);
			_loaded_fonts[str] = f;
			p.textFont(f)
		} else if (cmd_name == "TXT_FONT") {
			var size = parseInt(items.shift());
			var str = items.join(" ");
			var f = _loaded_fonts[str];
			if (f == null) {
				f = p.loadFont(str, size);
			}
			p.textFont(f);
		} else if (cmd_name == "PUSH_MAT") {
			p.pushMatrix();
		} else if (cmd_name == "POP_MAT") {
			p.popMatrix();
		} else if (cmd_name == "TRANSL_2i") {
			p.translate(parseInt(items[0]), parseInt(items[1]));
		} else if (cmd_name == "TRANSL_2f") {
			p.translate(parseFloat(items[0]), parseFloat(items[1]));
		} else if (cmd_name == "ROTATE") {
			p.rotate(parseFloat(items[0]));
		} else if (cmd_name == "CLEAR") {
			clearCanvas(p);
		} else if (cmd_name == "REG_CB") {
			for(i in items) {
				var event = items[i];
				if (event in _cb) {
					_cb[event] = true;
				}
			}
		}
	}
	function sendExpose(ws) {
		ws.send("EVENT EXPOSE\n")
	}
	function sendClick(ws, x, y, b) {
		if (!_cb['CLICK']) {
			return false;
		}
		str = "EVENT CLICK " + x + " " + y + " " + b + "\n"
		ws.send(str);
		
	}
	function sendMouseDown(ws, x, y, b) {
		if (!_cb['MDOWN']) {
			return false;
		}
		str = "EVENT MDOWN " + x + " " + y + " " + b + "\n"
		ws.send(str)
	}
	function sendMouseDrag(ws, x, y, dx, dy, b) {
		if (!_cb['MDRAG']) {
			return false;
		}
		str = "EVENT MDRAG " + x + " " + y + " " + dx + " " + dy + " " + b + "\n"
		ws.send(str)
	}
	function sendMouseMove(ws, x, y, dx, dy) {
		if (!_cb['MMOVE']) {
			return false;
		}
		str = "EVENT MMOVE " + x + " " + y + " " + dx + " " + dy + "\n"
		ws.send(str)
	}
	/* These should probably only be defined if callbacks exist 		*/
	/* Requires generating these methods on-demand on the server-side 	*/
	/* if and only a callback has been registered for the action.		*/
	/* This would help lighten network load for sure.					*/
	p.mouseReleased = function() {
		//this.println("Released (up/click)("+this.mouseX+","+this.mouseY+"), button: " + this.mouseButton);
		sendClick(ws, this.mouseX, this.mouseY, this.mouseButton);
	};
	
	p.mousePressed = function() {
		//this.println("Pressed (down) ("+this.mouseX+","+this.mouseY+"), button: " + this.mouseButton);
		sendMouseDown(ws, this.mouseX, this.mouseY, this.mouseButton);
	};
	p.mouseDragged = function() {
		sendMouseDrag(ws, this.mouseX, this.mouseY, this.mouseX - this.pmouseX, this.mouseY - this.pmouseY, this.mouseButton);
	}
	p.mouseMoved = function() {
		sendMouseMove(ws, this.mouseX, this.mouseY, this.mouseX - this.pmouseX, this.mouseY - this.pmouseY);
	}
	/* Adds mouse tracking beyond the boundaries of the canvas */
	$(document).mousemove(function(e) {
		var x;
		var y;
		if (_pmx === -1) _pmx = x;
		if (_pmy === -1) _pmy = y;
		if (e.pageX != undefined && e.pageY != undefined) {
			x = e.pageX;
			y = e.pageY;
		}
		else {
			x = e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
			y = e.clientY + document.body.scrollTop + document.documentElement.scrollTop;
		}
		
		x -= $(canvas).offset().left;
		y -= $(canvas).offset().top;
		
		sendMouseMove(ws, x, y, _pmx, _pmy);
		_pmx=x;
		_pmy=y;
	});
});
