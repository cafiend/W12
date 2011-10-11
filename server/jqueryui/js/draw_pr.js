function drawArc(p, x, y, width, height, start, stop) {
	p.arc(x,y,width,height,start,stop);
	//p.println("arc: " + x + ", " + y + ", " + width + ", " + height + ", " + start + ", " + stop);
}

function drawEllipse(p, x, y, width, height) {
	p.ellipse(x,y,width,height);
	//p.println("ellipse: " + x + ", " + y + ", " + width + ", " + height);
}

function drawLine2D(p, x0, y0, x1, y1) {
	p.line(x0, y0, x1, y1);
	//p.println("line2d: " + x0 + ", " + y0 + ", " + x1 + ", " + y1);
}

function drawPoint2D(p, x, y) {
	p.point(x, y);
	//p.println("point2d: " + x + ", " + y);
}

function drawQuad(p, x0, y0, x1, y1, x2, y2, x3, y3) {
	p.quad(x0, y0, x1, y1, x2, y2, x3, y3);
	//p.println("quad: " + x0 + ", " + y0 + ", " + x1 + ", " + y1 + ", "+ x2 + ", " + y2 + ", " +x3 + ", " + y3);
}

function drawRectangle(p, x, y, width, height) {
	p.rect(x,y,width,height);
	//p.println("rect: " + x + ", " + y + ", " + width + ", " + height);
}

function drawTriangle(p, x0, y0, x1, y1, x2, y2, x3, y3) {
	p.triangle(x0, y0, x1, y1, x2, y2);
	//p.println("triangle: " + x0 + ", " + y0 + ", " + x1 + ", " + y1 + ", "+ x2 + ", " + y2);
}

function drawText(p, x, y, text) {
	p.text(text, x, y);
}

function setBackground(p, r, g, b) {
	p.background(r,g,b);
}

function setColorMode(p, mode, r0, r1, r2) {
	/* mode is actually a processing constant int */
	p.colorMode(mode, r0, r1, r2);
	//p.println("colorMode: " + mode + ", " + r0 + ", " + r1 + ", " + r2);
}

function handleEvent(cmd, p) {
	$('#output').text(cmd);
	
	/* Processing doesn't always deal well with incorrect datatypes. 
	 * Casting numbers explicitly helps smooth things over
	 */
	items = cmd.split(' ')
	if (items[0] == "TXT") {
		var name = items.shift();
		var x = parseInt(items.shift());
		var y = parseInt(items.shift());
		var text = items.join(" ");
		drawText(p,x,y,text);
	} else if (items[0] == "ARC") {
		drawArc(p, parseInt(items[1]), parseInt(items[2]), parseInt(items[3]), parseInt(items[4]), parseFloat(items[5]), parseFloat(items[6]));
	} else if (items[0] == "ELIP") {
		drawEllipse(p, parseInt(items[1]), parseInt(items[2]), parseInt(items[3]), parseInt(items[4]));
	} else if (items[0] == "LI2D") {
		drawLine2D(p, parseInt(items[1]), parseInt(items[2]), parseInt(items[3]), parseInt(items[4]));
	} else if (items[0] == "PO2D") {
		drawPoint2D(p, parseInt(items[1]), parseInt(items[2]));
	} else if (items[0] == "QUAD") {
		drawQuad(p, parseInt(items[1]), parseInt(items[2]),parseInt(items[3]), parseInt(items[4]), parseInt(items[5]), parseInt(items[6]), parseInt(items[7]), parseInt(items[8]));
	} else if (items[0] == "RECT") {
		drawRectangle(p, parseInt(items[1]), parseInt(items[2]), parseInt(items[3]), parseInt(items[4]));
	} else if (items[0] == "TRI") {
		drawTriangle(p, parseInt(items[1]), parseInt(items[2]),parseInt(items[3]), parseInt(items[4]), parseInt(items[5]), parseInt(items[6]));
	} else if (items[0] == "BG") {
		setBackground(p, parseInt(items[1]), parseInt(items[2]),parseInt(items[3]));
	} else if (items[0] == "CM_D") {
		setColorMode(p, parseInt(items[1]), parseInt(items[2]),parseInt(items[3]), parseInt(items[4]));
	} else if (items[0] == "CM_F") {
		setColorMode(p, parseInt(items[1]), parseFloat(items[2]),parseFloat(items[3]), parseFloat(items[4]));
	} else if (items[0] == "ST_D") {
		p.stroke(parseInt(items[1]), parseInt(items[2]),parseInt(items[3]));
	} else if (items[0] == "ST_F") {
		p.stroke(parseFloat(items[1]), parseFloat(items[2]),parseFloat(items[3]));
	} else if (items[0] == "FI_D") {
		p.fill(parseInt(items[1]), parseInt(items[2]),parseInt(items[3]));
	} else if (items[0] == "FI_F") {
		p.fill(parseFloat(items[1]), parseFloat(items[2]),parseFloat(items[3]));
	} else if (items[0] == "STW") {
		p.strokeWeight(parseInt(items[1]));
	} else if (items[0] == "NOST") {
		p.noStroke();
	} else if (items[0] == "NOFI") {
		p.noFill();
	} else if (items[0] == "PUSH_STYLE") {
		p.pushStyle();
	} else if (items[0] == "POP_STYLE") {
		p.popStyle();
	} else if (items[0] == "SIZE") {
		p.size(parseInt(items[1]), parseInt(items[2]));
	} else if (items[0] == "ELIP_MODE") {
		p.ellipseMode(parseInt(items[1]));
	} else if (items[0] == "RECT_MODE") {
		p.rectMode(parseInt(items[1]));
	} else if (items[0] == "ST_CAP") {
		p.strokeCap(items[1]);
	} else if (items[0] == "ST_JOIN") {
		p.strokeJoin(items[1]);
	} else if (items[0] == "BEGIN_SHAPE") {
		p.beginShape(items[1]);
	} else if (items[0] == "END_SHAPE") {
		p.endShape(items[1]);
	} else if (items[0] == "VERTEX") {
		p.vertex(parseInt(items[1]), parseInt(items[2]));
	} else if (items[0] == "CLEAR") {
		clearCanvas(p);
	}
}

