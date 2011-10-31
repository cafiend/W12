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


#include "hlib.h"
#include "command.h"
#include "socket.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static char *int_to_string(int x)
{
    char tmp_buf[33];
    int ret = -1;

    ret = snprintf(tmp_buf, 32, "%d", x);
    if (ret <= 0)
        return NULL;

    tmp_buf[32] = '\0';
    return strdup(tmp_buf);
}

Display *OpenDisplay(char *hostname, int port)
{
    Display *display = NULL;
    Socket *socket = NULL;
    char *port_buf = NULL;

    port_buf = int_to_string(port);
    if (port_buf == NULL)
        return NULL;

    socket = socket_new(Socket_Blocking);
    if (socket_connect(socket, hostname, port_buf) != 0) {
        socket_free(socket);
        free(port_buf);
        return NULL;
    }

    free(port_buf);

    display = (Display *) malloc(sizeof(Display));
    
    display->socket = socket;
    display->hostname = strdup(hostname);
    display->port = port;
    display->callbacks = callbacks_new();
    
    return display;
}

void CloseDisplay(Display *display)
{
    if (display == NULL)
        return;

    printf ("CLOSING CONNECTION FOR DISPLAY %s:%d\n", 
            display->hostname, 
            display->port);

    if (display->hostname != NULL)
        free(display->hostname);

    if (display->socket != NULL)
        socket_free(display->socket);
        
    if (display->callbacks != NULL)
        callbacks_free(display->callbacks);

    free(display);
}

/* Draw an arc from point (x,y) of size (height,width) and angle defined by (start,stop) */
int DrawArc(Display *display, int x, int y, int width, int height, float start, float stop)
{
	Command *cmd = NULL;
	Socket *socket = NULL;
	
	socket = display->socket;
	
	/* command_format("ARC %d %d %d %d %f %f", x, y, width, height, start, stop);*/
	cmd = command_format_json("ARC", "%d %d %d %d %f %f", x, y, width, height, start, stop);
	
	if (cmd == NULL)
		return -1;
		
	if (command_send(cmd, socket) != 0) {
		command_free(cmd);
		return -1;
	}
	
	return 0;
}

