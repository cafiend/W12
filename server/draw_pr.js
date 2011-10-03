function drawRectangle(p, x, y, width, height) {
	p.rect(x,y,width,height);
}

function drawText(p, x, y, text) {
	//text() method goes wonky if args 2,3 aren't typed right
	x = parseInt(x);
	y = parseInt(y);
	p.text(text, x, y);
}

function handleEvent(cmd, p) {
	$('#output').text(cmd);
	
	items = cmd.split(' ')
	if (items[0] == "TXT") {
		drawText(p, items[1], items[2], items[3])
	} else if (items[0] == "RECT") {
		drawRectangle(p, items[1], items[2], items[3], items[4])
	} else if (items[0] == "CLEAR") {
		clearCanvas(p)
	}
}

function sendExpose(ws) {
	ws.send("EVENT EXPOSE\n")
}

function sendClick(ws, x, y) {
	
	str = "EVENT CLICK " + x + " " + y + "\n"
	ws.send(str)
}

function clearCanvas(p) {
	p.background(255);
}

function setupProcessing(p) {
	p.size(500,500);
	p.background(255);
	p.noLoop();
	p.stroke(0);
	p.strokeWeight(10);
	p.fill(0);
}


//Standard stuff. Set websocket.
$(document).ready(function() {
		
		var ws = new WebSocket(ws_server);
		var root_canvas = document.getElementById("root");
		var context = root_canvas.getContext("2d");
		root_canvas.addEventListener("click", OnClick, false);

		//This will need to be more flexible.
		var canvas = document.getElementById("root");
		// attaching the sketchProc function to the canvas
		var p = new Processing(canvas, setupProcessing);
		clearCanvas(p);
		
		ws.onmessage = function(evt) {
			  handleEvent(evt.data, p)
		}
		ws.onopen = function(evt) {
				$('#conn_status').html('<b>Connected</b>');
				sendExpose(ws)
		}
		ws.onerror = function(evt) {
				$('#conn_status').html('<b>Error</b>');
		}
		ws.onclose = function(evt) {
				clearCanvas()
				$('#conn_status').html('<b>Closed</b>');
		}
		
		//This might need to be revisited.
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

			sendClick(ws, x, y)
		}


});