function sendExpose(ws) {
	ws.send("EVENT EXPOSE\n")
}
function sendClick(ws, x, y, b) {
	str = "EVENT CLICK " + x + " " + y + " " + b + "\n"
	ws.send(str)
}
function sendMouseDown(ws, x, y, b) {
	str = "EVENT MDOWN " + x + " " + y + " " + b + "\n"
	ws.send(str)
}
function sendMouseDrag(ws, x, y, dx, dy, b) {
	str = "EVENT MDRAG " + x + " " + y + " " + dx + " " + dy + " " + b + "\n"
	Processing.instances[0].println(str);
	ws.send(str)
}
function sendMouseMove(ws, x, y, dx, dy) {
	str = "EVENT MMOVE " + x + " " + y + " " + dx + " " + dy + "\n"
	Processing.instances[0].println(str);
	ws.send(str)
}
function clearCanvas(p) {
	p.background(255);
}

function setupProcessing(p) {
	p.noLoop();
}


//Standard stuff. Set websocket.
$(document).ready(function() {
	
	var ws = new WebSocket(ws_server);
	var root_canvas = document.getElementById("root");
	var context = root_canvas.getContext("2d");
	//root_canvas.addEventListener("click", OnClick, false);

	//This will need to be more flexible.
	var canvas = document.getElementById("root");
	// attaching the sketchProc function to the canvas
	var p = new Processing(canvas, setupProcessing);
	clearCanvas(p);
	
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
	
	/*This might need to be revisited.
	function OnClick(e) {
		var x;
		var y;

		if (e.pageX != undefined && e.pageY != undefined) {
				x = e.pageX;
				y = e.pageY;
		}
		else {
				x = e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
				y = e.clientY + document.body.scrollTop + document.documentElement.scrollTop;
		}
		
		x -= root_canvas.offsetLeft;
		y -= root_canvas.offsetTop;
		Processing.instances[0].println("!!! ctrl key " + e.ctrlKey + ". shift key " + e.shiftKey + ". alt key " + e.altKey + ". meta key " + e.metaKey + ". button " + e.button + ". related target "+ e.relatedTarget);
		sendClick(ws, x, y)
	}
	
	$("canvas").live('click', clickHandler);
	$("canvas").live('mouseup', function(e) {
		// this provides click events for non-primary mouse buttons
		if (e.button != 0) {
			$(this).trigger('click', e);
		}
	});
	
	$(root_canvas).click( function(eventObj) {
		var x;
		var y;
		if (eventObj.pageX != undefined && eventObj.pageY != undefined) {
			x = eventObj.pageX;
			y = eventObj.pageY;
		}
		else {
			x = eventObj.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
			y = eventObj.clientY + document.body.scrollTop + document.documentElement.scrollTop;
		}
		var str = "x = " + eventObj.clientX + " + " + document.body.scrollLeft + " + " + document.documentElement.scrollLeft + " - " + $(this).offset().left + 
			"\n" +
			"y = " + eventObj.clientY + " + " + document.body.scrollTop + " + " + document.documentElement.scrollTop + " - " + $(this).offset().top;
		x -= $(this).offset().left;
		y -= $(this).offset().top;
		//Processing.instances[0].println(str);
		Processing.instances[0].println("jquery::(x,y) = (" + x + "," + y + ")" );
		//Processing.instances[0].println("ctrl key " + eventObj.ctrlKey + ". shift key " + eventObj.shiftKey + ". alt key " + eventObj.altKey + ". meta key " + eventObj.metaKey + ". button " + eventObj.button + ". related target "+ eventObj.relatedTarget);
		sendClick(ws, x, y);
	});
	*/
	$(function() {
		$( ".draggable" ).draggable({cancel: "canvas"});
		$( "div.draggable, canvas" ).disableSelection();
	});
	
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
});