/* Draw an ellipse at (x,y) of width X height */
int DrawEllipse(Display *display, int x, int y, int width, int height)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("ELIP %d %d %d %d", x, y, width, height); */
    cmd = command_format_json("ELIP", "%d %d %d %d", x, y, width, height); 
    
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int DrawLine2D(Display *display, int x0, int y0, int x1, int y1)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /*cmd = command_format("LI2D %d %d %d %d", x0, y0, x1, y1);*/
    cmd = command_format_json("LI2D", "%d %d %d %d", x0, y0, x1, y1);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int DrawPoint2D(Display *display, int x, int y)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("PO2D %d %d", x, y); */
    cmd = command_format_json("PO2D", "%d %d", x, y);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int DrawQuad(Display *display, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("QUAD %d %d %d %d %d %d %d %d", x0, y0, x1, y1, x2, y2, x3, y3); */
    cmd = command_format_json("QUAD", "%d %d %d %d %d %d %d %d", x0, y0, x1, y1, x2, y2, x3, y3);
    
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

/* Draw a rectangle at (x,y) of width X height */
int DrawRectangle(Display *display, int x, int y, int width, int height)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("RECT %d %d %d %d", x, y, width, height); */
    cmd = command_format_json("RECT", "%d %d %d %d", x, y, width, height);
    
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int DrawTriangle(Display *display, int x0, int y0, int x1, int y1, int x2, int y2)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("TRI %d %d %d %d %d %d", x0, y0, x1, y1, x2, y2); */
    cmd = command_format_json("TRI", "%d %d %d %d %d %d", x0, y0, x1, y1, x2, y2);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

/* Set the bg colour to a shade of grey, equivalent to Background3i(x,x,x) */
int Background1i(Display *display, int grey) {
	if (grey > 255) {
		/* this might not be necessary since we can in theory change the
		 * bounds on this on the fly... */
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("BG", "%d", grey);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Background3i(Display *display, int r, int g, int b) {
	if (r > 255 || g > 255 || b > 255) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("BG %d %d %d", r, g, b); */
    cmd = command_format_json("BG", "%d %d %d", r, g, b);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int ColorMode1f(Display *display, int mode, float r0) {
	if (mode != RGB && mode != HSB) {
		/* Only valid color modes are RGB and HSB */
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("CM_F", "%d %f", mode, r0);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int ColorMode1i(Display *display, int mode, int r0) {
	if (mode != RGB && mode != HSB) {
		/* Only valid color modes are RGB and HSB */
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("CM_D", "%d %d", mode, r0);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int ColorMode3f(Display *display, int mode, float r0, float r1, float r2) {
	if (mode != RGB && mode != HSB) {
		/* Only valid color modes are RGB and HSB */
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("CM_F %d %f %f %f", mode, r0, r1, r2); */
    cmd = command_format_json("CM_F", "%d %f %f %f", mode, r0, r1, r2);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int ColorMode3i(Display *display, int mode, int r0, int r1, int r2) {
	if (mode != RGB && mode != HSB) {
		/* Only valid color modes are RGB and HSB */
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("CM_D %d %d %d %d", mode, r0, r1, r2); */
    cmd = command_format_json("CM_D", "%d %d %d %d", mode, r0, r1, r2);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Stroke1i(Display *display, int val) {
	if (val > 255) {
		/* same as above, this bounds checking might not be necessary */
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("ST_D", "%d", val);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Stroke1f(Display *display, float val) {
	if (val > 1.0) {
		/* same as above, this bounds checking might not be necessary */
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("ST_F", "%f", val);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Stroke3i(Display *display, int r, int g, int b) {
	if (r > 255 || g > 255 || b > 255) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("ST_D %d %d %d", r, g, b); */
    cmd = command_format_json("ST_D", "%d %d %d", r, g, b);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Stroke3f(Display *display, float r, float g, float b) {
	if (r > 1.0 || g > 1.0 || b > 1.0) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("ST_F %f %f %f", r, g, b); */
    cmd = command_format_json("ST_F", "%f %f %f", r, g, b);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int StrokeWeight(Display *display, int w) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("STW %d", w); */
    cmd = command_format_json("STW", "%d", w);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int NoStroke(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("NOST"); */
    cmd = command_format_json("NOST", "");

    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Fill1i(Display *display, int val) {
	/* There's probably more error checking to do here since
	 * changing the colorMode actually defines the max values 
	 * acceptable here. Default is 255 so we'll start with that...
	 */
	if (val > 255) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("FI_D", "%d", val);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Fill1f(Display *display, float val) {
	/* There's probably more error checking to do here since
	 * changing the colorMode actually defines the max values 
	 * acceptable here. Default is 1.0 so we'll start with that...
	 */
	if (val > 1.0) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("FI_F", "%f", val);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Fill3i(Display *display, int r, int g, int b) {
	/* There's probably more error checking to do here since
	 * changing the colorMode actually defines the max values 
	 * acceptable here. Default is 255 so we'll start with that...
	 */
	if (r > 255 || g > 255 || b > 255) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("FI_D %d %d %d", r, g, b); */
    cmd = command_format_json("FI_D", "%d %d %d", r, g, b);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Fill3f(Display *display, int r, int g, int b) {
	/* There's probably more error checking to do here since
	 * changing the colorMode actually defines the max values 
	 * acceptable here. Default is 1.0 so we'll start with that...
	 */
	if (r > 1.0 || g > 1.0 || b > 1.0) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("FI_F %f %f %f", r, g, b); */
    cmd = command_format_json("FI_F", "%f %f %f", r, g, b);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int NoFill(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("NOFI"); */
    cmd = command_format_json("NOFI", "");
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int PushStyle(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("PUSH_STYLE"); */
    cmd = command_format_json("PUSH_STYLE", "");
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int PopStyle(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("POP_STYLE"); */
    cmd = command_format_json("POP_STYLE", "");
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int Size(Display *display, int width, int height) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("SIZE %d %d", width, height); */
    cmd = command_format_json("SIZE", "%d %d", width, height);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int EllipseMode(Display *display, int mode) {
	if (mode != CENTER && mode != RADIUS && mode != CORNER && mode != CORNERS) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("ELIP_MODE %d", mode); */
    cmd = command_format_json("ELIP_MODE", "%d", mode);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int RectMode(Display *display, int mode) {
	if (mode != CENTER && mode != RADIUS && mode != CORNER && mode != CORNERS) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("RECT_MODE %d", mode); */
    cmd = command_format_json("RECT_MODE", "%d", mode);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int StrokeCap(Display *display, const char* mode) {
	if (strcmp(SQUARE, mode) != 0 && strcmp(PROJECT, mode) != 0 && strcmp(ROUND, mode) != 0) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("ST_CAP %s", mode); */
    cmd = command_format_json("ST_CAP", "\"%s\"", mode);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int StrokeJoin(Display *display, const char* mode) {
	if (strcmp(MITER, mode) != 0 && strcmp(BEVEL, mode) != 0 && strcmp(ROUND, mode) != 0) {
		return -1;
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("ST_JOIN %s", mode); */
    cmd = command_format_json("ST_JOIN", "\"%s\"", mode);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int BeginShape(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("BEGIN_SHAPE "); */
    cmd = command_format_json("BEGIN_SHAPE", "");
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int EndShape(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("END_SHAPE "); */
    cmd = command_format_json("END_SHAPE","");
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int BeginShapeMode(Display *display, int mode) {
	if (mode != POINTS && mode != LINES && mode != TRIANGLES && mode != TRIANGLE_FAN && mode != TRIANGLE_STRIP && mode != QUADS && mode != QUAD_STRIP) {
		return BeginShape(display);
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("BEGIN_SHAPE %d", mode); */
    cmd = command_format_json("BEGIN_SHAPE", "%d", mode);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int EndShapeMode(Display *display, int mode) {
	if (mode != CLOSE ) 
	{
		return EndShape(display);
	}
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("END_SHAPE %d", mode); */
    cmd = command_format_json("END_SHAPE", "%d", mode);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Vertex2D(Display *display, int x, int y) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("VERTEX %d %d", x, y); */
    cmd = command_format_json("VERTEX", "%d %d", x, y);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
/* Used to preload fonts via css @font-face directives */
/* Right now, this relies on @font-face being loaded manually */
/* This could actually add inline <style> tags too and we could have two methods for adding fonts */
int CreateFont(Display *display, const char *fontName, const char *fontURL) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format_json("PRELOAD", "\"%s\" \"%s\"",  fontName, fontURL);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int LoadFont(Display *display, const char *fontName, int size) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("LOAD_FONT %d '%s'", size, fontName); */
    cmd = command_format_json("LOAD_FONT", "\"%s\" %d",  fontName, size);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int TextFont(Display *display, const char *fontName, int size) {
	/* 
	 * Typically, this will try to set the font to an already loaded font.
	 * If the font is not yet loaded, it will try to load it on the js side 
	 */
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("TXT_FONT %d '%s'", size, fontName); */
    cmd = command_format_json("TXT_FONT", "\"%s\" %d", fontName, size);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int TextAlign(Display *display, int h_align, int v_align) {
	return 0;
}
int TextLeading(Display *display, int dist) {
	return 0;
}
int TextSize(Display *display, int size) {
	return 0;
}

int PushMatrix(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("PUSH_MAT"); */
    cmd = command_format_json("PUSH_MAT", "");
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int PopMatrix(Display *display) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("POP_MAT"); */
    cmd = command_format_json("POP_MAT", "");
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Translate2i(Display *display, int x, int y) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("TRANSL_2i %d %d", x, y); */
    cmd = command_format_json("TRANSL_2i", "%d %d", x, y);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Translate2f(Display *display, float x, float y) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("TRANSL_2f %f %f", x, y); */
    cmd = command_format_json("TRANSL_2f", "%f %f", x, y);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Rotate(Display *display, float angle) {
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("ROTATE %f", angle); */
    cmd = command_format_json("ROTATE", "%f", angle);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int Scale1f(Display *display, float scale) {
	return 0;
}
int Scale2f(Display *display, float x, float y) {
	return 0;
}

/* TODO: remove this */
int SendText(Display *display, int x, int y, char *text)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    int textlen = 0;
    
    socket = display->socket;
        
    if (text == NULL)
        return -1;
    
    textlen = strlen(text);
    if (textlen == 0)
        return 0;
        
    //command_format("TXT %d %d %s", x, y, text);
    cmd = command_format_json("TXT", "\"%s\" %d %d", text, x, y);
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
/* NB: Maximum size of text is 122 - strlen(name) */
/* This is partly controlled by the BUFSIZE (currently 128) in command.c and partly due to JSON overhead. */
int OverwriteTextArea(Display *display, const char *target, const char *text)
{
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
        
    if (target == NULL || text == NULL)
        return -1;
    
    if (strlen(target) == 0)
        return 0;
       
    if (strlen(text) == 0)
		return 0;
        
    cmd = command_format_json("OVERWRITE", "\"%s\" \"%s\"", target, text);
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int AppendTextArea(Display *display, const char *target, const char *text)
{
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
        
    if (target == NULL || text == NULL)
        return -1;
    
    if (strlen(target) == 0)
        return 0;
       
    if (strlen(text) == 0)
		return 0;
        
    cmd = command_format_json("APPEND", "\"%s\" \"%s\"", target, text);
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
/* Create a text area over the canvas to receive text directly */
int CreateTextArea(Display *display, const char *id, int x, int y, int width, int height, int readonly)
{
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
        
    if (id == NULL)
        return -1;
    
    if (strlen(id) == 0)
        return 0;
       
    if (readonly != TRUE && readonly != FALSE)
		return 0;
        
    cmd = command_format_json("NEW_TXT_AREA", "\"%s\" %d %d %d %d %d", id, x, y, width, height, readonly);
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int ClearScreen(Display *display)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
      
    /* cmd = command_format("CLEAR"); */         
    cmd = command_format_json("CLEAR", "");
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int RegisterRemoteInt(Display *display, const char* name, int value)
{
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
     
    /* cmd = command_format("VAR %s %d", name, value); */          
    cmd = command_format_json("VAR", "\"%s\" %d", name, value);
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}
int RegisterRemoteFloat(Display *display, const char* name, float value)
{
	Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    /* cmd = command_format("VAR %s %f", name, value); */        
    cmd = command_format_json("VAR",  "\"%s\" %f", name, value);
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

/* TODO: Clean up */
Event *GetEvent(Display *display)
{
    Command *cmd = command_read(display->socket);
    Event *e = NULL;
    int len = 0;
    char* str = NULL;
    
    if (cmd == NULL)
        return NULL;
    
    if (cmd->params[0] == NULL)
        return NULL;
    
    len = strlen(cmd->params[0]);

    /* Check for expose */
    if (len == 6 && strncmp(cmd->params[0], "EXPOSE", 6) == 0) {
        e = event_expose_new();
        return e;
    }
    else if (len == 5 && strncmp(cmd->params[0], "SETUP", 5) == 0) {
    	int w 	= atoi(cmd->params[1]);
		int h 	= atoi(cmd->params[2]);

		e = event_setup_new(w,h);
		/* 
		 * Check the display's callbacks and send registration 
		 * message to the browser side as required.
		 * 
		 * Done in a single message, accumulating hanlders to register.
		 * 
		 * NB: When adding events, this string might need to grow!
		 */
		 str = calloc(60, sizeof(char));
		 if (display->callbacks != NULL) {
			 if(display->callbacks->clickHandlers != NULL) {
				 strcat(str, "\"CLICK\" ");
			 }
			 if(display->callbacks->mouseMoveHandlers != NULL) {
				 strcat(str, "\"MMOVE\" ");
			 }
			 if(display->callbacks->mouseDownHandlers != NULL) {
				 strcat(str, "\"MDOWN\" ");
			 }
			 if(display->callbacks->mouseDragHandlers != NULL) {
				 strcat(str, "\"MDRAG\" ");
			 }
			 if(display->callbacks->fileDropInitHandlers != NULL ||
					 display->callbacks->b64FileDropInitHandlers != NULL) {
				 /* A single callback handler for both un-encoded and base64 encoded transfers */
				 strcat(str, "\"DROP\" ");
			 }
			 if(display->callbacks->resizeHandlers != NULL) {
				 strcat(str, "\"RESIZE\" ");
			 }
			 /* Change all but the last space to a comma */
			 int i = 0;
			 for(i = 0; i < strlen(str)-1; i++) {
				if (str[i] == ' ') {
					str[i] = ',';
				}
			}
			/* Eventually, check the return value to determine success here */
			SendRegisterCallbackMsg(display, str);
		 }
		 free(str);
		return e;
	}
    else if (len == 5 && strncmp(cmd->params[0], "CLICK", 5) == 0) {
        int x = atoi(cmd->params[1]);
        int y = atoi(cmd->params[2]);
        int button = atoi(cmd->params[3]);
        e = event_click_new(x, y, button);
        return e;
    }
    else if (len == 5 && strncmp(cmd->params[0], "MDOWN", 5) == 0) {
        int x = atoi(cmd->params[1]);
        int y = atoi(cmd->params[2]);
        int button = atoi(cmd->params[3]);
        e = event_mousedown_new(x, y, button);
        return e;
    }
    else if (len == 5 && strncmp(cmd->params[0], "MMOVE", 5) == 0) {
        int x 	= atoi(cmd->params[1]);
        int y 	= atoi(cmd->params[2]);
        int dx	= atoi(cmd->params[3]);
        int dy	= atoi(cmd->params[4]);
        
        e = event_mousemove_new(x, y, dx, dy);
        return e;
    }
    else if (len == 5 && strncmp(cmd->params[0], "MDRAG", 5) == 0) {
        int x 	= atoi(cmd->params[1]);
        int y 	= atoi(cmd->params[2]);
        int dx	= atoi(cmd->params[3]);
        int dy	= atoi(cmd->params[4]);
        int button = atoi(cmd->params[5]);
        e = event_mousedrag_new(x, y, dx, dy, button);
        return e;
    }
    else if (len == 7 && strncmp(cmd->params[0], "PRELOAD", 7) == 0) {
        e = event_preload_new();
        return e;
    }
    else if (len == 6 && strncmp(cmd->params[0], "RESIZE", 6) == 0) {
		int w 	= atoi(cmd->params[1]);
		int h 	= atoi(cmd->params[2]);

		e = event_resize_new(w,h);
		return e;
	}
    else if (len == 4 && strncmp(cmd->params[0], "DROP", 4) == 0) {
		/* It's a paired message approach. First message sets the event
		 * metadata and establishes the number of chunks required for transfer.
		 *
		 * Message 0 format:
		 * 		EVENT DROP INIT <filename> <filetype> <filesize>
		 * 		Caveat: Filetype might not be known. Javascript support for it seems dodgy.
		 *
		 * 1 to N-1 Message pairs then follow in the format:
		 * 		a. EVENT DROP CHUNK <filename> <filetype> <filesize> <current chunk>
		 * 		b. <file chunk X>
		 *
		 * Final Message format:
		 * 		EVENT DROP END <filename> <filetype> <filesize>
		 */
    	char *name	= strdup(cmd->params[2]);
		char *type	= strdup(cmd->params[3]);
		unsigned int size	= atoi(cmd->params[4]);
		unsigned int nchunks	= CEIL((double)(size / CHUNKSIZE));

    	if ( (strlen(cmd->params[1]) == 5) && strncmp(cmd->params[1], "CHUNK", 5) == 0 ) {
    		/* This is a filechunk pair */
    		unsigned int chunk_size	= atoi(cmd->params[5]);
    		unsigned int cur_chunk	= atoi(cmd->params[6]);
    		char buf[CHUNKSIZE];
    		int ret = 0;

    		ret = socket_read(display->socket, buf, chunk_size);

    		e = event_filedrop_chunk_new(name, type, size, nchunks, chunk_size, cur_chunk, buf);
			return e;
    	} else if ( (strlen(cmd->params[1]) == 3) && strncmp(cmd->params[1], "END", 3) == 0 ) {
    		/* This is the end of the transfer */
    		e = event_filedrop_end_new(name, type, size, nchunks);
    		return e;
    	} else {
    		/* Filedrop Initalisation */
    		e = event_filedrop_init_new(name, type, size, nchunks);
    		return e;
    	}
    }
    else if (len == 6 && strncmp(cmd->params[0], "DROP64", 6) == 0) {
		/* The client application is responsible for base64 decoding
		 */
		char *name	= strdup(cmd->params[2]);
		char *type	= strdup(cmd->params[3]);
		unsigned int o_size	= atoi(cmd->params[4]);

		if ( (strlen(cmd->params[1]) == 5) && strncmp(cmd->params[1], "CHUNK", 5) == 0 ) {
			/* This is a filechunk pair */
			unsigned int e_size	= atoi(cmd->params[5]);
			unsigned int chunk_size	= atoi(cmd->params[6]);
			unsigned int cur_chunk	= atoi(cmd->params[7]);

			unsigned int nchunks	= CEIL((double)(e_size / CHUNKSIZE));

			char buf[CHUNKSIZE];
			int ret = 0;

			ret = socket_read(display->socket, buf, chunk_size);

			e = event_filedrop64_chunk_new(name, type, o_size, e_size, nchunks, chunk_size, cur_chunk, buf);
			return e;
		} else if ( (strlen(cmd->params[1]) == 3) && strncmp(cmd->params[1], "END", 3) == 0 ) {
			/* This is the end of the transfer */
			unsigned int e_size	= atoi(cmd->params[5]);
			unsigned int nchunks	= CEIL((double)(e_size / CHUNKSIZE));
			e = event_filedrop64_end_new(name, type, o_size, e_size, nchunks);
			return e;
		} else {
			/* Filedrop Initalisation */
			/* NB we don't actually know the encoded filesize nor the number of chunks yet */
			e = event_filedrop64_init_new(name, type, o_size);
			return e;
		}
	}

    return NULL;
}

int SendRegisterCallbackMsg(Display *display, char* events) 
{
	Command *cmd = NULL;
	Socket *socket = NULL;

	socket = display->socket;

	/*cmd = command_format("REG_CB %s", events); */
	/* this is a special case since the strings are already quoted */
	cmd = command_format_json("REG_CB", "%s", events);
	if (cmd == NULL)
		return -1;

	if (command_send(cmd, socket) != 0) {
		command_free(cmd);
		return -1;
	}

	return 0;
}	

void RegisterCallback(Display *display, EventType etype, EventCallback cb, void *data)
{
    callbacks_add(display->callbacks, etype, cb, data);
}

void MainLoop(Display *display)
{
    Event *e = NULL;
    while ((e = GetEvent(display))) {
        callbacks_call(display->callbacks, display, e);
    }
}
