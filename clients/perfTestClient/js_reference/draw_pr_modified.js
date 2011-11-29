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

/*

Modified for the sake of the javascript-only version

This file should not be used anywhere else.

*/
var handler = new Object();
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
	
	var root_canvas = document.getElementById("root");
	var context = root_canvas.getContext("2d");
	//root_canvas.addEventListener("click", OnClick, false);

	//This will need to be more flexible.
	var canvas = document.getElementById("root");
	// attaching the Processing engine to the canvas
	var p = new Processing(canvas);
	p.noLoop();

	$( ".draggable" ).draggable({cancel: "canvas"});
	$( "div.draggable, canvas" ).disableSelection();
	
	function handleEvent(cmd, p) {
		cmd = eval(cmd);
		//p.println(cmd.name + ", args: " + cmd.args);
		
		switch(cmd.name) {
			case "TXT":
				p.text(cmd.args[0], cmd.args[1], cmd.args[2]);
			break;
			case "ARC":
				p.arc(cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3], cmd.args[4], cmd.args[5]);
			break;
			case "ELIP":
				p.ellipse(cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3]);
			break;
			case "LI2D":
				p.line(cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3]);
			break;
			case "PO2D":
				p.point(cmd.args[0], cmd.args[1]);
			break;
			case "QUAD":
				p.quad(cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3], cmd.args[4], cmd.args[5], cmd.args[6], cmd.args[7]);
			break;
			case "RECT":
				p.rect(cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3]);
			break;
			case "TRI":
				p.triangle(cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3], cmd.args[4], cmd.args[5]);
			break;
			case "BG":
				p.background(cmd.args[0], cmd.args[1], cmd.args[2]);
			break;
			case "CM_D":
			case "CM_F":
				/* NB there's now redundancy since we don't need to cast arguments anymore */
				p.colorMode(cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3]);
			break;
			case "ST_D":
			case "ST_F":
				p.stroke(cmd.args[0], cmd.args[1], cmd.args[2]);
			break;
			case "FI_D":
			case "FI_F":
				p.fill(cmd.args[0], cmd.args[1], cmd.args[2]);
			break;
			case "STW":
				p.strokeWeight(cmd.args[0]);
			break;
			case "NOST":
				p.noStroke();
			break;
			case "NOFI":
				p.noFill();
			break;
			case "PUSH_STYLE":
				p.pushStyle();
			break;
			case "POP_STYLE":
				p.popStyle();
			break;
			case "SIZE":
				p.size(cmd.args[0], cmd.args[1]);
				$(canvas).parent().width(cmd.args[0]);
				$(canvas).parent().height(cmd.args[1]);
			break;
			case "ELIP_MODE":
				p.ellipseMode(cmd.args[0]);
			break;
			case "RECT_MODE":
				p.rectMode(cmd.args[0]);
			break;
			case "ST_CAP":
				p.strokeCap(cmd.args[0]);
			break;
			case "ST_JOIN":
				p.strokeJoin(cmd.args[0]);
			break;
			case "BEGIN_SHAPE":
				p.beginShape(cmd.args[0]);
			break;
			case "END_SHAPE":
				p.endShape(cmd.args[0]);
			break;
			case "VERTEX":
				p.vertex(cmd.args[0], cmd.args[1]);
			break;
			case "LOAD_FONT":
				var f = p.loadFont(cmd.args[0], cmd.args[1]);
				_loaded_fonts[cmd.args[0]] = f;
				p.textFont(f);
			break;
			case "TXT_FONT":
				var f = _loaded_fonts[cmd.args[0]];
				if (f == null) {
					f = p.loadFont(cmd.args[0], cmd.args[1]);
				}
				p.textFont(f);
			break;
			case "PUSH_MAT":
				p.pushMatrix();
			break;
			case "POP_MAT":
				p.popMatrix();
			break;
			case "TRANSL_2i":
			case "TRANSL_2f":
				p.translate(cmd.args[0], cmd.args[1]);
			break;
			case "ROTATE":
				p.rotate(cmd.args[0]);
			break;
			case "CLEAR":
				/* TODO: evaluate if this command even makes sense...*/
				clearCanvas(p);
			break;
			case "REG_CB":
				for(i in cmd.args) {
				var event = cmd.args[i];
					if (event in _cb) {
						_cb[event] = true;
						if ( event === 'DROP' ) {
							canvas.addEventListener('dragover', dropHelper, false);
							canvas.addEventListener('drop', dropListener, false);
						}
					}
				}
			break;
			case "CB_KEY_T":
				if (cmd.args[0] === "ALL") {
					_cb['KEYST'] = true;
				} else {
					_cb['KEYST'] = cmd.args;
				}
			break;
			case "CB_KEY_T_STOP":
				if (cmd.args.length > 1)
					_keys['TYP'] = cmd.args;
				else
					_keys['TYP'] = cmd.args[0];
			break;
			case "CB_KEY_P":
				if (cmd.args[0] === "ALL") {
					_cb['KEYSP'] = true;
				} else {
					_cb['KEYSP'] = cmd.args;
				}
			break;
			case "CB_KEY_P_STOP":
				if (cmd.args.length > 1)	
					_keys['PRE'] = cmd.args;
				else
					_keys['PRE'] = cmd.args[0];
			break;
			case "CB_KEY_R":
				if (cmd.args[0] === "ALL") {
					_cb['KEYSR'] = true;
				} else {
					_cb['KEYSR'] = cmd.args;
				}
			break;
			case "VAR":
				setRemoteVariable(cmd.args[0], cmd.args[1]);
			break;
			case "NEW_TXT_AREA":
				/* Create a floating TextArea. Position is relative to the main display. */
				/* Add the new TextArea to the list of defined TextAreas before returning. */
				var root = $("#root");
				var str = "<textarea id='"+cmd.args[0]+"'";
				str += (cmd.args[5] == 1) ? " readonly/>" : "/>";
				$(root).parent().append(str);
				var txt_area = $("#"+cmd.args[0]);
				var o = new Object;
				o.left = $(root).offset().left + cmd.args[1];
				o.top = $(root).offset().top + cmd.args[2]; 
				$(txt_area).offset(o);
				$(txt_area).width(cmd.args[3]);
				$(txt_area).height(cmd.args[4]);
				_text_areas[cmd.args[0]] = txt_area;
				// $(txt_area).keypress( p.keyPressed() );
				// $(txt_area).keyup( p.keyReleased() );
			break;
			case "TXT_AREA_CSS":
				var id 	= cmd.args[0];
				var n	= cmd.args[1];
				var v	= cmd.args[2];
				if (id in _text_areas) {
					var txt_area = _text_areas[id];
					$(txt_area).css(n, v);
				}
			break;
			case "OVERWRITE":
				if (cmd.args[0] in _text_areas) {
					$("#"+cmd.args[0]).text(cmd.args[1]);
				}
			break;
			case "APPEND":
				if (cmd.args[0] in _text_areas) {
					var t = $("#"+cmd.args[0]).text();
					$("#"+cmd.args[0]).text(t + cmd.args[1]);
				}
			break;
			case "PRELOAD":
				// $("body").prepend("<div class='preload' style='font-family:"+cmd.args[0]+";'>preloaded: "+cmd.args[0]+"</div>");
				// $(".preload").hide();
			break;
			default:
				p.println("Received an unknown command:: " + cmd.name + " " + cmd.args);
		}
	}
	
	handler.handleEvent = function(str) {handleEvent("("+str+")", p)};


	function clearCanvas(p) {
		p.background(255);
	}
});
